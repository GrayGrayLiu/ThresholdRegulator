//
// Created by 或者 on 2021/6/19.
//

#ifndef ADJUSTMOTORSPEED_USARTSCREEN_H
#define ADJUSTMOTORSPEED_USARTSCREEN_H

#include "stm32f4xx.h"
#include "main.h"

#define ScreenUsart huart2

typedef struct
{
    uint8_t NowColor;                                              //白:0 黑:1 红:2 绿:3 蓝:4 黄:5 混:6
    uint8_t LL;
    uint8_t HL;
    uint8_t LA;
    uint8_t HA;
    uint8_t LB;
    uint8_t HB;

    uint8_t ThresholdChange;                                    //阈值发生改变标志位 0:未改变 1:已改变
    uint8_t ReadThreshold;                                      //读取阈值命令 0:不读取 1:需要读取
    uint8_t ReadColor;                                          //需要读取阈值的颜色
    uint8_t WriteThreshold;                                     //写入阈值命令 0:不写入 1:需要写入
    uint8_t WriteColor;                                         //需要写入阈值的颜色
}ScreenSt;

void UsartScreenReceive(uint8_t data);                          //接收串口屏发送到单片机的数据
void UsartScreenAnalysis(uint8_t *data_buffer,uint8_t len);     //解析串口屏发送到单片机的数据

void UsartScreenInit(void);                                     //串口屏初始化
void SendScreenThreshold(uint8_t color,uint8_t LL,uint8_t HL,uint8_t LA,uint8_t HA,uint8_t LB,uint8_t HB);//单片机向串口屏发送对应颜色的阈值
void SendScreenReturnValue(uint8_t color,uint8_t LL,uint8_t HL,uint8_t LA,uint8_t HA,uint8_t LB,uint8_t HB);//单片机向串口屏返回接收到的值
void UsartScreenWriteSuccess(void);                             //串口屏写入阈值成功

extern ScreenSt Screen;

#endif //ADJUSTMOTORSPEED_USARTSCREEN_H
