/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l4xx_hal.h"
#include "procflow.h"
#include "sgp30.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/// SGP30 commands
#define SGP30_ADDRESS  (0x58 << 1)
#define CMD_INIT_AIR_QUALITY  {0x20, 0x03}   // Starten van metingen
#define CMD_MEASURE_AIR_QUALITY {0x20, 0x08} // Metingen uitvoeren
#define CMD_GET_FEATURES  {0x20, 0x2F} // Check of sensor bereikbaar is, info ophalen
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
const uint8_t device_id = 0x22;
uint8_t uart_rx_buffer[uart_buff_size] = {0};   // this line is around 31% of memory
                                                // I think some of the code must also be in memory 

uint8_t **uart_pdu_ptr[128] = {0};
uint8_t *uart_fast_pdu = 0;
int uart_pdu_wrinting_point = 0;
int uart_current_pdu = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * Use this when it is likely that buff is not filled
 */
int bus_check_has_recieved(char *buff, int length) {
	const int len_offset_buff =  ((int) ( (void *) buff - (void *) uart_rx_buffer ) );

	if (length + len_offset_buff  > uart_buff_size) {
		return 2; // overflow... Wrap not jet implemented TODO
	}

	if (len_offset_buff > uart_buff_size - hdma_usart1_rx.Instance->CNDTR) {
		return 0; // an wrap has occurred
	}

	if (length + len_offset_buff < uart_buff_size - hdma_usart1_rx.Instance->CNDTR) {
		return 0; // it fits
	}

}

// Init voor de SGP30 Sensor
void SGP30_Init(void) {
	uint8_t command[] = CMD_INIT_AIR_QUALITY;
	HAL_I2C_Master_Transmit(&hi2c3, SGP30_ADDRESS, command, sizeof(command), 100);
}

// Aflezen waarden CO2 sensor
uint16_t SGP30_ReadCO2(void) {
	uint8_t command[] = CMD_MEASURE_AIR_QUALITY;
	uint8_t buffer[6];  // Data buffer (CO₂eq, TVOC)

	HAL_I2C_Master_Transmit(&hi2c3, SGP30_ADDRESS, command, sizeof(command), 100);
	HAL_Delay(12);  // Sensor heeft 12ms nodig voor meting
	HAL_I2C_Master_Receive(&hi2c3, SGP30_ADDRESS, buffer, sizeof(buffer), 100);

	uint16_t co2_value = (buffer[0] << 8) | buffer[1];  // Omzetten bytes naar enkele 16-bit waarden
	return co2_value;
}

void UART_Print(const char *str) {
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

uint8_t SGP30_CheckConnection(void) {
	uint8_t command[] = CMD_GET_FEATURES;
	uint8_t response[3] = {0};
	HAL_StatusTypeDef status =  HAL_I2C_Master_Transmit(&hi2c3, SGP30_ADDRESS, command, sizeof(command), HAL_MAX_DELAY);
	if ( status != HAL_OK) {
		return 0;  // Sensor reageert niet
	}
	if (HAL_I2C_Master_Receive(&hi2c3, SGP30_ADDRESS, response, sizeof(response), 100) != HAL_OK) {
		return 0;  // Geen respons ontvangen
	}

	return 1;  // Sensor OK
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 *
 * Index:                   <br>
 * HAL init                 <br>
 * Enable all peripherals   <br>
 * Enable user sensors      <br>
 *
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
  MX_I2C3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	// Als sensor niet gevonden, stop

	SGP30_Init();  // Init SGP30 sensor

	while (!SGP30_CheckConnection() && 0) {

		UART_Print("ERROR: CO2 sensor niet gevonden!\r\n");
		HAL_Delay(800);
	}

	// Als sensor gevonden, print en wacht 15 sec voor stabielere meetwaarden
	UART_Print("CO2 sensor gedetecteerd!\r\n");
	SGP30_Init();

	uart_init_configuration(&huart1);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	long int loopcounter = 0;
	int sensor_value = 0;
	while (1)
	{

		if (hdma_usart1_rx.State == HAL_DMA_STATE_READY) {
			HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, uart_buff_size);
		}

		while (uart_current_pdu  != uart_pdu_wrinting_point) {
			procflow_handle_pdu(uart_current_pdu, &huart1, &huart2);

			uart_current_pdu++;
			if (uart_current_pdu == 126) {
				uart_current_pdu = 0;
			}

		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		sensor_value = SGP30_ReadCO2();
		char uart_buffer[50];
		long int len = snprintf(uart_buffer, sizeof(uart_buffer), "%-2d CO2: %d ppm\r\n", loopcounter, sensor_value);
		HAL_UART_Transmit(&huart2, (uint8_t*)uart_buffer, len, HAL_MAX_DELAY);
		procflow_register_u64(co2, sensor_value);
		HAL_Delay(40);
		loopcounter++;
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x10303EC7;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*void parse_pdu() {
    char key[20];
    char value[20];

    if (sscanf((char *)rxDataBuff, "%19[^=]=%19[^\n]", key, value) == 2) {
        int size = strlen(key) + strlen(value) + 2;  // Key=Value + newline
        busRxReadPtr += size;
        if (busRxReadPtr >= rxDataBuff + uart_buff_size) {
            busRxReadPtr = rxDataBuff;
        }

        if (strcmp(key, "ENCODERRPM") == 0) {
            rpm = atof(value) * 100;
        }
    }
}*/


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	//parse_pdu();
	//memset(rxDataBuff, 0, 50);
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
