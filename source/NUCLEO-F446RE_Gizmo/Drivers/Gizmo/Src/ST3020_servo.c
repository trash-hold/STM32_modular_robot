#include "ST3020_servo.h"

#define SERVO_TIMEOUT 200

#define SERVO_WRITE_INS 0x03
#define SERVO_READ_INS 0x02
#define SERVO_PING_INS 0x01

#define SERVO_ACC_REG 0x29
#define SERVO_TEMP_REG 0x3F
#define SERVO_POS_REG 0x38

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
			return C_UART_HANDLE;
	}

	return G_SUCCESS;
}

static ReturnCode ServoWrite(UART_HandleTypeDef* uart, uint8_t cmd, uint8_t reg, uint8_t *data_buffer, uint8_t len)
{
	// Enable transmission mode
	HAL_StatusTypeDef status = HAL_HalfDuplex_EnableTransmitter(uart);
	if (status != HAL_OK)
	{
		return C_UART_TRANSMIT;
	}

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
	tx_buffer[4] = cmd;		// Instruction code - write = 0x03
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

	status = HAL_UART_Transmit(uart, tx_buffer, 6, SERVO_TIMEOUT);
	if (status != HAL_OK)
		return C_UART_TRANSMIT;

	// Load data to tx_buff
	for (uint8_t i = 0; i < len; i++)
	{
		tx_buffer[i] = *(data_buffer + i);
	}
	tx_buffer[len] = (~checksum);

	status = HAL_UART_Transmit(uart, tx_buffer, len + 1, SERVO_TIMEOUT);
	if( status != HAL_OK)
		return C_UART_TRANSMIT;

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
			return C_UART_HANDLE;
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

	return ServoWrite(uart, SERVO_WRITE_INS, SERVO_ACC_REG, buff, 7);
}

ReturnCode ServoRead(uint8_t servo_line, uint8_t reg, uint8_t* data_buffer, uint8_t bytes)
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
			return C_UART_HANDLE;
	}

	// Send data receive request
	if (ServoWrite(uart, SERVO_READ_INS, reg, &bytes, 1) != G_SUCCESS)
		return G_SERVO_WRTIE;

	// Change mode into Receiver
	HAL_StatusTypeDef status = HAL_HalfDuplex_EnableReceiver(uart);
	if (status != HAL_OK)
		return C_UART_RECEIVE;

	// Servo sends additional header message
	uint8_t header_buffer[5];
	// All transmissions start with 0xFF if read byte is different discard it
	HAL_UART_Receive(uart, header_buffer, 1, SERVO_TIMEOUT);
	if( header_buffer[0] != 0xFF)
		HAL_UART_Receive(uart, header_buffer, 5, SERVO_TIMEOUT);
	else
		HAL_UART_Receive(uart, header_buffer + 1, 4, SERVO_TIMEOUT);

	status = HAL_UART_Receive(uart, data_buffer, bytes, SERVO_TIMEOUT);
	if (status != HAL_OK)
		return C_UART_RECEIVE;

	// Verify ID and transmission length
	if ( (header_buffer[2] != 0x01) || (header_buffer[3] != bytes + 0x02) )
		return C_UART_RECEIVE;

	return G_SUCCESS;
}

ReturnCode ServoPing(uint8_t servo_line, uint8_t id)
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
			return C_UART_HANDLE;
	}

	HAL_StatusTypeDef status = HAL_HalfDuplex_EnableTransmitter(uart);
	if (status != HAL_OK)
	{
		return C_UART_TRANSMIT;
	}

	// Ping buffer
	uint8_t buffer[6];
	uint8_t checksum = ~(id + 0x02 + 0x01);

	// Buffering
	buffer[0] = 0xFF;
	buffer[1] = 0xFF;
	// Sending transmission details
	buffer[2] = id;		// ID
	buffer[3] = 0x02;	// Transmission length
	buffer[4] = 0x01;	// Instruction code - ping = 0x01

	// Send details
	status = HAL_UART_Transmit(uart, buffer, 5, SERVO_TIMEOUT);
	if (status != HAL_OK)
		return C_UART_TRANSMIT;

	// Send checksum
	status = HAL_UART_Transmit(uart, &checksum, 1, SERVO_TIMEOUT);
		if (status != HAL_OK)
			return C_UART_TRANSMIT;

	// Change mode into receiver and get ping response
	status = HAL_HalfDuplex_EnableReceiver(uart);
	if (status != HAL_OK)
		return C_UART_RECEIVE;

	status = HAL_UART_Receive(uart, buffer, 1, SERVO_TIMEOUT);
	if (status != HAL_OK)
		return C_UART_RECEIVE;

	if(buffer[0] != 0xFF)
		status = HAL_UART_Receive(uart, buffer, 6, SERVO_TIMEOUT);
	else
		status = HAL_UART_Receive(uart, buffer + 1, 5, SERVO_TIMEOUT);

	if (status != HAL_OK)
		return C_UART_RECEIVE;

	// First two bytes are buffer so they are skipped

	if ( buffer[2] != id )
		return G_SERVO_READ;

	if ( buffer[3] != 0x02)
		return G_SERVO_READ;

	checksum = buffer[2] + buffer[3] + buffer[4];
	checksum = ~checksum;
	if ( checksum != buffer[5] )
		return G_SERVO_READ;

	return G_SUCCESS;
}


ReturnCode ServoTemp(uint8_t servo_line, uint8_t* temp)
{
	return ServoRead(servo_line, SERVO_TEMP_REG, temp, 1);
}

ReturnCode ServoCurrentPosition(uint8_t servo_line, int16_t* result)
{
	uint8_t pos_bytes[2];
	ReturnCode status = ServoRead(servo_line, SERVO_POS_REG, pos_bytes, 2);

	*(result) = ((( (uint16_t) pos_bytes[1] & 0x7F) << 8) | pos_bytes[0] );

	// Pos is negative
	if ( pos_bytes[1] & 0x80 != 0x00)
		*(result) = -*(result);

	return status;
}
