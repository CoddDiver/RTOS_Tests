################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/ipv4/autoip.c \
../lwip/src/core/ipv4/icmp.c \
../lwip/src/core/ipv4/igmp.c \
../lwip/src/core/ipv4/inet.c \
../lwip/src/core/ipv4/inet_chksum.c \
../lwip/src/core/ipv4/ip.c \
../lwip/src/core/ipv4/ip_addr.c \
../lwip/src/core/ipv4/ip_frag.c 

OBJS += \
./lwip/src/core/ipv4/autoip.o \
./lwip/src/core/ipv4/icmp.o \
./lwip/src/core/ipv4/igmp.o \
./lwip/src/core/ipv4/inet.o \
./lwip/src/core/ipv4/inet_chksum.o \
./lwip/src/core/ipv4/ip.o \
./lwip/src/core/ipv4/ip_addr.o \
./lwip/src/core/ipv4/ip_frag.o 

C_DEPS += \
./lwip/src/core/ipv4/autoip.d \
./lwip/src/core/ipv4/icmp.d \
./lwip/src/core/ipv4/igmp.d \
./lwip/src/core/ipv4/inet.d \
./lwip/src/core/ipv4/inet_chksum.d \
./lwip/src/core/ipv4/ip.d \
./lwip/src/core/ipv4/ip_addr.d \
./lwip/src/core/ipv4/ip_frag.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/ipv4/%.o: ../lwip/src/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M4 -I"C:\Embedded_Programs\Network_Controller\lpc_chip_40xx\inc" -I"C:\Embedded_Programs\Network_Controller\lpc_board_ea_devkit_4088\inc" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\example\inc" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\lwip\inc" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\lwip\inc\ipv4" -I"C:\Embedded_Programs\Network_Controller\AC_Wireless_Controller\freertos\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


