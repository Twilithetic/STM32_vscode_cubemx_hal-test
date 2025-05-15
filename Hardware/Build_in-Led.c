#include "Build_in-Led.h"


// 初始化PC13引脚作为LED输出
void Build_in_LED_Init(void) {
    // 使能 GPIO 时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();  // 根据实际使用的 GPIO 端口修改
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = Build_in_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Build_in_LED_PORT, &GPIO_InitStruct);
}

uint32_t tcnt = 0;

void Build_in_Delay_us(uint32_t us)
{
    //uint32_t start_ticks2 = SysTick->VAL;
     if (us == 0) return;
    
    uint32_t reload = SysTick->LOAD;       // 71999
    uint32_t ticks_us = HAL_RCC_GetHCLKFreq() / (1000 * 1000); // HAL_RCC_GetHCLKFreq()返回的是72000000 Hz 如果你在cubemx里配置的HCLK的时钟是72Mhz的话
    
    // 计算目标延时对应的SysTick计数值
    uint32_t ticks_num = us * ticks_us;
    
    // 处理溢出情况（超过1ms的延时）
    if (ticks_num > reload) {
        uint32_t ms = us / 1000;
        HAL_Delay(ms);  // 使用HAL库的毫秒延时处理整毫秒部分
        us = us % 1000; // 剩余微秒部分
        if (us == 0) return;
        ticks_num = us * ticks_us;
    }
    
    // 记录开始时的计数值
    uint32_t start_ticks = SysTick->VAL;
    
    // 循环等待直到达到目标延时
    while (1) {
        uint32_t current_ticks = SysTick->VAL;
        uint32_t elapsed;
        
        // 计算已经过去的计数值（考虑计数器溢出）
        if (current_ticks <= start_ticks) { // 注意 systick 是从7199 到0 下降的，这里是没有溢出的情况
            elapsed = start_ticks - current_ticks;
        } else { // 现在是溢出的
            elapsed = start_ticks + (reload - current_ticks);
        }
        
        // 如果达到或超过目标延时，则退出
        if (elapsed >= ticks_num) break; // 运行一次大概 28 systicks 或者56（溢出时）都不到1us（72systicks）,担心什么
    }

        // uint32_t HCLK_Freq_Mhz = HAL_RCC_GetHCLKFreq() / (1000 * 1000);
    // uint32_t ticks = us *  HCLK_Freq; // 计算需要延时的总时钟数
    // uint32_t reload = SysTick->LOAD;  // 获取SysTick的重装载值
    // uint32_t told = SysTick->VAL;  // 记录当前计数值
    
    // while(1)
    // {
    //     uint32_t tnow = SysTick->VAL;  // 读取当前计数值
    //     if(tnow != told)  // 如果计数值发生了变化
    //     {
    //         uint32_t diff;
    //         if(tnow < told)  // 正常计数情况
    //             diff = told - tnow;
    //         else  // 发生了计数溢出的情况
    //             diff = reload - tnow + told;
                
    //         tcnt += diff;  // 累计经过的时钟数
    //         told = tnow;  // 更新上次记录值
            
    //         if(tcnt >= ticks)  // 如果累计时钟数达到了目标值
    //             break;  // 退出循环，延时结束
    //     }
    // }
}
