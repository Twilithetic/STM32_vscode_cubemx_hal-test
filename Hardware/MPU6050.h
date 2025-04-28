#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "tim.h"
#include "Build_in-Led.h"
#include <stdbool.h>
#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


#define MPU6050_SCL_PORT GPIOB
#define MPU6050_SCL_PIN GPIO_PIN_14
#define MPU6050_SDA_PORT GPIOB
#define MPU6050_SDA_PIN GPIO_PIN_15

#define IS_UPLOAD_FIRMWARE false

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
    float pitch;
    float roll;
    float yaw;
    float accel_x;
    float accel_y;
    float accel_z;
} MPU6050_DMP_Data_typedef;


extern uint8_t ID;
extern MPU6050_Data_typedef MPU6050_Data; // 生的数据
extern MPU6050_DMP_Data_typedef MPU6050_DMP_Data;



void delay_us(uint16_t us);

void MPU6050_Init(void);

void MPU6050_Test_ack();

void MPU6050_Reg_Write(uint8_t RegAddress, uint8_t Data);

uint8_t MPU6050_Reg_Read(uint8_t RegAddress);

void MPU6050_Update_Data(void);

void MPU6050_Updata_DMP_Data(void);

uint8_t MPU6050_Reg_Write_DMP_Lib_use(uint8_t MPU6050_Address, uint8_t RegAddress, uint8_t length, uint8_t *Data);

uint8_t MPU6050_Reg_Read_DMP_Lib_use(uint8_t MPU6050_Address, uint8_t RegAddress, uint8_t length, uint8_t *Data);

void MPU6050_Start(void);

void MPU6050_SendAck_Done();

void MPU6050_Stop(void);

void MPU6050_SendByte(uint8_t byte);

uint8_t MPU6050_ReceiveByte(void);

void MPU6050_SendAck_Continue();

bool MPU6050_ReceiveAck(void);


#define MPU6050_SELF_TESTX		0X0D	//自检寄存器X
#define MPU6050_SELF_TESTY		0X0E	//自检寄存器Y
#define MPU6050_SELF_TESTZ		0X0F	//自检寄存器Z
#define MPU6050_SELF_TESTA		0X10	//自检寄存器A
#define MPU6050_SMPLRT_DIV		0X19	//采样频率分频器
#define MPU6050_CONFIG			0X1A	//配置寄存器
#define MPU6050_GYRO_CONFIG		0X1B	//陀螺仪配置寄存器
#define MPU6050_ACCEL_CONFIG	0X1C	//加速度计配置寄存器
#define MPU6050_MOTION_DET		0X1F	//运动检测阀值设置寄存器
#define MPU6050_FIFO_EN			0X23	//FIFO使能寄存器
#define MPU6050_I2CMST_CTRL		0X24	//IIC主机控制寄存器
#define MPU6050_I2CSLV0_ADDR	0X25	//IIC从机0器件地址寄存器
#define MPU6050_I2CSLV0			0X26	//IIC从机0数据地址寄存器
#define MPU6050_I2CSLV0_CTRL	0X27	//IIC从机0控制寄存器
#define MPU6050_I2CSLV1_ADDR	0X28	//IIC从机1器件地址寄存器
#define MPU6050_I2CSLV1			0X29	//IIC从机1数据地址寄存器
#define MPU6050_I2CSLV1_CTRL	0X2A	//IIC从机1控制寄存器
#define MPU6050_I2CSLV2_ADDR	0X2B	//IIC从机2器件地址寄存器
#define MPU6050_I2CSLV2			0X2C	//IIC从机2数据地址寄存器
#define MPU6050_I2CSLV2_CTRL	0X2D	//IIC从机2控制寄存器
#define MPU6050_I2CSLV3_ADDR	0X2E	//IIC从机3器件地址寄存器
#define MPU6050_I2CSLV3			0X2F	//IIC从机3数据地址寄存器
#define MPU6050_I2CSLV3_CTRL	0X30	//IIC从机3控制寄存器
#define MPU6050_I2CSLV4_ADDR	0X31	//IIC从机4器件地址寄存器
#define MPU6050_I2CSLV4			0X32	//IIC从机4数据地址寄存器
#define MPU6050_I2CSLV4_DO		0X33	//IIC从机4写数据寄存器
#define MPU6050_I2CSLV4_CTRL	0X34	//IIC从机4控制寄存器
#define MPU6050_I2CSLV4_DI		0X35	//IIC从机4读数据寄存器

#define MPU6050_I2CMST_STA		0X36	//IIC主机状态寄存器
#define MPU6050_INTBP_CFG		0X37	//中断/旁路设置寄存器
#define MPU6050_INT_EN			0X38	//中断使能寄存器
#define MPU6050_INT_STA			0X3A	//中断状态寄存器

#define MPU6050_ACCEL_XOUTH		0X3B	//加速度值,X轴高8位寄存器
#define MPU6050_ACCEL_XOUTL		0X3C	//加速度值,X轴低8位寄存器
#define MPU6050_ACCEL_YOUTH		0X3D	//加速度值,Y轴高8位寄存器
#define MPU6050_ACCEL_YOUTL		0X3E	//加速度值,Y轴低8位寄存器
#define MPU6050_ACCEL_ZOUTH		0X3F	//加速度值,Z轴高8位寄存器
#define MPU6050_ACCEL_ZOUTL		0X40	//加速度值,Z轴低8位寄存器

#define MPU6050_TEMP_OUTH		0X41	//温度值高八位寄存器
#define MPU6050_TEMP_OUTL		0X42	//温度值低8位寄存器

#define MPU6050_GYRO_XOUTH		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU6050_GYRO_XOUTL		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU6050_GYRO_YOUTH		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU6050_GYRO_YOUTL		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU6050_GYRO_ZOUTH		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU6050_GYRO_ZOUTL		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU6050_I2CSLV0_DO		0X63	//IIC从机0数据寄存器
#define MPU6050_I2CSLV1_DO		0X64	//IIC从机1数据寄存器
#define MPU6050_I2CSLV2_DO		0X65	//IIC从机2数据寄存器
#define MPU6050_I2CSLV3_DO		0X66	//IIC从机3数据寄存器

#define MPU6050_I2CMST_DELAY	0X67	//IIC主机延时管理寄存器
#define MPU6050_SIGPATH_RST		0X68	//信号通道复位寄存器
#define MPU6050_MDETECT_CTRL	0X69	//运动检测控制寄存器
#define MPU6050_USER_CTRL		0X6A	//用户控制寄存器
#define MPU6050_PWR_MGMT_1		0X6B	//电源管理寄存器1
#define MPU6050_PWR_MGMT_2		0X6C	//电源管理寄存器2 
#define MPU6050_FIFO_CNTH		0X72	//FIFO计数寄存器高八位
#define MPU6050_FIFO_CNTL		0X73	//FIFO计数寄存器低八位
#define MPU6050_FIFO_RW			0X74	//FIFO读写寄存器
#define MPU6050_DEVICE_ID		0X75	//器件ID寄存器

//DMP相关
#define DMP_BANL_SEL            0x6D    //固件读写
#define DMP_MEM_R_W             0x6F
#define DMP_PRGM_START_H        0x70   //固件写入完成后要修改程序的开始位置

#define DMP_CODE_SIZE           3062
#define DMP_LOAD_CHUNK          16      /* Must divide evenly into st.hw->bank_size to avoid bank crossings. */
#define DMP_START_ADDRESS       0x0400



#endif