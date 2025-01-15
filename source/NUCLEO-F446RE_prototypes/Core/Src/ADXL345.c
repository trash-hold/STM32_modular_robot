#include "ADXL345.h"

typedef struct ACC_CONFIG{
	I2C_HandleTypeDef* handle;
	uint32_t pin;
	GPIO_TypeDef *port;
}acc_config;

acc_config acc0;
acc_config acc1;
int16_t read_buff[3];
uint8_t data_buffer[6];

ReturnCode Acc_AddController(I2C_HandleTypeDef* handler, GPIO_TypeDef *port, uint32_t pin, uint8_t line)
{
	switch(line)
	{
		case ACC0_LINE:
			acc0.handle = handler;
			acc0.pin = pin;
			acc0.port = port;
			break;
		case ACC1_LINE:
			acc1.handle = handler;
			acc1.pin = pin;
			acc1.port = port;
			break;
		default:
			return C_ACC_HANDLE;
	}

	return G_SUCCESS;
}

static ReturnCode Acc_Cmd(I2C_HandleTypeDef* handler, uint8_t reg, uint8_t value)
{
	uint8_t tx_buffer[] = {reg, value};
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(handler, ACC_ALT_ADDRESS, tx_buffer, 2, 200);

	if(status != HAL_OK)
		return C_I2C_TRANSMIT;

	return G_SUCCESS;
}

// Perform single or sequential blocking read
static ReturnCode Acc_Read(I2C_HandleTypeDef* handler, uint8_t* buffer, uint8_t address, uint8_t bytes_number)
{
	// HAL doc 524
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(handler, ACC_ALT_ADDRESS, address, 1, buffer, bytes_number, ACC_I2C_TIMEOUT);

	if(status != HAL_OK)
		return C_I2C_RECEIVE;

	return G_SUCCESS;
}

ReturnCode Acc_Config(uint8_t line)
{
	if(line > ACC1_LINE)
		return C_ACC_HANDLE;

	I2C_HandleTypeDef* acc_i2c = (line == ACC0_LINE ? acc0.handle : acc1.handle);

	// Turn power off
	ReturnCode status = Acc_Cmd(acc_i2c, ACC_PWR_CTRL_REG, 0x00);
	if (status != G_SUCCESS)
		return status;

	// Put accelometer to measure with 3200Hz baudrate
	status = Acc_Cmd(acc_i2c, ACC_BW_RATE_REG, 0x0F);
	if (status != G_SUCCESS)
		return status;

	// Change Fifo mode to stream, and change watermark to 0x07 = 11|0|00111
	status = Acc_Cmd(acc_i2c, ACC_FIFO_CTL_REG, 0x87);
	if (status != G_SUCCESS)
		return status;

	// Clear interrupt bits by reading register
	uint8_t temp;
	status = Acc_Read(acc_i2c, &temp, 0x30, 1);
	if (status != G_SUCCESS)
		return status;

	// Enable data_ready intterrupt
	status = Acc_Cmd(acc_i2c, ACC_INT_ENB_REG, 0x80);
	if (status != G_SUCCESS)
		return status;

	// Get acc to measurment mode
	status = Acc_Cmd(acc_i2c, ACC_PWR_CTRL_REG, 0x08);
	if (status != G_SUCCESS)
		return status;

	return G_SUCCESS;
}

// Perform read of XYZ entry
ReturnCode Acc_RawMeasurment(int16_t* xyz_buffer, uint8_t line)
{
	if(line > ACC1_LINE)
		return C_ACC_HANDLE;

	ReturnCode status = Acc_Read( (line == ACC0_LINE ? acc0.handle : acc1.handle), data_buffer, ACC_X0_REG, 6);

	if(status != G_SUCCESS)
		return status;

	// Combine readings
	*(xyz_buffer) = ((data_buffer[1] << 8) | data_buffer[0]);
	*(xyz_buffer + 1) = ((data_buffer[3] << 8) | data_buffer[2]);
	*(xyz_buffer + 2) = ((data_buffer[5] << 8) | data_buffer[4]);

	return G_SUCCESS;
}

