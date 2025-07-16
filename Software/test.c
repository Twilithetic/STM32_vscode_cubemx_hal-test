#include "test.h"

void test_Init(){
    // Init
    __HAL_DBGMCU_FREEZE_TIM3(); // 断点不停止时钟
    Build_in_LED_Init();
}

void test(){
    Motor_Control();
}