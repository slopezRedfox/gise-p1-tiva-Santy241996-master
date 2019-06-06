/*
 * tivarpc.c
 *
 * Implementa la funcionalidad RPC entre la TIVA y el interfaz de usuario
 */

#include <tivarpc.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "drivers/rgb.h"
#include "usb_dev_serial.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "rpc_commands.h"
#include "drivers/configADC.h"

#include "drivers/i2c_if.h"
#include "sparkfun_apds9960drv.h"

float Intensidad;

extern uint8_t ADC_Automatico;
extern uint8_t Muestras8;
extern uint8_t i12;
extern uint8_t i8;
extern uint8_t Prox_int;
extern uint8_t Prox_Gest;
extern uint8_t Prox_Gest_Qt;

//Variables para evitar errores de envio simultaneo de paquetes de datos por USB
extern xSemaphoreHandle g_pUARTSemaphore;

#define PROX_INT_LOW    0  // No far interrupt

//Defino a un tipo que es un puntero a funcion con el prototipo que tienen que tener las funciones que definamos
typedef int32_t (*rpc_function_prototype)(uint32_t param_size, void *param);

static uint8_t Rxframe[MAX_FRAME_SIZE]; //Usar una global permite ahorrar pila en la tarea de RX.
static uint8_t Txframe[MAX_FRAME_SIZE]; //Usar una global permite ahorrar pila en las tareas, pero hay que tener cuidado al transmitir desde varias tareas!!!!
static uint32_t gRemoteProtocolErrors=0;

//Funciones "internas//
static int32_t TivaRPC_ReceiveFrame(uint8_t *frame, int32_t maxFrameSize);

// *********************************************************************************************
// ********************* FUNCIONES RPC (se ejecutan en respuesta a comandos RPC recibidos desde el interfaz Qt *********************
// *********************************************************************************************

//Funcion que se ejecuta cuando llega un paquete indicando comando rechazado
static int32_t RPC_RejectedCommand(uint32_t param_size, void *param)
{
    //He recibido "Comando rechazado" desde el PC
    //TODO, por hacer: Tratar dicho error??
    gRemoteProtocolErrors++;
    return 0;
}


//Funcion que se ejecuta cuando llega un PING
static int32_t RPC_Ping(uint32_t param_size, void *param)
{
    int32_t numdatos;

    numdatos=TivaRPC_SendFrame(COMMAND_PING,NULL,0);
    return numdatos;
}



//Funcion que se ejecuta cuando llega el comando que configura los LEDS
static int32_t RPC_LEDGpio(uint32_t param_size, void *param)
{
    PARAMETERS_LED_GPIO parametro;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,parametro.value);
        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

//Funcion que se ejecuta cuando recibimos un comando que no tenemos aun implementado
static int32_t RPC_UnimplementedCommand(uint32_t param_size, void *param)
{
    return PROT_ERROR_UNIMPLEMENTED_COMMAND;
}

//Funcion que se ejecuta cuando llega el comando que configura el BRILLO
static int32_t RPC_LEDPwmBrightness(uint32_t param_size, void *param)
{
    PARAMETERS_LED_PWM_BRIGHTNESS parametro;


    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        Intensidad = parametro.rIntensity;
        RGBIntensitySet(parametro.rIntensity);

        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

//Funcion que se ejecuta cuando llega el comando cambio de modo de operacion
static int32_t RPC_MODO(uint32_t param_size, void *param)
{
    PARAMETERS_COMMAND_MODO parametro;


    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        if (parametro.Modo == 1)
        {
            UARTprintf("Cambio a modo PWM (rgb)\n");
            RGBEnable();
        }
        else
        {
            UARTprintf("cambio a modo GPIO\n");
            RGBDisable();
            ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
        }

        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

static int32_t RPC_RGB_MODE(uint32_t param_size, void *param)
{
    PARAMETERS_RGB parametro;
    uint32_t arrayRGB[3];

    int Auxiliar0; //Por alguna razon que desconozco, se necesita que se use una variable auxiliar
    int Auxiliar1; //Y no se puede usar array, debe ser variables individuales
    int Auxiliar2;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        Auxiliar0 = parametro.rojo;
        Auxiliar1 = parametro.verde;
        Auxiliar2 = parametro.azul;

        arrayRGB[0] = ((unsigned int)Auxiliar0)<<8; //En los comandos ingresados por terminal se usa una conversion a
        arrayRGB[1] = ((unsigned int)Auxiliar1)<<8; //int unsigned, aqui se hace directamente.
        arrayRGB[2] = ((unsigned int)Auxiliar2)<<8;

        RGBColorSet(arrayRGB);
        return 0;   //Devuelve Ok (valor mayor no negativo)
    }

    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

//CONSULTA STATUS
static int32_t RPC_ESTATUS_PERIFERICOS(uint32_t param_size, void *param)
{
    PARAMETERS_ESTATUS_PERIFERICOS parametro;
    int32_t numdatos;
    int8_t Auxiliar = 0;
    int32_t a = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

    Auxiliar = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)<<5;
    Auxiliar = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) + Auxiliar;

    Auxiliar = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)+ Auxiliar;
    Auxiliar = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)+ Auxiliar;
    Auxiliar = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)+ Auxiliar;

    parametro.value = Auxiliar;
    xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
    numdatos=TivaRPC_SendFrame(COMMAND_ESTATUS_PERIFERICOS ,&parametro,sizeof(parametro));
    xSemaphoreGive(g_pUARTSemaphore);
    return numdatos;
}


