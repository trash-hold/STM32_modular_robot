/*
 * ADXL345.h
 *
 *  Created on: Dec 17, 2024
 *      Author: Barabash
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "stm32f4xx_hal.h"
#include "error_codes.h"

#define ACC_I2C_TIMEOUT 100
#define ACC_ALT_ADDRESS 0xA6
//#define ACC_READ_OPERATION 0xA7
//#define ACC_WRITE_OPERATION 0xA6

#define ACC_X0_REG 0x32

extern I2C_HandleTypeDef *acc_i2c;

void AccAdd_I2CHandler(I2C_HandleTypeDef* handler);
ReturnCode AccRead(uint8_t* buffer, uint8_t address, uint8_t bytes_number);
ReturnCode AccRawMeasurment(uint16_t* xyz_buffer);

#endif /* INC_ADXL345_H_ */
