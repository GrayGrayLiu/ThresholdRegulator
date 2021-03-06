//
// Created by 或者 on 2021/5/26.
//

#include "scheduler.h"
#include "Uart.h"
#include "UsartScreen.h"
#include "OMV_Thr.h"
#include "db.h"

static void Loop_1000Hz(void) //1ms执行一次
{
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
}

static void Loop_500Hz(void) //2ms执行一次
{
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
}

static void Loop_250Hz(void) //4ms执行一次
{
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
}

static void Loop_200Hz(void) //5ms执行一次
{
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
}

static void Loop_100Hz(void) //10ms执行一次
{
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
}

static void Loop_50Hz(void) //20ms执行一次
{
    //////////////////////////////////////////////////////////////////////
}

static void Loop_20Hz(void) //50ms执行一次
{
    //////////////////////////////////////////////////////////////////////
    db_task();
    //////////////////////////////////////////////////////////////////////
}

static void Loop_10Hz(void) //100ms执行一次
{
    ThrToOMV();
}

static void Loop_5Hz(void) //200ms执行一次
{
}

static void Loop_4Hz(void) //250ms执行一次
{
}

static void Loop_2Hz(void) //500ms执行一次
{
}

static void Loop_1Hz(void) //1000ms执行一次
{
//    printf("Test\r\n");
}

//////////////////////////////////////////////////////////////////////
//调度器初始化
//////////////////////////////////////////////////////////////////////
//系统任务配置，创建不同执行频率的“线程”
static sched_task_t sched_tasks[] =
{
    {Loop_1000Hz, 1000, 0, 0},
    {Loop_500Hz, 500, 0, 0},
    {Loop_250Hz, 250, 0, 0},
    {Loop_200Hz, 200, 0, 0},
    {Loop_100Hz, 100, 0, 0},
    {Loop_50Hz, 50, 0, 0},
    {Loop_20Hz, 20, 0, 0},
    {Loop_10Hz, 10, 0, 0},
    {Loop_5Hz, 5, 0, 0},
    {Loop_4Hz, 4, 0, 0},
    {Loop_2Hz, 2, 0, 0},
    {Loop_1Hz, 1, 0, 0},
};
//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks) / sizeof(sched_task_t))

void Scheduler_Setup(void)
{
    uint8_t index = 0;
    //初始化任务表
    for (index = 0; index < TASK_NUM; index++) {
        //计算每个任务的延时周期数
        sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
        //最短周期为1，也就是1ms
        if (sched_tasks[index].interval_ticks < 1) {
            sched_tasks[index].interval_ticks = 1;
        }
    }
}

//这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
void Scheduler_Run(void)
{
    uint8_t index = 0;
    //循环判断所有线程，是否应该执行

    for (index = 0; index < TASK_NUM; index++) {
        //获取系统当前时间，单位MS
        uint32_t tnow = HAL_GetTick();
        //进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
        if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks) {

            //更新线程的执行时间，用于下一次判断
            sched_tasks[index].last_run = tnow;
            //执行线程函数，使用的是函数指针
            sched_tasks[index].task_func();
        }
    }
}