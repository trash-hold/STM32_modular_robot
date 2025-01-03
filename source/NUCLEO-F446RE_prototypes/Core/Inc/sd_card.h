#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "rtc.h"
#include "error_codes.h"

ReturnCode InitLogging(RTC_HandleTypeDef *handler);
ReturnCode StopLogging();
ReturnCode SD_LogError(ReturnCode error);

void SD_AlarmRoutine();

#endif /* INC_SD_CARD_H_ */
