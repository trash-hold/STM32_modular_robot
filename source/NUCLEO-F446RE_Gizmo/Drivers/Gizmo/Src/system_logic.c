#include "system_logic.h"

UART_OP_STATUS PeripheralAddCMD(peripheral_state* per, UART_COMMANDS new_cmd)
{
	// If there is something wrong with peripheral ignore cmd
	if( (per->state) == PER_FAIL)
		return UART_ERROR;

	if( per->cmd != COM_IDLE)
		return UART_FIFO_FULL;

	per->cmd = new_cmd;
	per->state = PER_WAITING;

	return UART_OK;
}

UART_OP_STATUS UART_Decode(uint8_t* msg_buffer)
{
	uint8_t data_bytes = *(msg_buffer);
	uint8_t checksum = 0;

	// Check checksum
	for(uint16_t i = 0; i < data_bytes - 1; i++)
	{
		checksum += *(msg_buffer + i);
	}

	checksum = ~checksum;

	if ( checksum != *(msg_buffer + data_bytes - 1) )
		return UART_ERROR;

	// If ID different than this module it needs to be transmitted further into the net via CAN
	if( *(msg_buffer + ID_POS_BYTE) != module_id)
	{
		// CAN routine
		return UART_OK;
	}

	switch( *(msg_buffer + CMD_POS_BYTE))
	{
		case COM_SERVO_POS_READ:
			// Check if there is enough data
			if(data_bytes != 5)
				return UART_ERROR;

			// Servo line id
			if( *(msg_buffer + SERVO_LINE_BYTE) == 0x00)
				return PeripheralAddCMD(servo0, COM_SERVO_POS_READ);

			else if ( *(msg_buffer + SERVO_LINE_BYTE) == 0x01)
				return PeripheralAddCMD(servo0, COM_SERVO_POS_READ);

			return UART_ERROR;

		case COM_SERVO_POS_SET:
			// Check if there is enough data
			if(data_bytes != 10)
				return UART_ERROR;

			uint8_t servo_line = *(msg_buffer + SERVO_LINE_BYTE);
			// Servo line id
			if( servo_line == 0x00)
			{
				if ( PeripheralAddCMD(servo0, COM_SERVO_POS_SET) != UART_OK )
					return UART_ERROR;
			}
			else if ( servo_line == 0x01 )
			{
				if ( PeripheralAddCMD(servo1, COM_SERVO_POS_SET) != UART_OK )
					return UART_ERROR;
			}
			else
				return UART_ERROR;

			uint16_t *buffer = servo_line == 0x00 ? servo0_tx_buff: servo1_tx_buff;

			// Write data into tx buffer
			*(buffer) =  ((uint16_t) *(msg_buffer + SERVO_LINE_BYTE + 1) << 8) | ((uint16_t) *(msg_buffer + SERVO_LINE_BYTE + 2) );
			*(buffer + 1) =  ((uint16_t) *(msg_buffer + SERVO_LINE_BYTE + 3) << 8) | ((uint16_t) *(msg_buffer + SERVO_LINE_BYTE + 4) );
			*(buffer + 2) = (uint16_t) *(msg_buffer + + SERVO_LINE_BYTE + 5);

			return UART_OK;

		default:
			return UART_ERROR;
	}
}

void PeripheralUpdateState(peripheral_state* per, ReturnCode status)
{
	if( status != G_SUCCESS)
	{
		per->error_count += 1;
		if ( per->error_count >= MAX_ERROR_COUNT)
		{
			per->state = PER_FAIL;
		}
		else
		{
			per->state = PER_ERROR;
		}

	}
	else
	{
		per->error_count = 0;
		per->state = PER_DONE;
	}

	per->last_code = status;
}

