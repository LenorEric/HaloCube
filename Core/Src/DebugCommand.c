//
// Created by Lenor on 2021/8/13.
//

#include "DebugCommand.h"

uint8_t *CMD_Handler;
extern uint8_t UART_RX_BUF[UART_RX_BUF_SIZE];
extern volatile uint16_t UART_RX_STA;

void UART_RxCpltCallBack(void){
    /// todo: Make the command decoder better
    CMD_Handler = UART_RX_BUF;
    if (UART_RX_BUF[0] != '/'){
        printf("%s", UART_RX_BUF);
        return;
    }
    if (!strcmp("/HC -v\r\n", UART_RX_BUF)){
        printf("Firmware Version: Alpha34.ac4cbb5\r\n");
        return;
    }
    if (!strcmp("/EEPROM -fc\r\n", UART_RX_BUF)){
        printf("Begin to clear EEPROM\r\n");
        EEPROM_Clear();
        return;
    }
    if (!strcmp("/EEPROM -fc\r\n", UART_RX_BUF)){
        BT_Push(EEPROM_Clear);
        printf("EEPROM Clear On Schedule\r\n");
        return;
    }
    printf("Unknown Command\r\n");
}