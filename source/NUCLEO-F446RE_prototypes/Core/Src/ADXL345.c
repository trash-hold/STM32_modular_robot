#include "ADXL345.h"

I2C_HandleTypeDef *acc_i2c;
int16_t read_buff[3];
uint8_t data_buffer[6];

void AccAdd_I2CHandler(I2C_HandleTypeDef* handler)
{
	acc_i2c = handler;
}

ReturnCode AccCmd(uint8_t reg, uint8_t value)
{
	uint8_t tx_buffer[] = {reg, value};
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(acc_i2c, ACC_ALT_ADDRESS, tx_buffer, 2, 200);

	if(status != HAL_OK)
		return G_ERROR;

	return G_SUCCESS;
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
ReturnCode AccRawMeasurment(int16_t* xyz_buffer)
{
	ReturnCode status = AccRead(&data_buffer, ACC_X0_REG, 6);

	if(status != G_SUCCESS)
		return G_ERROR;

	// Combine readings
	*(xyz_buffer) = ((data_buffer[1] << 8) | data_buffer[0]);
	*(xyz_buffer + 1) = ((data_buffer[3] << 8) | data_buffer[2]);
	*(xyz_buffer + 2) = ((data_buffer[5] << 8) | data_buffer[4]);

	return G_SUCCESS;
}

ReturnCode AccAvgMeasurment(int16_t *xyz_buffer, uint32_t samples)
{
	int32_t avg_x = 0;
	int32_t avg_y = 0;
	int32_t avg_z = 0;

	// Get sum
	for(uint16_t i = 1; i <= samples; i++)
	{
		ReturnCode status = AccRawMeasurment(read_buff);

		if(status != G_SUCCESS)
			return G_ERROR;

		avg_x += read_buff[0];
		avg_y += read_buff[1];
		avg_z += read_buff[2];

		// Feedback from accelometer - wait to collect data
		HAL_Delay(100);
	}

	// Get average
	*(xyz_buffer) = avg_x / samples;
	*(xyz_buffer + 1) = avg_y / samples;
	*(xyz_buffer + 2) = avg_z / samples;

	return G_SUCCESS;
}


ReturnCode AccSelfTest(int16_t* result_buffer)
{
	ReturnCode status;

	// Turn on power
	status = AccCmd(ACC_PWR_CTRL_REG, 0x00);
	if(status != G_SUCCESS)
		return G_ERROR;
	HAL_Delay(5);

	// Change data format to 16g, full res (13bit)
	status = AccCmd(ACC_DATA_FORMAT_REG, 0x0B);
	if(status != G_SUCCESS)
		return G_ERROR;
	// Back to measurment mode
	status = AccCmd(ACC_PWR_CTRL_REG, 0x08);
	if(status != G_SUCCESS)
		return G_ERROR;

	// Begin self test
	status = AccAvgMeasurment(read_buff, 64);
	if(status != G_SUCCESS)
		return G_ERROR;
	*(result_buffer) = (-read_buff[0]);
	*(result_buffer + 1) = (-read_buff[1]);
	*(result_buffer + 2) = (-read_buff[2]);

	// Turn on self-test
	status = AccCmd(ACC_DATA_FORMAT_REG, 0x8B);
	if(status != G_SUCCESS)
		return G_ERROR;
	HAL_Delay(5);

	status = AccAvgMeasurment(read_buff, 64);
	if(status != G_SUCCESS)
		return G_ERROR;
	*(result_buffer) += read_buff[0];
	*(result_buffer + 1) += read_buff[1];
	*(result_buffer + 2) += read_buff[2];


	// Turn of self-test
	status = AccCmd(ACC_DATA_FORMAT_REG, 0x01);
	if(status != G_SUCCESS)
		return G_ERROR;

	// Check results of the test

	// First check if signs are correct
	if((*(result_buffer) < 0) || (*(result_buffer + 1) > 0) || (*(result_buffer + 2) < 0))
		return G_ERROR;

	uint16_t x, y, z;

	x = (*(result_buffer) & 0x00FF);
	y = ~(*(result_buffer + 1));	// Taking absolute value of y
	y = (y & 0x00FF) + 0x01;
	z = (*(result_buffer + 2) & 0x00FF);

	// OX condition
	if( (x < 6) || (x > 67) )
		return G_ERROR;

	// OY condition
	if( (y < 6) || (y > 67) )
			return G_ERROR;
	// OZ condition
	if( (z < 10) || (z > 110) )
			return G_ERROR;

	return G_SUCCESS;
}

