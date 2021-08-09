//
// Created by Lenor on 2021/8/9.
//

#include "Bulb.h"

uint8_t Bulb_ON_FLAG = 0;

void TurnOffBulb() {
    GetBulbStatus();
    char receive_cmd[20][nRF_UART_RX_CMD_SIZE];
    Bulb_ON_FLAG = 0;
    const char BulbOff[] = "HC+BulbOff";
    nRF24L01_TR_CMD(BulbOff, receive_cmd);
    Power_BulbSetStatus(0);
}

void TurnOnBulb() {
    GetBulbStatus();
    char receive_cmd[20][nRF_UART_RX_CMD_SIZE];
    Bulb_ON_FLAG = 1;
    const char BulbOff[] = "HC+BulbOn";
    nRF24L01_TR_CMD(BulbOff, receive_cmd);
    Power_BulbSetStatus(1);
}

uint8_t ReturnBulbStatus() {
    return Bulb_ON_FLAG;
}

uint8_t GetBulbStatus() {
    char receive_cmd[20][nRF_UART_RX_CMD_SIZE];
    const char BulbGetStatus[] = "HC+BulbGetStatus";
    nRF24L01_TR_CMD(BulbGetStatus, receive_cmd);
    if (strcmp("HC_OK", receive_cmd[1]))
        THROW_ERROR;
    if (receive_cmd[0][0]){
        Bulb_ON_FLAG = 1;
        return 1;
    }
    else{
        Bulb_ON_FLAG = 0;
        return 0;
    }
}