ReturnCode Acc_AvgMeasurment(int16_t *xyz_buffer, uint32_t samples, uint8_t line)
{
	if(line > ACC1_LINE)
		return C_ACC_HANDLE;

	acc_config *acc = line == 0x00 ? &acc0 : &acc1;

	int32_t avg_x = 0;
	int32_t avg_y = 0;
	int32_t avg_z = 0;

	// Get sum
	uint32_t start_time;
	for(uint16_t i = 1; i <= samples; i++)
	{
		start_time = HAL_GetTick();
		// Feedback from accelometer - wait to collect data
		while( HAL_GPIO_ReadPin(acc->port, acc->pin) != GPIO_PIN_SET )
		{
			if (HAL_GetTick() - start_time > ACC_I2C_TIMEOUT)
				return G_ACC_READ;
		}
		ReturnCode status = Acc_RawMeasurment(read_buff, line);

		if(status != G_SUCCESS)
			return status;

		avg_x += read_buff[0];
		avg_y += read_buff[1];
		avg_z += read_buff[2];
	}

	// Get average
	*(xyz_buffer) = avg_x / samples;
	*(xyz_buffer + 1) = avg_y / samples;
	*(xyz_buffer + 2) = avg_z / samples;

	return G_SUCCESS;
}


ReturnCode Acc_SelfTest(int16_t* result_buffer, uint8_t line)
{
	if(line > ACC1_LINE)
			return C_ACC_HANDLE;

	I2C_HandleTypeDef* acc_i2c = (line == ACC0_LINE ? acc0.handle : acc1.handle);


	// Turn off power
	ReturnCode status = Acc_Cmd(acc_i2c, ACC_PWR_CTRL_REG, 0x00);
	if(status != G_SUCCESS)
		return status;
	HAL_Delay(5);

	// Change data format to 16g, full res (13bit)
	status = Acc_Cmd(acc_i2c, ACC_DATA_FORMAT_REG, 0x0B);
	if(status != G_SUCCESS)
		return status;

	// Back to measurment mode
	status = Acc_Cmd(acc_i2c, ACC_PWR_CTRL_REG, 0x08);
	if(status != G_SUCCESS)
		return status;

	// Begin self test
	status = Acc_AvgMeasurment(read_buff, 64, line);
	if(status != G_SUCCESS)
		return status;

	*(result_buffer) = (-read_buff[0]);
	*(result_buffer + 1) = (-read_buff[1]);
	*(result_buffer + 2) = (-read_buff[2]);

	// Turn on self-test
	status = Acc_Cmd(acc_i2c, ACC_DATA_FORMAT_REG, 0x8B);
	if(status != G_SUCCESS)
		return status;
	HAL_Delay(5);

	status = Acc_AvgMeasurment(read_buff, 64, line);
	if(status != G_SUCCESS)
		return status;
	*(result_buffer) += read_buff[0];
	*(result_buffer + 1) += read_buff[1];
	*(result_buffer + 2) += read_buff[2];


	// Turn of self-test
	status = Acc_Cmd(acc_i2c, ACC_DATA_FORMAT_REG, 0x01);
	if(status != G_SUCCESS)
		return status;

	// Check results of the test

	// First check if signs are correct
	if((*(result_buffer) < 0) || (*(result_buffer + 1) > 0) || (*(result_buffer + 2) < 0))
		return G_ACC_READ;

	uint16_t x, y, z;

	x = (*(result_buffer) & 0x00FF);
	y = ~(*(result_buffer + 1));	// Taking absolute value of y
	y = (y & 0x00FF) + 0x01;
	z = (*(result_buffer + 2) & 0x00FF);

	// OX condition
	if( (x < 6) || (x > 67) )
		return G_ACC_READ;

	// OY condition
	if( (y < 6) || (y > 67) )
			return G_ACC_READ;
	// OZ condition
	if( (z < 10) || (z > 110) )
			return G_ACC_READ;

	return G_SUCCESS;
}
