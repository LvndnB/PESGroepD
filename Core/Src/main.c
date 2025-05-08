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
#include "stm32_tm1637.h"
#include "sht3x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define uart_buff_size 800
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
sht3x_handle_t sht3x1;

// basic moving buffer for processing
uint8_t rxDataBuff[1000] = {0};
uint8_t *busRxReadPtr = rxDataBuff;
uint8_t *busRxWritePtr = rxDataBuff;
int WowKanDit = 3;


int loopCounter = 0;

uint8_t rx_data_ready;
uint8_t *rx_data_ready_ptn = 0;

enum {
	idle,
	addressed,
	Speaking,
	Listening,
} ProcmosState = idle;


int rpm;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void parse_data();
bool MX_sht3x_init();
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  tm1637Init();
  if (!MX_sht3x_init()) {

	  tm1637DisplayDecimal(hi2c1.ErrorCode+1000, 0);
	  HAL_Delay(1000);
	  MX_sht3x_init();
  }

  // ps. not all registers should be disabled when uart isn't running.
  // i'm doing it so because it is a nice order. For details of what each registry
  // does please refer to the Reference Manual.

  huart1.Instance->CR1 &= ~USART_CR1_RE; // shut down read
  huart1.Instance->CR1 &= ~USART_CR1_UE; // shut everything down

  // ability for uart to go to mute mode (Mute Mode Enable)
  huart1.Instance->CR1 |= USART_CR1_MME;

  uint8_t address = 0x20;
  huart1.Instance->CR2 &= ~USART_CR2_ADD_Msk; // set addr to 0
  huart1.Instance->CR2 |= address << USART_CR2_ADD_Pos; // set addr

  huart1.Instance->CR1 |= USART_CR1_WAKE; // set wake method to that id is found.
                                          // if set to 0 the uart wakes when line is down.

  huart1.Instance->CR1 |= USART_CR1_RE; //enable read
  huart1.Instance->CR1 &= ~USART_CR1_TE_Msk; // disable sending
  huart1.Instance->CR1 |= USART_CR1_UE; // power on


  huart1.Instance->CR1 |= USART_CR1_CMIE;   // enable Character match interrupt (based on addr)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // loop counters
  uint8_t procindex = 0;
  while (1)
  {
	  // procflow
	  if (huart1.Instance->CR1 & USART_CR1_TE) {
		  for (int i = 0; i < 10; i++) {
			  HAL_UART_Transmit(&huart1, &procindex, 1, 100);
		  }


		  HAL_Delay(70);
		  procindex++;

		  if (procindex > 7) {
			  huart1.Instance->CR1 &= ~USART_CR1_TE; // power off transmitter
			  procindex = 0;
		  }
		  continue;
	  }

	  // make sure that the char match interrupt is on when waiting.
	  if ((huart1.Instance->CR1 & USART_CR1_TE) == 0) {
		  huart1.Instance->CR1 |= USART_CR1_CMIE;
		  //huart1.Lock
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 // Sensor data
	 float temperature, humidity = 0;
	 bool success = sht3x_read_temperature_and_humidity(&sht3x1, &temperature, &humidity);

	 int displayContent;
	 if (success) {
		 displayContent = temperature;
	 } else {
		 displayContent = 1000+hi2c1.ErrorCode;

		 // the following is reporting the error details over usb for debugging case
		 char errorMsg[20];
		 sprintf(errorMsg, "%d",hi2c1.ErrorCode);
		 HAL_UART_Transmit(&huart2, errorMsg, strlen(errorMsg), 5000);
		 HAL_Delay(3);

		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_BERR )) {
			HAL_UART_Transmit(&huart2, " BERR ", 6, 5000 );
		 }    /*!< BERR error            */

		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_AF)) {//ACKF error
		 	HAL_UART_Transmit(&huart2, " ACK ", 5, 5000 );
		 }
		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_ARLO)) {
		 	HAL_UART_Transmit(&huart2, " ARLO ", 6, 5000 );
		 }
		 if (hi2c1.ErrorCode & ( HAL_I2C_ERROR_OVR)) {
			 HAL_UART_Transmit(&huart2, " OVR ", 5, 5000 );
		 }
		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_DMA)) {  /// DMA transfer error
			 HAL_UART_Transmit(&huart2, " DMA tra ", 9, 5000 );
		 }
		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_TIMEOUT )) {
			 HAL_UART_Transmit(&huart2, " timeout ", 9, 5000 );
		 }
		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_SIZE)) { /// Size Management error
			 HAL_UART_Transmit(&huart2, " size ", 6, 5000 );
		 }
		 if (hi2c1.ErrorCode & (HAL_I2C_ERROR_DMA_PARAM)) { // DMA Parameter Error
			 HAL_UART_Transmit(&huart2, " DMA par ", 9, 5000 );
		 }    /*!<    */

		 HAL_UART_Transmit(&huart2, "\r\n", 2, 5000 );

	 } // end of if success

	 // Set data to display
	 tm1637DisplayDecimal(displayContent, 1);

	 HAL_Delay(10);
	 loopCounter++;
	 char buffer[200] = {0};

	 echoFound();
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20202E44;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart1.Init.Parity = UART_PARITY_EVEN;
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
  if (HAL_RS485Ex_Init(&huart2, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
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



void parse_pdu() {
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
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	//
    int is_id_found =  (huart->Instance->ISR & USART_ISR_CMF_Msk) && 1;
	//HAL_UART_Transmit(&huart1, is_id_found?"yes\0":"NO\0\0" , 4, 5000);
	// Always listening :3
	// HAL_UART_Receive_IT(&huart1, rxDataBuff, 50);

	parse_pdu();

	memset(rxDataBuff, 0, 50);
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	HAL_UART_Transmit(&huart1, rxDataBuff, Size, HAL_MAX_DELAY);
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, rxDataBuff, uart_buff_size);

}

void echoFound() {
	uint8_t msg = 20;
	HAL_UART_Transmit(&huart1, &msg, 1, 1000);
	//char msg[8];
	//if (is_id_found) {
	//	char m[] = "yes";
    //		HAL_UART_Transmit(&huart1, m, strlen(m), 5000);
	//	HAL_UART_Transmit(&huart1, m, strlen(m), 5000);
	//	__HAL_UART_GET_FLAG
	//	USART_Fl
	//} else {
	//	char m[] = "Ea quis consequatur fugiat ut. Rerum doloremque est qui in minus doloremque rerum aut. Culpa maxime officiis dolore ad at sed ut.\n\r Nemo fugiat a dolorem. Vel voluptatum quis beatae dolorem aut animi ut. Sit incidunt numquam ea inventore reiciendis. Sed et quia nostrum libero enim laudantium. Voluptatem voluptatem sed magni eum illum exercitationem.\n\r Alias saepe quaerat laborum sit. Ipsa quia molestiae quidem non quidem non at est. Esse neque harum voluptas voluptatibus ut commodi repellat. Doloribus et natus ut. Voluptatem enim sed aliquam et voluptatibus sint doloribus quo. Repellendus laboriosam libero et aut quam at exercitationem error. Qui nulla illo suscipit ut nulla nulla.\n\r\n\r\n\r";
	//	uint8_t a = 0b1011011;
	//	HAL_UART_Transmit(&huart1, &a , 2, 5000);
	//	HAL_UART_Transmit(&huart1, m, strlen(m), HAL_MAX_DELAY);

	//}
}

bool MX_sht3x_init() {
	sht3x1.i2c_handle = &hi2c1;
	sht3x1.device_address = 0x44;
	//sht3x1.device_address = ;

	return sht3x_init(&sht3x1);

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
