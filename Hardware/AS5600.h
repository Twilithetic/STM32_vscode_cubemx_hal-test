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


#define AS5600_Delay_us(us) Build_in_Delay_us(us)
#define AS5600_SCL_Clr()  HAL_GPIO_WritePin(AS5600_SCL_PORT,AS5600_SCL_PIN, RESET)
#define AS5600_SCL_Set()  HAL_GPIO_WritePin(AS5600_SCL_PORT,AS5600_SCL_PIN, SET)
#define AS5600_SDA_Clr()  HAL_GPIO_WritePin(AS5600_SDA_PORT,AS5600_SDA_PIN, RESET)
#define AS5600_SDA_Set()  HAL_GPIO_WritePin(AS5600_SDA_PORT,AS5600_SDA_PIN, SET)
#define AS5600_SDA_Read()  HAL_GPIO_ReadPin(AS5600_SDA_PORT,AS5600_SDA_PIN)
#define AS5600_SCL_Read()  HAL_GPIO_ReadPin(AS5600_SCL_PORT,AS5600_SCL_PIN)

void AS5600_Init(void);

void AS5600_Test_ack();

void AS5600_Start(void);

void AS5600_SendByte(uint8_t byte);

bool AS5600_ReceiveAck(void);

uint8_t AS5600_ReceiveByte(void);

void AS5600_SendAck_Continue();

void AS5600_SendAck_Done();

void AS5600_Stop(void);


#endif // !__AS5600_H

