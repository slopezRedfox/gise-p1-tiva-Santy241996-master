################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
commands.obj: ../commands.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="commands.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

crc.obj: ../crc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="crc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

serialprotocol.obj: ../serialprotocol.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="serialprotocol.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sparkfun_apds9960drv.obj: ../sparkfun_apds9960drv.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="sparkfun_apds9960drv.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tivarpc.obj: ../tivarpc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tivarpc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_dev_serial.obj: ../usb_dev_serial.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usb_dev_serial.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_serial_structs.obj: ../usb_serial_structs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usb_serial_structs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


