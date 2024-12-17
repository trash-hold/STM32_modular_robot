/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
//#include <stdarg.h> //for va_list var arg functions
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t counter;
// FATFS variables
FATFS fatfs;	// File system
FIL file;		// File object
char path[4];

// Read write operations
char file_name[] = "test.txt";
char sent_bytes[4];
uint32_t read_bytes, write_bytes;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_SDIO_SD_Init();
  /* USER CODE BEGIN 2 */

  //HAL_Delay(2000); //a short delay is important to let the SD card settle

  // Mount SD
  if( f_mount(&fatfs, path, 0) == FR_OK)
  {
	  // Try to open the file
	  FRESULT res = f_open(&file, file_name,  FA_OPEN_EXISTING | FA_READ);


	  if (res == FR_OK)
	  {
		  // File exists
		  if ( f_read(&file, &counter, sizeof(counter), (void*) &read_bytes) == FR_OK)
		  {
			  // Read was successful
			  /*
			  uint32_t temp;

			  // Decode BCD ASCII
			  temp += (counter & 0xFF) - 0x30;
			  temp += (((counter & 0xFF00) >> 8) - 0x30)*10;
			  temp += (((counter & 0xFF0000) >> 16) - 0x30)*100;
			  temp += (((counter & 0xFF000000) >> 24) - 0x30)*1000;

			  counter = temp;
			  */
		  }

		  f_close(&file);
	  }
	  else
	  {
		  // File doesnt exist - create
		  if( f_open(&file, file_name, FA_CREATE_NEW | FA_WRITE) == FR_OK)
		  {
			  // File sucessfully created
			  if (f_write(&file, &counter, sizeof(counter), (void*) &write_bytes) == FR_OK)
			  {
				  // Write data into file
			  }

			  f_close(&file);
		  }
		  else
		  {
			  // Couldnt create file
			  Error_Handler();
		  }
	  }
  }
  else
  {
	  // Mount unsuccessful
	  Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	// Increment counter
	HAL_Delay(1000);
	counter++;

	if (counter > 9999)
		counter = 0;
	// Write to file	if( f_open(&file, file_name, FA_WRITE) == FR_OK)

	/*
	uint32_t rest = counter;
	uint32_t temp = counter;

	temp = rest / 1000;
	rest -= temp*1000;
	sent_bytes[3] = temp + 0x30;

	temp = rest / 100;
	rest -= temp*100;
	sent_bytes[2] = temp + 0x30;

	temp = rest / 10;
	rest -= temp*10;
	sent_bytes[1] = temp + 0x30;

	sent_bytes[0] = rest +  0x30;
	*/
	if( f_open(&file, file_name, FA_OPEN_EXISTING | FA_WRITE) == FR_OK)
	{
		//FRESULT res = f_write(&file, sent_bytes, sizeof(sent_bytes), (void *)&write_bytes);
		FRESULT res = f_write(&file, &counter, sizeof(counter), (void *)&write_bytes);
		f_close(&file);
	}

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
