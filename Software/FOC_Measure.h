#ifndef __FOC_MEASURE_H
#define __FOC_MEASURE_H

// 依赖的硬件驱动
#include "AS5600.h"
#include "INA199A1.h"
// 数学
#include "math.h"

// 咱鼠的这个文件是FOC检测的入口

// 传感器收集到的数据 三相电流(电流向量) 和 机械角度
struct FOC_measured_data_typedef
{
    float current_a; // 单位 A
    float current_b;
    float current_c;
    float Machine_Rad;
    float Machine_Rad_Total;
    float Machine_Rad_Speed;
};
extern struct FOC_measured_data_typedef FOC_measured_data;

void FOC_Measure_Update();

#endif // !__FOC_INPUT_H
