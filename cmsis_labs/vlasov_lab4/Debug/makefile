################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
vlasov_lab4.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
vlasov_lab4.list \

OBJCOPY_BIN += \
vlasov_lab4.bin \


# All Target
all: vlasov_lab4.elf secondary-outputs

# Tool invocations
vlasov_lab4.elf: $(OBJS) $(USER_OBJS) C:\Users\antpo\Desktop\ITMO\Master\2\ semester\RSH\stm32examples\cmsis_labs\vlasov_lab4\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "vlasov_lab4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\antpo\Desktop\ITMO\Master\2 semester\RSH\stm32examples\cmsis_labs\vlasov_lab4\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="vlasov_lab4.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

vlasov_lab4.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "vlasov_lab4.list"
	@echo 'Finished building: $@'
	@echo ' '

vlasov_lab4.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "vlasov_lab4.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
