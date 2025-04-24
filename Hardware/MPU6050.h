#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "tim.h"
#include "Build_in-Led.h"
#include <stdbool.h>


#define MPU6050_SCL_PORT GPIOB
#define MPU6050_SCL_PIN GPIO_PIN_14
#define MPU6050_SDA_PORT GPIOB
#define MPU6050_SDA_PIN GPIO_PIN_15



#define MPU6050_SCL_Clr()  HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_RESET)
#define MPU6050_SCL_Set()  HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_SET)
#define MPU6050_SDA_Clr()  HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_RESET)
#define MPU6050_SDA_Set()  HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_SET)
#define MPU6050_SDA_Read()  HAL_GPIO_ReadPin(MPU6050_SDA_PORT, MPU6050_SDA_PIN)

void delay_us(uint32_t us);

void MPU6050_Init(void);

void MPU6050_Test_ack();

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);

uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Start(void);

void MPU6050_SendAck_Done();

void MPU6050_Stop(void);

void MPU6050_SendByte(uint8_t byte);

uint8_t MPU6050_ReceiveByte(void);

void MPU6050_SendAck_Continue();

bool MPU6050_ReceiveAck(void);

#endif