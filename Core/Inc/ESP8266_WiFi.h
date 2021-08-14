//
// Created by Lenor on 2021/8/4.
//

#ifndef F407TEMP_ESP8266_WIFI_H
#define F407TEMP_ESP8266_WIFI_H

#define ESP8266_UART_PORT huart2
#define ESP8266_IRQ_HANDLER USART2_IRQHandler
#define ESP_UART_RX_BUF_SIZE 1024
#define ESP_UART_RX_CMD_SIZE 256
#define ESP_UART_RX_CMD_BUFF_SIZE 32

typedef enum {
    ESP_OK = 0,
    ESP_ERROR,
    ESP_FAIL
} ESP8266_STATE;

#include "GLOBAL_INCLUDE.h"
#include "USART_REFACTOR.h"
#include <string.h>
#include "time.h"



void ESP8266_WiFi_INIT();

uint64_t getTimeStamp();

void ESP8266_UART_RxCpltCallBack(void);
ESP8266_STATE ESP8266_TR_CMD(char *cmd2send, char **receive_cmd);


#endif //F407TEMP_ESP8266_WIFI_H
