//
// Created by Lenor on 2021/8/2.
//

#ifndef F407TEMP_USART_REFACTOR_H
#define F407TEMP_USART_REFACTOR_H

///HEY! I'M HERE
#define REFACTOR_UART_IRQ
#define REFACTOR_PRINTF_SCANF
#define UART_IO_PORT huart1

#include "GLOBAL_INCLUDE.h"

#ifdef REFACTOR_UART_IRQ
#define UART_RX_BUF_SIZE 1024
//extern uint8_t UART_RX_BUF[UART_RX_BUF_SIZE];
//extern volatile uint16_t UART_RX_STA;

void REFACTORED_UART_DMA_INIT();

///HEY! I'M HERE
#define UART_IQR_HANDLER USART1_IRQHandler
#define UART_RxCpltCallBack RF_USART1_RxCpltCallBack
void UART_RxCpltCallBack(void);
#endif

#ifdef REFACTOR_PRINTF_SCANF
#define PSRFED
#include <stdio.h>
#endif


#endif //F407TEMP_USART_REFACTOR_H
