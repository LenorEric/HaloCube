//
// Created by Lenor on 2021/8/4.
//

#ifndef F407TEMP_ESP8266_WIFI_H
#define F407TEMP_ESP8266_WIFI_H

#define ESP8266_UART_PORT huart2
#define ESP8266_IRQ_HANDLER USART2_IRQHandler
#define ESP_UART_RX_BUF_SIZE 1024
#define ESP_UART_RX_CMD_SIZE 64

typedef enum {
    ESP_OK = 0,
    ESP_ERROR,
    ESP_FAIL
} ESP8266_STATE;

#include "GLOBAL_INCLUDE.h"
#include "USART_REFACTOR.h"
#include <string.h>
#include "time.h"


typedef struct {
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} GL_TM;

extern GL_TM GLOBAL_TIME_INDICATOR;

void ESP8266_WiFi_INIT();

uint64_t getTimeStamp();

void getTime(uint64_t Net_timeStamp);

void ESP8266_UART_RxCpltCallBack(void);

#endif //F407TEMP_ESP8266_WIFI_H
