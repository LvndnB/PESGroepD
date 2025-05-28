# Stappenplan uart installeren. 

In dit document kan je instructies vinden om procflow (working title) op een stm32 te installeren op een met STM32CubeIDE. 

Om dat op de meeste STM’s uart 1 stabielste en snelste is. Wordt er in dit  

 

# Stap 1 zet uart uart ioc. 

stel uart1 aan met de gewenste pinnen. 

De parameters moeten de volgende zijn: 
- parity off 
- Wordlenght 8 
- Baudrate moet hetzelfde zijn van de pi. 
- Zet uart1 global interrupt aan. 
- Zet uart1 dma rx aan.

 

## instructies dma 

Ga naar: ioc->communicatie->uart1  

Onderaan kan de dma vinden.  

Klik op add. 
Selecteer rx.  
Dat was het. 

---

Sla de ioc op. En genereer code. 

 
Stap 2: Kopieer de laatste versie van procflow.c en procflow.h naar je project. 

 

Stap 3: Installeer de interrupt handdeler in stm32l4xx_it.c  

 
```c
/* USER CODE BEGIN USART1_IRQn 0 */ 
// this handles the character match interrupt which is used addressing and start of msg 

if (huart1.Instance->ISR & USART_ISR_CMF_Msk) { // on character match interrupt  

    handle_charactor_match_interupt(&hdma_usart1_rx, &huart1); 

    huart1.Instance->ICR |= USART_ICR_CMCF; // reset character match interrupt flag 

   return; 

}
/* USER CODE END USART1_IRQn 0 */ 
```

# Stap 4: Installeer de variabelen in main.c en verander de device id. 

Zie main.c `/* USER CODE BEGIN PV */`


# Stap 5: Maak een aantal variabelen van main.c zichtbaar door main.h 

Zie main.h `/* USER CODE BEGIN ET */`
 
# Stap 6: Installeer init code. 

Roep bij de rest van de init functies bij `/* USER CODE BEGIN 2 */` de functie `uart_init_configuration(&huart1)`

Dit zet de address van de uart bus. En tx lijn uit.

# Stap 7 verander main loop. 

 

### Installeer de starter van de uart. 

 
```c
if (hdma_usart1_rx.State == HAL_DMA_STATE_READY) { 

    HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, uart_buff_size); 

} 
```
 

 
### Installeer de logica dat bepaalt wanneer ie een bericht verwerkt: 

 
```c
while (uart_current_pdu != uart_pdu_wrinting_point) { 

    procflow_handle_pdu(uart_current_pdu, &huart1, &huart2); 
 

    uart_current_pdu++; 

    if (uart_current_pdu == 126) { 

        uart_current_pdu = 0; 

    } 

} 
```
 
