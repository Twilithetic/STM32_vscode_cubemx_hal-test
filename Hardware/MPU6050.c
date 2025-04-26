#include "MPU6050.h"

#define MPU6050_ADDRESS_W 0xD0
#define MPU6050_ADDRESS_R 0xD1

QuaternionTypedef Quaternion= {1.0f, 0.0f, 0.0f, 0.0f};  // 初始化为单位四元数;
MPU6050_Data_typedef MPU6050_Data;
uint8_t ID = 0;

/// @brief 没有这个 I2C会跑到接近 800kHz 的速度，而MPU6050 的fastI2C 只有500kHz 从机跟不上主机的速度 这个还要TIM4的硬件资源
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
/// @param us 
void TIM4_delay_us(uint16_t us) {
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

    // 初始化了哪些寄存器
	// MPU_PWR_MGMT1_REG,0X80 复位MPU6050
	// MPU_PWR_MGMT1_REG,0X00 唤醒MPU6050 
	// MPU_GYRO_CFG_REG，0x18 陀螺仪传感器,±2000dps
	// MPU_ACCEL_CFG_REG，0x18 加速度传感器,±2g
	// MPU_SAMPLE_RATE_REG，0x19 设置采样率50Hz
	// MPU_INT_EN_REG,0X00 关闭所有中断
	// MPU_USER_CTRL_REG,0X00 I2C主模式关闭
	// MPU_FIFO_EN_REG,0X00 关闭FIFO
	// MPU_INTBP_CFG_REG,0X80 INT引脚低电平有效

	// MPU_PWR_MGMT1_REG,0X01 设置CLKSEL,PLL X轴为参考
	// MPU_PWR_MGMT2_REG,0X00 加速度与陀螺仪都工作

    ID = MPU6050_ReadReg(0x75);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 解除MPU6050的睡眠模式 采样时钟为陀螺仪x的时钟 才能对其他寄存器写入数据
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); // 6轴采样待机位，不待机

    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 时钟分频， 10分频
    MPU6050_WriteReg(MPU6050_CONFIG, 0x03); // 滤波器的值 5ms延迟

    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪满量程选择
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计满量程选择

    MPU6050_WriteReg(MPU6050_USER_CTRL, 0x04); // 寄存器启用 DMP DMP_EN=1（手册上不是这样写的，豆包这样说）
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


/// @brief 发送两个字节 接受14个字节 发送[MPU6050_ADDRESS_W MPU6050_ACCEL_XOUT_H] 接受[MPU6050_ACCEL_XOUT_H .....]一口气从 0x19 读到 0x48 并把数据都放到IMU_Data里 主打一个高效 大概 1ms耗时
/// @param  
/// @return 
void MPU6050_Update_Data(void) {
    uint8_t DataH, DataL;
    
    MPU6050_Start();
    MPU6050_SendByte(MPU6050_ADDRESS_W);
    MPU6050_ReceiveAck();
    // 设置读取的开始地址 0x3B
    MPU6050_SendByte(MPU6050_ACCEL_XOUT_H);
    MPU6050_ReceiveAck();

    MPU6050_SCL_Clr();// Start要额外拉低一次 完成MPU6050_ReceiveAck的时序， MPU6050_ReceiveAck在CLK置1后就不动了而start又不会把先CLK置0，其他的MPU6050_SendByte也是在完成后CLK置1的
    delay_us(1);
    MPU6050_Start();
    MPU6050_SendByte(MPU6050_ADDRESS_R);
    MPU6050_ReceiveAck();

    // Accel_x
    DataH = MPU6050_ReceiveByte();// 0x3B的数据
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();// 0x3C的数据
    MPU6050_SendAck_Continue();
    MPU6050_Data.Accel_x = (DataH << 8) | DataL;

    // Accel_y
    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    MPU6050_Data.Accel_y = (DataH << 8) | DataL;
    
    // Accel_z
    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    MPU6050_Data.Accel_z = (DataH << 8) | DataL;

    // Tempature
    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    MPU6050_Data.Tempature = (DataH << 8) | DataL;

    // Gyro_x
    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    MPU6050_Data.Gyro_x = (DataH << 8) | DataL;

    // Gyro_y
    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    MPU6050_Data.Gyro_y = (DataH << 8) | DataL;

    // Gyro_z
    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Done();
    MPU6050_Data.Gyro_z = (DataH << 8) | DataL;

    MPU6050_Stop();
}


void MPU6050_Update_Quaternion(void){
    MPU6050_Start();
    MPU6050_SendByte(MPU6050_ADDRESS_W);
    MPU6050_ReceiveAck();
    // 设置读取的开始地址 0x3B
    MPU6050_SendByte(MPU6050_ACCEL_XOUT_H);
    MPU6050_ReceiveAck();

    MPU6050_SCL_Clr();// Start要额外拉低一次 完成MPU6050_ReceiveAck的时序， MPU6050_ReceiveAck在CLK置1后就不动了而start又不会把先CLK置0，其他的MPU6050_SendByte也是在完成后CLK置1的
    delay_us(1);
    MPU6050_Start();
    MPU6050_SendByte(MPU6050_ADDRESS_R);
    MPU6050_ReceiveAck();

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
    MPU6050_SCL_Set();// 担心！ 万一在SCL置1时下个指令是置SDA为0 这不成Start时序了吗(。_。) 。其实不担心，下个无非是接受或发送字节的程序，他们都会在发送前将 SCL置0的，但 MPU6050_SendAck_Continue()就没那么好运了
    delay_us(2); // 只是在示波器上好看
    uint8_t AckBit = MPU6050_SDA_Read();
    
    return !AckBit;

}


/// @brief 接收一个Bit SCL置0时 从机放要给的数据，SCL置1时 主机读取数据在SDA上
/// @param  
/// @return 
uint8_t MPU6050_ReceiveByte(void){
    uint8_t byte = 0x00;
    // 
    MPU6050_SDA_Set(); // 乌鸡之谈 -->主机是开漏输出，如果此时从机的最后一个bit数据是0，不置1松开SDA的话（开漏输出不是推挽输出，只能拉低或者撒手不管（置1）），从机的数据就全是0了
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
    delay_us(2); // 只是在示波器上好看
    MPU6050_SCL_Clr();
    MPU6050_SDA_Set(); // 先 Clr置0 再SDA松手， 不影响后面的时序

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