#include "FOC_Measure.h"
FOC_measured_data_typedef FOC_measured_data;

void FOC_Measure_Update(){
    FOC_Measure_AS5600_Update();



}

const float ANGLE_THRESHOLD = 0.001f;// 0.001弧度（0.052°)
int32_t circle_cnt = 0;    // 圈数（正转+1，反转-1）// 顺时针 + 逆时针 -
float above_Machine_Rad = 0.0f;
void FOC_Measure_AS5600_Update(){
    AS5600_Update_Data();
    FOC_measured_data.Machine_Rad = (float)( AS5600_Data.Angle_Raw * (2 * M_PI / 4096.0f) );
    // 多圈累计功能
    // 新增：计算当前角度与上一次角度的差值（考虑跨0点情况）
    float rad_diff = FOC_measured_data.Machine_Rad - above_Machine_Rad;
    // 规范化到[-π, π]
    if (rad_diff > M_PI) rad_diff -= 2*M_PI;
    if (rad_diff < -M_PI) rad_diff += 2*M_PI;
    
    // 只有角度变化超过阈值时，才更新累计角度
    if (fabs(rad_diff) >= ANGLE_THRESHOLD) {
        // 检测圈数变化（正转：角度从2π附近跳到0附近，圈数+1；反转反之）
        if (FOC_measured_data.Machine_Rad < above_Machine_Rad - M_PI) {// 正转多一圈（如350°→10°，差-340°≈-1.89π < -π）
            circle_cnt++;
        } else if (FOC_measured_data.Machine_Rad > above_Machine_Rad + M_PI) {// 反转少一圈（如10°→350°，差+340°≈+1.89π > +π）
            circle_cnt--;
        }
        
        // 计算累计角度（多圈角度，单位：弧度）
        FOC_measured_data.Machine_Rad_Total = circle_cnt * 2*M_PI + FOC_measured_data.Machine_Rad;
        
        // 更新上一次角度
        above_Machine_Rad = FOC_measured_data.Machine_Rad;
    }
    // 否则：Angle_Rad_Total保持不变，不累计微小波动
    
    // 计算速度
    FOC_Measure_AS5600_Speed_Update();
}

static float last_total_rad = 0.0f; // 上一次的累计角度
static uint32_t AS5600_current_Ticks;
static uint32_t AS5600_above_Ticks;
uint16_t AS5600_Speed_Updata_delay_ms_cnt;
void FOC_Measure_AS5600_Speed_Update(){
    // 控制更新频率 太快 浮点数太小算不出 只能1PI 2PI的变
    // if (AS5600_Speed_Updata_delay_ms_cnt) return;
    // AS5600_Speed_Updata_delay_ms_cnt = 1;
    //计算时间
    uint32_t elapsed;
    uint32_t reload   = SysTick->LOAD;  
    uint32_t ticks_per_sec = HAL_RCC_GetHCLKFreq();
    AS5600_current_Ticks = SysTick->VAL; 
    if (AS5600_current_Ticks <= AS5600_above_Ticks)// 注意 systick 是从71999 到0 下降的，这里是没有溢出的情况
    {
        elapsed = AS5600_above_Ticks - AS5600_current_Ticks;
    } else {
        elapsed = AS5600_above_Ticks + (reload - AS5600_current_Ticks);
    }  
    float dt = ((float)elapsed / (float)ticks_per_sec); // dt 单位：second

    // 计算角度变化量
    float angle_delta = FOC_measured_data.Machine_Rad_Total - last_total_rad;

    // 容差 好坏的float 人类的十进制太逊了0.002弧度（0.104°）
    if (fabs(angle_delta) < 0.002f) {angle_delta = 0.0f;}

    // 计算角速度 (rad/s)
    FOC_measured_data.Machine_Rad_Speed = angle_delta / dt;

    // 更新上一次的累计角度和时间
    last_total_rad = FOC_measured_data.Machine_Rad_Total;
    AS5600_above_Ticks = AS5600_current_Ticks;
}





