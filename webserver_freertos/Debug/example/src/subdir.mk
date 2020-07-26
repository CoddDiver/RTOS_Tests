################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/cr_redlib_heap_fix.c \
../example/src/cr_startup_lpc40xx.c \
../example/src/http_messages.c \
../example/src/lwip_fs.c \
../example/src/netconn_fs.c \
../example/src/sysinit.c \
../example/src/webserver_freertos.c 

OBJS += \
./example/src/cr_redlib_heap_fix.o \
./example/src/cr_startup_lpc40xx.o \
./example/src/http_messages.o \
./example/src/lwip_fs.o \
./example/src/netconn_fs.o \
./example/src/sysinit.o \
./example/src/webserver_freertos.o 

C_DEPS += \
./example/src/cr_redlib_heap_fix.d \
./example/src/cr_startup_lpc40xx.d \
./example/src/http_messages.d \
./example/src/lwip_fs.d \
./example/src/netconn_fs.d \
./example/src/sysinit.d \
./example/src/webserver_freertos.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M4 -I"C:\Embedded_Programs\Network_Controller\lpc_chip_40xx\inc" -I"C:\Embedded_Programs\Network_Controller\lpc_board_ea_devkit_4088\inc" -I"C:\Embedded_Programs\Network_Controller\webserver_freertos\example\inc" -I"C:\Embedded_Programs\Network_Controller\webserver_freertos\lwip\inc" -I"C:\Embedded_Programs\Network_Controller\webserver_freertos\lwip\inc\ipv4" -I"C:\Embedded_Programs\Network_Controller\webserver_freertos\freertos\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


