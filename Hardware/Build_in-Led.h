#ifndef LED_BLINK_H
#define LED_BLINK_H
#define STM32f103_version
// 根据选择的平台编译不同的代码
#ifdef TMS32F280049C_version
    // TI平台相关定义
    #include "driverlib.h"
    #include "device.h"
    #include "board.h"
    // 引脚定义
    #define Build_in_LED_PIN 23
    // 引脚操作
    #define Build_in_LED_Clr()  GPIO_writePin(Build_in_LED_PIN, 0)
    #define Build_in_LED_Set()  GPIO_writePin(Build_in_LED_PIN, 1)
    #define Build_in_LED_Toggle() GPIO_togglePin(Build_in_LED_PIN)
    // 延时计数器
    #define Build_in_Delay_us(us) DEVICE_DELAY_US(us)
    #define Build_in_Delay_ms(ms) DEVICE_DELAY_US((unsigned long long)(ms) * 1000) // 将宏定义中的乘法结果转为 64 位整数（如 unsigned long long），确保运算不溢出

#elif defined(STM32f103_version)
    // STM32平台相关定义
    #include "stm32f1xx_hal.h" // 根据你的芯片型号替换fxxx
    #include "usart.h"
    #include "tim.h"
    // 引脚定义
    #define Build_in_LED_PORT GPIOC
    #define Build_in_LED_PIN GPIO_PIN_13
    // 引脚操作
    #define Build_in_LED_Clr()  HAL_GPIO_WritePin(Build_in_LED_PORT, Build_in_LED_PIN, GPIO_PIN_RESET)
    #define Build_in_LED_Set()  HAL_GPIO_WritePin(Build_in_LED_PORT, Build_in_LED_PIN, GPIO_PIN_SET)
    // 延时计数器
    #define Build_in_Delay_us(us) _Build_in_Delay_us(us)
    #define Build_in_Delay_ms(ms) _Build_in_Delay_us((unsigned long long)(ms) * 1000)
    // 获取TIM3当前计数值（单位：us，假设定时器时钟为1MHz，1计数=1us）
    #define Timestamp_us_Count() __HAL_TIM_GET_COUNTER(&htim3) //务必保持计数从start 高（71999）到 now 低（0）的顺序；
    #define Timestamp_us_period SysTick->LOAD
    uint16_t Build_timestamp_us_Compute(uint16_t start, uint16_t now);
    // 串口DEBUG
    #define Build_in_SCI_Print(str) _Build_in_SCI_Print(str)
#else
    // 如果没有定义任何平台，给出错误提示
    #error "请定义USE_STM32或USE_TI来选择目标平台,例如#define TMS32F280049C_version"
#endif

// 初始化PC13引脚作为LED输出
void Build_in_LED_Init(void);

void _Build_in_Delay_us(uint32_t us);

void _Build_in_SCI_Print(char *str);



#endif /* LED_BLINK_H */    