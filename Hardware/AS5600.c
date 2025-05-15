
#include "AS5600.h"

void AS5600_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 GPIO 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();  // 根据实际使用的 GPIO 端口修改

    // 配置 AS5600 引脚
    GPIO_InitStruct.Pin = AS5600_SCL_PIN | AS5600_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AS5600_SCL_PORT, &GPIO_InitStruct);

    AS5600_SDA_Set();
    AS5600_SCL_Set();

}


//******************************** */
//            I2C 实现
//******************************** */

/// @brief 这个函数会在I2C上 发送一个字节 有Ack LED会亮 [AS5600_ADDRESS_W]
void AS5600_Test_ack(){
    // AS5600_SDA_Set();
    // AS5600_Delay_us(1);
    AS5600_SCL_Set();
    AS5600_Delay_us(100);
    AS5600_SCL_Clr();
    AS5600_Delay_us(100);
    // AS5600_Start();
    // AS5600_SendByte(AS5600_ADDRESS_W);
    // if(AS5600_ReceiveAck()){
    //   Build_in_LED_Set();
    // } else  
    // {
    //   Build_in_LED_Clr();
    // }
    
    // AS5600_Stop();
}

/// @brief 在SCL 置1时 如果SDA从1到0 是开始时序（读取数据时 SCL同样SCL置1 但SDA不应该变化）
/// @param  
void AS5600_Start(void){
    AS5600_SDA_Set();
    AS5600_Delay_us(1);
    AS5600_SCL_Set();
    AS5600_Delay_us(1);
    AS5600_SDA_Clr();
    AS5600_Delay_us(1); 
}


/// @brief 发送一个bit SCL置0时 放主机要发送的数据，SCL置1时 从机读取数据 从SDA上
/// @param byte 
void AS5600_SendByte(uint8_t byte){

    for (uint8_t i = 0; i < 8; i++)
    {
        
        AS5600_SCL_Clr();
        AS5600_Delay_us(1);
        if (byte & 0x80)
        {
            AS5600_SDA_Set();
        }
        else
        {
            AS5600_SDA_Clr();
        }
        AS5600_Delay_us(1);
        AS5600_SCL_Set(); // 此时从机 会读取SDA线上的电平来获取数据， 他们想两座离的很远的房子一样。
        AS5600_Delay_us(1);
        while (!AS5600_SCL_Read()); // 适应时钟延展 如果AS5600在睡觉，在交流前会把SCL置0，这个时候这个句子才有用
        AS5600_Delay_us(1);        
        byte <<= 1;
    }
    //AS5600_SCL_Clr();
    
}


/// @brief 主机接受从机确认收到字节的应答 主机的每个字节发送完成后，主机会释放SDA（SDA置1），如果从机按约定在此时拉低（SDA置0），并且主机发现有人拉低了，那么主机发送的字节就确认被收到了
/// @param  
/// @return  如果确认收到了(此时AckBit == 0)，会返回True（stdbool.h提供），由于!AckBit
bool AS5600_ReceiveAck(void){
    AS5600_SCL_Clr();
    AS5600_Delay_us(1);
    AS5600_SDA_Set();// 释放SDA
    AS5600_Delay_us(1);
    AS5600_SCL_Set();// 担心！ 万一在SCL置1时下个指令是置SDA为0 这不成Start时序了吗(。_。) 。其实不担心，下个无非是接受或发送字节的程序，他们都会在发送前将 SCL置0的，但 AS5600_SendAck_Continue()就没那么好运了
    AS5600_Delay_us(2); // 只是在示波器上好看
    while (!AS5600_SCL_Read()); // 适应时钟延展 如果AS5600在睡觉，在交流前会把SCL置0，这个时候这个句子才有用
    AS5600_Delay_us(1);
    uint8_t AckBit = AS5600_SDA_Read();
    
    return !AckBit;

}


/// @brief 接收一个Bit SCL置0时 从机放要给的数据，SCL置1时 主机读取数据在SDA上
/// @param  
/// @return 
uint8_t AS5600_ReceiveByte(void){
    uint8_t byte = 0x00;
    // 
    AS5600_SDA_Set(); // 乌鸡之谈 -->主机是开漏输出，如果此时从机的最后一个bit数据是0，不置1松开SDA的话（开漏输出不是推挽输出，只能拉低或者撒手不管（置1）），从机的数据就全是0了
    AS5600_Delay_us(1);
    for (uint8_t i = 0; i < 8; i++)
    {
        AS5600_SCL_Clr();
        AS5600_Delay_us(1);   
        AS5600_SCL_Set();
        AS5600_Delay_us(1);
        while (!AS5600_SCL_Read()); // 适应时钟延展 如果AS5600在睡觉，在交流前会把SCL置0，这个时候这个句子才有用
        AS5600_Delay_us(1);
        if(AS5600_SDA_Read() == 1){byte |= (0x80 >> i);}
    }

    return byte;

}


/// @brief 从机接受主机确认收到字节的应答，从机发送完一个字节的数据，从机会释放SDA（SDA置1），如果主机按约定在此时拉低SDA（SDA置0），并且从机发现了，拿那从机发送的字节就确认被收到啦(●'◡'●)。
void AS5600_SendAck_Continue(){
    AS5600_SCL_Clr();
    AS5600_Delay_us(1);
    AS5600_SDA_Clr();
    AS5600_Delay_us(1);
    AS5600_SCL_Set();
    AS5600_Delay_us(2); // 只是在示波器上好看
    AS5600_SCL_Clr();
    AS5600_SDA_Set(); // 先 Clr置0 再SDA松手， 不影响后面的时序

}


/// @brief 从机接受主机确认收到字节的应答，从机发送完一个字节的数据，从机会释放SDA（SDA置1），如果主机按约定在此时拉低SDA（SDA置0），并且从机发现了，拿那从机发送的字节就确认被收到啦(●'◡'●)。
void AS5600_SendAck_Done(){
    AS5600_SCL_Clr();
    AS5600_Delay_us(1);
    AS5600_SDA_Set();
    AS5600_Delay_us(1);
    AS5600_SCL_Set();
    AS5600_Delay_us(2);
}


/// @brief 在SCL 置1时 如果SDA从0到1 是停止时序（读取数据时 SCL同样SCL置1 但SDA不应该变化）
/// @param  
void AS5600_Stop(void){
    AS5600_SCL_Clr();
    AS5600_Delay_us(1);
    AS5600_SDA_Clr();
    AS5600_Delay_us(1);
    AS5600_SCL_Set();
    AS5600_Delay_us(1);
    AS5600_SDA_Set();
    AS5600_Delay_us(1);
}

