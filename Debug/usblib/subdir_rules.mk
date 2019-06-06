################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
usblib/usbbuffer.obj: ../usblib/usbbuffer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbbuffer.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbdesc.obj: ../usblib/usbdesc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbdesc.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbdma.obj: ../usblib/usbdma.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbdma.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbkeyboardmap.obj: ../usblib/usbkeyboardmap.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbkeyboardmap.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbmode.obj: ../usblib/usbmode.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbmode.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbringbuf.obj: ../usblib/usbringbuf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbringbuf.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbtick.obj: ../usblib/usbtick.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbtick.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usblib/usbulpi.obj: ../usblib/usbulpi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/include" --include_path="C:/Users/santiago/Dropbox/TEC/Malaga/Empotrados/Proyecta1_1_Git/gise-p1-tiva-Santy241996/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usblib/usbulpi.d_raw" --obj_directory="usblib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


