#ifndef GIZMO_INC_SYSTEM_LOGIC_H_
#define GIZMO_INC_SYSTEM_LOGIC_H_

#include "error_codes.h"
#include <stdint.h>

#define MODULE_CONTROLLER 0x01
#define MODULE_TARGET 0x00

#define MAX_ERROR_COUNT 5
#define CMD_LIST_SIZE 3

#define ID_POS_BYTE 1
#define CMD_POS_BYTE 2
#define PER_LINE_BYTE 3

#define SERVO_MAX_ANGLE 568

#define DMA_RX_SIZE 10
#define DMA_TX_SIZE 10


typedef enum COM_COMMANDS{
	COM_IDLE = 0x00,
	COM_SERVO_POS_READ = 0x01,
	COM_SERVO_POS_SET = 0x02,
	COM_SERVO_READ_TEMP = 0x03,
	COM_SERVO_PING = 0x04,
	COM_ACC_ANGLES_READ = 0x05,
	COM_ACC_STATUS = 0x06
}COM_COMMAND;

typedef enum PERIPHERAL_STATES{
	PER_IDLE = 0x00,
	PER_DONE = 0x01,
	PER_WORKING = 0x02,
	PER_WAITING = 0x03,
	PER_ERROR = 0x04,
	PER_FAIL = 0x05
}PERIPHERAL_STATE;

typedef enum CAN_WEIGHTS{
	CAN_CRITICAL = 0x00,
	CAN_URGENT = 0x01,
	CAN_NORMAL = 0x02,
	CAN_CONTINUE_WRITE = 0x03,
	CAN_WRITE = 0x05,
	CAN_CONTINUE_READ = 0x06,
	CAN_READ = 0x07
}CAN_WEIGHT;

typedef struct peri_states{
	uint8_t cmd;
	PERIPHERAL_STATE state;
	ReturnCode last_code;
	uint8_t error_count;
}peripheral_state;

typedef enum TRANSMISSION_STATUS{
	TRANS_OK = 0x00,
	TRANS_ERROR = 0x01,
}TRANS_STATUS;

typedef struct servo_struct{
	uint16_t *tx_buffer;
	uint8_t servo_line;
	int16_t last_write_pos;
	int16_t last_read_pos;
	peripheral_state *state;
}servo;

typedef struct acc_struct{
	float *angles;
	int16_t *raw_measurement;
	uint8_t acc_line;
	peripheral_state *state;
}accelometer;

typedef struct can_struct{
	CAN_WEIGHT weight;
	uint8_t receiver_id;
	uint8_t finished_trans;
	uint8_t* tx_buffer;
	uint8_t* rx_buffer;
	peripheral_state *state;
}can;


extern uint8_t module_id;
extern uint8_t module_role;
extern peripheral_state *servo0, *servo1, *acc0, *acc1, *can0;
extern can* can0_struct;

extern uint16_t servo0_tx_buff[3];
extern uint16_t servo1_tx_buff[3];


ReturnCode UART_Decode(uint8_t* msg_buffer);
ReturnCode CAN_Decode(uint8_t *rx_buffer, uint8_t bytes);
void PeripheralUpdateState(peripheral_state* per, ReturnCode status);

#endif /* GIZMO_INC_SYSTEM_LOGIC_H_ */
