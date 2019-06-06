//*****************************************************************************
//
// Codigo de partida Practica 1.
// Autores: Eva Gonzalez, Ignacio Herrero, Jose Manuel Cano
//
//*****************************************************************************

#include <serialprotocol.h>
#include <serialprotocol.h>
#include<stdbool.h>
#include<stdint.h>

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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "utils/cpu_usage.h"
#include "timers.h"              // FreeRTOS: definiciones relacionadas con timers

#include "drivers/rgb.h"
#include <tivarpc.h>
#include "drivers/configADC.h"

/* Incluye librerias para el uso de Sensor Sparkfun*/
#include "drivers/i2c_if.h"
//Implementa un driver de alto nivel I2C que realiza diferentes tipos de trensacciones por el bus I2C
#include "sparkfun_apds9960drv.h"
//Implementa una API que permite configurar y gestionar el sensor sparkfun
#include "event_groups.h"        // FreeRTOS: definiciones relacionadas con grupos de eventos


// -----------------------------------------------------------------------------------
// ----------------------------    VARIABLES GLOBALES      ---------------------------
// -----------------------------------------------------------------------------------

// Variables que se incorporan desde el codigo base
uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;

//Variables para interrupcion por pulsadores (Punto 1 del Proyecto 1)
//xSemaphoreHandle semaforo_freertos2;

// Variables para uso del ADC (Punto 2 del Proyecto 1)
uint8_t i12 = {0};
uint8_t i8  = {0};
uint8_t ADC_Automatico = {0};
uint8_t Muestras8      = {0};

uint16_t chan12[4][4];              // Variables auxiliares para sondeo de ADC con resolucion de 12bits
uint8_t  chan8[4][8];               // Variables auxiliares para sondeo de ADC con resolucion de 8bits

//Variables para evitar errores de envio simultaneo de paquetes de datos por USB
xSemaphoreHandle g_pUARTSemaphore;  //Semaforo del mutex

//Interrupcion por sensor
//xSemaphoreHandle semaforo_freertos3;
uint8_t Prox_int = {0};
uint8_t Aux_ProxSen = {0};
uint8_t proximity_data;

uint8_t Prox_Gest = {0};

//Punto 4 del proyecto (Optimizacion)
EventGroupHandle_t FlagsEventos;

#define BUTTON1a_FLAG 0x0001
#define BUTTON2a_FLAG 0x0002

//

uint8_t Prox_Gest_Qt = {0};
extern SF_APDS9960_gesture_data_type gesture_data_;


extern void vUARTTask( void *pvParameters );

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
// Esta funcion se llama si la biblioteca driverlib o FreeRTOS comprueban la existencia de un error (mediante
// las macros ASSERT(...) y configASSERT(...)
// Los parametros nombrefich y linea contienen informacion de en que punto se encuentra el error...
//
//*****************************************************************************

#ifdef DEBUG
void __error__(char *nombrefich, uint32_t linea)
{
    while(1)
    {
    }
}
#endif

//*****************************************************************************
//
// Aqui incluimos los "ganchos" a los diferentes eventos del FreeRTOS
//
//*****************************************************************************

//Esto es lo que se ejecuta cuando el sistema detecta un desbordamiento de pila
//

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//Esto se ejecuta cada Tick del sistema. LLeva la estadistica de uso de la CPU (tiempo que la CPU ha estado funcionando)
void vApplicationTickHook( void )
{
    static uint8_t count = 0;

    if (++count == 10)
    {
        g_ui32CPUUsage = CPUUsageTick();
        count = 0;
    }
    //return;
}

//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationIdleHook (void)
{
    SysCtlSleep();
}


//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationMallocFailedHook (void)
{
    while(1);
}



//*****************************************************************************
//
// A continuacion van las tareas...
//
//*****************************************************************************

// El codigo de esta tarea esta definida en el fichero command.c, es la que se encarga de procesar los comandos del interprete a traves
// del terminal serie (puTTY)
//Aqui solo la declaramos para poderla referenciar en la funcion main

extern void vUARTTask( void *pvParameters );


