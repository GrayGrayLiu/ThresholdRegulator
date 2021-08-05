//
// Created by 或者 on 2021/8/4.
//

#include "OMV_Thr.h"
#include "usart.h"
#include "UsartScreen.h"

uint8_t threshold_num[3];
uint8_t threshold[100];
uint8_t part = 0, uart_num;
uint8_t get_command = 0, sent_command = 0x00;
uint8_t state[100] = {0}, empty[1];
uint8_t task = 0;
uint8_t sent_star = 0XAA, sent_end = 0X55;
uint8_t omv_id, uart_len, sent_state;
uint8_t OMV_Online=0;
uint8_t Omv_Send[100];

void OMV_Receive(void)
{
    if (threshold_num[0] == 0XAA && part == 0) {
        uart_num = threshold_num[2];
        HAL_UART_Receive_IT(&OmvUART, threshold, uart_num);
        part = 1;
    } else if (part == 1 && threshold[uart_num - 1] == 0X55) {
        task = 1;
        get_command = threshold[0];
        state[get_command] = threshold[1];
        HAL_UART_Receive_IT(&OmvUART, threshold_num, 3);
        part = 0;
        Omv_Online();
    } else if (empty[0] == 0X55) {
        HAL_UART_Receive_IT(&OmvUART, threshold_num, 3);
    } else
        HAL_UART_Receive_IT(&OmvUART, empty, 1);
}

void Omv_Write(uint8_t *data, uint8_t len) {
    while (len--) {
        while ((USART3->SR & 0X40U) == 0);
        USART3->DR = *data++;
    }
}

void Omv_Online(void){
    if (get_command == 0) {
        if (state[get_command] == 0X00) {
            omv_id = 0x00;
            uart_len = 0X07;
            sent_command = 0x00;
            sent_state = 0X01;
            Omv_Send[0] = sent_star;
            Omv_Send[1] = omv_id;
            Omv_Send[2] = uart_len - 3;
            Omv_Send[3] = sent_command;
            Omv_Send[4] = sent_state;
            Omv_Send[5] = 0X00;
            Omv_Send[6] = sent_end;
            Omv_Write(Omv_Send, 0X07);
        } else if (state[get_command] == 0X01) {
//            OMV_Online=1;
        }
    }
}

void ThrToOMV(void)
{
    if (state[0] == 0X01&&(Screen.ThresholdChange==1||Screen.WriteThreshold==1)) {
        omv_id = 0x00;
        uart_len = 0X0D;
        sent_command = 0x04;
        sent_state = Screen.WriteThreshold;
        Omv_Send[0] = sent_star;
        Omv_Send[1] = omv_id;
        Omv_Send[2] = uart_len - 3;
        Omv_Send[3] = sent_command;
        Omv_Send[4] = sent_state;
        Omv_Send[5]=Screen.NowColor;
        Omv_Send[6]=Screen.LL;
        Omv_Send[7]=Screen.HL;
        Omv_Send[8]=Screen.LA;
        Omv_Send[9]=Screen.HA;
        Omv_Send[10]=Screen.LB;
        Omv_Send[11]=Screen.HB;
        Omv_Send[12] = sent_end;
        Omv_Write(Omv_Send, uart_len);
        Screen.ThresholdChange=0;
//        Screen.WriteThreshold=0;
    }
}

void Omv_Init()
{
    HAL_UART_Receive_IT(&OmvUART, threshold_num, 3);
}