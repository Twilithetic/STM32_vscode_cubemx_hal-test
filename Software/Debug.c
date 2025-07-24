#include "Debug.h"
// 这个.c文件维护的数据
struct Debug_Data_typedef Debug_Data;


uint8_t Build_in_SCI_log_ms_cnt;

void Debug_Init(){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)UART1_RX_buffer, sizeof(UART1_RX_buffer));
    Build_in_SCI_Receive_Proc = Debug_UART_Receive_proc;
}

/// @brief 发送FOC_debug_us_data的数据
void Debug_UART_Print(){  
    Build_in_UART_Printf(
        "\r%d,%lu,%d,%lu,%d,%lu,%d,%lu\n", 
        Debug_Data.measure_us_data.us, Debug_Data.measure_us_data.period_us, 
        Debug_Data.PID_curr_to_vlot_us_data.us, Debug_Data.PID_curr_to_vlot_us_data.period_us,
        Debug_Data.PID_rad_to_speed_us_data.us, Debug_Data.PID_rad_to_speed_us_data.period_us,
        Debug_Data.PID_speed_to_curr_us_data.us, Debug_Data.PID_speed_to_curr_us_data.period_us
    );
}


// 测量函数运行周期并更新调试数据（核心函数）
inline void Debug_func_us(void (*target_func)(void), uint32_t period_us, struct Debug_us_typedef* data) {
    uint64_t start_us = Timestamp_us_Count();
    target_func();
    if (data == NULL) {return;}
    
    uint64_t now_us = Timestamp_us_Count();
    uint64_t elapsed_us = Build_in_Elapsed_us_Compute(start_us, now_us);
    
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
    // 更新周期
    data->period_us = period_us;
    
}



void Debug_UART_Receive_proc(UART_HandleTypeDef *huart, uint16_t Size){
    if (huart != &huart1) return;  // 只处理USART1的数据
    
    // 1. 将接收到的字节转为C字符串（添加结束符'\0'）
    // 注意：确保缓冲区有足够空间（Size < UART1_RX_BUF_SIZE）
    if (Size >= sizeof(UART1_RX_buffer)) {
        Size = sizeof(UART1_RX_buffer) - 1;  // 防止越界
    }
    UART1_RX_buffer[Size] = '\0';  // 手动添加字符串结束符

    // 2. 去除字符串前的空格（比如" mode 3"→"mode 3"）
    char *cmd_str = UART1_RX_buffer;
    // while (*cmd_str && isspace((unsigned char)*cmd_str)) {
    //     cmd_str++;  // 跳过前导空格
    // }

    // 3. 解析命令（"mode X" 或 "test X"）
    // 3.1 匹配 "mode X" 指令（X为数字）
    if (strncmp(cmd_str, "mode ", 5) == 0) {  // 比较前5个字符是否为"mode "
        char *param_str = cmd_str + 5;  // 参数部分（如"1"）
        int mode_int = atoi(param_str);    // 字符串转整数（"1"→1）
        Mode_Command(mode_int);            // 执行mode指令
    } else if (strncmp(cmd_str, "period ", 7) == 0) {// 解析 "period X Y" 命令
        char *task_id_str = cmd_str + 7;  // 任务ID部分（如"0"）
        char *period_str = strchr(task_id_str, ' ');  // 查找第二个空格
        
        if (period_str != NULL) {  // 确保找到了第二个空格
            *period_str = '\0';    // 截断字符串，使task_id_str成为完整的任务ID字符串
            period_str++;          // 指向周期值（如"10000"）
            
            // 转换为整数
            int task_id = atoi(task_id_str);
            uint32_t period = atoi(period_str);
            
            // 修改对应任务的周期
            Period_Set_Command(task_id, period);
        }
    } else {
        // 3.3 未知指令
        Unknown_Command();
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)UART1_RX_buffer, sizeof(UART1_RX_buffer));
    

}
