//
// Created by 或者 on 2021/8/4.
//

#ifndef THRESHOLDREGULATOR_OMV_THR_H
#define THRESHOLDREGULATOR_OMV_THR_H

#include "main.h"

#define OmvUART huart3

void OMV_Receive(void);
void Omv_Write(uint8_t *data, uint8_t len);
void ThrToOMV(void);
void Omv_Online(void);
void Omv_Init();

extern uint8_t threshold_num[3];
extern uint8_t threshold[100];
extern uint8_t part, uart_num;
extern uint8_t get_command, sent_command;
extern uint8_t state[100] , empty[1];
extern uint8_t task;
extern uint8_t sent_star, sent_end;
extern uint8_t omv_id, uart_len, sent_state;

#endif //THRESHOLDREGULATOR_OMV_THR_H
