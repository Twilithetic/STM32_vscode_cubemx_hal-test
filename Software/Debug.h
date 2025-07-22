#ifndef __FOC_DEBUG_H
#define __FOC_DEBUG_H

// 芯片依赖
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "Build_in-Led.h"
// 标准库依赖
#include <stdio.h>
#include <string.h>
#include <ctype.h>  // 用于处理空格（isspace函数）
#include <stdlib.h> //atoi
// 次模块依赖
#include "Debug_proc.h"
/// @brief .c维护的数据
struct Debug_us_typedef
{   uint32_t period_us;
    uint16_t us, avg_us, max_us;
    uint16_t count, sum; 
};
struct Debug_Data_typedef
{
    struct Debug_us_typedef measure_us_data;
    struct Debug_us_typedef PID_curr_to_vlot_us_data;
    struct Debug_us_typedef PID_speed_to_curr_us_data;
    struct Debug_us_typedef PID_rad_to_speed_us_data;

};
extern struct Debug_Data_typedef Debug_Data;

void Debug_Init();

void Debug_UART_Print();

// #define FOC_DEBUG_FUNC_US(func, type) Debug_func_us(func, &Debug_Data.type##_us_data)
void Debug_func_us(void (*target_func)(void), uint32_t period_us, struct Debug_us_typedef *data);

//#define HAL_UARTEx_RxEventCallback FOC_Debug_UART_Reviev_proc // 用宏将 HAL_UARTEx_RxEventCallback 替换为你自己的函数名
void Debug_UART_Receive_proc(UART_HandleTypeDef *huart, uint16_t Size);

#endif // !__MOTOR_DEBUG_H


