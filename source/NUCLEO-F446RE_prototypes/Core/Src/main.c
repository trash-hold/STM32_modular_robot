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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "DEV_config.h"
//#include "LCD.h"
#include "LCD_driver.h"
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
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */

  Screen_Init();
  Sreen_DrawInitScreen();
  /*
  	LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;//SCAN_DIR_DFT = D2U_L2R
	LCD_Init( Lcd_ScanDir );
	LCD_Clear(BLACK);

	LCD_DisplayString(3, 10, "Gizmo Robot", &Font16, BLACK, WHITE);

	int px_number = (sizeof(white_px_LUT) / sizeof(uint16_t)) / 2;
	for(int i = 0; i < px_number; i++)
	{
		LCD_DrawPoint(white_px_LUT[i*2], white_px_LUT[i*2 + 1], WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	}

	px_number = (sizeof(l_grey_px_LUT) / sizeof(uint16_t)) / 2;
	for(int i = 0; i < px_number; i++)
	{
		LCD_DrawPoint(l_grey_px_LUT[i*2], l_grey_px_LUT[i*2 + 1], GRAY, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	}

	px_number = (sizeof(d_grey_px_LUT) / sizeof(uint16_t)) / 2;
	for(int i = 0; i < px_number; i++)
	{
		LCD_DrawPoint(d_grey_px_LUT[i*2], d_grey_px_LUT[i*2 + 1], 0x4a69, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	}

	LCD_DisplayString(39, 125, "Servo init", &Font8, BLACK, GREEN);
	LCD_DisplayString(36, 135, "Acc init", &Font12, BLACK, WHITE);
	LCD_DisplayString(44, 150, "SD init", &Font8, BLACK, GRAY);

	*/
	/*---------------------------------------------------------------------------------
					//LCD basic skill
	---------------------------------------------------------------------------------*/
	//LCD_Show();
	/*
	uint32_t PURPLE = 0x60cf;
	uint32_t DARK_RED = 0xc186;
	uint32_t DARK_GREEN = 0x2e69;
	LCD_Clear(BLACK);
	LCD_DisplayString(5, 5, "Information panel", &Font12, BLACK, WHITE);
	LCD_DrawLine(0, 25, 128, 25, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);
	LCD_DrawLine(0, 30, 128, 30, PURPLE, LINE_SOLID, DOT_PIXEL_2X2);

	// Servo panel
	LCD_DisplayString(20, 40, "Servo angles [deg]", &Font8, BLACK, WHITE);

	LCD_DisplayString(5, 55, "Ref:  ", &Font8, BLACK, WHITE);
	LCD_DisplayString(5, 65, "Meas: ", &Font8, BLACK, WHITE);

	LCD_DisplayNum(35, 55, 21037, &Font8, BLACK, WHITE);
	LCD_DisplayNum(35, 65, 21037, &Font8, BLACK, WHITE);
	LCD_DrawLine(64, 55, 64, 75, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	LCD_DisplayString(69, 55, "Ref:  ", &Font8, BLACK, WHITE);
	LCD_DisplayString(69, 65, "Meas: ", &Font8, BLACK, WHITE);

	LCD_DisplayNum(99, 55, 21037, &Font8, BLACK, WHITE);
	LCD_DisplayNum(99, 65, 21037, &Font8, BLACK, WHITE);

	// Acc panel
	LCD_DrawLine(0, 80, 128, 80, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);
	LCD_DisplayString(5, 85, "Module orientation [deg]", &Font8, BLACK, WHITE);
	LCD_DisplayString(5, 100,  "A: ", &Font8, BLACK, WHITE);
	LCD_DisplayString(5, 110, "B: ", &Font8, BLACK, WHITE);
	LCD_DisplayString(5, 120, "C: ", &Font8, BLACK, WHITE);

	LCD_DisplayNum(25, 100, 21037, &Font8, BLACK, WHITE);
	LCD_DisplayNum(25, 110, 21037, &Font8, BLACK, WHITE);
	LCD_DisplayNum(25, 120, 21037, &Font8, BLACK, WHITE);

	LCD_DrawLine(64, 100, 64, 130, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	LCD_DisplayString(69, 100, "A: ", &Font8, BLACK, WHITE);
	LCD_DisplayString(69, 110, "B: ", &Font8, BLACK, WHITE);
	LCD_DisplayString(69, 120, "C: ", &Font8, BLACK, WHITE);

	LCD_DisplayNum(89, 100, 21037, &Font8, BLACK, WHITE);
	LCD_DisplayNum(89, 110, 21037, &Font8, BLACK, WHITE);
	LCD_DisplayNum(89, 120, 21037, &Font8, BLACK, WHITE);

	LCD_DrawLine(0, 140, 128, 140, PURPLE, LINE_SOLID, DOT_PIXEL_1X1);
	LCD_DrawLine(0, 143, 128, 143, PURPLE, LINE_DOTTED, DOT_PIXEL_1X1);

	// Status panel
	LCD_DisplayString(5, 150, "S1", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(25, 150, "S2", &Font8, BLACK, DARK_RED);
	LCD_DisplayString(45, 150, "A1", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(65, 150, "A2", &Font8, BLACK, DARK_RED);
	LCD_DisplayString(85, 150, "SD", &Font8, BLACK, DARK_GREEN);
	LCD_DisplayString(105, 150, "CAN", &Font8, BLACK, DARK_GREEN);
	*/





	Driver_Delay_ms(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
