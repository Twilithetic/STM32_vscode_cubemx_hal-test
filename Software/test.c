#include "test.h"

void test_Init(){
    Build_in_LED_Init();
}

void test(){
    Build_in_LED_Set();
    Build_in_Delay_ms(500);
    Build_in_LED_Clr();
    Build_in_Delay_ms(500);
    Build_in_SCI_Print("\r11111\n");
}