//CONSULTA STATUS
static int32_t RPC_INTERRUPCION(uint32_t param_size, void *param)
{
    PARAMETERS_INTERRUPCION parametro;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        if(parametro.Interrupcion == 1)
        {
            IntEnable(INT_GPIOF);
        }
        else
        {
            IntDisable(INT_GPIOF);
        }
        return 0;
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}


static int32_t RPC_ADCSample(uint32_t param_size, void *param)
{
    PARAMETERS_SENSORRGB parametro;
    int32_t numdatos;
    uint16_t r, g, b, c;

    SF_APDS9960_readAmbientLight(&c);
    SF_APDS9960_readRedLight(&r);
    SF_APDS9960_readGreenLight(&g);
    SF_APDS9960_readBlueLight(&b);

    parametro.R = r;
    parametro.G = g;
    parametro.B = b;
    parametro.I = c;

    xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
    numdatos=TivaRPC_SendFrame(COMMAND_SENSORRGB ,&parametro,sizeof(parametro));
    xSemaphoreGive(g_pUARTSemaphore);

    if (ADC_Automatico == 1)
    {

    }

    else
    {
        configADC_DisparaADC(); //Dispara la conversion (por software)
    }

    return numdatos;
}

static int32_t RPC_ADSGraf(uint32_t param_size, void *param)
{
    uint32_t ui32Period;
    PARAMETERS_ADC_GRAF parametro;
    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        //float Aux_f = 1/(parametro.frecuencia);
        ui32Period = SysCtlClockGet()/(parametro.frecuencia);

        TimerLoadSet(TIMER5_BASE, TIMER_A, ui32Period);

        if(parametro.Interrupcion == 1)
        {
            if (parametro.NumMuestras == 0)
            {
                //CONFIGURAR SECUENCIADOR 1
                ADCSequenceDisable(ADC0_BASE,1);

                //Configuramos la velocidad de conversion al maximo (1MS/s)
                ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

                TimerControlTrigger(TIMER5_BASE, TIMER_A,1);
                ADCSequenceConfigure(ADC0_BASE,1,ADC_TRIGGER_TIMER,0);  //Disparo software (Timer trigger)
                ADCHardwareOversampleConfigure(ADC0_BASE, 2);

                ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH0);
                ADCSequenceStepConfigure(ADC0_BASE,1,1,ADC_CTL_CH1);
                ADCSequenceStepConfigure(ADC0_BASE,1,2,ADC_CTL_CH2);
                ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH3|ADC_CTL_IE |ADC_CTL_END );   //La ultima muestra provoca la interrupcion
                ADCSequenceEnable(ADC0_BASE,1); //ACTIVO LA SECUENCIA

                // Activa el Timer5 (empezara a funcionar)
                TimerEnable(TIMER5_BASE, TIMER_A);

                ADC_Automatico = 1;
                Muestras8 = 0;
                i12 = 0;
                i8 = 0;
            }
            else
            {
                //CONFIGURAR SECUENCIADOR 1
                ADCSequenceDisable(ADC0_BASE,1);

                //Configuramos la velocidad de conversion al maximo (1MS/s)
                ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

                TimerControlTrigger(TIMER5_BASE, TIMER_A,1);
                ADCSequenceConfigure(ADC0_BASE,1,ADC_TRIGGER_TIMER,0);  //Disparo software (Timer trigger)
                ADCHardwareOversampleConfigure(ADC0_BASE, 0);

                ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH0);
                ADCSequenceStepConfigure(ADC0_BASE,1,1,ADC_CTL_CH1);
                ADCSequenceStepConfigure(ADC0_BASE,1,2,ADC_CTL_CH2);
                ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH3|ADC_CTL_IE |ADC_CTL_END );   //La ultima muestra provoca la interrupcion
                ADCSequenceEnable(ADC0_BASE,1); //ACTIVO LA SECUENCIA

                // Activa el Timer5 (empezara a funcionar)
                TimerEnable(TIMER5_BASE, TIMER_A);

                ADC_Automatico = 1;
                Muestras8 = 1;
                i12 = 0;
                i8 = 0;
            }
        }

        else
        {
            if (parametro.NumMuestras == 0)
            {
                TimerDisable(TIMER5_BASE, TIMER_A);

                //CONFIGURAR SECUENCIADOR 1
                ADCSequenceDisable(ADC0_BASE,1);

                //Configuramos la velocidad de conversion al maximo (1MS/s)
                ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

                TimerControlTrigger(TIMER5_BASE, TIMER_A, 0);
                ADCSequenceConfigure(ADC0_BASE,1,ADC_TRIGGER_PROCESSOR,0);  //Disparo software (processor trigger)
                ADCHardwareOversampleConfigure(ADC0_BASE, 2);

                ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH0);
                ADCSequenceStepConfigure(ADC0_BASE,1,1,ADC_CTL_CH1);
                ADCSequenceStepConfigure(ADC0_BASE,1,2,ADC_CTL_CH2);
                ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH3|ADC_CTL_IE |ADC_CTL_END );   //La ultima muestra provoca la interrupcion
                ADCSequenceEnable(ADC0_BASE,1); //ACTIVO LA SECUENCIA

                ADC_Automatico = 0;
                Muestras8 = 0;
                i12 = 0;
                i8 = 0;
            }

            else
            {
                TimerDisable(TIMER5_BASE, TIMER_A);

                //CONFIGURAR SECUENCIADOR 1
                ADCSequenceDisable(ADC0_BASE,1);

                //Configuramos la velocidad de conversion al maximo (1MS/s)
                ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

                TimerControlTrigger(TIMER5_BASE, TIMER_A, 0);
                ADCSequenceConfigure(ADC0_BASE,1,ADC_TRIGGER_PROCESSOR,0);  //Disparo software (processor trigger)
                ADCHardwareOversampleConfigure(ADC0_BASE, 0);

                ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH0);
                ADCSequenceStepConfigure(ADC0_BASE,1,1,ADC_CTL_CH1);
                ADCSequenceStepConfigure(ADC0_BASE,1,2,ADC_CTL_CH2);
                ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH3|ADC_CTL_IE |ADC_CTL_END );   //La ultima muestra provoca la interrupcion
                ADCSequenceEnable(ADC0_BASE,1); //ACTIVO LA SECUENCIA

                ADC_Automatico = 0;
                Muestras8 = 1;
                i12 = 0;
                i8 = 0;
            }
        }
        return 0;
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

