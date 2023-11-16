/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "drv_bmp280.h"
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//HAL_StatusTypeDef HAL_OK;


#define RX_BUFFER_SIZE  50
char rxData[RX_BUFFER_SIZE];
char receivedChar;
int32_t coef_temperature;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//uint8_t temp_calib[3];
//uint8_t press_calib[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == UART4){

		BMP280_S32_t temperature_COMP;
		BMP280_S32_t rawTemperature;
		BMP280_S32_t rawPressure;
		BMP280_S32_t pressure_COMP;

		// Ajouter le caractère reçu au tampon
		static uint8_t rxIndex = 0;
		rxData[rxIndex++] = receivedChar;

		if (rxIndex >= 5) {

			if (strncmp(rxData, "GET_T", 5) == 0) {
				//printf("caractere reçu : %c", rxData);
				rawTemperature = BMP280_get_temperature();
				temperature_COMP = BMP280_compensate_temp(rawTemperature);
				HAL_UART_Transmit(&huart4, temperature_COMP, sizeof(temperature_COMP), HAL_MAX_DELAY);
			}

			else if (strncmp(rxData, "GET_P", 5) == 0) {
				rawPressure = BMP280_get_pressure();
				pressure_COMP = BMP280_compensate_press(rawPressure);
				HAL_UART_Transmit(&huart4, pressure_COMP, sizeof(pressure_COMP), HAL_MAX_DELAY);

			}
			else if (strncmp(rxData, "SET_K=1234", 10) == 0){

			}
			else if (strncmp(rxData, "GET_K", 5) == 0){

				HAL_UART_Transmit(&huart4, coef_temperature, sizeof(coef_temperature), HAL_MAX_DELAY);

			}

			else if (strncmp(rxData, "GET_A", 5) == 0) {


			}
			// Réinitialiser l'index pour la prochaine réception
			rxIndex = 0;
		}

		// Continuez à attendre le prochain caractère
		HAL_UART_Receive_IT(&huart4, &receivedChar, 1);
	}
}
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
	//int32_t 			  coef_temperature;
	CAN_TxHeaderTypeDef   pHeader;
	uint8_t               aData[8];
	uint32_t              TxMailbox;

	BMP280_S32_t compensated_temperature;
	BMP280_S32_t temp;
	BMP280_S32_t press;
	BMP280_S32_t compensated_pressure;

	int temp_max = 35;
	int angle_max = 180;

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_CAN1_Init();
	MX_I2C1_Init();
	MX_UART4_Init();
	/* USER CODE BEGIN 2 */

	BMP280_check();
	BMP280_init();

	//HAL_CAN_Init(&hcan1);

	HAL_CAN_Start (&hcan1);

	pHeader.IDE = CAN_ID_STD;
	pHeader.StdId = 0x61;
	pHeader.RTR = CAN_RTR_DATA;
	pHeader.DLC = 2;
	pHeader.TransmitGlobalTime = DISABLE;


	aData[0] = 0x01;
	aData[1] = 0x01;


	//HAL_CAN_AddTxMessage (&hcan1, &pHeader, aData, &TxMailbox);


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		press = BMP280_get_pressure();
		temp = BMP280_get_temperature();

		compensated_temperature = BMP280_compensate_temp(temp);
		coef_temperature = compensated_temperature/100;
		printf("Compensated Temperature: %ld\r\n", compensated_temperature/100);


		coef_temperature = (coef_temperature - 23) * ((angle_max)/(temp_max-23));
		printf("Coeff Temperature: %ld\r\n", coef_temperature);

		aData[0] = coef_temperature;


		HAL_CAN_AddTxMessage (&hcan1, &pHeader, aData, &TxMailbox);


		//printf("temperature = %ld",coef_temperature);
		//aData[0] = aData[0] + compensated_temperature/100;

		compensated_pressure = BMP280_compensate_press(press);
		printf("Compensated Pressure: %ld\r\n", compensated_pressure/256);

		HAL_Delay(500);

		/*
		if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
		{
		   Error_Handler ();
		}
		 */
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