static portTASK_FUNCTION(Interrupcion,pvParameters)
{
    int8_t Auxiliar = 0;
    PARAMETERS_ESTATUS_PERIFERICOS parametro;

    while(1)
    {
        xEventGroupWaitBits(FlagsEventos,BUTTON1a_FLAG,pdTRUE,pdFALSE,portMAX_DELAY);

        Auxiliar = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) <<5;
        Auxiliar = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)+ Auxiliar;

        Auxiliar = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)+ Auxiliar;
        Auxiliar = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)+ Auxiliar;
        Auxiliar = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)+ Auxiliar;

        parametro.value = Auxiliar;
        xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
        TivaRPC_SendFrame(COMMAND_ESTATUS_PERIFERICOS,&parametro,sizeof(parametro));
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static portTASK_FUNCTION(SENSORPROX_SEND,pvParameters)
{
    PARAMETERS_SENSORPROX_SEND parametro_P;
    PARAMETERS_SENSORGEST_SEND parametro_G;
    PARAMETERS_SENSOR_SEND parametro_PG;
    PARAMETERS_SENSORGEST_SEND_QT parametro_GQ;
    PARAMETERS_SENSOR_SEND_QT parametro_PGQ;

    while(1)
    {
        xEventGroupWaitBits(FlagsEventos,BUTTON2a_FLAG,pdTRUE,pdFALSE,portMAX_DELAY);

        if ((Prox_Gest == 1) && (Prox_Gest_Qt == 1))
        {
            SF_APDS9960_readGesture();

            if(Prox_int == 1)
            {
                SF_APDS9960_readProximity(&proximity_data);

                int l;
                int Cuenta;
                int n;

                Cuenta = 0;
                for(l =0; l<4; l++)
                {
                    for (n =0; n<8 ;n++)
                    {
                        parametro_PGQ.u_data[n] = gesture_data_.u_data[Cuenta];
                        parametro_PGQ.d_data[n] = gesture_data_.d_data[Cuenta];
                        parametro_PGQ.l_data[n] = gesture_data_.l_data[Cuenta];
                        parametro_PGQ.r_data[n] = gesture_data_.r_data[Cuenta];
                        Cuenta = Cuenta +1;
                    }

                    parametro_PGQ.ON = Aux_ProxSen;
                    parametro_PGQ.proximity_data = proximity_data;

                    xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
                    TivaRPC_SendFrame(COMMAND_SENSOR_SEND_QT,(void *)&parametro_PGQ,sizeof(parametro_PGQ));
                    xSemaphoreGive(g_pUARTSemaphore);
                    Aux_ProxSen  = !Aux_ProxSen;
                }

                SF_APDS9960_clearProximityInt();
            }

            else
            {
                int l;
                int Cuenta;
                int n;

                Cuenta = 0;
                for(l =0; l<8; l++)
                {
                    for (n =0;n<8;n++)
                    {
                        parametro_GQ.u_data[n] = gesture_data_.u_data[Cuenta];
                        parametro_GQ.d_data[n] = gesture_data_.d_data[Cuenta];
                        parametro_GQ.l_data[n] = gesture_data_.l_data[Cuenta];
                        parametro_GQ.r_data[n] = gesture_data_.r_data[Cuenta];
                        Cuenta = Cuenta +1;
                    }

                    xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
                    TivaRPC_SendFrame(COMMAND_SENSORGEST_SEND_QT,(void *)&parametro_GQ,sizeof(parametro_GQ));
                    xSemaphoreGive(g_pUARTSemaphore);
                }

                SF_APDS9960_clearProximityInt();
            }
        }

        else if((Prox_int == 1) && (Prox_Gest == 1) && (Prox_Gest_Qt == 0))
        {
            SF_APDS9960_readProximity(&proximity_data);
            parametro_PG.ON = Aux_ProxSen;
            parametro_PG.proximity_data = proximity_data;

            if (SF_APDS9960_isGestureAvailable())
            {
                switch (SF_APDS9960_readGesture())
                {
                case SF_APDS9960_DIR_UP:
                    parametro_PG.GEST = 0x01;
                    break;

                case SF_APDS9960_DIR_DOWN:
                    parametro_PG.GEST = 0x02;
                    break;

                case SF_APDS9960_DIR_LEFT:
                    parametro_PG.GEST = 0x04;
                    break;

                case SF_APDS9960_DIR_RIGHT:
                    parametro_PG.GEST = 0x08;
                    break;

                case SF_APDS9960_DIR_NEAR:
                    parametro_PG.GEST = 0x10;
                    break;

                case SF_APDS9960_DIR_FAR:
                    parametro_PG.GEST = 0x20;
                    break;

                default:
                    parametro_PG.GEST = 0x00;
                }
            }

            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
            TivaRPC_SendFrame(COMMAND_SENSOR_SEND,(void *)&parametro_PG,sizeof(parametro_PG));
            xSemaphoreGive(g_pUARTSemaphore);
            Aux_ProxSen  = !Aux_ProxSen;
            SF_APDS9960_clearProximityInt();
        }

        else if((Prox_int == 1) && (Prox_Gest == 0) && (Prox_Gest_Qt == 0))
        {
            SF_APDS9960_readProximity(&proximity_data);
            parametro_P.ON = Aux_ProxSen;
            parametro_P.proximity_data = proximity_data;

            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
            TivaRPC_SendFrame(COMMAND_SENSORPROX_SEND,(void *)&parametro_P,sizeof(parametro_P));
            xSemaphoreGive(g_pUARTSemaphore);
            Aux_ProxSen  = !Aux_ProxSen;
            SF_APDS9960_clearProximityInt();
        }
        else if((Prox_int == 0) && (Prox_Gest == 1) && (Prox_Gest_Qt == 0))
        {
            if (SF_APDS9960_isGestureAvailable())
            {
                switch (SF_APDS9960_readGesture())
                {
                case SF_APDS9960_DIR_UP:
                    parametro_G.GEST = 0x01;
                    break;

                case SF_APDS9960_DIR_DOWN:
                    parametro_G.GEST = 0x02;
                    break;

                case SF_APDS9960_DIR_LEFT:
                    parametro_G.GEST = 0x04;
                    break;

                case SF_APDS9960_DIR_RIGHT:
                    parametro_G.GEST = 0x08;
                    break;

                case SF_APDS9960_DIR_NEAR:
                     parametro_G.GEST = 0x10;
                     break;

                 case SF_APDS9960_DIR_FAR:
                     parametro_G.GEST = 0x20;
                     break;

                default:
                    parametro_G.GEST = 0x00;
                }
            }

            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
            TivaRPC_SendFrame(COMMAND_SENSORGEST_SEND,(void *)&parametro_G,sizeof(parametro_G));
            xSemaphoreGive(g_pUARTSemaphore);
            SF_APDS9960_clearProximityInt();

        }
    }
}

