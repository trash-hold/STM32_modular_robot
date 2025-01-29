#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "stm32f4xx_hal.h"
#include "error_codes.h"

#define ACC0_LINE 0x00
#define ACC1_LINE 0x01

#define ACC_I2C_TIMEOUT 100
#define ACC_ALT_ADDRESS 0xA6

#define ACC_X0_REG 0x32
#define ACC_PWR_CTRL_REG 0x2D
#define ACC_DATA_FORMAT_REG 0x31

extern I2C_HandleTypeDef *acc0_i2c;
extern I2C_HandleTypeDef *acc1_i2c;

ReturnCode Acc_AddController(I2C_HandleTypeDef* handler, uint8_t line);
ReturnCode Acc_RawMeasurment(int16_t* xyz_buffer, uint8_t line);
ReturnCode Acc_AvgMeasurment(int16_t *xyz_buffer, uint32_t samples, uint8_t line);
ReturnCode Acc_SelfTest(int16_t* result_buffer, uint8_t line);

#endif /* INC_ADXL345_H_ */
