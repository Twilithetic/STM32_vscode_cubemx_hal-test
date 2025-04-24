// IMU（Inertial Measurement Unit，惯性测量单元）
#include "IMU.h"

uint8_t ID = 0;

void IMU_Init(){
    ID = MPU6050_ReadReg(0x75);
}