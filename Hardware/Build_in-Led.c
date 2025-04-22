#include "Build_in-Led.h"

// 初始化PC13引脚作为LED输出
void LED_Init(void) {
    // 使能 GPIO 时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();  // 根据实际使用的 GPIO 端口修改
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = Build_in_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Build_in_LED_PORT, &GPIO_InitStruct);
}

// 控制LED闪烁
void LED_Blink() {
    // 点亮PC13引脚
    Build_in_LED_Set();
    HAL_Delay(1000);
    Build_in_LED_Clr();
    HAL_Delay(1000);
}    