#ifndef __JC2804_H
#define __JC2804_H
// 芯片依赖
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "can.h"
#include "Build_in-Led.h"
// 标准库依赖
#include <math.h> // 用于round函数

typedef struct JC2804_Data
{
    uint8_t id;
    float angle;
} JC2804_Data;
extern JC2804_Data JC2804_Data_list[];



extern uint8_t JC2804_is_test;


void JC2804_Init();

void test_PID();

void JC2804_pull_data();

void set_Abs_Point(uint8_t id, float angle);

void test_JC2804_loopback();

#endif // !__JC2804_H


