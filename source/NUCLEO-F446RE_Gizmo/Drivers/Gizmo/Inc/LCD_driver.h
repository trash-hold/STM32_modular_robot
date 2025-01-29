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
	CAN = 0x00,
	SERVO_0 = 0x01,
	SERVO_1 = 0x02,
	ACC_0 = 0x03,
	ACC_1 = 0x04,
	SD = 0x05
}PERIPHERAL;

typedef enum SCREEN{
	INIT_SCREEN = 0x01,
	INFO_SCREEN = 0x02
}SCREEN;

void Screen_Init();
void Screen_DrawInitScreen();
void Screen_DrawInfoScreen();

ReturnCode Screen_DrawNextInit(PERIPHERAL per);
ReturnCode Screen_UpdateData(PERIPHERAL per, float* data, uint8_t length);
ReturnCode Screen_UpdateStatus(PERIPHERAL per, uint8_t is_ok);

#endif /* USER_LIBS_LCD_LCD_DRIVER_H_ */
