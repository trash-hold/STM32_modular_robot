#include "sd_card.h"
#include <stdio.h>


// RTC
static RTC_HandleTypeDef* rtc;

// FATFS
static FATFS fatfs;
static FIL file;
static char path[4];
static char file_name[] = "log_00.00.00.txt";

// State machine
typedef enum SD_STATE{
	SD_NOT_INIT = 0x00,
	SD_READY = 0x01,
	SD_UPDATE = 0x02
}SD_STATE;

static SD_STATE state = SD_NOT_INIT;

ReturnCode InitLogging(RTC_HandleTypeDef *handler)
{
	// Add RTC handler
	rtc = handler;

	// Mount SD card
	if (f_mount(&fatfs, path, 0) != FR_OK)
		return C_MOUNT_ERROR;

	// Get current date and time
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_StatusTypeDef HAL_status = HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    if( HAL_status != HAL_OK)
    	return C_RTC_ERROR;

    HAL_status = HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    if( HAL_status != HAL_OK)
		return C_RTC_ERROR;

    // Create/Open file for logging current day
    snprintf(file_name, sizeof(file_name), "log_%02d_%02d_%02d.txt", date.Date, date.Month, date.Year);

    FRESULT res = f_open(&file, file_name, FA_OPEN_EXISTING);
    if ( res != FR_OK )
    {
    	res = f_open(&file, file_name, FA_CREATE_NEW );
    	if ( res != FR_OK )
    		return G_FILE_READ;
    }


    f_close(&file);

    state = SD_READY;
    return G_SUCCESS;
}

ReturnCode StopLogging()
{
	if (state == SD_NOT_INIT)
		return G_ERROR;

	state = SD_NOT_INIT;
	return G_SUCCESS;
}

static ReturnCode SD_WriteTimestamp()
{
	if(state == SD_NOT_INIT)
			return G_ERROR;

	// Get current date and time
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	HAL_StatusTypeDef HAL_status = HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	if( HAL_status != HAL_OK)
		return C_RTC_ERROR;

	HAL_status = HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	if( HAL_status != HAL_OK)
		return C_RTC_ERROR;

	if (state == SD_UPDATE)
	{
		// Create/Open file for logging current day
		snprintf(file_name, sizeof(file_name), "log_%02d_%02d_%02d.txt", date.Date, date.Month, date.Year);

		state = SD_READY;
	}

	// Open file
	if ( f_open(&file, file_name,FA_OPEN_APPEND | FA_WRITE) != FR_OK )
		return G_FILE_WRITE;

	// Write timestamp
	f_printf(&file, "%02d.%02d.%02d", date.Date, date.Month, date.Year);
	f_printf(&file, " %02d:%02d:%02d   ", time.Hours, time.Minutes, time.Seconds);

	return G_SUCCESS;
}


ReturnCode SD_LogStatus(ReturnCode error)
{
	ReturnCode retcode = SD_WriteTimestamp();
	if (retcode != G_SUCCESS)
	{
		if (retcode == G_FILE_WRITE)
			f_close(&file);

		return retcode;
	}

	// Write error message
	switch(error)
	{
		case G_ERROR:
			f_puts("General error!\n", &file);
			break;
		case G_COM_OVERFLOW:
			f_puts("COM: Too much data over!\n", &file);
			break;
		case G_COM_TRANSMIT:
			f_puts("COM: Bad transmit data!\n", &file);
			break;
		case G_COM_RECEIVE:
			f_puts("COM: Error during receive op!\n", &file);
			break;
		case C_RTC_ERROR:
			f_puts("RTC: Can't read data!\n", &file);
			break;
		case C_UART_TRANSMIT:
			f_puts("UART: Can't transmit data!\n", &file);
			break;
		case C_UART_RECEIVE:
			f_puts("UART: Can't read data!\n", &file);
			break;
		case C_UART_HANDLE:
			f_puts("UART: Wrong handle!\n", &file);
			break;
		case G_SERVO_WRTIE:
			f_puts("Servo: Can't write data!\n", &file);
			break;
		case G_SERVO_READ:
			f_puts("Servo: Can't read data!\n", &file);
			break;
		default:
			f_puts("Unknown error!\n", &file);
			break;
	}

	f_close(&file);
	return G_SUCCESS;
}

ReturnCode SD_LogMsg(const char* string)
{
	ReturnCode retcode = SD_WriteTimestamp();
	if (retcode != G_SUCCESS)
	{
		if (retcode == G_FILE_WRITE)
			f_close(&file);

		return retcode;
	}

	for(uint16_t i = 0; *(string + i) != 0; i++)
	{
		f_putc(*(string + i), &file);
	}

	f_close(&file);

	return G_SUCCESS;
}

void SD_AlarmRoutine()
{
	state = SD_UPDATE;
}



