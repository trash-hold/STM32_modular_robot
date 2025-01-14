################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LCD_waveshare/Fonts/font12.c \
../Drivers/LCD_waveshare/Fonts/font16.c \
../Drivers/LCD_waveshare/Fonts/font20.c \
../Drivers/LCD_waveshare/Fonts/font24.c \
../Drivers/LCD_waveshare/Fonts/font8.c 

OBJS += \
./Drivers/LCD_waveshare/Fonts/font12.o \
./Drivers/LCD_waveshare/Fonts/font16.o \
./Drivers/LCD_waveshare/Fonts/font20.o \
./Drivers/LCD_waveshare/Fonts/font24.o \
./Drivers/LCD_waveshare/Fonts/font8.o 

C_DEPS += \
./Drivers/LCD_waveshare/Fonts/font12.d \
./Drivers/LCD_waveshare/Fonts/font16.d \
./Drivers/LCD_waveshare/Fonts/font20.d \
./Drivers/LCD_waveshare/Fonts/font24.d \
./Drivers/LCD_waveshare/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LCD_waveshare/Fonts/%.o Drivers/LCD_waveshare/Fonts/%.su Drivers/LCD_waveshare/Fonts/%.cyclo: ../Drivers/LCD_waveshare/Fonts/%.c Drivers/LCD_waveshare/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/LCD_waveshare/LCD" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/LCD_waveshare/Fonts" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/Gizmo/Src" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/Gizmo/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LCD_waveshare-2f-Fonts

clean-Drivers-2f-LCD_waveshare-2f-Fonts:
	-$(RM) ./Drivers/LCD_waveshare/Fonts/font12.cyclo ./Drivers/LCD_waveshare/Fonts/font12.d ./Drivers/LCD_waveshare/Fonts/font12.o ./Drivers/LCD_waveshare/Fonts/font12.su ./Drivers/LCD_waveshare/Fonts/font16.cyclo ./Drivers/LCD_waveshare/Fonts/font16.d ./Drivers/LCD_waveshare/Fonts/font16.o ./Drivers/LCD_waveshare/Fonts/font16.su ./Drivers/LCD_waveshare/Fonts/font20.cyclo ./Drivers/LCD_waveshare/Fonts/font20.d ./Drivers/LCD_waveshare/Fonts/font20.o ./Drivers/LCD_waveshare/Fonts/font20.su ./Drivers/LCD_waveshare/Fonts/font24.cyclo ./Drivers/LCD_waveshare/Fonts/font24.d ./Drivers/LCD_waveshare/Fonts/font24.o ./Drivers/LCD_waveshare/Fonts/font24.su ./Drivers/LCD_waveshare/Fonts/font8.cyclo ./Drivers/LCD_waveshare/Fonts/font8.d ./Drivers/LCD_waveshare/Fonts/font8.o ./Drivers/LCD_waveshare/Fonts/font8.su

.PHONY: clean-Drivers-2f-LCD_waveshare-2f-Fonts

