//
// Created by 或者 on 2021/5/26.
//

#ifndef ELECTROMAGNETICARTILLERY_SCHEDULER_H
#define ELECTROMAGNETICARTILLERY_SCHEDULER_H

#include "stm32f4xx.h"
#include "main.h"

#define TICK_PER_SECOND	1000

typedef struct
{
    void(*task_func)(void);
    uint16_t rate_hz;
    uint16_t interval_ticks;
    uint32_t last_run;
}sched_task_t;

void Scheduler_Setup(void);
void Scheduler_Run(void);

#endif //ELECTROMAGNETICARTILLERY_SCHEDULER_H
