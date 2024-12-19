#ifndef INC_ST3020_SERVO_H_
#define INC_ST3020_SERVO_H_

#include "stm32f4xx_hal.h"
#include "error_codes.h"

extern UART_HandleTypeDef* uart_s1;
extern UART_HandleTypeDef* uart_s2;

ReturnCode ServoSetPos(uint8_t servo_line, uint16_t pos, uint16_t speed, uint8_t acc);

#endif /* INC_ST3020_SERVO_H_ */
