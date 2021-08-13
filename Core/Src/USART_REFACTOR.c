//
// Created by Lenor on 2021/8/2.
//

#include "USART_REFACTOR.h"

#ifdef REFACTOR_PRINTF_SCANF
extern UART_HandleTypeDef UART_IO_PORT;
uint8_t ch_r;

///重写这个函数,重定向printf函数到串口
///Rewrite different function according to different compiler
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&UART_IO_PORT, (uint8_t *) &ch, 1, 10);
    return ch;
}

///重定向scanf函数到串口 意思就是说接受串口发过来的数据
/// WARNING: THE FOLLOWING FUNCTION MAY NOT WORK PROPERLY
GETCHAR_PROTOTYPE {
    HAL_UART_Receive(&UART_IO_PORT, &ch_r, 1, 0xffff);
    return ch_r;
}

#endif

#ifdef REFACTOR_UART_IRQ
uint8_t UART_RX_BUF[UART_RX_BUF_SIZE];
volatile uint16_t UART_RX_STA;

__weak void UART_RxCpltCallBack(void){
}

void UART_IQR_HANDLER(void) {
    if (__HAL_UART_GET_FLAG(&UART_IO_PORT, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
    {
        __HAL_UART_CLEAR_IDLEFLAG(&UART_IO_PORT);  /// 清除中断标记
        HAL_UART_DMAStop(&UART_IO_PORT);           /// 停止DMA接收
        UART_RX_STA = UART_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(UART_IO_PORT.hdmarx);  // 总数据量减去未接收到的数据量为已经接收到的数据量
        UART_RX_BUF[UART_RX_STA] = 0;  /// 添加结束符
        UART_RX_STA |= 0X8000;         /// 标记接收结束
        HAL_UART_Receive_DMA(&UART_IO_PORT, UART_RX_BUF, UART_RX_BUF_SIZE);  /// 重新启动DMA接收
        UART_RxCpltCallBack();
    }
}

void REFACTORED_UART_DMA_INIT(){
    HAL_UART_Receive_DMA(&UART_IO_PORT, UART_RX_BUF, UART_RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(&UART_IO_PORT, UART_IT_IDLE);
}
#endif