#ifndef __IMU_H
#define __IMU_H

// IMU（Inertial Measurement Unit，惯性测量单元）
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "MPU6050.h"
#include <math.h>

// 四元数结构体
typedef struct {
    float q_w, q_x, q_y, q_z;  // 四元数 [w, x, y, z]
} QuaternionTypedef;

// 姿态 欧拉角或四元数
extern float roll;
extern float pitch;
extern float yaw;
extern QuaternionTypedef Quaternion;

// 运动 （x, y, z）单位向量的加速度方向 单位g
extern float ax;
extern float ay;
extern float az;


#endif // !IMU_H

void IMU_Init();

void IMU_Update();
