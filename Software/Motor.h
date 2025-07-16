#ifndef __MOTOR__H
#define __MOTOR__H

// 芯片依赖
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "Build_in-Led.h"

// 其他模块依赖
#include "FOC_Measure.h"

typedef struct Motor_typedef
{
    uint8_t Pole_Pairs 7;
};

extern struct Motor_typedef Motor;

// 任务结构体
typedef struct Motor_Ctrl_t
{
    void (*task_func)(void);  // 任务函数指针
    uint32_t period_us;         // 执行周期（毫秒）
    uint32_t last_run;        // 上次执行时间（初始化为 0，每次运行时刷新）
};

#endif // !__MOTOR__H
