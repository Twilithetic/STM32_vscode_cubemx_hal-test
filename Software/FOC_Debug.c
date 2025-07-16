#include "FOC_Debug.h"
// 这个.c文件维护的数据
struct FOC_debug_us_data_typedef FOC_debug_us_data;

static char buffer_update[256];
uint8_t Build_in_SCI_log_ms_cnt;
void FOC_Debug_UART(){
    snprintf(buffer_update, sizeof(buffer_update),
        "\r%d,%d,%d,%d\n", 
        FOC_debug_us_data.measure_us_data.us,
        FOC_debug_us_data.PID_curr_to_vlot_us_data.us,
        FOC_debug_us_data.PID_rad_to_speed_us_data.us,
        FOC_debug_us_data.PID_speed_to_curr_us_data.us
    );
    Build_in_SCI_Print(buffer_update);
}

// 测量函数运行周期并更新调试数据（核心函数）
static inline void FOC_Debug_us(void (*target_func)(void), struct Debug_us_typedef* data) {
    target_func();
    uint32_t now_us = Timestamp_us_Count();
    uint32_t elapsed_us = Build_in_Elapsed_us_Compute(data->start_us, now_us);
    
    // 更新测量数据
    data->us = (uint16_t)elapsed_us;
    
    // 更新计数器和累加器
    if (data->count < 0xFFFF) data->count++;  // 防止溢出
    if (data->sum < 0xFFFF - elapsed_us) {    // 防止溢出
        data->sum += (uint16_t)elapsed_us;
    } else {
        // 累加器即将溢出时重置
        data->count = 1;
        data->sum = (uint16_t)elapsed_us;
    }
    
    // 计算平均值
    data->avg_us = (data->count > 0) ? (data->sum / data->count) : 0;
    
    // 更新最大值
    if (elapsed_us > data->max_us) {
        data->max_us = (uint16_t)elapsed_us;
    }

    data->start_us = now_us;
}