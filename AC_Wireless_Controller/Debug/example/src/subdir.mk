################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/AC_Wireless_Controller.c \
../example/src/Axis_Configuration.c \
../example/src/Axis_Control.c \
../example/src/Network_Link.c \
../example/src/Profiler.c \
../example/src/TCP_Connection.c \
../example/src/UART_Manager.c \
../example/src/cr_redlib_heap_fix.c \
../example/src/cr_startup_lpc40xx.c \
../example/src/http_messages.c \
../example/src/myprint.c \
../example/src/netconn_fs.c \
../example/src/sysinit.c 

OBJS += \
./example/src/AC_Wireless_Controller.o \
./example/src/Axis_Configuration.o \
./example/src/Axis_Control.o \
./example/src/Network_Link.o \
./example/src/Profiler.o \
./example/src/TCP_Connection.o \
./example/src/UART_Manager.o \
./example/src/cr_redlib_heap_fix.o \
./example/src/cr_startup_lpc40xx.o \
./example/src/http_messages.o \
./example/src/myprint.o \
./example/src/netconn_fs.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/AC_Wireless_Controller.d \
./example/src/Axis_Configuration.d \
./example/src/Axis_Control.d \
./example/src/Network_Link.d \
./example/src/Profiler.d \
./example/src/TCP_Connection.d \
./example/src/UART_Manager.d \
./example/src/cr_redlib_heap_fix.d \
./example/src/cr_startup_lpc40xx.d \
./example/src/http_messages.d \
./example/src/myprint.d \
./example/src/netconn_fs.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M4 -I"C:\Embedded_Programs\Network_Controller\lpc_chip_40xx\inc" -I"C:\Embedded_Programs\Network_Controller\lpc_board_ea_devkit_4088\inc" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\example\inc" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\lwip\inc" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\lwip\inc\ipv4" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\freertos\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


