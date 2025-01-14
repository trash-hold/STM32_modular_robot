################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LCD_waveshare/LCD/DEV_Config.c \
../Drivers/LCD_waveshare/LCD/LCD.c 

OBJS += \
./Drivers/LCD_waveshare/LCD/DEV_Config.o \
./Drivers/LCD_waveshare/LCD/LCD.o 

C_DEPS += \
./Drivers/LCD_waveshare/LCD/DEV_Config.d \
./Drivers/LCD_waveshare/LCD/LCD.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LCD_waveshare/LCD/%.o Drivers/LCD_waveshare/LCD/%.su Drivers/LCD_waveshare/LCD/%.cyclo: ../Drivers/LCD_waveshare/LCD/%.c Drivers/LCD_waveshare/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/LCD_waveshare/LCD" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/LCD_waveshare/Fonts" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/Gizmo/Src" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/Gizmo/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LCD_waveshare-2f-LCD

clean-Drivers-2f-LCD_waveshare-2f-LCD:
	-$(RM) ./Drivers/LCD_waveshare/LCD/DEV_Config.cyclo ./Drivers/LCD_waveshare/LCD/DEV_Config.d ./Drivers/LCD_waveshare/LCD/DEV_Config.o ./Drivers/LCD_waveshare/LCD/DEV_Config.su ./Drivers/LCD_waveshare/LCD/LCD.cyclo ./Drivers/LCD_waveshare/LCD/LCD.d ./Drivers/LCD_waveshare/LCD/LCD.o ./Drivers/LCD_waveshare/LCD/LCD.su

.PHONY: clean-Drivers-2f-LCD_waveshare-2f-LCD