static portTASK_FUNCTION(ADCTask,pvParameters)
{
    MuestrasADC muestras;
    PARAMETERS_ADC_SAMPLE    parameter;
    PARAMETERS_ADC_SAMPLE8_s parameter8s;
    PARAMETERS_ADC_SAMPLE12  parameter12A;
    PARAMETERS_ADC_SAMPLE8   parameter8A;

    while(1)
    {
        configADC_LeeADC(&muestras);

        if(ADC_Automatico == 0 && Muestras8 ==0)
        {
            parameter.chan1=muestras.chan1;
            parameter.chan2=muestras.chan2;
            parameter.chan3=muestras.chan3;
            parameter.chan4=muestras.chan4;

            //Encia el mensaje hacia QT
            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
            TivaRPC_SendFrame(COMMAND_ADC_SAMPLE,(void *)&parameter,sizeof(parameter));
            xSemaphoreGive(g_pUARTSemaphore);
        }


        else if(ADC_Automatico == 0 && Muestras8 ==1)
        {
            parameter8s.chan1=((muestras.chan1 & 0x0FF0)>>4);
            parameter8s.chan2=((muestras.chan2 & 0x0FF0)>>4);
            parameter8s.chan3=((muestras.chan3 & 0x0FF0)>>4);
            parameter8s.chan4=((muestras.chan4 & 0x0FF0)>>4);

            //Encia el mensaje hacia QT
            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
            TivaRPC_SendFrame(COMMAND_ADC_SAMPLE8_s,(void *)&parameter8s,sizeof(parameter8s));
            xSemaphoreGive(g_pUARTSemaphore);
        }

        else if(ADC_Automatico == 1 && Muestras8 == 0)
        {
            chan12[0][i12]= (muestras.chan1);
            chan12[1][i12]= (muestras.chan2);
            chan12[2][i12]= (muestras.chan3);
            chan12[3][i12]= (muestras.chan4);

            i12 = i12 + 1;

            if (i12 > 3)
            {
                int i;
                for (i=0;i<4;i++)
                {
                    parameter12A.chan1[i]= chan12[0][i];
                    parameter12A.chan2[i]= chan12[1][i];
                    parameter12A.chan3[i]= chan12[2][i];
                    parameter12A.chan4[i]= chan12[3][i];
                }
                xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
                TivaRPC_SendFrame(COMMAND_ADC_SAMPLE12,(void *)&parameter12A,sizeof(parameter12A));
                xSemaphoreGive(g_pUARTSemaphore);
                i12 = 0;

                int j;
                for (j=0;j<4;j++)
                {
                    chan12[0][j] = 0;
                    chan12[1][j] = 0;
                    chan12[2][j] = 0;
                    chan12[3][j] = 0;
                }
            }
        }


        else if(ADC_Automatico == 1 && Muestras8 == 1)
        {
            chan8[0][i8]= ((muestras.chan1 & 0x0FF0)>>4);
            chan8[1][i8]= ((muestras.chan2 & 0x0FF0)>>4);
            chan8[2][i8]= ((muestras.chan3 & 0x0FF0)>>4);
            chan8[3][i8]= ((muestras.chan4 & 0x0FF0)>>4);

            i8 = i8 + 1;

            if (i8 > 7)
            {
                int i;
                for (i=0;i<8;i++)
                {
                    parameter8A.chan1[i]= chan8[0][i];
                    parameter8A.chan2[i]= chan8[1][i];
                    parameter8A.chan3[i]= chan8[2][i];
                    parameter8A.chan4[i]= chan8[3][i];
                }

                xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
                TivaRPC_SendFrame(COMMAND_ADC_SAMPLE8,(void *)&parameter8A,sizeof(parameter8A));
                xSemaphoreGive(g_pUARTSemaphore);

                i8 = 0;

                int j;
                for (j=0;j<8;j++)
                {
                    chan8[0][i] = 0;
                    chan8[1][i] = 0;
                    chan8[2][i] = 0;
                    chan8[3][i] = 0;
                }
            }
        }
        else
        {
        }
    }
}

