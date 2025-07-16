#include "test.h"

void test_Init(){
    // Init
    __HAL_DBGMCU_FREEZE_TIM3(); // 断点不停止时钟
    Build_in_LED_Init();
}

static char buffer_update[256];
uint8_t Build_in_SCI_log_ms_cnt;
void Build_in_SCI_log(){

    Build_in_SCI_Print(buffer_update);
    
}
float dt_ms;
uint8_t is_log = 1;
void test(){

}
