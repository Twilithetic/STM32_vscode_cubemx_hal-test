#include "Build_in-Led.h"

#ifdef STM32f103_version
// 初始化PC13引脚作为LED输出
void Build_in_LED_Init(void)
{
    // 使能 GPIO 时钟
    __HAL_RCC_GPIOC_CLK_ENABLE(); // 根据实际使用的 GPIO 端口修改
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = Build_in_LED_PIN;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Build_in_LED_PORT, &GPIO_InitStruct);
}

void _Build_in_Delay_us(uint32_t us)
{
    if (us == 0) return;
    // 记录开始时的计数值
    uint32_t start_ticks = SysTick->VAL;

    // 处理要延时实际要多少的 systick 并转换成 systick 来处理
    uint32_t reload   = SysTick->LOAD;                   // 71999
    uint32_t ticks_us = HAL_RCC_GetHCLKFreq() / 1000000; // HAL_RCC_GetHCLKFreq()返回的是72000000 Hz 如果你在cubemx里配置的HCLK的时钟是72Mhz的话

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

    // 从进入到这大概 40 systick

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
        if (elapsed >= ticks_num) break; // 运行一次大概 28 systicks 或者56（溢出时）都不到1us（72systicks）,担心什么,最快情况delay_us(1)是大概 68 systick
    }
}


// 封装NULL结尾字符串的发送函数
void _Build_in_SCI_Print(char *str) {
    if (str == NULL) return;
    
    uint8_t length = 0;
    while (str[length] != '\0' && length < 255) { // 防止超长字符串
        length++;
    }
    
    // 仅当DMA不忙时启动新传输

      // 启动DMA发送
      HAL_UART_Transmit_DMA(&huart1, (uint8_t*)str, length);

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

}

#else
    // 如果没有定义任何平台，给出错误提示
    #error "请定义USE_STM32或USE_TI来选择目标平台,例如#define TMS32F280049C_version"
#endif
