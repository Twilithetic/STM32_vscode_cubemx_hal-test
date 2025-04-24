#include "MPU6050.h"

#define MPU6050_ADDRESS_W 0xD0
#define MPU6050_ADDRESS_R 0xD1


void delay_us(uint32_t us) {
    __HAL_TIM_SET_COUNTER(&htim4, 0); // 计数器清零
    __HAL_TIM_ENABLE(&htim4);         // 启动定时器
    while (__HAL_TIM_GET_COUNTER(&htim4) < us); // 轮询等待
    __HAL_TIM_DISABLE(&htim4);        // 停止定时器
}

void MPU6050_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 GPIO 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();  // 根据实际使用的 GPIO 端口修改

    // 配置 MPU6050 引脚
    GPIO_InitStruct.Pin = MPU6050_SCL_PIN | MPU6050_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MPU6050_SCL_PORT, &GPIO_InitStruct);

    MPU6050_SDA_Set();
    MPU6050_SCL_Set();

    // while (1)
    // {
    //     MPU6050_SCL_Set();
    //     delay_us(1);
    //     MPU6050_SCL_Clr(); 
    //     delay_us(1);
    // } // 273kHz

    // while (1)
    // {
    //     MPU6050_SCL_Set();

    //     MPU6050_SCL_Clr(); 

    // } // 1.56mHz


}

/// @brief 这个函数会在I2C上 发送一个字节 有Ack LED会亮 [MPU6050_ADDRESS_W]
void MPU6050_Test_ack(){
    MPU6050_Start();
    MPU6050_SendByte(MPU6050_ADDRESS_W);
    if(MPU6050_ReceiveAck()){
      Build_in_LED_Set();
    } else  
    {
      Build_in_LED_Clr();
    }
    
    MPU6050_Stop();
    HAL_Delay(1000);
}

/// @brief 这个函数会在I2C上 发送三个字节 实现写寄存器数据的功能 [MPU6050_ADDRESS_W RegAddress Data]
/// @param RegAddress 
/// @param Data 
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data){

    MPU6050_Start();

    MPU6050_SendByte(MPU6050_ADDRESS_W);
    MPU6050_ReceiveAck();
    MPU6050_SendByte(RegAddress);
    MPU6050_ReceiveAck();
    MPU6050_SendByte(Data);
    MPU6050_ReceiveAck();

    MPU6050_Stop();

}

/// @brief 这个函数会在I2C上 发送两个字节 接收一个字节 实现读寄存器数据的功能 发送[MPU6050_ADDRESS_W RegAddress] 读取到[Data]
/// @param RegAddress 
/// @param Data 
/// @return 
uint8_t MPU6050_ReadReg(uint8_t RegAddress){

    MPU6050_Start();

    MPU6050_SendByte(MPU6050_ADDRESS_W);
    MPU6050_ReceiveAck();
    MPU6050_SendByte(RegAddress);
    MPU6050_ReceiveAck();

    MPU6050_SCL_Clr();// Start要额外拉低一次 完成MPU6050_ReceiveAck的时序， MPU6050_ReceiveAck在CLK置1后就不动了而start又不会把先CLK置0，其他的MPU6050_SendByte也是在完成后CLK置1的
    delay_us(1);
    MPU6050_Start();
    MPU6050_SendByte(MPU6050_ADDRESS_R);
    MPU6050_ReceiveAck();
    uint8_t Data = MPU6050_ReceiveByte();
    MPU6050_SendAck_Done();

    MPU6050_Stop();

    return Data;
}









//******************************** */
//            I2C 实现
//******************************** */

/// @brief 在SCL 置1时 如果SDA从1到0 是开始时序（读取数据时 SCL同样SCL置1 但SDA不应该变化）
/// @param  
void MPU6050_Start(void){
    MPU6050_SDA_Set();
    delay_us(1);
    MPU6050_SCL_Set();
    delay_us(1);
    MPU6050_SDA_Clr();
    delay_us(1); 
}


/// @brief 发送一个bit SCL置0时 放主机要发送的数据，SCL置1时 从机读取数据 从SDA上
/// @param byte 
void MPU6050_SendByte(uint8_t byte){

    for (uint8_t i = 0; i < 8; i++)
    {
        
        MPU6050_SCL_Clr();
        delay_us(1);
        if (byte & 0x80)
        {
            MPU6050_SDA_Set();
        }
        else
        {
            MPU6050_SDA_Clr();
        }
        delay_us(1);
        MPU6050_SCL_Set(); // 此时从机 会读取SDA线上的电平来获取数据， 他们想两座离的很远的房子一样。
        delay_us(1);
        byte <<= 1;
    }
    //MPU6050_SCL_Clr();
    
}


/// @brief 主机接受从机确认收到字节的应答 主机的每个字节发送完成后，主机会释放SDA（SDA置1），如果从机按约定在此时拉低（SDA置0），并且主机发现有人拉低了，那么主机发送的字节就确认被收到了
/// @param  
/// @return  如果确认收到了(此时AckBit == 0)，会返回True（stdbool.h提供），由于!AckBit
bool MPU6050_ReceiveAck(void){
    MPU6050_SCL_Clr();
    delay_us(1);
    MPU6050_SDA_Set();// 释放SDA
    delay_us(1);
    MPU6050_SCL_Set();
    delay_us(1);
    uint8_t AckBit = MPU6050_SDA_Read();
    
    return !AckBit;

}


/// @brief 接收一个Bit SCL置0时 从机放要给的数据，SCL置1时 主机读取数据在SDA上
/// @param  
/// @return 
uint8_t MPU6050_ReceiveByte(void){
    uint8_t byte = 0x00;
    MPU6050_SDA_Set(); // 主机是开漏输出，如果此时从机的最后一个bit数据是0，不置1松开SDA的话（开漏输出不是推挽输出，只能拉低或者撒手不管（置1）），从机的数据就全是0了
    delay_us(1);
    for (uint8_t i = 0; i < 8; i++)
    {
        MPU6050_SCL_Clr();
        delay_us(1);   
        MPU6050_SCL_Set();
        delay_us(1);
        if(MPU6050_SDA_Read() == 1){byte |= (0x80 >> i);}
    }

    return byte;

}


/// @brief 从机接受主机确认收到字节的应答，从机发送完一个字节的数据，从机会释放SDA（SDA置1），如果主机按约定在此时拉低SDA（SDA置0），并且从机发现了，拿那从机发送的字节就确认被收到啦(●'◡'●)。
void MPU6050_SendAck_Continue(){
    MPU6050_SCL_Clr();
    delay_us(1);
    MPU6050_SDA_Clr();
    delay_us(1);
    MPU6050_SCL_Set();
    delay_us(1);
}


/// @brief 从机接受主机确认收到字节的应答，从机发送完一个字节的数据，从机会释放SDA（SDA置1），如果主机按约定在此时拉低SDA（SDA置0），并且从机发现了，拿那从机发送的字节就确认被收到啦(●'◡'●)。
void MPU6050_SendAck_Done(){
    MPU6050_SCL_Clr();
    delay_us(1);
    MPU6050_SDA_Set();
    delay_us(1);
    MPU6050_SCL_Set();
    delay_us(1);
}


/// @brief 在SCL 置1时 如果SDA从0到1 是停止时序（读取数据时 SCL同样SCL置1 但SDA不应该变化）
/// @param  
void MPU6050_Stop(void){
    MPU6050_SCL_Clr();
    delay_us(1);
    MPU6050_SDA_Clr();
    delay_us(1);
    MPU6050_SCL_Set();
    delay_us(1);
    MPU6050_SDA_Set();
    delay_us(1);
}