//
// Created by Lenor on 2021/8/8.
//

#include "nRF24L01.h"

extern UART_HandleTypeDef nRF24L01_UART_PORT;

uint8_t nRF_UART_RX_BUF[nRF_UART_RX_BUF_SIZE];
volatile uint16_t nRF_UART_RX_STA = 0;

void nRF24L01_IRQ_HANDLER(void) {
    if (__HAL_UART_GET_FLAG(&nRF24L01_UART_PORT, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
    {
        __HAL_UART_CLEAR_IDLEFLAG(&nRF24L01_UART_PORT);  // 清除中断标记
        HAL_UART_DMAStop(&nRF24L01_UART_PORT);           // 停止DMA接收
        nRF_UART_RX_STA =
                nRF_UART_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(nRF24L01_UART_PORT.hdmarx);  // 总数据量减去未接收到的数据量为已经接收到的数据量
        nRF_UART_RX_BUF[nRF_UART_RX_STA] = 0;  // 添加结束符
        nRF_UART_RX_STA |= 0X8000;         // 标记接收结束
        HAL_UART_Receive_DMA(&nRF24L01_UART_PORT, nRF_UART_RX_BUF, nRF_UART_RX_BUF_SIZE);  // 重新启动DMA接收
        nRF24L01_UART_RxCpltCallBack();
    }
}

nRF24L01_STATE nRF_Receive_CMD(char receive_cmd[][nRF_UART_RX_CMD_SIZE]) {
    uint16_t p = 0, pd = 0;
    uint8_t nRF_CMD_BUF[nRF_UART_RX_CMD_SIZE];
    uint8_t count = 0;
    uint32_t tickstart = HAL_GetTick();
    while (1) {
        while (!(nRF_UART_RX_STA & 0x8000))
            if (HAL_GetTick() - tickstart >= 200){
#if DebugLevel >= DL_WARNING
                printf("!!Warning!!: nRF Timeout\r\n");
#endif
                return nRF_FAIL;
            }
        p = 0;
        while (p < (nRF_UART_RX_STA & 0x7fff)) {
            while (1) {
                if (nRF_UART_RX_BUF[p] == 0x0D) {
                    if (nRF_UART_RX_BUF[p + 1] == 0x0A)
                        break;
                    pd = 0;
                } else
                    nRF_CMD_BUF[pd++] = nRF_UART_RX_BUF[p];
                p++;
            }
            p += 2;
            if (!pd) continue;
            nRF_CMD_BUF[pd] = 0;
            pd = 0;
            strcpy(&receive_cmd[count++][0], nRF_CMD_BUF);
            if (!strcmp("OK", nRF_CMD_BUF)) {
                nRF_UART_RX_STA = 0;
                return nRF_OK;
            }
            if (!strcmp("HC_OK", nRF_CMD_BUF)) {
                nRF_UART_RX_STA = 0;
                return nRF_OK;
            }
            if (!strcmp("ERROR", nRF_CMD_BUF)) {
                nRF_UART_RX_STA = 0;
                return nRF_ERROR;
            }
            if (!strcmp("FAIL", nRF_CMD_BUF)) {
                nRF_UART_RX_STA = 0;
                return nRF_FAIL;
            }
        }
        nRF_UART_RX_STA = 0;
    }
}

nRF24L01_STATE nRF24L01_TR_CMD(char cmd2send[], char **receive_cmd) {
    nRF_UART_RX_STA = 0;
    HAL_UART_Transmit(&nRF24L01_UART_PORT, cmd2send, strlen(cmd2send)+1, 100);
    uint8_t try = 245;
    nRF24L01_STATE state = nRF_Receive_CMD(receive_cmd);
    while (state && ++try) {
        nRF_UART_RX_STA = 0;
        HAL_UART_Transmit(&nRF24L01_UART_PORT, cmd2send, strlen(cmd2send)+1, 100);
        state = nRF_Receive_CMD(receive_cmd);
    }
    return state;
}

void nRF24L01_INIT() {
    char receive_cmd[20][nRF_UART_RX_CMD_SIZE];
    HAL_UART_Receive_DMA(&nRF24L01_UART_PORT, nRF_UART_RX_BUF, nRF_UART_RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(&nRF24L01_UART_PORT, UART_IT_IDLE);
    const char test_for[] = "AT?\0";
    while (nRF24L01_TR_CMD(test_for, receive_cmd));
    HAL_UART_Transmit(&nRF24L01_UART_PORT, "nRF Test", 9, 100);
}

void nRF24L01_UART_RxCpltCallBack(void) {
}
