#include "test.h"

void test_Init(){
}

void test(){
    Build_in_LED_Set();
    Build_in_Delay_us(1000 * 500);
    Build_in_LED_Clr();
    Build_in_Delay_us(1000 * 500);
}
