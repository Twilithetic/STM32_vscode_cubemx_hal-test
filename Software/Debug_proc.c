#include "Debug_proc.h" 
 
void Mode_Command(uint8_t mode_int){
    switch (mode_int){
        case 1:
            /* mode 1执行的相关代码 */

            break;
        case 2:
            /* mode 1执行的相关代码 */

            break;
        default:
            break;
    }
}


void Unknown_Command(){

    Build_in_UART_Printf("Unknown Command{%s}\n", UART1_RX_buffer);
}

void Period_Set_Command(uint8_t task_id, uint32_t period_us){

    // 修改周期
    Scheduler_task_list[task_id].period_us = period_us;
    Build_in_UART_Printf("成功：任务%d的周期已设置为 %lu 微秒\n", task_id, period_us);
}
 
 
 
 
 
