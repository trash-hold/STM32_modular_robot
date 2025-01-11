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
#include "can.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sd_card.h"
#include "LCD_driver.h"
#include "error_codes.h"
#include "ADXL345.h"
#include "trig.h"
#include "ST3020_servo.h"
#include "system_logic.h"
#include <stdio.h>
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

// Servo Functions
void ServoRoutine(servo* servo);
ReturnCode Screen_ServoUpdate(servo* servo);

// Acc Functions
void AccelometerRoutine(accelometer* acc);
void AccelometerPrepareTransmitData(accelometer* acc, uint8_t* data);

// CAN functions
ReturnCode CAN_Config();
void CAN_ConfigMsg(CAN_TxHeaderTypeDef *header,can* can, uint8_t is_response, COM_COMMAND cmd);
void CANRoutine(can* can);

// COM functions
ReturnCode COM_TransmitResponse(ReturnCode status, uint8_t *data, uint8_t bytes);
ReturnCode CAN_TransmitResponse(ReturnCode status, uint8_t *data, uint8_t bytes, COM_COMMAND cmd);
ReturnCode TransmitResponse(ReturnCode status, uint8_t *data, uint8_t bytes, COM_COMMAND cmd);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t module_id = 0x01;
uint8_t module_role = MODULE_TARGET;

// Uart DMA
uint8_t tx_buffer_DMA[DMA_TX_SIZE];
uint8_t rx_buffer_DMA[DMA_RX_SIZE];
uint8_t header_received = 0x00;
uint8_t header_sent = 0x00;

//Peripherals
servo Servo0_struct;
accelometer Acc0_struct;

can Can_struct;
can* can0_struct;
CAN_TxHeaderTypeDef tx_header;
CAN_RxHeaderTypeDef rx_header;
uint32_t tx_mail;
CAN_FilterTypeDef can0_filter;

peripheral_state Servo0, Servo1, Acc0, Acc1, Can0;
peripheral_state *servo0, *servo1, *acc0, *acc1, *can0;

float acc0_angles[3];
float acc1_angles[3];
uint16_t servo0_tx_buff[3];
uint16_t servo1_tx_buff[3];
int16_t acc0_measurment[3];
int16_t acc1_measurment[3];
uint8_t can_tx_buffer[8];
uint8_t can_rx_buffer[8];

