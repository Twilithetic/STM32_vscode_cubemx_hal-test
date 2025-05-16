#ifndef __AS5600_H
#define __AS5600_H

#include "stm32f1xx_hal.h" // 根据你的芯片型号替换fxxx
#include "stdbool.h"
#include "Build_in-Led.h"

#define AS5600_ADDRESS_W 0x6C
#define AS5600_ADDRESS_R 0x6D


#define AS5600_SCL_PORT GPIOB
#define AS5600_SCL_PIN GPIO_PIN_14
#define AS5600_SDA_PORT GPIOB
#define AS5600_SDA_PIN GPIO_PIN_15


typedef struct 
{
    float Angle; // ex: 19.1度
    uint8_t AGC; //ex: 128 反应安装的好不好的
} AS5600_Data_typedef;

extern AS5600_Data_typedef AS5600_Data;


#define AS5600_Delay_us(us) Build_in_Delay_us(us)
#define AS5600_SCL_Clr()  HAL_GPIO_WritePin(AS5600_SCL_PORT,AS5600_SCL_PIN, RESET)
#define AS5600_SCL_Set()  HAL_GPIO_WritePin(AS5600_SCL_PORT,AS5600_SCL_PIN, SET)
#define AS5600_SDA_Clr()  HAL_GPIO_WritePin(AS5600_SDA_PORT,AS5600_SDA_PIN, RESET)
#define AS5600_SDA_Set()  HAL_GPIO_WritePin(AS5600_SDA_PORT,AS5600_SDA_PIN, SET)
#define AS5600_SDA_Read()  HAL_GPIO_ReadPin(AS5600_SDA_PORT,AS5600_SDA_PIN)
#define AS5600_SCL_Read()  HAL_GPIO_ReadPin(AS5600_SCL_PORT,AS5600_SCL_PIN)

void AS5600_Init(void);

void AS5600_Configure_ZERO_Angle_in_Hardware(void);

void Updata_AS5600_Data();

void Updata_Angle_Data();

void Updata_AGC_Data();

void _init_config(void);

void _init_config_SF_FTH(void);

void AS5600_Test_ack();

void AS5600_Start(void);

void AS5600_SendByte(uint8_t byte);

bool AS5600_ReceiveAck(void);

uint8_t AS5600_ReceiveByte(void);

void AS5600_SendAck_Continue();

void AS5600_SendAck_Done();

void AS5600_Stop(void);

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


#endif // !__AS5600_H

