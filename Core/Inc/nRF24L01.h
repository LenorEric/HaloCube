//
// Created by Lenor on 2021/8/8.
//

#ifndef HALOCUBE_NRF24L01_H
#define HALOCUBE_NRF24L01_H

#define nRF24L01_UART_PORT huart3
#define nRF24L01_IRQ_HANDLER USART3_IRQHandler
#define nRF_UART_RX_BUF_SIZE 1024
#define nRF_UART_RX_CMD_SIZE 64

typedef enum {
    nRF_OK = 0,
    nRF_ERROR,
    nRF_FAIL
} nRF24L01_STATE;

#include "GLOBAL_INCLUDE.h"
#include "USART_REFACTOR.h"
#include <string.h>

void nRF24L01_INIT();
nRF24L01_STATE nRF24L01_TR_CMD(char *cmd2send, char **receive_cmd);
void nRF24L01_UART_RxCpltCallBack(void);

#endif //HALOCUBE_NRF24L01_H
