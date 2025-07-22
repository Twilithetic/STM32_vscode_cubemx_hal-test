#ifndef ___FOC_DEBUG_PROC_H
#define ___FOC_DEBUG_PROC_H

// 芯片依赖
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "Build_in-Led.h"
// 子模块依赖
#include "Scheduler.h"


#endif // !___FOC_DEBUG_PROC_H

void Mode_Command(uint8_t mode_int);

void Unknown_Command();

void Period_Set_Command(uint8_t task_id, uint32_t period_us);
