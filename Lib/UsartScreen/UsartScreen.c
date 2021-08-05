//
// Created by 或者 on 2021/6/19.
//

#include "UsartScreen.h"
#include "usart.h"
#include "Uart.h"

ScreenSt Screen=
        {
            .NowColor=0,
            .LL=0,
            .HL=0,
            .LA=0,
            .HA=0,
            .LB=0,
            .HB=0,

            .ThresholdChange=0,
            .ReadThreshold=0,
            .ReadColor=0,
            .WriteThreshold=0,
            .WriteColor=0,
        };

//接收串口屏发送到单片机的数据
void UsartScreenReceive(uint8_t data)
{
    static uint8_t DataBuffer[15]; //数据缓存数组
    static uint8_t State;          //接收状态 1:接收帧头 2:接收功能帧 3:接收数据长度 4:接收完数据位 5:接收帧尾
    static uint8_t Len=0;          //数据长度
    static uint8_t Count=0;        //统计接收数据位字节数；

    if(State==0&&data==0XAA)       //帧头
    {
        DataBuffer[0]=data;
        State=1;
    }
    else if(State==1 && (data==0X01||data==0X02||data==0X03))              //功能帧
    {
        DataBuffer[1]=data;
        State=2;
    }
    else if(State==2)             //数据长度
    {
        DataBuffer[2]=data;
        Len=data;
        Count=0;
        State=3;
    }
    else if(State==3&&Len>0)             //接收数据位
    {
        Len--;
        DataBuffer[3+Count++]=data;
        if(Len==0)
        {
            State=4;
        }
    }
    else if(State==4&&data==0XBB) //帧尾
    {
        State=5;
        DataBuffer[3+Count]=data;
        UsartScreenAnalysis(DataBuffer,Count+4);
        State=0;
    }
    else
    {
        State=0;
        Count=0;
    }
}

//解析串口屏发送到单片机的数据
void UsartScreenAnalysis(uint8_t *data_buffer,uint8_t len)
{
    if(data_buffer[2]!=len-4)                              //判断数据位长度是否正确
    {
        return;
    }

    //根据功能帧对数据进行解析
    if(*(data_buffer+1)==0X01)                             //接收当前屏幕输入的阈值
    {
        Screen.NowColor=data_buffer[3];
        Screen.LL=data_buffer[4];
        Screen.HL=data_buffer[5];
        Screen.LA=data_buffer[6];
        Screen.HA=data_buffer[7];
        Screen.LB=data_buffer[8];
        Screen.HB=data_buffer[9];
        Screen.ThresholdChange=1;
        SendScreenReturnValue(Screen.NowColor,Screen.LL,Screen.HL,Screen.LA,Screen.HA,Screen.LB,Screen.HB);
    }
    else if(*(data_buffer+1)==0X02)                        //读取已经保存的对应颜色的阈值命令
    {
        Screen.ReadColor=data_buffer[3];
        Screen.ReadThreshold=1;
//        SendScreenThreshold(Screen.NowColor,Screen.LL,Screen.HL,Screen.LA,Screen.HA,Screen.LB,Screen.HB);  //发送阈值给串口屏
    }
    else if(*(data_buffer+1)==0X03)                        //写入对应颜色的阈值命令
    {
        Screen.WriteColor=data_buffer[3];
        Screen.WriteThreshold=1;
//        UsartScreenWriteSuccess();                         //发送写入成功提示音
    }
}

//串口屏初始化
void UsartScreenInit(void)
{
    HAL_UART_Receive_IT(&ScreenUsart,&ScreenUsartBuffer,1);
}

//单片机向串口屏发送对应颜色的阈值
void SendScreenThreshold(uint8_t color,uint8_t LL,uint8_t HL,uint8_t LA,uint8_t HA,uint8_t LB,uint8_t HB)
{
    static uint8_t StrCommand[30];
    uint8_t StrLen=0;      //字符串命令长度

    sprintf(StrCommand,"cb0.val=%d",color);                     //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"h0.val=%d",LL);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"h1.val=%d",HL);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"h2.val=%d",LA);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"h3.val=%d",HA);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"h4.val=%d",LB);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"h5.val=%d",HB);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"wav0.en=1");                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏
}

//单片机向串口屏返回接收到的值
void SendScreenReturnValue(uint8_t color,uint8_t LL,uint8_t HL,uint8_t LA,uint8_t HA,uint8_t LB,uint8_t HB)
{
    static uint8_t StrCommand[30];
    uint8_t StrLen=0;      //字符串命令长度

    sprintf(StrCommand,"cb0.val=%d",color);                     //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"n6.val=%d",LL);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"n7.val=%d",HL);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"n8.val=%d",LA);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"n9.val=%d",HA);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"n10.val=%d",LB);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏

    sprintf(StrCommand,"n11.val=%d",HB);                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏
}

//串口屏写入阈值成功
void UsartScreenWriteSuccess(void)
{
    static uint8_t StrCommand[30];
    uint8_t StrLen=0;      //字符串命令长度

    //发送提示音
    sprintf(StrCommand,"wav0.en=1");                         //拼接 字符串命令主干+值
    StrLen=strlen(StrCommand);                                  //计算字符串命令的长度
    StrCommand[StrLen]=0XFF;                                    //给出完整字符串命令，末尾加3个0XFF
    StrCommand[StrLen+1]=0XFF;
    StrCommand[StrLen+2]=0XFF;
    StrCommand[StrLen+3]=0X00;                                  //给字符串后加\0，防止之前的数据影响
    HAL_UART_Transmit(&ScreenUsart, StrCommand, StrLen+3, 100);//发送字符串命令给串口屏
}
