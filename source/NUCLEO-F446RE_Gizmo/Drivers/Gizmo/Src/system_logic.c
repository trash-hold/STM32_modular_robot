#include "system_logic.h"

static TRANS_STATUS Local_AddCMD(peripheral_state* per, COM_COMMAND new_cmd)
{
	// If there is something wrong with peripheral ignore cmd
	if( (per->state) == PER_FAIL)
		return TRANS_ERROR;

	if( per->cmd != COM_IDLE)
		return TRANS_ERROR;

	per->cmd = new_cmd;
	per->state = PER_WORKING;

	return TRANS_OK;
}

static TRANS_STATUS CAN_AddCMD(COM_COMMAND new_cmd, uint8_t id)
{
	// Add details about transmission
	can0_struct->receiver_id = id;
	can0_struct->weight = CAN_WRITE;
	*(can0_struct->tx_buffer) = new_cmd;

	// Add command to CAN
	return Local_AddCMD(can0_struct->state, new_cmd);
}

static TRANS_STATUS DecodeOpCode(uint8_t* msg, uint8_t cmd, uint8_t bytes, uint8_t received_id)
{
	// If ID different than this module it needs to be transmitted further into the net via CAN
	uint8_t write_can = 0x00;
	if( received_id != module_id)
	{
		if (module_role == MODULE_TARGET)
			return TRANS_ERROR;

		write_can = 0x01;
		if (CAN_AddCMD(cmd, received_id) != TRANS_OK)
			return TRANS_ERROR;
	}

	switch( cmd )
	{
		case COM_SERVO_POS_SET:
		{
			// Check if there is enough data
			if(bytes != 7)
				return TRANS_ERROR;

			uint8_t servo_line = *(msg);

			// Add command to CAN or local perihperal
			if(write_can == 0x00)
			{
				// Servo line id
				if( servo_line == 0x00)
				{
					if ( Local_AddCMD(servo0, COM_SERVO_POS_SET) != TRANS_OK )
						return TRANS_ERROR;
				}
				else if ( servo_line == 0x01 )
				{
					if ( Local_AddCMD(servo1, COM_SERVO_POS_SET) != TRANS_OK )
						return TRANS_ERROR;
				}
				else
					return TRANS_ERROR;

				// Get transmit data
				uint16_t *buffer = servo_line == 0x00 ? servo0_tx_buff: servo1_tx_buff;
				for(uint8_t i = 0; i < 3; i++)
				{
					*(buffer + i) =  ((uint16_t) *(msg + i*2 + 1) << 8) | ((uint16_t) *(msg + i*2 + 2) );
				}
			}
			else
			{
				*(can0_struct->tx_buffer + 1) = servo_line;
				// Get data to transmit through CAN
				for(uint8_t i = 0; i < 6; i++)
				{
					*(can0_struct->tx_buffer + i + 2) =  *(msg + i + 1);
				}
			}


			return TRANS_OK;
		}

		case COM_SERVO_POS_READ:
		case COM_SERVO_PING:
		case COM_SERVO_READ_TEMP:
		{
			// Check if there is enough data
			if(bytes != 1)
				return TRANS_ERROR;

			uint8_t servo_line = *(msg);

			if (write_can == 0x00)
			{
				// Servo line id
				if( servo_line == 0x00)
				{
					if ( Local_AddCMD(servo0, cmd) != TRANS_OK )
						return TRANS_ERROR;
				}
				else if ( servo_line == 0x01 )
				{
					if ( Local_AddCMD(servo1, cmd) != TRANS_OK )
						return TRANS_ERROR;
				}
			}
			else
			{
				*(can0_struct->tx_buffer + 1) = servo_line;
			}

			return TRANS_OK;
		}

		case COM_ACC_STATUS:
		case COM_ACC_ANGLES_READ:
		{
			// Check if there is enough data
			if(bytes != 1)
				return TRANS_ERROR;

			// Acc line id
			uint8_t acc_line = *(msg);
			if (write_can == 0x00)
			{
				if( acc_line == 0x00)
				{
					if ( Local_AddCMD(acc0, cmd) != TRANS_OK )
						return TRANS_ERROR;
				}
				else if ( acc_line == 0x01 )
				{
					if ( Local_AddCMD(acc1, cmd) != TRANS_OK )
						return TRANS_ERROR;
				}
			}
			else
			{
				*(can0_struct->tx_buffer + 1) = acc_line;
			}

			return TRANS_OK;

		}

		default:
			return TRANS_ERROR;
	}
}

ReturnCode UART_Decode(uint8_t* msg_buffer)
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
		return G_COM_RECEIVE;

	uint8_t received_id = *(msg_buffer + ID_POS_BYTE);
	uint8_t received_cmd = *(msg_buffer + CMD_POS_BYTE);

	if (DecodeOpCode(msg_buffer + PER_LINE_BYTE, received_cmd, data_bytes - 3, received_id) != TRANS_OK)
		return C_COM_DECODE;

	return G_SUCCESS;
}

ReturnCode CAN_Decode(uint8_t *rx_buffer, uint8_t bytes)
{
	if (DecodeOpCode(rx_buffer + 1, *(rx_buffer), bytes, module_id) != TRANS_OK)
		return C_COM_DECODE;

	return G_SUCCESS;
}

void PeripheralUpdateState(peripheral_state* per, ReturnCode status)
{
	if( status != G_SUCCESS)
	{
		if ( per->error_count >= MAX_ERROR_COUNT)
		{
			per->state = PER_FAIL;
		}
		else
		{
			per->error_count += 1;
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

