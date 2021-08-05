//
// Created by 或者 on 2021/6/19.
//

#include "Uart.h"
#include "usart.h"
#include "UsartScreen.h"
#include "OMV_Thr.h"

uint8_t ScreenUsartBuffer;   //串口接收1字节缓存

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == ScreenUsart.Instance)
    {
        UsartScreenReceive(ScreenUsartBuffer);
        HAL_UART_Receive_IT(&ScreenUsart,&ScreenUsartBuffer,1);
    }
    else if (huart->Instance == OmvUART.Instance)
    {
        OMV_Receive();
    }
}
