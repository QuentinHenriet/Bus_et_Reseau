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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define I2C_Address 0x77<<1
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
char message[50];

uint8_t temp1[1];
uint8_t temp2[1];
uint8_t temp3[1];

uint8_t pression1[1];
uint8_t pression2[1];
uint8_t pression3[1];

uint8_t buffer_transmit[1]={0xD0};
uint8_t buffer_receive[1];

uint8_t mode_normal[2] = {0xF4,0x57}; //

uint8_t recup_temp2[1]={0xFA};
uint8_t recup_temp1[1]={0xFB};
uint8_t recup_temp3[1]={0xFC};

uint8_t recup_pression1[1]={0xF8};
uint8_t recup_pression2[1]={0xF7};
uint8_t recup_pression3[1]={0xF9};
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	//char message[50];
	HAL_StatusTypeDef test;
	HAL_StatusTypeDef test_pression;

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
	/* USER CODE BEGIN 2 */
	test = HAL_I2C_Master_Transmit(&hi2c1, I2C_Address, buffer_transmit, sizeof(buffer_transmit), HAL_MAX_DELAY);

	HAL_I2C_Master_Receive( &hi2c1,  I2C_Address,  buffer_receive,  sizeof(buffer_receive), HAL_MAX_DELAY);

	while(test != HAL_OK){

		printf("Erreur de communication \r\n");
		HAL_Delay(1000);

	}

	printf("BMP280 connecte \r\n");
	printf("ID : %x\r\n",buffer_receive[0]);

	HAL_I2C_Master_Transmit(&hi2c1, I2C_Address, mode_normal, 2, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, I2C_Address, buffer_receive, 1, HAL_MAX_DELAY);

	printf("le mode est : %x\r\n",buffer_receive[0]);
	printf("mode normal, Pressure oversampling x16, Temperature oversampling x2\r\n");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		test_pression = HAL_I2C_Master_Transmit(&hi2c1, I2C_Address, recup_temp1, 1, HAL_MAX_DELAY);

		HAL_I2C_Master_Receive(&hi2c1, I2C_Address, temp1,1,HAL_MAX_DELAY);

		HAL_I2C_Master_Transmit(&hi2c1, I2C_Address, recup_temp2, 1, HAL_MAX_DELAY);

		HAL_I2C_Master_Receive(&hi2c1, I2C_Address, recup_pression1,1,HAL_MAX_DELAY);

		HAL_I2C_Master_Transmit(&hi2c1, I2C_Address, recup_temp2, 1, HAL_MAX_DELAY);

		HAL_I2C_Master_Receive(&hi2c1, I2C_Address, recup_pression2,1,HAL_MAX_DELAY);

		while(test_pression != HAL_OK)
		{
			printf("Aucune temperature\r\n");
			HAL_Delay(1000);
		}

		HAL_Delay(100);
		printf("La temperature est : %x %x %x\r\n",temp1,temp2, temp3);

		printf("La pression est : %x\r\n",pression1, pression2, pression3);
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
