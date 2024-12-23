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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "ADXL345.h"
#include "error_codes.h"
#include "trig.h"
#include "ST3020_servo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADXL345_ALT_ADR 0xA6 // val shifted left
#define PWR_CTR_REG 0x2D

// REGISTERS
#define ACC_REG 41

// COMMANDS
#define WRITE_INST 0x03
#define READ_INST 0x02

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
uint8_t servo_tx_buff[13];
uint8_t servo_rx_buff[10];

void TempServoSetPos(uint8_t id, uint16_t pos, uint8_t* buff)
{
	HAL_StatusTypeDef status;
	// Declare operating speed
	uint16_t speed = 3400;

	// Fist sending two bytes of 0xFF
	*buff = 0xFF;
	*(buff + 1) = 0xFF;

	// Sending transmission details
	*(buff + 2) = id;
	*(buff + 3) = 0x0A; 		// number of messages
	*(buff + 4) = WRITE_INST;	// code of servo instruction
	*(buff + 5) = ACC_REG;		// servo memory address
	status = HAL_UART_Transmit(&huart4, buff, 6, HAL_MAX_DELAY);

	uint8_t checksum = id + 0x0A + WRITE_INST + ACC_REG;

	// Acceleration data
	*(buff + 0) = (uint8_t) 50;
	// Position data
	*(buff + 1) = (uint8_t) (pos & 0x00FF);	// lower byte of position
	*(buff + 2) = (uint8_t) ((pos & 0xFF00) >> 8);	// higher byte of position
	// Time data
	*(buff + 3) = 0x00;	// lower  byte of time
	*(buff + 4) = 0x00;	// higher  byte of time
	// Speed data
	*(buff + 5) = (uint8_t) (speed & 0x00FF);	// lower byte of speed
	*(buff + 6) = (uint8_t) ((speed & 0xFF00) >> 8);	// higher byte of speed

	for(int i = 0; i < 7; i++)
	{
		uint8_t data = *(buff + i);
		checksum += data;
	}

	// Sending checksum
	*(buff + 7) = ~checksum;
	status = HAL_UART_Transmit(&huart4, buff, 8, HAL_MAX_DELAY);
}

void TempServoRead(uint8_t id, uint8_t memory_register, uint8_t len)
{
	HAL_StatusTypeDef status;
	uint8_t checksum = 0;

	// Preparing receive request
	// Fist sending two bytes of 0xFF
	servo_tx_buff[0] = 0xFF;
	servo_tx_buff[1] = 0xFF;

	// Sending transmission details
	servo_tx_buff[2] = id;
	servo_tx_buff[3] = len + 2; 		// number of messages
	servo_tx_buff[4] = READ_INST;	// code of servo instruction
	servo_tx_buff[5] = memory_register;		// servo memory address
	status = HAL_UART_Transmit(&huart4, servo_tx_buff, 6, HAL_MAX_DELAY);

	// Adding params to checksum
	for(uint8_t i = 2; i<6; i++)
		checksum += servo_tx_buff[i];

	// Ending transmission with zeros
	servo_tx_buff[0] = len;
	checksum += len;
	servo_tx_buff[1] = ~checksum;
	status = HAL_UART_Transmit(&huart4, servo_tx_buff, 2, HAL_MAX_DELAY);

	HAL_HalfDuplex_EnableReceiver(&huart4);
	// Waiting for data
	status = HAL_UART_Receive(&huart4, servo_rx_buff, 8, 500);

}
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
  MX_I2C1_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

  Servo_AddControler(0x00, &huart4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ReturnCode status = ServoPing(0x00, 0x01);
	  status = ServoSetPos(0x00, 0x00, 3400, 50);
	  HAL_Delay(1000);
	  status = ServoSetPos(0x00, 0x7FF, 3400, 10);
	  HAL_Delay(1000);
	  status = ServoSetPos(0x00, 0x3FF, 3400, 10);
	  HAL_Delay(1000);
	  status = ServoSetPos(0x00, 0x7FF, 3400, 10);
	  HAL_Delay(1000);
	  /*
	  HAL_HalfDuplex_EnableTransmitter(&huart4);
 	  ServoSetPos(0x01, 0, servo_tx_buff);
	  HAL_Delay(2000);
	  ServoSetPos(0x01, 0x3FF, servo_tx_buff);
	  HAL_Delay(2000);
	  ServoSetPos(0x01, 0x7FE, servo_tx_buff);
	  HAL_Delay(2000);
	  ServoSetPos(0x01, 0x3FF, servo_tx_buff);
	  HAL_Delay(2000);
	  */


	  //ServoRead(0x01, 0x38, 2);
	  //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       HAL_Delay(2000);

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
