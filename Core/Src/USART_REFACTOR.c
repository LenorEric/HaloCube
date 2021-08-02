//
// Created by Lenor on 2021/8/2.
//

#include "USART_REFACTOR.h"

#ifdef REFACTOR_PRINTF_SCANF
extern UART_HandleTypeDef IO_PORT;
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
    HAL_UART_Transmit(&IO_PORT, (uint8_t *) &ch, 1, 10);
    return ch;
}

///重定向scanf函数到串口 意思就是说接受串口发过来的数据
/// WARNING: THE FOLLOWING FUNCTION MAY NOT WORK PROPERLY
GETCHAR_PROTOTYPE {
    HAL_UART_Receive(&IO_PORT, &ch_r, 1, 0xffff);
    return ch_r;
}

#endif