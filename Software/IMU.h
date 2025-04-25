#ifndef __IMU_H
#define __IMU_H

// IMU（Inertial Measurement Unit，惯性测量单元）
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "MPU6050.h"
#include "SH1122.h"
#include <math.h>
#include <stdio.h>

// 四元数结构体

// 姿态 欧拉角或四元数
extern float IMU_roll;
extern float IMU_pitch;
extern float IMU_yaw;


// 运动 （x, y, z）单位向量的加速度方向 单位g
extern float IMU_accel_x;
extern float IMU_accel_y;
extern float IMU_accel_z;


#endif // !IMU_H

void IMU_Init();

void IMU_test();

void IMU_Update();
