#include "MPU6050.h"


void MPU6050_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 GPIO 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();  // 根据实际使用的 GPIO 端口修改

    // 配置 MPU6050 引脚
    GPIO_InitStruct.Pin = MPU6050_SCL_PIN | MPU6050_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MPU6050_SCL_PORT, &GPIO_InitStruct);

    while (true)
    {
        MPU6050_SCL_Set();
        MPU6050_SCL_Clr();
    }
    
}

void MPU6050_start(){
    MPU6050_SDA_Set();
    MPU6050_SCL_Set();
    MPU6050_SDA_Clr();
    MPU6050_SCL_Clr();


}