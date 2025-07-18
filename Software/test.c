#include "test.h"
void test_Init(){
    // Init
    __HAL_DBGMCU_FREEZE_TIM3(); // 断点不停止时钟

    Build_in_LED_Init();
    Motor_Init();
}

void test(){
    //Motor_Ctrl_task_run();
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"1111\n", 5);
    Build_in_Delay_ms(500);
}

void test_blink(){
    Build_in_LED_Clr();
    Build_in_Delay_ms(500);
    Build_in_LED_Set();
    Build_in_Delay_ms(500);
}