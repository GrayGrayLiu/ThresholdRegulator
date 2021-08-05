//
// Created by wirano on 2021/8/5.
//

#ifndef DB_H
#define DB_H

#include "main.h"

typedef struct {
    uint8_t LL;
    uint8_t HL;
    uint8_t LA;
    uint8_t HA;
    uint8_t LB;
    uint8_t HB;
}db_data;

void db_task(void);

#endif //DB_H
