//
// Created by Lenor on 2021/8/10.
//

#include "PC.h"

uint8_t PC_ON_FLAG = 0;

uint8_t GetPCStatus() {
    return PC_ON_FLAG;
}

uint8_t ReturnPCStatus() {
    return PC_ON_FLAG;
}

void TurnOnPC() {
    GetPCStatus();
    PC_ON_FLAG = 1;
//    char receive_cmd[20][nRF_UART_RX_CMD_SIZE];
//    const char PCOn[] = "HC+PCOn";
//    nRF24L01_TR_CMD(PCOn, receive_cmd);
    Power_PCSetStatus(1);
}

void TurnOffPC() {
    GetPCStatus();
    PC_ON_FLAG = 0;
//    char receive_cmd[20][nRF_UART_RX_CMD_SIZE];
//    const char PCOff[] = "HC+PCOff";
//    nRF24L01_TR_CMD(PCOff, receive_cmd);
    Power_PCSetStatus(0);
}

