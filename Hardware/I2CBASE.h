#ifndef __I2CBASE_H
#define __I2CBASE_H

#include "stm32f1xx_hal.h" // 根据你的芯片型号替换fxxx
#include "stdbool.h"
#include "Build_in-Led.h"

#define I2CBASE_ADDRESS_W 0x11
#define I2CBASE_ADDRESS_R 0x22


#define I2CBASE_SCL_PORT GPIOB
#define I2CBASE_SCL_PIN GPIO_PIN_14
#define I2CBASE_SDA_PORT GPIOB
#define I2CBASE_SDA_PIN GPIO_PIN_15


typedef struct 
{
    float Angle; // ex: 19.1度
    uint8_t AGC; //ex: 128 磁场强度正常范围: AGC=50-200, magnitude=500-4000
} I2CBASE_Data_typedef;

extern I2CBASE_Data_typedef I2CBASE_Data;


#define I2CBASE_Delay_us(us) Build_in_Delay_us(us)
#define I2CBASE_SCL_Clr()  HAL_GPIO_WritePin(I2CBASE_SCL_PORT,I2CBASE_SCL_PIN, RESET)
#define I2CBASE_SCL_Set()  HAL_GPIO_WritePin(I2CBASE_SCL_PORT,I2CBASE_SCL_PIN, SET)
#define I2CBASE_SDA_Clr()  HAL_GPIO_WritePin(I2CBASE_SDA_PORT,I2CBASE_SDA_PIN, RESET)
#define I2CBASE_SDA_Set()  HAL_GPIO_WritePin(I2CBASE_SDA_PORT,I2CBASE_SDA_PIN, SET)
#define I2CBASE_SDA_Read()  HAL_GPIO_ReadPin(I2CBASE_SDA_PORT,I2CBASE_SDA_PIN)
#define I2CBASE_SCL_Read()  HAL_GPIO_ReadPin(I2CBASE_SCL_PORT,I2CBASE_SCL_PIN)

void I2CBASE_Init(void);

void Updata_I2CBASE_Data();

void Updata_Angle_Data();

void Updata_AGC_Data();

void I2CBASE_Configure_ZERO_Angle_in_Hardware(void);

void _init_config(void);

void _init_config_SF_FTH(void);

void I2CBASE_Test_ack();

void I2CBASE_Start(void);

void I2CBASE_SendByte(uint8_t byte);

bool I2CBASE_ReceiveAck(void);

uint8_t I2CBASE_ReceiveByte(void);

void I2CBASE_SendAck_Continue();

void I2CBASE_SendAck_Done();

void I2CBASE_Stop(void);

// AS5600寄存器地址定义
#define AS5600_ZMCO       0x00  // 自动调零配置
#define AS5600_ZPOS_H     0x01  // 零点位置高位
#define AS5600_ZPOS_L     0x02  // 零点位置低位
#define AS5600_MPOS_H     0x03  // 最大位置高位
#define AS5600_MPOS_L     0x04  // 最大位置低位
#define AS5600_MANG_H     0x05  // 最大角度高位
#define AS5600_MANG_L     0x06  // 最大角度低位
#define AS5600_CONF_H     0x07  // 配置寄存器高位
#define AS5600_CONF_L     0x08  // 配置寄存器低位
#define AS5600_RAWANG_H   0x0C  // 原始角度高位
#define AS5600_RAWANG_L   0x0D  // 原始角度低位
#define AS5600_ANGLE_H    0x0E  // 角度值高位（带动态校准）
#define AS5600_ANGLE_L    0x0F  // 角度值低位
#define AS5600_STATUS     0x0B  // 状态寄存器
#define AS5600_AGC        0x1A  // 自动增益控制
#define AS5600_MAGN_H     0x1B  // 磁场强度高位
#define AS5600_MAGN_L     0x1C  // 磁场强度低位



#endif // !__I2CBASE_H

