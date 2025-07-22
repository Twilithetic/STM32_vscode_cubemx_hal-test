#ifndef __SCHEDULER__H
#define __SCHEDULER__H

// 芯片基础
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "Build_in-Led.h"

// 其他模块依赖
#include "Debug.h"

typedef struct Scheduler_typedef
{
    uint8_t Pole_Pairs;
} Scheduler_typedef;
extern struct Scheduler_typedef Motor;

// 任务结构体
typedef struct Scheduler_Task
{
    void (*task_func)(void);  // 任务函数指针
    struct Debug_us_typedef* debug_data;  // 调试数据指针
    uint32_t period_us;         // 执行周期（毫秒）
    uint32_t last_start;        // 上次执行时间（初始化为 0，每次运行时刷新）
} Scheduler_Task;
extern Scheduler_Task Scheduler_task_list[];

#endif // !__MOTOR__H

void Scheduler_Init();

void Scheduler_Start();
