#ifndef GIZMO_INC_SYSTEM_LOGIC_H_
#define GIZMO_INC_SYSTEM_LOGIC_H_

#include "error_codes.h"
#include <stdint.h>
#define MAX_ERROR_COUNT 5
#define CMD_LIST_SIZE 3

#define ID_POS_BYTE 1
#define CMD_POS_BYTE 2
#define SERVO_LINE_BYTE 3

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
}UART_COMMANDS;

typedef enum PERIPHERAL_STATE{
	PER_IDLE = 0x00,
	PER_DONE = 0x01,
	PER_WAITING = 0x02,
	PER_ERROR = 0x03,
	PER_FAIL = 0x04
}PERIPHERAL_STATE;

typedef struct peri_state{
	uint8_t cmd;
	PERIPHERAL_STATE state;
	ReturnCode last_code;
	uint8_t error_count;
}peripheral_state;

typedef enum UART_OP_STATUS{
	UART_OK = 0x00,
	UART_ERROR = 0x01,
	UART_FIFO_FULL = 0x02
}UART_OP_STATUS;

typedef struct servo_struct{
	uint16_t *tx_buffer;
	uint8_t servo_line;
	peripheral_state *state;
}servo;

typedef struct acc_struct{
	float *angles;
	uint8_t id;
	uint8_t cs_pin;
	uint8_t cs_port;
	peripheral_state *state;
}accelometer;


extern uint8_t module_id;
extern peripheral_state *servo0, *servo1, *acc0, *acc1;

extern uint16_t servo0_tx_buff[3];
extern uint16_t servo1_tx_buff[3];


UART_OP_STATUS UART_Decode(uint8_t* msg_buffer);
void PeripheralUpdateState(peripheral_state* per, ReturnCode status);

#endif /* GIZMO_INC_SYSTEM_LOGIC_H_ */
