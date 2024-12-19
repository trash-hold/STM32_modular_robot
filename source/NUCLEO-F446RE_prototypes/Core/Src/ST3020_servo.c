#include "ST3020_servo.h"

#define SERVO_TIMEOUT 200

#define SERVO_ACC_REG 0x29

UART_HandleTypeDef* uart_s1;
UART_HandleTypeDef* uart_s2;

uint8_t tx_buffer[8];

ReturnCode Servo_AddControler(uint8_t servo_line, UART_HandleTypeDef* handler)
{
	switch(servo_line)
	{
		case 0x00:
			uart_s1 = handler;
			break;
		case 0x01:
			uart_s2 = handler;
			break;
		default:
			return G_ERROR;
	}

	return G_SUCCESS;
}

ReturnCode ServoWrite(UART_HandleTypeDef* uart, uint8_t reg, uint8_t *data_buffer, uint8_t len)
{
	// Enable transmission mode
	HAL_HalfDuplex_EnableTransmitter(uart);

	// Create helper buffer
	const uint8_t buffer_size = (len > 5) ? len + 1 : 6;
	uint8_t tx_buffer[buffer_size];

	// Stabilize line
	tx_buffer[0] = 0xFF;
	tx_buffer[1] = 0xFF;

	// Transmission details
	//!!!!!!!!!!!!!!!!!!!!!!!!!
	//	TBI changing IDS
	//!!!!!!!!!!!!!!!!!!!!!!!!!
	tx_buffer[2] = 0x01;		// ID
	tx_buffer[3] = len + 0x03;	// Transmission length
	tx_buffer[4] = 0x03;		// Instruction code - write = 0x03
	tx_buffer[5] = reg;			// Servo memory address

	uint8_t checksum = 0;
	// Calculate checksum from tx_buffer
	for (uint8_t i = 0; i < len; i++)
	{
		checksum += *(data_buffer + i);
	}

	// Update checksum - skip buffering 0xFF bytes
	for ( uint8_t i = 2; i < 6; i++)
	{
		checksum += tx_buffer[i];
	}

	HAL_StatusTypeDef status = HAL_UART_Transmit(uart, tx_buffer, 6, SERVO_TIMEOUT);
	if (status != HAL_OK)
		return G_ERROR;

	// Load data to tx_buff
	for (uint8_t i = 0; i < len; i++)
	{
		tx_buffer[i] = *(data_buffer + i);
	}
	tx_buffer[len] = (~checksum);

	status = HAL_UART_Transmit(uart, tx_buffer, len + 1, SERVO_TIMEOUT);
	if( status != HAL_OK)
		return G_ERROR;

	return G_SUCCESS;
}

ReturnCode ServoSetPos(uint8_t servo_line, uint16_t pos, uint16_t speed, uint8_t acc)
{
	UART_HandleTypeDef* uart;
	switch(servo_line)
	{
		case 0x00:
			uart = uart_s1;
			break;
		case 0x01:
			uart = uart_s2;
			break;
		default:
			return G_ERROR;
	}

	uint8_t buff[7];

	// Acceleration data
	buff[0] = acc;
	// Position data
	buff[1] = (uint8_t) (pos & 0x00FF); 			// Lower byte
	buff[2] = (uint8_t) ((pos & 0xFF00) >> 8);		// Higher byte
	// Time data
	buff[3] = 0x00; 	// Lower byte
	buff[4] = 0x00;		// Higher byte
	// Speed data
	buff[5] = (uint8_t) (speed & 0x00FF);			// Lower byte
	buff[6] = (uint8_t) ((speed & 0xFF00) >> 8);	// Higher byte

	return ServoWrite(uart, SERVO_ACC_REG, buff, 7);
}