static int32_t RPC_SENSOR_RGB(uint32_t param_size, void *param)
{
    PARAMETERS_SENSORRGB parametro;
    int32_t numdatos;
    uint16_t r, g, b, c;

    SF_APDS9960_readAmbientLight(&c);
    SF_APDS9960_readRedLight(&r);
    SF_APDS9960_readGreenLight(&g);
    SF_APDS9960_readBlueLight(&b);

    parametro.R = r;
    parametro.G = g;
    parametro.B = b;
    parametro.I = c;

    xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
    numdatos=TivaRPC_SendFrame(COMMAND_SENSORRGB ,&parametro,sizeof(parametro));
    xSemaphoreGive(g_pUARTSemaphore);
    return numdatos;
}

static int32_t RPC_SENSOR_PROX(uint32_t param_size, void *param)
{
    PARAMETERS_SENSORPROX parametro;
    uint8_t proximity_Interrupt = 0;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        if(parametro.Interrupcion_on == 1)
        {
            proximity_Interrupt = parametro.Distancia;
            Prox_int = 1;

            SF_APDS9960_setProximityGain(SF_APDS9960_PGAIN_2X);
            SF_APDS9960_setProximityIntLowThreshold(PROX_INT_LOW);
            SF_APDS9960_setProximityIntHighThreshold(proximity_Interrupt);
            SF_APDS9960_enableProximitySensor(1);
        }
        else
        {
            SF_APDS9960_disableProximitySensor();
            Prox_int = 2;
        }
        return 0;
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

static int32_t RPC_SENSOR_GEST(uint32_t param_size, void *param)
{
    PARAMETERS_SENSORGEST parametro;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        if(parametro.Interrupcion_on == 1)
        {
            Prox_Gest = 1;
            SF_APDS9960_enableGestureSensor(1);
        }
        else
        {
            SF_APDS9960_disableGestureSensor();
            Prox_Gest = 0;
        }

        if(parametro.Data_Qt == 1)
        {
            Prox_Gest_Qt = 1;
        }
        else
        {
            Prox_Gest_Qt = 0;
        }

        return 0;
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

// *********************************************************************************************
// ********************* Tabla de  FUnciones RPC
// *********************************************************************************************
/* Array que contiene las funciones que se van a ejecutar en respuesta a cada comando */
static const rpc_function_prototype rpc_function_table[]={
                                            RPC_RejectedCommand, /* Responde al paquete comando rechazado */
                                            RPC_Ping, /* Responde al comando ping */
                                            RPC_LEDGpio, /* Responde al comando LEDS */
                                            RPC_LEDPwmBrightness, /* Responde al comando Brillo */
                                            RPC_MODO, //Responde al comando modo de operacion
                                            RPC_RGB_MODE,
                                            RPC_ESTATUS_PERIFERICOS,
                                            RPC_INTERRUPCION,
                                            RPC_ADCSample,
                                            RPC_ADSGraf,
                                            RPC_SENSOR_RGB,
                                            RPC_SENSOR_PROX,
                                            RPC_SENSOR_GEST,
                                            RPC_UnimplementedCommand /* Este comando no esta implementado aun */
};


// Codigo para procesar los comandos recibidos a traves del canal USB del micro ("conector lateral")
//Esta tarea decodifica los comandos y ejecuta la función que corresponda a cada uno de ellos (por posicion)
//También gestiona posibles errores en la comunicacion
static portTASK_FUNCTION( TivaRPC_ServerTask, pvParameters ){

    //Frame es global en este fichero, se reutiliza en las funciones que envian respuestas ---> CUIDADO!!!

    int32_t numdatos;
    uint8_t command;
    void *ptrtoreceivedparam;

    /* The parameters are not used. (elimina el warning)*/
    ( void ) pvParameters;

    for(;;)
    {
        //Espera hasta que se reciba una trama con datos serializados por el interfaz USB
        numdatos=TivaRPC_ReceiveFrame(Rxframe,MAX_FRAME_SIZE); //Esta funcion es bloqueante

        if (numdatos>0)
        {
            //Si no ha habido errores recibiendo la trama, la intenta decodificar y procesar
            //primero se "desestufa" y se comprueba el checksum
            numdatos=destuff_and_check_checksum(Rxframe,numdatos);
            if (numdatos<0)
            {
                //Error de checksum (PROT_ERROR_BAD_CHECKSUM), ignorar el paquete
                gRemoteProtocolErrors++;
                // Procesamiento del error (TODO, POR HACER!!)
            }
            else
            {
                //El paquete esta bien, luego procedo a tratarlo.
                //Obtiene el valor del campo comando
                command=decode_command_type(Rxframe);
                //Obtiene un puntero al campo de parametros y su tamanio.
                numdatos=get_command_param_pointer(Rxframe,numdatos,&ptrtoreceivedparam);

                //Accedemos a la tabla de funciones y ejecutarmos la correspondiente al comando que ha llegado
                //Primero hay que comprobar que el tamanio de dicha tabla es suficiente.

                if (command<(sizeof(rpc_function_table)/sizeof(rpc_function_prototype)))
                {
                    int32_t error_status;
                    //Aqui es donde se ejecuta a funcion de la tabla que corresponde con el valor de comando que ha llegado
                    error_status=rpc_function_table[command](numdatos,ptrtoreceivedparam); //La funcion puede devolver códigos de error.

                    //Una vez ejecutado, se comprueba si ha habido errores.
                    switch(error_status)
                    {

                        //Se procesarían a continuación
                        case PROT_ERROR_NOMEM:
                        {
                            // Procesamiento del error NO MEMORY
                            UARTprintf("RPC Error: not enough memory\n");
                        }
                        break;
                        case PROT_ERROR_STUFFED_FRAME_TOO_LONG:
                        {
                            // Procesamiento del error STUFFED_FRAME_TOO_LONG
                            UARTprintf("RPC Error: Frame too long. Cannot be created\n");
                        }
                        break;
                        case PROT_ERROR_COMMAND_TOO_LONG:
                        {
                            // Procesamiento del error COMMAND TOO LONG
                            UARTprintf("RPC Error: Packet too long. Cannot be allocated\n");
                        }
                        break;
                        case PROT_ERROR_INCORRECT_PARAM_SIZE:
                        {
                            // Procesamiento del error INCORRECT PARAM SIZE
                            UARTprintf("RPC Error: Incorrect parameter size\n");
                        }
                        break;
                        case PROT_ERROR_UNIMPLEMENTED_COMMAND:
                        {
                            PARAMETERS_COMMAND_REJECTED parametro;
                            parametro.command=command;
                            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
                            numdatos=TivaRPC_SendFrame(COMMAND_REJECTED,&parametro,sizeof(parametro));
                            xSemaphoreGive(g_pUARTSemaphore);
                            UARTprintf("RPC Error: Unexpected command: %x\n",(uint32_t)command);
                            gRemoteProtocolErrors++;
                            //Aqui se podria, ademas, comprobar numdatos....
                        }
                        break;
                        //AÑadir casos de error adicionales aqui...
                        default:
                            /* No hacer nada */
                            break;
                    }
                }
                else
                {
                    /* El valor del comando que ha llegado es superior al numero de comandos que hay implementados en la tabla  */
                    // Se envia el "Comando Rechazado" hacia el GUI
                    PARAMETERS_COMMAND_REJECTED parametro;
                    parametro.command=command;
                    xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
                    numdatos=TivaRPC_SendFrame(COMMAND_REJECTED,&parametro,sizeof(parametro));
                    xSemaphoreGive(g_pUARTSemaphore);
                    UARTprintf("RPC Error: Unexpected command: %x\n",(uint32_t)command);
                    //Aqui se podria, ademas,  comprobar numdatos....
                    gRemoteProtocolErrors++;
                }
            }
        }
        else
        { // if (numdatos >0)
            //Error de recepcion de trama(PROT_ERROR_RX_FRAME_TOO_LONG), ignorar el paquete
            gRemoteProtocolErrors++;
            // Procesamiento del error (TODO)
        }
    }
}


//Inicializa la tarea que recibe comandos (se debe llamar desde main())
void TivaRPC_Init(void)
{
    USBSerialInit(32,32);   //Inicializo el  sistema USB-serie

    //
    // Crea la tarea que gestiona los comandos USB (definidos en CommandProcessingTask)
    //
    if(xTaskCreate(TivaRPC_ServerTask, (portCHAR *)"usbser",TIVARPC_TASK_STACK, NULL, TIVARPC_TASK_PRIORITY, NULL) != pdTRUE)
    {
        while(1);
    }

}


// TODO
//Ojo!! TxFrame es global (para ahorrar memoria de pila en las tareas) --> Se deben tomar precauciones al usar esta funcion varias tareas
//IDEM en lo que respecta al envio por la conexion USB serie desde varias tareas....
//Estas precauciones no se han tomado en este codigo de parti1da, pero al realizar la practica se deberian tener en cuenta....
// "TODO" (por hacer)
int32_t TivaRPC_SendFrame(uint8_t comando,void *parameter,int32_t paramsize)
{
    int32_t numdatos;

    numdatos=create_frame(Txframe,comando,parameter,paramsize,MAX_FRAME_SIZE);
    if (numdatos>=0)
    {
        USBSerialWrite(Txframe,numdatos,portMAX_DELAY);
    }

    return numdatos;
}

/* Recibe una trama (sin los caracteres de inicio y fin */
/* Utiliza la funcion bloqueante xSerialGetChar ---> Esta funcion es bloqueante y no se puede llamar desde una ISR !!!*/
// Esta funcion es INTERNA de la biblioteca y solo se utiliza en la rarea TivaRPC_ServerTask
static int32_t TivaRPC_ReceiveFrame(uint8_t *frame, int32_t maxFrameSize)
{
    int32_t i;
    uint8_t rxByte;

    do
    {
        //Elimino bytes de la cola de recepcion hasta llegar a comienzo de nueva trama
        USBSerialGetChar( ( char *)&rxByte, portMAX_DELAY);
    } while (rxByte!=START_FRAME_CHAR);

    i=0;
    do
    {
        USBSerialGetChar( ( char *)frame, portMAX_DELAY);
        i++;
    } while ((*(frame++)!=STOP_FRAME_CHAR)&&(i<=maxFrameSize));

    if (i>maxFrameSize)
    {
        return PROT_ERROR_RX_FRAME_TOO_LONG;    //Numero Negativo indicando error
    }
    else
    {

        if (i<(MINIMUN_FRAME_SIZE-START_SIZE))
        {
            return PROT_ERROR_BAD_SIZE; //La trama no tiene el tamanio minimo
        }
        else
        {
            return (i-END_SIZE); //Tamanio de la trama sin los bytes de inicio y fin
        }
    }
}
