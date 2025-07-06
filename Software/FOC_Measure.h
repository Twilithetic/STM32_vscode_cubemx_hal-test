#ifndef __FOC_MEASURE_H
#define __FOC_MEASURE_H

// 依赖的硬件驱动
#include "AS5600.h"
// 数学
#include "math.h"

// 咱鼠的这个文件是FOC检测的入口
#define  Pole_Pairs 7 // 极对数：7 dian角度 = 机械角度 x 极对数 
// 传感器收集到的数据 三相电流(电流向量) 和 机械角度
typedef struct 
{
    float current_a; // 单位 A
    float current_b;
    float current_c;
    float Machine_Rad;
    float Machine_Rad_Total;
    float Machine_Rad_Speed;
} FOC_measured_data_typedef;

extern FOC_measured_data_typedef FOC_measured_data;
extern uint8_t AS5600_Speed_Updata_delay_ms_cnt;
extern uint8_t AS5600_Rad_Updata_delay_ms_cnt;



//debug
extern float dt_t;

#endif // !__FOC_INPUT_H

void FOC_Measure_Update();

void FOC_Measure_AS5600_Update();

void FOC_Measure_AS5600_Speed_Update();
