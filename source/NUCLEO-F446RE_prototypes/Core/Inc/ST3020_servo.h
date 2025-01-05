#ifndef INC_ST3020_SERVO_H_
#define INC_ST3020_SERVO_H_

#include "stm32f4xx_hal.h"
#include "error_codes.h"

extern UART_HandleTypeDef* uart_s1;
extern UART_HandleTypeDef* uart_s2;

ReturnCode Servo_AddControler(uint8_t servo_line, UART_HandleTypeDef* handler);
ReturnCode ServoSetPos(uint8_t servo_line, uint16_t pos, uint16_t speed, uint8_t acc);
ReturnCode ServoRead(uint8_t servo_line, uint8_t reg, uint8_t* data_buffer, uint8_t bytes);
ReturnCode ServoPing(uint8_t servo_line, uint8_t id);
uint16_t ServoTemp(uint8_t servo_line);

#endif /* INC_ST3020_SERVO_H_ */
