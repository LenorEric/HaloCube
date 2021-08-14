//
// Created by Lenor on 2021/8/4.
//

#include "ESP8266_WiFi.h"

extern UART_HandleTypeDef ESP8266_UART_PORT;

uint8_t ESP_UART_RX_BUF[ESP_UART_RX_BUF_SIZE];
volatile uint16_t ESP_UART_RX_STA = 0;

uint8_t strcmp_s(const char *str1, const char *str2) {
    uint8_t i = 0;
    while (str1[i] && str2[i]) {
        if (str1[i] - str2[i])
            return str1[i] - str2[i];
        i++;
    }
    return 0;
}

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
    uint32_t tickstart = HAL_GetTick();
    while (1) {
        while (!(ESP_UART_RX_STA & 0x8000))
            if (HAL_GetTick() - tickstart >= 6000) {
                printf("ESP FAIL: Read Timeout\r\n");
                ESP_UART_RX_STA = 0;
                char temp_receive_cmd[ESP_UART_RX_CMD_BUFF_SIZE][ESP_UART_RX_CMD_SIZE];
                ESP8266_TR_CMD("AT+RST\r\n", temp_receive_cmd);
                HAL_Delay(6000);
                return ESP_FAIL;
            }
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
                if (pd == ESP_UART_RX_CMD_SIZE) {
                    printf("ESP FAIL: CMD overflow - size\r\n");
                    ESP_UART_RX_STA = 0;
                    return ESP_FAIL;
                }
#if DebugLevel == 3
                if (pd == 128) { ;;
                }
#endif
            }
            p += 2;
            if (!pd) continue;
            ESP_CMD_BUF[pd] = 0;
            pd = 0;
            if (count == ESP_UART_RX_CMD_BUFF_SIZE) {
                printf("ESP FAIL: CMD overflow - buff\r\n");
                ESP_UART_RX_STA = 0;
                return ESP_FAIL;
            }
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
            if (!strcmp("busy p...", ESP_CMD_BUF)) {
                ESP_UART_RX_STA = 0;
                printf("ESP FAIL: busy p...\r\n");
                char temp_receive_cmd[ESP_UART_RX_CMD_BUFF_SIZE][ESP_UART_RX_CMD_SIZE];
                ESP8266_TR_CMD("AT+RST\r\n", temp_receive_cmd);
                HAL_Delay(6000);
                return ESP_FAIL;
            }
        }
        ESP_UART_RX_STA = 0;
    }
}

ESP8266_STATE ESP8266_TR_CMD(char *cmd2send, char **receive_cmd) {
    ESP_UART_RX_STA = 0;
    HAL_UART_Transmit(&ESP8266_UART_PORT, cmd2send, strlen(cmd2send), 100);
    uint8_t try = 254;
    ESP8266_STATE state = ESP_Receive_CMD(receive_cmd);
    while (state && ++try) {
        ESP_UART_RX_STA = 0;
        HAL_UART_Transmit(&ESP8266_UART_PORT, cmd2send, strlen(cmd2send), 100);
        state = ESP_Receive_CMD(receive_cmd);
        HAL_Delay(10);
    }
    return state;
}

ESP8266_STATE ESP8266_TR_CMD_Once(char *cmd2send, char **receive_cmd) {
    ESP_UART_RX_STA = 0;
    HAL_UART_Transmit(&ESP8266_UART_PORT, cmd2send, strlen(cmd2send), 100);
    ESP8266_STATE state = ESP_Receive_CMD(receive_cmd);
    return state;
}

