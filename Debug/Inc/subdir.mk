################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/system_stm32f4xx.c 

OBJS += \
./Inc/system_stm32f4xx.o 

C_DEPS += \
./Inc/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/%.o Inc/%.su Inc/%.cyclo: ../Inc/%.c Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/Mini_RTOS/ARTOS/Inc" -I"D:/Mini_RTOS/ARTOS/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc

clean-Inc:
	-$(RM) ./Inc/system_stm32f4xx.cyclo ./Inc/system_stm32f4xx.d ./Inc/system_stm32f4xx.o ./Inc/system_stm32f4xx.su

.PHONY: clean-Inc

