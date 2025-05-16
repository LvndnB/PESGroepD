
#ifndef procflow_lock

#define procflow_lock
void uart_init_configuration(UART_HandleTypeDef *huart);

void handle_charactor_match_interupt(DMA_HandleTypeDef *hdma_usartx_rx, UART_HandleTypeDef *huart);
void procflow_handle_pdu(int pdu_index, UART_HandleTypeDef *bus_uart, UART_HandleTypeDef *usb_uart);

typedef enum {
	klok = 'k',
	co2 = 'c',
	temp = 't'
} sensors_and_actuator_enum;

#endif
