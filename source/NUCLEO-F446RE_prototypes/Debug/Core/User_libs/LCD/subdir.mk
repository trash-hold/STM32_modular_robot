################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/User_libs/LCD/DEV_Config.c \
../Core/User_libs/LCD/LCD.c 

OBJS += \
./Core/User_libs/LCD/DEV_Config.o \
./Core/User_libs/LCD/LCD.o 

C_DEPS += \
./Core/User_libs/LCD/DEV_Config.d \
./Core/User_libs/LCD/LCD.d 


# Each subdirectory must supply rules for building sources it contributes
Core/User_libs/LCD/%.o Core/User_libs/LCD/%.su Core/User_libs/LCD/%.cyclo: ../Core/User_libs/LCD/%.c Core/User_libs/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_prototypes/Core/User_libs/LCD" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_prototypes/Core/User_libs/Fonts" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-User_libs-2f-LCD

clean-Core-2f-User_libs-2f-LCD:
	-$(RM) ./Core/User_libs/LCD/DEV_Config.cyclo ./Core/User_libs/LCD/DEV_Config.d ./Core/User_libs/LCD/DEV_Config.o ./Core/User_libs/LCD/DEV_Config.su ./Core/User_libs/LCD/LCD.cyclo ./Core/User_libs/LCD/LCD.d ./Core/User_libs/LCD/LCD.o ./Core/User_libs/LCD/LCD.su

.PHONY: clean-Core-2f-User_libs-2f-LCD

