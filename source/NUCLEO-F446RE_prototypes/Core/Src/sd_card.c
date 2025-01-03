#include "sd_card.h"

// RTC
static RTC_HandleTypeDef* rtc;

// FATFS
static FATFS fatfs;
static FIL file;
static char path[4];
static char file_name[] = "log_00.00.00.txt";
static uint32_t read_bytes, write_bytes;

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
    HAL_StatusTypeDef HAL_status = HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
    if( HAL_status != HAL_OK)
    	return C_RTC_ERROR;

    HAL_status = HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
    if( HAL_status != HAL_OK)
		return C_RTC_ERROR;

    // Create/Open file for logging current day
    snprintf(file_name, sizeof(file_name), "log_%d%d_%d%d_%d%d.txt", ((date.Date & 0xF0) >> 4), (date.Date & 0x0F), ((date.Month & 0xF0) >> 4), (date.Month & 0x0F), ((date.Year & 0xF0) >> 4), (date.Year & 0x0F));

    if ( f_open(&file, file_name,  FA_OPEN_EXISTING | FA_CREATE_NEW) != FR_OK )
		return G_FILE_READ;

    state = SD_READY;
    return G_SUCCESS;
}

ReturnCode StopLogging()
{
	if (state == SD_NOT_INIT)
		return G_ERROR;

	state = SD_NOT_INIT;
}

static ReturnCode SD_WriteTimestamp()
{
	if(state == SD_NOT_INIT)
			return G_ERROR;

	// Get current date and time
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	HAL_StatusTypeDef HAL_status = HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
	if( HAL_status != HAL_OK)
		return C_RTC_ERROR;

	HAL_status = HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
	if( HAL_status != HAL_OK)
		return C_RTC_ERROR;

	if (state == SD_UPDATE)
	{
		// Create/Open file for logging current day
		snprintf(file_name, sizeof(file_name), "log_%d%d_%d%d_%d%d.txt", ((date.Date & 0xF0) >> 4), (date.Date & 0x0F), ((date.Month & 0xF0) >> 4), (date.Month & 0x0F), ((date.Year & 0xF0) >> 4), (date.Year & 0x0F));

		state = SD_READY;
	}

	// Open file
	if ( f_open(&file, file_name,FA_OPEN_APPEND | FA_WRITE) != FR_OK )
		return G_FILE_WRITE;

	// Write timestamp
	f_printf(&file, "%d%d.%d%d.%d%d", ((date.Date & 0xF0) >> 4), (date.Date & 0x0F), ((date.Month & 0xF0) >> 4), (date.Month & 0x0F), ((date.Year & 0xF0) >> 4), (date.Year & 0x0F));
	f_printf(&file, " %d%d:%d%d:%d%d   ", ((time.Hours & 0xF0) >> 4), (time.Hours & 0x0F), ((time.Minutes & 0xF0) >> 4), (time.Minutes & 0x0F), ((time.Seconds & 0xF0) >> 4), (time.Seconds & 0x0F));

	return G_SUCCESS;
}


ReturnCode SD_LogError(ReturnCode error)
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
		default:
			f_puts("Unknown error!\n", &file);
	}

	f_close(&file);
	return G_SUCCESS;
}

void SD_AlarmRoutine()
{
	state = SD_UPDATE;
}

