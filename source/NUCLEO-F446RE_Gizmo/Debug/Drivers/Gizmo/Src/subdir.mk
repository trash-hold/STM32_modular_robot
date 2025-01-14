################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Gizmo/Src/ADXL345.c \
../Drivers/Gizmo/Src/LCD_driver.c \
../Drivers/Gizmo/Src/ST3020_servo.c \
../Drivers/Gizmo/Src/sd_card.c \
../Drivers/Gizmo/Src/system_logic.c \
../Drivers/Gizmo/Src/trig.c 

OBJS += \
./Drivers/Gizmo/Src/ADXL345.o \
./Drivers/Gizmo/Src/LCD_driver.o \
./Drivers/Gizmo/Src/ST3020_servo.o \
./Drivers/Gizmo/Src/sd_card.o \
./Drivers/Gizmo/Src/system_logic.o \
./Drivers/Gizmo/Src/trig.o 

C_DEPS += \
./Drivers/Gizmo/Src/ADXL345.d \
./Drivers/Gizmo/Src/LCD_driver.d \
./Drivers/Gizmo/Src/ST3020_servo.d \
./Drivers/Gizmo/Src/sd_card.d \
./Drivers/Gizmo/Src/system_logic.d \
./Drivers/Gizmo/Src/trig.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Gizmo/Src/%.o Drivers/Gizmo/Src/%.su Drivers/Gizmo/Src/%.cyclo: ../Drivers/Gizmo/Src/%.c Drivers/Gizmo/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/LCD_waveshare/LCD" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/LCD_waveshare/Fonts" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/Gizmo/Src" -I"F:/Coding/Gizmo_firmware/source/NUCLEO-F446RE_Gizmo/Drivers/Gizmo/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Gizmo-2f-Src

clean-Drivers-2f-Gizmo-2f-Src:
	-$(RM) ./Drivers/Gizmo/Src/ADXL345.cyclo ./Drivers/Gizmo/Src/ADXL345.d ./Drivers/Gizmo/Src/ADXL345.o ./Drivers/Gizmo/Src/ADXL345.su ./Drivers/Gizmo/Src/LCD_driver.cyclo ./Drivers/Gizmo/Src/LCD_driver.d ./Drivers/Gizmo/Src/LCD_driver.o ./Drivers/Gizmo/Src/LCD_driver.su ./Drivers/Gizmo/Src/ST3020_servo.cyclo ./Drivers/Gizmo/Src/ST3020_servo.d ./Drivers/Gizmo/Src/ST3020_servo.o ./Drivers/Gizmo/Src/ST3020_servo.su ./Drivers/Gizmo/Src/sd_card.cyclo ./Drivers/Gizmo/Src/sd_card.d ./Drivers/Gizmo/Src/sd_card.o ./Drivers/Gizmo/Src/sd_card.su ./Drivers/Gizmo/Src/system_logic.cyclo ./Drivers/Gizmo/Src/system_logic.d ./Drivers/Gizmo/Src/system_logic.o ./Drivers/Gizmo/Src/system_logic.su ./Drivers/Gizmo/Src/trig.cyclo ./Drivers/Gizmo/Src/trig.d ./Drivers/Gizmo/Src/trig.o ./Drivers/Gizmo/Src/trig.su

.PHONY: clean-Drivers-2f-Gizmo-2f-Src

