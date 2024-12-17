/*
 * ADXL345.c
 *
 *  Created on: Dec 17, 2024
 *      Author: Barabash
 */


#include "ADXL345.h"

I2C_HandleTypeDef *acc_i2c;

void AccAdd_I2CHandler(I2C_HandleTypeDef* handler)
{
	acc_i2c = handler;
}

// Perform single or sequential blocking read
ReturnCode AccRead(uint8_t* buffer, uint8_t address, uint8_t bytes_number)
{
	// HAL doc 524
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(acc_i2c, ACC_ALT_ADDRESS, address, 1, buffer, bytes_number, ACC_I2C_TIMEOUT);

	if(status != HAL_OK)
		return G_ERROR;

	return G_SUCCESS;
}

// Perform read of XYZ entry
ReturnCode AccRawMeasurment(uint16_t* xyz_buffer)
{
	uint8_t read_buffer[6];
	ReturnCode status = AccRead(&read_buffer, ACC_X0_REG, 6);

	if(status != G_SUCCESS)
		return G_ERROR;

	// Combine readings
	*(xyz_buffer) = (read_buffer[1] << 8) | read_buffer[0];
	*(xyz_buffer + 1) = (read_buffer[3] << 8) | read_buffer[2];
	*(xyz_buffer + 2) = (read_buffer[5] << 8) | read_buffer[4];

	return G_SUCCESS;
}


