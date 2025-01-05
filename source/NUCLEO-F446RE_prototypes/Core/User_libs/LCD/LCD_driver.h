#ifndef USER_LIBS_LCD_LCD_DRIVER_H_
#define USER_LIBS_LCD_LCD_DRIVER_H_
#include "LCD.h"
#include "DEV_config.h"
#include "error_codes.h"

#define PURPLE 0x60CF
#define DARK_RED 0xC186
#define DARK_GREEN 0x2E69
#define DARK_GREY 0x4A69

typedef enum PERIPHERAL{
	SERVO_0 = 0x00,
	SERVO_1 = 0x01,
	ACC_0 = 0x02,
	ACC_1 = 0x03,
	SD = 0x04,
	CAN = 0x05
}PERIPHERAL;

typedef enum SCREEN{
	INIT_SCREEN = 0x01,
	INFO_SCREEN = 0x02
}SCREEN;

void Screen_Init();
void Screen_DrawInitScreen();

ReturnCode Screen_UpdateData(PERIPHERAL per, float* data, uint8_t length);

#endif /* USER_LIBS_LCD_LCD_DRIVER_H_ */
