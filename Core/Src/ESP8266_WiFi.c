//
// Created by Lenor on 2021/8/4.
//

#include "ESP8266_WiFi.h"

extern UART_HandleTypeDef ESP8266_UART_PORT;

uint8_t ESP_UART_RX_BUF[ESP_UART_RX_BUF_SIZE];
uint16_t ESP_UART_RX_STA;

void ESP8266_UART_RxCpltCallBack(void) {
}

void ESP8266_IRQ_HANDLER(void) {
    if (__HAL_UART_GET_FLAG(&ESP8266_UART_PORT, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
    {
        __HAL_UART_CLEAR_IDLEFLAG(&ESP8266_UART_PORT);  // 清除中断标记
        HAL_UART_DMAStop(&ESP8266_UART_PORT);           // 停止DMA接收
        ESP_UART_RX_STA =
                ESP_UART_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(ESP8266_UART_PORT.hdmarx);  // 总数据量减去未接收到的数据量为已经接收到的数据量
        ESP_UART_RX_BUF[ESP_UART_RX_STA] = 0;  // 添加结束符
        ESP_UART_RX_STA |= 0X8000;         // 标记接收结束
        HAL_UART_Receive_DMA(&ESP8266_UART_PORT, ESP_UART_RX_BUF, ESP_UART_RX_BUF_SIZE);  // 重新启动DMA接收
        ESP8266_UART_RxCpltCallBack();
    }
}

ESP8266_STATE ESP_Receive_CMD(char receive_cmd[][ESP_UART_RX_CMD_SIZE]) {
    uint16_t p = 0, pd = 0;
    uint8_t ESP_CMD_BUF[ESP_UART_RX_CMD_SIZE];
    uint8_t count = 0;
    while (1) {
        while (!(ESP_UART_RX_STA & 0x8000));
        p = 0;
        while (p < (ESP_UART_RX_STA & 0x7fff)) {
            while (1) {
                if (ESP_UART_RX_BUF[p] == 0x0D) {
                    if (ESP_UART_RX_BUF[p + 1] == 0x0A)
                        break;
                    pd = 0;
                } else
                    ESP_CMD_BUF[pd++] = ESP_UART_RX_BUF[p];
                p++;
            }
            p += 2;
            if (!pd) continue;
            ESP_CMD_BUF[pd] = 0;
            pd = 0;
            strcpy(&receive_cmd[count++][0], ESP_CMD_BUF);
            if (!strcmp("OK", ESP_CMD_BUF)) {
                ESP_UART_RX_STA = 0;
                return ESP_OK;
            }
            if (!strcmp("ERROR", ESP_CMD_BUF)) {
                ESP_UART_RX_STA = 0;
                return ESP_ERROR;
            }
            if (!strcmp("FAIL", ESP_CMD_BUF)) {
                ESP_UART_RX_STA = 0;
                return ESP_FAIL;
            }
        }
        ESP_UART_RX_STA = 0;
    }
}

ESP8266_STATE ESP8266_TR_CMD(char *cmd2send, char **receive_cmd) {
    HAL_UART_Transmit(&ESP8266_UART_PORT, cmd2send, strlen(cmd2send), 100);
    uint8_t try = 0;
    ESP8266_STATE state = ESP_Receive_CMD(receive_cmd);
    while (state && ++try) {
        HAL_UART_Transmit(&ESP8266_UART_PORT, cmd2send, strlen(cmd2send), 100);
        state = ESP_Receive_CMD(receive_cmd);
    }
    return state;
}

void ESP8266_WiFi_INIT() {
    char receive_cmd[20][ESP_UART_RX_CMD_SIZE];
    memset(receive_cmd, 0, sizeof(receive_cmd));
    HAL_UART_Receive_DMA(&ESP8266_UART_PORT, ESP_UART_RX_BUF, ESP_UART_RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(&ESP8266_UART_PORT, UART_IT_IDLE);
#ifdef ESP_QUIT_AP_WHEN_INIT
    const char quit_ap[] = "AT+CWQAP\r\n";
    ESP8266_TR_CMD(quit_ap, receive_cmd);
#endif
    const char list_ap[] = "AT+CWJAP?\r\n";
    const char connect_ap[] = "AT+CWJAP=\"Lenoro\",\"ForAzeroth\"\r\n";
    ESP8266_TR_CMD(list_ap, receive_cmd);
    if (!(strcmp("No AP", receive_cmd[0]))) {
        ESP8266_TR_CMD(connect_ap, receive_cmd);
    }
}

uint64_t getTimeStamp() {
    uint64_t Net_timeStamp = 0;
    char receive_cmd[20][ESP_UART_RX_CMD_SIZE];
    const char connect_NTP[] = "AT+CIPSTART=\"UDP\",\"time.windows.com\",123\r\n";
    const char CIPMODE[] = "AT+CIPMODE=0\r\n";
    const char CIPSEND[] = "AT+CIPSEND=48\r\n";
    const char NTP_UDP_DATA[] = {0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0x6d,
                                 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00};
    const char CIPCLOSE[] = "AT+CIPCLOSE\r\n";
    HAL_UART_Transmit(&ESP8266_UART_PORT, CIPCLOSE, sizeof(CIPCLOSE), 100);
    while (1){
        if (ESP8266_TR_CMD(connect_NTP, receive_cmd))
            continue;
        if (ESP8266_TR_CMD(CIPMODE, receive_cmd))
            continue;
        if (ESP8266_TR_CMD(CIPSEND, receive_cmd))
            continue;
        break;
    }
    HAL_UART_Transmit(&ESP8266_UART_PORT, NTP_UDP_DATA, 48, 100);
    while (!(ESP_UART_RX_STA & 0x8000));
    ESP_UART_RX_STA = 0;
    while (!(ESP_UART_RX_STA & 0x8000));
    ESP_UART_RX_STA = 0;
    for (uint8_t i = 50;i<58;i++){
        Net_timeStamp <<= 8;
        Net_timeStamp |= ESP_UART_RX_BUF[i];
    }
    ESP8266_TR_CMD(CIPCLOSE, (char **) receive_cmd);
    return Net_timeStamp;
}

