################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lwip/src/netif/subdir.mk
-include lwip/src/core/ipv4/subdir.mk
-include lwip/src/core/subdir.mk
-include lwip/src/arch/subdir.mk
-include lwip/src/api/subdir.mk
-include freertos/src/subdir.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: AC_Network_Controller.axf

# Tool invocations
AC_Network_Controller.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Embedded_Programs\Network_Controller\lpc_chip_40xx\Debug" -L"C:\Embedded_Programs\Network_Controller\lpc_board_ea_devkit_4088\Debug" -Xlinker -Map="AC_Network_Controller.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "lwip_tcpecho_freertos_Debug.ld" -o "AC_Network_Controller.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) AC_Network_Controller.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "AC_Network_Controller.axf"; # arm-none-eabi-objcopy -v -O binary "AC_Network_Controller.axf" "AC_Network_Controller.bin" ; # checksum -p LPC4088 -d "AC_Network_Controller.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