void ESP8266_WiFi_INIT() {
    char receive_cmd[ESP_UART_RX_CMD_BUFF_SIZE][ESP_UART_RX_CMD_SIZE];
    memset(receive_cmd, 0, sizeof(receive_cmd));
    HAL_UART_Receive_DMA(&ESP8266_UART_PORT, ESP_UART_RX_BUF, ESP_UART_RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(&ESP8266_UART_PORT, UART_IT_IDLE);
    char temp_receive_cmd[ESP_UART_RX_CMD_BUFF_SIZE][ESP_UART_RX_CMD_SIZE];
#ifdef ESP_RESTART_WHEN_INIT
    ESP8266_TR_CMD("AT+RST\r\n", temp_receive_cmd);
    HAL_Delay(6000);
#endif
#ifdef ESP_QUIT_AP_WHEN_INIT
    const char quit_ap[] = "AT+CWQAP\r\n";
    ESP8266_TR_CMD(quit_ap, receive_cmd);
#endif
#ifdef ESP_CONNECT_AP_WHEN_INIT
    const char list_ap[] = "AT+CWJAP?\r\n";
    const char connect_ap[] = "AT+CWJAP=\"Lenoro\",\"ForAzeroth\"\r\n";
    if (ESP8266_TR_CMD(list_ap, receive_cmd) || strcmp_s("+CWJAP:", receive_cmd[0])) {
        ESP8266_TR_CMD(connect_ap, receive_cmd);
    }
#endif
}

uint64_t getTimeStamp() {
    char receive_cmd[ESP_UART_RX_CMD_BUFF_SIZE][ESP_UART_RX_CMD_SIZE];
    const char list_ap[] = "AT+CWJAP?\r\n";
    const char connect_ap[] = "AT+CWJAP=\"Lenoro\",\"ForAzeroth\"\r\n";
    const char connect_NTP[] = "AT+CIPSTART=\"UDP\",\"time.windows.com\",123\r\n";
    const char CIPMODE[] = "AT+CIPMODE=0\r\n";
    const char CIPSEND[] = "AT+CIPSEND=48\r\n";
    const char NTP_UDP_DATA[] = {0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6, 0x6d,
                                 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00};
    const char CIPCLOSE[] = "AT+CIPCLOSE\r\n";
    uint8_t i;
    TEST_AP_STATE:   ///todo: This code is absolutely dangerous and offending, try to refine it
    if (ESP8266_TR_CMD(list_ap, receive_cmd)) {
        printf("Get time failed: Check AP state failed\r\n");
        return 0;
    } else {
#define SEARCH_DEPTH 10
        for (i = 0; i < SEARCH_DEPTH; i++) {
            if (strcmp_s("+CWJAP:", receive_cmd[i])) {
                if (!strcmp_s("No AP", receive_cmd[i])) {
                    if (ESP8266_TR_CMD_Once(connect_ap, receive_cmd)) {
                        printf("Get time failed: Connect AP Failed\r\n");
                        return 0;
                    } else
                        goto TEST_AP_STATE;
                }
            } else
                break;
        }
        if (i == SEARCH_DEPTH){
            printf("Get time failed: Unexpected cmd back\r\n");
            return 0;
        }
    }
    uint64_t Net_timeStamp = 0;
    HAL_UART_Transmit(&ESP8266_UART_PORT, CIPCLOSE, sizeof(CIPCLOSE), 100);
    ESP_UART_RX_STA = 0;
    HAL_Delay(5);
    if (ESP8266_TR_CMD_Once(connect_NTP, receive_cmd)) {
        printf("Get time failed: Failed Connection\r\n");
        return 0;
    }
    HAL_Delay(5);
    if (ESP8266_TR_CMD_Once(CIPMODE, receive_cmd)) {
        printf("Get time failed: Failed Connection\r\n");
        return 0;
    }
    HAL_Delay(5);
    if (ESP8266_TR_CMD_Once(CIPSEND, receive_cmd)) {
        printf("Get time failed: Failed Connection\r\n");
        return 0;
    }
    HAL_Delay(5);
    HAL_UART_Transmit(&ESP8266_UART_PORT, NTP_UDP_DATA, 48, 100);
    uint32_t tickstart = HAL_GetTick();
    uint8_t Response_Empty_Flag = 1;
    while (Response_Empty_Flag) {
        if (HAL_GetTick() - tickstart >= 4000) {
            printf("Get time failed: Wait for data - Timeout\r\n");
            return 0;
        }
        if (ESP_UART_RX_STA & 0x8000) {
            i = 0;
            while (i < (ESP_UART_RX_STA & 0x7fff)) {
                if (!strcmp_s("+IPD,48:", &ESP_UART_RX_BUF[i])) {
                    Response_Empty_Flag = 0;
                    break;
                }
                while (i < (ESP_UART_RX_STA & 0x7fff) && (ESP_UART_RX_BUF[i] != '\r' || ESP_UART_RX_BUF[i + 1] != '\n'))
                    i++;
                i += 2;
            }
            ESP_UART_RX_STA = 0;
        }
    }
    for (i = 50; i < 58; i++) {
        Net_timeStamp <<= 8;
        Net_timeStamp |= ESP_UART_RX_BUF[i];
    }
    ESP8266_TR_CMD(CIPCLOSE, (char **) receive_cmd);
    return Net_timeStamp;
}


