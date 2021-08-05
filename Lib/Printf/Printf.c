//
// Created by 或者 on 2020/9/20.
//

#include "Printf.h"
#include "usart.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    while ((USART1->SR & 0X40U) == 0);
    USART1->DR = (uint8_t) ch;
    return ch;
}
