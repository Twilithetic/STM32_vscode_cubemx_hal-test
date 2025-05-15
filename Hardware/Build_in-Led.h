#ifndef LED_BLINK_H
#define LED_BLINK_H

#include "stm32f1xx_hal.h" // 根据你的芯片型号替换fxxx


#define Build_in_LED_PORT GPIOC
#define Build_in_LED_PIN GPIO_PIN_13


#define Build_in_LED_Clr()  HAL_GPIO_WritePin(Build_in_LED_PORT, Build_in_LED_PIN, GPIO_PIN_RESET)
#define Build_in_LED_Set()  HAL_GPIO_WritePin(Build_in_LED_PORT, Build_in_LED_PIN, GPIO_PIN_SET)


// 初始化PC13引脚作为LED输出
void Build_in_LED_Init(void);

void Build_in_Delay_us(uint32_t us);

#endif /* LED_BLINK_H */    