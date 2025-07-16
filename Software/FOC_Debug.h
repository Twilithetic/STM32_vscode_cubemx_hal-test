#ifndef __FOC_DEBUG_H
#define __FOC_DEBUG_H

// 芯片依赖
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "Build_in-Led.h"
// 标准库依赖
#include <stdio.h>

/// @brief .c维护的数据
struct FOC_debug_us_data_typedef
{
    struct Debug_us_typedef measure_us_data;
    struct Debug_us_typedef PID_curr_to_vlot_us_data;
    struct Debug_us_typedef PID_speed_to_curr_us_data;
    struct Debug_us_typedef PID_rad_to_speed_us_data;

};
extern struct FOC_debug_us_data_typedef FOC_debug_us_data;

struct Debug_us_typedef
{
    uint16_t us, avg_us, max_us;
    uint16_t count, sum, start_us;
};
extern struct Debug_us_typedef measure_us_data;
extern struct Debug_us_typedef PID_curr_to_vlot_us_data;
extern struct Debug_us_typedef PID_speed_to_curr_us_data;
extern struct Debug_us_typedef PID_rad_to_speed_us_data;

#define FOC_Debug_us(func, type) \
    FOC_Debug_us(func, &FOC_debug_us_data.type##_us_data)
static void FOC_Debug_us(void (*target_func)(void), Debug_us_typedef *data);

#endif // !__MOTOR_DEBUG_H