// SD
char msg_buffer[32];
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_SPI3_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_UART4_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  	InitLogging(&hrtc);
  	Screen_Init();
  	Screen_DrawInitScreen();

	servo0 = &Servo0;
	servo1 = &Servo1;
	acc0 = &Acc0;
	acc1 = &Acc1;

	Servo0_struct.tx_buffer = servo0_tx_buff;
	Servo0_struct.servo_line = 0x00;
	Servo0_struct.state = servo0;

	Acc0_struct.angles = acc0_angles;
	Acc0_struct.raw_measurement = acc0_measurment;
	Acc0_struct.acc_line = 0x00;
	Acc0_struct.state = acc0;


	CAN_Config();



	Servo_AddControler(0x00, &huart4);
	ServoSetPos(0x00, 0x00, 3400, 50);

	ReturnCode status = Acc_AddController(&hi2c1, 0x00);
	status = Acc_SelfTest(acc0_measurment, 0x00);

	Screen_DrawInfoScreen();

	HAL_UART_Receive_DMA(&huart2, rx_buffer_DMA, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(module_role == MODULE_CONTROLLER)
		  CANRoutine(can0_struct);
	  ServoRoutine(&Servo0_struct);
	  AccelometerRoutine(&Acc0_struct);


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
ReturnCode TransmitResponse(ReturnCode status, uint8_t *data, uint8_t bytes, COM_COMMAND cmd)
{
	if (module_role == MODULE_CONTROLLER)
		return COM_TransmitResponse(status, data, bytes);
	else
	{
		return CAN_TransmitResponse(status, data, bytes, cmd);
	}
}


ReturnCode COM_TransmitResponse(ReturnCode status, uint8_t *data, uint8_t bytes)
{
	if (module_role == MODULE_TARGET)
		return G_SUCCESS;

	uint8_t checksum = 0;
	uint8_t transmit_size = 4;

	if (bytes - 4 > DMA_TX_SIZE)
	{
		// If transmit too big output error
		transmit_size = 4;
		status = G_COM_OVERFLOW;
	}
	else if( data != NULL)
	{
		for(uint16_t i = 0; i < bytes; i++)
		{
			// Write data to TX buffer
			tx_buffer_DMA[i + 3] = *(data + i);
			transmit_size++;

			// Increment checksum
			checksum += *(data + i);
		}
	}

	checksum += transmit_size + module_id + status;
	checksum = ~checksum;

	// Fill in standard frame
	tx_buffer_DMA[0] = transmit_size;
	tx_buffer_DMA[1] = module_id;
	tx_buffer_DMA[2] = status;
	tx_buffer_DMA[transmit_size - 1] = checksum;

	header_sent = 0x01;
	if ( HAL_UART_Transmit_DMA(&huart2, tx_buffer_DMA, 1) != HAL_OK )
		return G_COM_TRANSMIT;

	return G_SUCCESS;
}

ReturnCode Screen_ServoUpdate(servo* servo)
{
	float last_write = (servo->last_write_pos) * 360 / 4095;
	float last_read = (servo->last_read_pos) * 360 / 4095;
	float data[] = {last_write, last_read};

	return Screen_UpdateData( ((servo->servo_line == 0x00) ? SERVO_0: SERVO_1), data, 2);
}

void ServoRoutine(servo *servo)
{
	peripheral_state *per_state = servo->state;

	switch( per_state->state )
	{
		case PER_IDLE:
			return;

		case PER_ERROR:
		{
			// Log error
			per_state->state = PER_IDLE;
			per_state->cmd = COM_IDLE;
			SD_LogStatus(per_state->last_code);
			return;
		}

		case PER_DONE:
		{
			// Log success
			per_state->state = PER_IDLE;
			per_state->cmd = COM_IDLE;

			return;
		}

		case PER_WORKING:
		{
			if( per_state->cmd == COM_SERVO_POS_SET )
			{
				int16_t pos = *(servo->tx_buffer);

				if (pos > SERVO_MAX_ANGLE)
					pos = SERVO_MAX_ANGLE;
				else if (pos < -SERVO_MAX_ANGLE && pos < 0)
					pos = -SERVO_MAX_ANGLE;

				ReturnCode status = ServoSetPos(servo->servo_line, (uint16_t) pos, servo0_tx_buff[1], servo0_tx_buff[2]);
				PeripheralUpdateState(servo->state, status);

				status = TransmitResponse(status, NULL, 0, COM_SERVO_POS_SET);

				// Update value
				servo->last_write_pos = pos;

				// Log
				snprintf(msg_buffer, sizeof(msg_buffer), "S%02d write: %d\n", (uint8_t) servo->servo_line, pos);
				status =SD_LogMsg(msg_buffer);

				status = Screen_ServoUpdate(servo);
				return;
			}
			else if( per_state->cmd == COM_SERVO_POS_READ)
			{
				uint16_t pos;
				ReturnCode status = ServoCurrentPosition(servo->servo_line, &pos);
				PeripheralUpdateState(servo->state, status);

				if (status == G_SUCCESS)
				{
					// Translate pos into 2 bytes
					uint8_t buff[] = { (( pos & 0xFF00) >> 8), (pos & 0x0FF)};

					// Send response
					status = TransmitResponse(status, buff, 2, COM_SERVO_POS_READ);
				}
				else
				{
					status = TransmitResponse(status, NULL, 0, COM_SERVO_POS_READ);
				}

				// Update
				servo->last_read_pos = pos;

				// Log
				snprintf(msg_buffer, sizeof(msg_buffer), "S%02d read: %d\n", (uint8_t) servo->servo_line, pos);
				status = SD_LogMsg(msg_buffer);

				status = Screen_ServoUpdate(servo);
				return;
			}
			else if ( per_state->cmd == COM_SERVO_PING )
			{
				ReturnCode status = ServoPing(servo->servo_line, 0x01);
				PeripheralUpdateState(servo->state, status);
				status = TransmitResponse(status, NULL, 0, COM_SERVO_PING);

				// Log
				snprintf(msg_buffer, sizeof(msg_buffer), "S%02d status: %d\n", (uint8_t) servo->servo_line, status);
				SD_LogMsg(msg_buffer);

				status = Screen_UpdateStatus(((servo->servo_line == 0x00) ? SERVO_0: SERVO_1), per_state->last_code);
				return;
			}
			else if ( per_state->cmd == COM_SERVO_READ_TEMP )
			{
				uint8_t temp;
				ReturnCode status = ServoTemp(servo->servo_line, &temp);

				PeripheralUpdateState(servo->state, status);
				status = TransmitResponse(status, &temp, 1, COM_SERVO_READ_TEMP);

				// Log
				snprintf(msg_buffer, sizeof(msg_buffer), "S%02d temp: %d\n", (uint8_t) servo->servo_line, temp);
				status = SD_LogMsg(msg_buffer);

				return;
			}
		}
		default:
			return;
	}
	return;
}

void AccelometerPrepareTransmitData(accelometer* acc, uint8_t* data)
{
	for( uint8_t i = 0; i<3; i++)
	{
		float angle = *(acc->angles + i);
		float temp;
		if(angle < 0)
		{
			angle = -angle;
			*(data + 2*i + 1) = ((uint8_t) angle) | (0x01 << 7);
		}
		else
		{
			*(data + 2*i + 1) = ((uint8_t) angle);
		}

		temp = angle * 100 - ((int) angle) * 100;
		*(data + 2*i) = (uint8_t) temp;
	}
}

void AccelometerRoutine(accelometer* acc)
{
	peripheral_state *per_state = acc->state;

	switch( per_state->state )
	{
		case PER_IDLE:
			return;

		case PER_ERROR:
		{
			// Log error
			per_state->state = PER_IDLE;
			per_state->cmd = COM_IDLE;
			SD_LogStatus(per_state->last_code);
			return;
		}

		case PER_DONE:
		{
			// Log success
			per_state->state = PER_IDLE;
			per_state->cmd = COM_IDLE;

			return;
		}

		case PER_WORKING:
		{
			if( per_state->cmd == COM_ACC_ANGLES_READ )
			{

				ReturnCode status = Acc_AvgMeasurment(acc->raw_measurement, 32, acc->acc_line);
				if(status != G_SUCCESS)
				{
					PeripheralUpdateState(per_state, status);
					status = TransmitResponse(status, NULL, 0, COM_ACC_ANGLES_READ);

					return;

				}

				// Calculate angles
				GetTiltAngles(acc->angles, acc->raw_measurement);

				uint8_t data[6];
				AccelometerPrepareTransmitData(acc, data);
				PeripheralUpdateState(per_state, status);

				status = TransmitResponse(status, data, 6, COM_ACC_ANGLES_READ);

				// Log
				snprintf(msg_buffer, sizeof(msg_buffer), "A%02d: %2.2f %2.2f %2.2f\n", acc->acc_line, *(acc->angles), *(acc->angles + 1), *(acc->angles + 2));
				status = SD_LogMsg(msg_buffer);

				status = Screen_UpdateData((acc->acc_line == 0x00 ? ACC_0 : ACC_1) , acc->angles, 3);
				return;
			}
			else if ( per_state->cmd == COM_ACC_STATUS)
			{
				ReturnCode status ;

				status = TransmitResponse(status, NULL, 0, COM_ACC_STATUS);
				PeripheralUpdateState(per_state, G_SUCCESS);

				// Log
				status = SD_LogStatus(per_state->last_code);

				status = Screen_UpdateStatus((acc->acc_line == 0x00 ? ACC_0 : ACC_1) , per_state->last_code);
				return;
			}
		}

		default:
			return;
	}
	return;
}

ReturnCode CAN_Config()
{
	// Init
	can0 = &Can0;
	Can_struct.state = can0;
	Can_struct.tx_buffer = can_tx_buffer;
	Can_struct.rx_buffer = can_rx_buffer;

	can0_struct = &Can_struct;


	// Define filter
	can0_filter.FilterActivation = CAN_FILTER_ENABLE;
	can0_filter.FilterBank = 0x00;
	can0_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	can0_filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can0_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can0_filter.FilterMaskIdHigh = 0x00;
	can0_filter.FilterMaskIdLow = 0x00;
	//can0_filter.FilterIdHigh = 0x00;
	//can0_filter.FilterIdLow = module_id;

	/*
	// Add filter
	if ( HAL_CAN_ConfigFilter(&hcan1, &can0_filter) != HAL_OK)
		return C_CAN_CONFIG;

	// Start CAN
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
		return C_CAN_CONFIG;


	// Add interrupt on msg received
	if( HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
		return C_CAN_CONFIG;
	*/
	return G_SUCCESS;
}



void CAN_ConfigMsg(CAN_TxHeaderTypeDef *header, can* can, uint8_t is_response, COM_COMMAND cmd)
{
	// Get proper identifier
	header->StdId =  ((can->weight) << 8) | (can->receiver_id);

	if (is_response == 0x00)
	{
		// Get data length
		switch(cmd)
		{
			case COM_SERVO_POS_SET:
				header->DLC = 8;
				break;
			default:
				header->DLC = 2;
				break;
		}
	}
	else
	{
		// Get data length
		switch(cmd)
		{
			case COM_SERVO_POS_SET:
				header->DLC = 3;
				break;
			case COM_ACC_ANGLES_READ:
				header->DLC = 7;
				break;
			default:
				header->DLC = 2;
				break;
		}
	}


	header->RTR = CAN_RTR_DATA;
}

ReturnCode CAN_TransmitResponse(ReturnCode status, uint8_t *data, uint8_t bytes, COM_COMMAND cmd)
{
	if (bytes > 7)
		return C_CAN_OVERFLOW;

	can0_struct->weight = CAN_WRITE;
	*(can0_struct->tx_buffer) = cmd;
	CAN_ConfigMsg(&tx_header, can0_struct, 0x01, cmd);

	for(uint8_t i = 0; i < bytes; i++)
	{
		*(can0_struct->tx_buffer + i + 1) = *(data + i);
	}

	if ( HAL_CAN_AddTxMessage(&hcan1, &tx_header, can0_struct->tx_buffer, &tx_mail) != HAL_OK)
		return C_CAN_TXADD;

	return G_SUCCESS;
}

void CANRoutine(can* can)
{
	peripheral_state *per_state = can->state;

	switch( per_state->state )
	{
		case PER_IDLE:
			return;

		case PER_ERROR:
		{
			// Log error
			per_state->state = PER_IDLE;
			per_state->cmd = COM_IDLE;
			SD_LogStatus(per_state->last_code);
			return;
		}
		case PER_DONE:
		{
			// Log success
			per_state->state = PER_IDLE;
			per_state->cmd = COM_IDLE;

			return;
		}
		case PER_WORKING:
		{

			CAN_ConfigMsg(&tx_header, can, 0x00, (per_state->cmd));

			// Add new message
			if ( HAL_CAN_AddTxMessage(&hcan1, &tx_header, can->tx_buffer, &tx_mail) != HAL_OK)
				PeripheralUpdateState(can->state, C_CAN_TXADD);

			// Log
			snprintf(msg_buffer, sizeof(msg_buffer), "CAN transmit: %d\n", can->state->cmd);
			ReturnCode status = SD_LogMsg(msg_buffer);

			can->finished_trans = 0x00;
			per_state->state = PER_WAITING;
			return;
		}
		case PER_WAITING:
		{
			// Wait for response from other module
			if (can->finished_trans == 0x00)
				return;

			// If there was an error return status
			if(per_state->last_code != G_SUCCESS)
				COM_TransmitResponse(per_state->last_code, NULL, 0);

			// If operation was successful sent results
			uint8_t number_bytes = 0;
			switch( *(can->rx_buffer))
			{
				case COM_SERVO_READ_TEMP:
					number_bytes = 1;
					break;
				case COM_SERVO_POS_READ:
					number_bytes = 2;
					break;
				case COM_ACC_ANGLES_READ:
					number_bytes = 6;
					break;
				default:
					number_bytes = 0;
					break;
			}

			if (number_bytes == 0)
				COM_TransmitResponse(per_state->last_code, NULL, 0);
			else
				COM_TransmitResponse(per_state->last_code, (can->rx_buffer + 1), number_bytes);

			// Update state
			PeripheralUpdateState(per_state, G_SUCCESS);

		}
		default:
			return;
	}
	return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if( huart == &huart2)
	{
		// First messege wasn't received yet
		if(header_received == 0x00)
		{
			HAL_UART_Receive_DMA(&huart2, rx_buffer_DMA + 1, rx_buffer_DMA[0] - 1);
			header_received = 0x01;
		}
		else
		{
			// Change state of correct peripheral
			UART_Decode(rx_buffer_DMA);

			HAL_UART_Receive_DMA(&huart2, rx_buffer_DMA, 1);
			header_received = 0x00;
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2)
	{
		if(header_sent == 0x01)
		{
			header_sent = 0x00;
			HAL_UART_Transmit_DMA(&huart2, tx_buffer_DMA + 1, tx_buffer_DMA[0] - 1);
		}
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan )
{
	if( HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, (can0_struct->rx_buffer)) != HAL_OK)
	{
		PeripheralUpdateState(can0, C_CAN_READ);
		return;
	}

	if (module_role == MODULE_CONTROLLER)
	{
		if ( can0->cmd != *(can0_struct->rx_buffer))
		{
			PeripheralUpdateState(can0, C_CAN_READ);
			return;
		}

		PeripheralUpdateState(can0, G_SUCCESS);
		can0_struct->finished_trans = 0x01;
	}
	else
	{
		if( (rx_header.StdId & 0x0F) != module_id)
		{
			PeripheralUpdateState(can0, C_CAN_READ);
			return;
		}

		ReturnCode code = CAN_Decode(can0_struct->rx_buffer, rx_header.DLC - 1);
		if (code != G_SUCCESS)
		{
			// Error handling
			PeripheralUpdateState(can0, code);
			return;
		}

		PeripheralUpdateState(can0, G_SUCCESS);
		can0_struct->finished_trans = 0x01;
	}
}
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
