/**
 ******************************************************************************
 * @file           : procflow.c
 * @brief          : methods for a makeshift protocol
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 Aron Dingemanse.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "main.h"
#include "procflow.h"

void uart_init_configuration(UART_HandleTypeDef *huart) {
	// c binary operations cheat sheet
	// ~ bitwise NOT
	// & bitwise AND
	// ^ bitwise XOR
	// | bitwise OR


	huart->Instance->CR1 &= ~USART_CR1_RE; // shut down read
	huart->Instance->CR1 &= ~USART_CR1_UE; // shut everything down


	huart->Instance->CR2 &= ~USART_CR2_ADD_Msk; // set addr to 0
	huart->Instance->CR2 |= device_id << USART_CR2_ADD_Pos; // set addr

	huart->Instance->CR1 |= USART_CR1_CMIE;   // enable Character match interrupt (based on CR2_ADD)

	huart->Instance->CR1 |= USART_CR1_RE; // enable read
	huart->Instance->CR1 &= ~USART_CR1_TE_Msk; // disable sending
	huart->Instance->CR1 |= USART_CR1_UE; // power on
}

/**
 * this is called in stm32l4xx_it.c USART1_IRQHandler
 *
 * and
 */
void handle_charactor_match_interupt(DMA_HandleTypeDef *hdma_usartx_rx, UART_HandleTypeDef *huart) {
	uint32_t dma_writingpoint  =  uart_buff_size - hdma_usartx_rx->Instance->CNDTR;

	// Find the address location
	//
	// because there is a slight timing difference when DMA catches a new char and when this is run. We need to check an range
	//
	uint8_t *id_location = 0;
	for (int i = -1; i < 3; i++) {                                  // negative 1 is so it also gonna look for a future byte in case that this interrupt is faster than when CNDTR
		if (uart_rx_buffer[dma_writingpoint - i] == device_id) {
			id_location = &uart_rx_buffer[dma_writingpoint - i];
			break;
		}
	}
	uart_pdu_ptr[uart_pdu_wrinting_point] = id_location;

	if (id_location == 0) {
		return;
	}

	// pdu example
	// +---------+-----------+--------+-----+-----+
	// | method  | Device ID | DATA   : ... | null|
	// +---------+-----------+--------+-----+-----+

	// attach transmitter early so that the pi does not sees values in noise.
	if ( *(id_location - 1) == 'r') {
		huart->Instance->CR1 |= USART_CR1_TE; // attach transmitter
		uart_fast_pdu = id_location;
		// TODO: force context switch so it does not wait 50ms per device. Or 150 ms total because it is 3 devices.
	}

	uart_pdu_wrinting_point++;
	if (uart_pdu_wrinting_point == 128 /*max*/ - 1 /* corrects the index because its starts at 0*/) {
		uart_pdu_wrinting_point = 0;
	}
}
uint64_t co2value = 0;

void procflow_handle_pdu(int pdu_index, UART_HandleTypeDef *bus_uart, UART_HandleTypeDef *usb_uart) {

	// pdu example
	// +---------+-----------+--------+-----+-----+
	// | method  | Device ID | DATA   : ... | null|
	// +---------+-----------+--------+-----+-----+

	uint8_t *Device_id = uart_pdu_ptr[pdu_index];
	uint8_t method = Device_id[-1];
	uint8_t *begin_data = Device_id+1;

	switch (method) {

	case 's': // Rpi has (S)end data to stm
		HAL_Delay(40);
		char _[99]  = "";
		int lcount = 0;
		int val = 0;
		if (strlen(begin_data)!= 0) {
			sscanf(begin_data, "%20[^:]:%d:%d", _, &lcount, &val);
			snprintf(_, sizeof(_), "Got lcount: %d val: %d\r\n", lcount, val);
			HAL_UART_Transmit(usb_uart, _, strlen(_), 5000);
		}
		HAL_UART_Transmit(&usb_uart, "\r\n", 2, 5000);
		break;

	case 'r': // rpi (R)equest data from stm

		GPIO_PinState st = 0;

		for (int i = 0; i < 4; i++) {
			st = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
			if (st == GPIO_PIN_SET) {
				break;
			}
			HAL_Delay(1);
		}
		//HAL_Delay(12);

		uint8_t ascii_start = 2; // (ascii start of text. But i'm using it as start of transmission)
		HAL_UART_Transmit(bus_uart, &ascii_start, 1, 1000);

		if (st == GPIO_PIN_SET) {
			uint8_t msg[200] = "GPIO:111111111111";
			HAL_UART_Transmit(bus_uart, msg, strlen(msg)+1, 1000);
			HAL_Delay(10);
		} else {
			uint8_t msg[200] = "HOI dit is aron";
			HAL_UART_Transmit(bus_uart, msg, strlen(msg)+1, 1000);
			HAL_Delay(10);

		}

		bus_uart->Instance->CR1 &= ~USART_CR1_TE_Msk; // disable transever
		break;

	case 'h': // request already (H)andled
		break;

	default: // idk pdu mallformed?
		break;
	}

}

void procflow_register_string(sensors_and_actuator_enum dev, char *val) {
}

void procflow_register_u64(sensors_and_actuator_enum dev, uint64_t val) {

	co2value = val;
}
