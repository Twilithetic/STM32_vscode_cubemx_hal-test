#include "FOC_Debug_proc.h" 
 
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

    sniprintf(UART1_TX_buffer, sizeof(UART1_TX_buffer), "Unknown Command{%s}\n", UART1_RX_buffer);
    Build_in_SCI_Transmit_Print(UART1_TX_buffer);
}

void Period_Set_Command(uint8_t task_id, uint32_t period_us){

    // 修改周期
    motor_task_list[task_id].period_us = period_us;
    sniprintf(UART1_TX_buffer, sizeof(UART1_TX_buffer), "成功：任务%d的周期已设置为 %lu 微秒\n", task_id, period_us);
    Build_in_SCI_Transmit_Print(UART1_TX_buffer);
}
 
 
 
 
 
