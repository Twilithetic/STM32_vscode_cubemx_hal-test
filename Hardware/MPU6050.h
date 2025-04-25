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


#define delay_us(us) TIM4_delay_us(us)
#define MPU6050_SCL_Clr()  HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_RESET)
#define MPU6050_SCL_Set()  HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_SET)
#define MPU6050_SDA_Clr()  HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_RESET)
#define MPU6050_SDA_Set()  HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_SET)
#define MPU6050_SDA_Read()  HAL_GPIO_ReadPin(MPU6050_SDA_PORT, MPU6050_SDA_PIN)

typedef struct {
    int16_t Accel_x;
    int16_t Accel_y;
    int16_t Accel_z;
    int16_t Tempature;
    int16_t Gyro_x;
    int16_t Gyro_y;
    int16_t Gyro_z;
} MPU6050_Data_typedef;

typedef struct {
    float q_w, q_x, q_y, q_z;  // 四元数 [w, x, y, z]
} QuaternionTypedef;

extern uint8_t ID;
extern MPU6050_Data_typedef MPU6050_Data; // 生的数据

extern QuaternionTypedef Quaternion; // 经过硬件DMP的四元数数据



void delay_us(uint16_t us);

void MPU6050_Init(void);

void MPU6050_Test_ack();

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);

uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Update_Data(void);

void MPU6050_Start(void);

void MPU6050_SendAck_Done();

void MPU6050_Stop(void);

void MPU6050_SendByte(uint8_t byte);

uint8_t MPU6050_ReceiveByte(void);

void MPU6050_SendAck_Continue();

bool MPU6050_ReceiveAck(void);


#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define MPU6050_USER_CTRL       0x6A
#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75



#endif