#ifndef __IMU_H
#define __IMU_H

// IMU（Inertial Measurement Unit，惯性测量单元）
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "MPU6050.h"




#endif // !IMU_H

void IMU_Init();

void IMU_test();

void IMU_test2();

void IMU_test3();

MPU6050_Data_typedef IMU_Updata_Data();