static portTASK_FUNCTION(LEDTask,pvParameters)
{
    PARAMETERS_SENSORPROX_SEND parametro;//

    int32_t i32Estado_led=0;
    portTickType ui32LastTime;
    uint32_t ui32Intervalo;
    ui32Intervalo =(0.25*configTICK_RATE_HZ);

    ui32LastTime = xTaskGetTickCount();

    while(1)
    {
        i32Estado_led = !i32Estado_led;
        vTaskDelayUntil(&ui32LastTime, ui32Intervalo); //Espera del RTOS (eficiente, no gasta CPU)
        if (Prox_int)
        {
            Aux_ProxSen  = 0;
            parametro.ON = 0;
            parametro.proximity_data = proximity_data;

            xSemaphoreTake(g_pUARTSemaphore,portMAX_DELAY);
            TivaRPC_SendFrame(COMMAND_SENSORPROX_SEND,(void *)&parametro,sizeof(parametro));
            xSemaphoreGive(g_pUARTSemaphore);
            if(Prox_int == 2){Prox_int = 0;}
        }
        else
        {

        }
    }
}


//*****************************************************************************
//
// Funcion main(), Inicializa los perifericos, crea las tareas, etc... y arranca el bucle del sistema
//
//*****************************************************************************

int main(void)
{

    //
    // Set the clocking to run at 40 MHz from the PLL.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
            SYSCTL_OSC_MAIN);   //Ponermos el reloj principal a 40 MHz (200 Mhz del Pll dividido por 5)

    // Get the system clock speed.
    g_ulSystemClock = SysCtlClockGet();


    //Habilita el clock gating de los perifericos durante el bajo consumo --> perifericos que se desee activos en modo Sleep
    //                                                                        deben habilitarse con SysCtlPeripheralSleepEnable
    ROM_SysCtlPeripheralClockGating(true);

    // Inicializa el subsistema de medida del uso de CPU (mide el tiempo que la CPU no esta dormida)
    // Para eso utiliza un timer, que aqui hemos puesto que sea el TIMER3 (ultimo parametro que se pasa a la funcion)
    // (y por tanto este no se deberia utilizar para otra cosa).
    CPUUsageInit(g_ulSystemClock, configTICK_RATE_HZ/10, 3);

    //
    // Inicializa la UARTy la configura a 115.200 bps, 8-N-1 .
    //se usa para mandar y recibir mensajes y comandos por el puerto serie
    // Mediante un programa terminal como gtkterm, putty, cutecom, etc...
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0,115200,SysCtlClockGet());

    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);   //La UART tiene que seguir funcionando aunque el micro este dormido
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);   //La UART tiene que seguir funcionando aunque el micro este dormido


    //Inicializa los LEDs usando libreria RGB --> usa Timers 0 y 1 (eliminar si no se usa finalmente)
    RGBInit(1);
    SysCtlPeripheralSleepEnable(GREEN_TIMER_PERIPH);
    SysCtlPeripheralSleepEnable(BLUE_TIMER_PERIPH);
    SysCtlPeripheralSleepEnable(RED_TIMER_PERIPH);  //Redundante porque BLUE_TIMER_PERIPH y GREEN_TIMER_PERIPH son el mismo

    //Volvemos a configurar los LEDs en modo GPIO POR Defecto
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Inicializa los botones (tambien en el puerto F) y habilita sus interrupciones
    ButtonsInit();
    IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
    GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS,GPIO_BOTH_EDGES);

    // Configuracion TIMER5
    // Habilita periferico Timer5
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER5);
    // Configura el Timer5 para cuenta periodica de 32 bits (no lo separa en TIMER0A y TIMER0B)
    TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
    TimerControlTrigger(TIMER5_BASE, TIMER_A,1);

    //Inicializa los botones (tambien en el puerto B) y habilita sus interrupciones
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_1);

    IntPrioritySet(INT_GPIOB,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    GPIOIntDisable(GPIO_PORTB_BASE,GPIO_PIN_0);
    GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_1);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_1,GPIO_FALLING_EDGE);
    IntEnable(INT_GPIOB);


    //****************************************************************************************************//
    //*******************************           IMPORTANTE           *************************************//
    //*******************************       CREACION DE TAREAS       ************************************ //

    // Crea la tarea que gestiona los comandos UART (definida en el fichero commands.c
    // Esta tarea funciona con el puerto serie y la terminal (teclado)

    if((xTaskCreate(vUARTTask, (portCHAR *)"Uart", 1024,NULL,tskIDLE_PRIORITY, NULL) != pdTRUE))
    {
        while(1);
    }

    configADC_IniciaADC();
    if((xTaskCreate(ADCTask, (portCHAR *)"ADC", 1024,NULL,tskIDLE_PRIORITY, NULL) != pdTRUE))
        {
            while(1);
        }

    if((xTaskCreate(LEDTask, "Led_Sensor_Prox", configMINIMAL_STACK_SIZE, NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
        while (1);

    if (xTaskCreate( Interrupcion, "Interrupcion", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL )!=pdPASS)
        while (1);

    if (xTaskCreate( SENSORPROX_SEND, "Interrupcion_Sensor", 512, NULL, tskIDLE_PRIORITY+1, NULL )!=pdPASS)
        while (1);


    //Crea el grupo de eventos
    FlagsEventos = xEventGroupCreate();
    if( FlagsEventos == NULL )
    {
        while(1);
    }

    //Crea los semaforos compartidos por tareas e ISR
    g_pUARTSemaphore=xSemaphoreCreateMutex();
    if ((g_pUARTSemaphore==NULL))
    {
        while (1);  //No hay memoria para los semaforo
    }


    //Esta funcion crea internamente una tarea para las comunicaciones USB.
    //Ademas, inicializa el USB y configura el perfil USB-CDC
    //Esta tarea funciona con el USB y la interfaz grafica del Qt creator

    //Inicializo la aplicacion de comunicacion con el PC (Remote). Ver fichero tivarpc.c

    TivaRPC_Init();


    //****************************************************************************************************//

    // Arranca el  scheduler.  Pasamos a ejecutar las tareas que se hayan activado.
    //
    vTaskStartScheduler();  //el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas
    //
    //De la funcion vTaskStartScheduler no se sale nunca... a partir de aqui pasan a ejecutarse las tareas.

    while(1)
    {
        //Si llego aqui es que algo raro ha pasado
    }
}

void GPIOFIntHandle(void)
{
    BaseType_t xHigherPriorityTaskWoken=pdFALSE;
    int32_t i32PinStatus=ROM_GPIOPinRead(GPIO_PORTF_BASE,ALL_BUTTONS);
    int32_t i32PinStatus_S = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1);

    if (!(i32PinStatus & LEFT_BUTTON)||!(i32PinStatus & RIGHT_BUTTON))
    {
        if (!(i32PinStatus_S & GPIO_PIN_1))
        {
            xEventGroupSetBitsFromISR(FlagsEventos, BUTTON2a_FLAG, &xHigherPriorityTaskWoken );
            xEventGroupSetBitsFromISR(FlagsEventos, BUTTON1a_FLAG, &xHigherPriorityTaskWoken );

            MAP_GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);
            MAP_GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_1);
        }
        else
        {
            //Activa los flags BUTTON1a_FLAG y BUTTON1b_FLAG
            xEventGroupSetBitsFromISR(FlagsEventos, BUTTON1a_FLAG, &xHigherPriorityTaskWoken );
            MAP_GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);
        }
    }

    else if (!(i32PinStatus_S & GPIO_PIN_1))
    {
        xEventGroupSetBitsFromISR(FlagsEventos, BUTTON2a_FLAG, &xHigherPriorityTaskWoken );
        MAP_GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_1);
    }

    else
    {
        xEventGroupSetBitsFromISR(FlagsEventos, BUTTON1a_FLAG, &xHigherPriorityTaskWoken );
        MAP_GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);
    }

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

