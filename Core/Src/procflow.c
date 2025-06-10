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
#include "stm32_tm1637.h"
/***
 * This functions writes the prefered setting to the correct registers. And it is preferdly called in
 * stm32l4xx_hal_msp.c. But for refactoring purposes it is currently called in the main function. (So i do not forget)
 */
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
 * Use this when it is likely that buff is not filled
 *
int procflow_check_if_buff_size_is_at_least_been_recieved(char *buff, int length) {
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

}*/

/**
 * this is called in stm32l4xx_it.c USART1_IRQHandler
 *
 * It stores the found address location in the next item of uart_pdu_ptr array. 
 * And its also handels some quick actions that is required.
 */
void handle_charactor_match_interupt(DMA_HandleTypeDef *hdma_usartx_rx, UART_HandleTypeDef *huart) {
	uint32_t dma_writingpoint  =  uart_buff_size - hdma_usartx_rx->Instance->CNDTR;

	/* Find the location of the address byte */

	// because there is a slight timing difference when DMA catches a new char and when this
    // is run. We need to check an range of locations including a location that has been reported that has not been received. (i = -1)
	uint8_t *id_location = 0;
	for (int i = -1; i < 3; i++) {                                  
		if (uart_rx_buffer[dma_writingpoint - i] == device_id) {
			id_location = &uart_rx_buffer[dma_writingpoint - i];
			break;
		}
	}
	uart_pdu_ptr[uart_pdu_wrinting_point] = id_location;

	if (id_location == 0) {
		return;
	}

    /* handeling method's requirements */
    const uint8_t method = *(id_location - 1);

    switch (method) {

        // attach transmitter early so that the pi does not sees values in noise.
        case 'r':
            huart->Instance->CR1 |= USART_CR1_TE; // attach transmitter
            uart_fast_pdu = id_location;
            break;
    }

	uart_pdu_wrinting_point++;
	if (uart_pdu_wrinting_point == 128 /*max*/ - 1 /* corrects the index because its starts at 0*/) {
		uart_pdu_wrinting_point = 0;
	}
}

uint64_t co2value = 0;
float tempvalue = 0;
float humidityValue = 0;

/**
 * It is called in the main while and handles request send from master.
 * We do not handle everything in the interrupt because than it is likely 
 * we break sensors and the DMA.
 *
 */
void procflow_handle_pdu(int pdu_index, UART_HandleTypeDef *bus_uart, UART_HandleTypeDef *usb_uart) {

	// pdu example
	// +---------+-----------+--------+-----+-----+
	// | method  | Device ID | DATA   : ... | null|
	// +---------+-----------+--------+-----+-----+

	uint8_t *Device_id = uart_pdu_ptr[pdu_index];
	uint8_t method = Device_id[-1];
	uint8_t *begin_data = Device_id+1;

	uint8_t response_buffer[100];

	switch (method) {

	case 's': // Rpi has (S)end data to stm
		HAL_Delay(40);
		char key[99]  = "";
		char val[99] = "";
		if (strlen(begin_data)!= 0) { // WARNING: when copying. Make sure that the nullbyte is also send.
			sscanf(begin_data, "%20[^=]=%s", key, val);  // parsing van key=value // op de pi moet:	bus.sendDataToDevice(device, "key=value", strlen(key=value));

			if (strcmp(key, "tim") == 0) {

				if (val[0] == ':') { // display int without colum (:).
					tm1637SetBrightness(7);
					tm1637DisplayDecimal(atoi(val+1), 0);

				} else if (val[0] == '|') { // display int colum (:)
					tm1637SetBrightness(7);
					tm1637DisplayDecimal(atoi(val+1), 2);

				} else if (val[0] == 'f') { // display float
					double value = atof(val+1);
					int valueAsInt = value * 100; // move decemal
					tm1637DisplayDecimal(valueAsInt, 2); // display with colum as dot
				}

			}

			// MOGELIJKE DEBUGGING HIERONDER TOT BREAK
				// snprintf(_, sizeof(key), "Got lcount: %d val: %d\r\n", lcount, val);
				// HAL_UART_Transmit(usb_uart, key, strlen(key), 5000);
		}
		// HAL_UART_Transmit(&usb_uart, "\r\n", 2, 5000);
		break;

	case 'r': // rpi (R)equest data from stm
		if (strlen(begin_data) != 0) {

			switch (*begin_data) { // switch case voor de juiste key voor request
				case 't':	// request temperatuur, kan veranderd worden met een andere key die op de pi procflow.h staat

					//char[key] = "temp"; // key die op de pi wordt gebruikt om te controleren of de waarde van temperatuur is
					//  <- hier moet de waarde als return van een functie in de main.c (bijv. value = getTemp();)

					snprintf(response_buffer, 100, "temp=%f", tempvalue); // hier wordt de key=value string gemaakt die naar de pi wordt gestuurd
					procflow_send(bus_uart, response_buffer, strlen(response_buffer));
					break;

				case 'c': // request co2 value


					snprintf(response_buffer, 100, "co2=%d", tempvalue); // hier wordt de key=value string gemaakt die naar de pi wordt gestuurd
					procflow_send(bus_uart, response_buffer, strlen(response_buffer));
					break;
				}

				case 'v':
					snprintf(response_buffer, 100, "lucht=%f", tempvalue); // hier wordt de key=value string gemaakt die naar de pi wordt gestuurd
					procflow_send(bus_uart, response_buffer, strlen(response_buffer));
					break;

				bus_uart->Instance->CR1 &= ~USART_CR1_TE_Msk; // disable transmitter
				break;

			default: // idk pdu malformed?
				break;
			}
		}

		

}

/**
 * NOT IMPLEMENTED. TODO: implement this
 */
void procflow_register_float(sensors_and_actuator_enum dev, float val) {
	switch (dev) {
		case temp:
			tempvalue = val;
			break;
		case hum:
			humidityValue = val;

		case co2: // is int
			break;
	}
}

/**
 * This registers unsigned 64 bit values to be send by the `r` method. For sensors (co2)
 * NOT IMPLEMENTED. TODO: implement this
 */
void procflow_register_u64(sensors_and_actuator_enum dev, uint64_t val) {

	co2value = val;
}

int procflow_send(UART_HandleTypeDef *bus_uart, uint8_t *arr, int len) {
	uint8_t startbyte = '!'; // (ascii start of text. But i'm using it as start of transmission)
	HAL_UART_Transmit(bus_uart, &startbyte, 1, 1000);

	HAL_UART_Transmit(bus_uart, arr, len, 1000);

	uint8_t endbyte = 0;
	HAL_UART_Transmit(bus_uart, &endbyte, 1, 1000);
	HAL_Delay(10);

	return 1; // TODO ack
}


