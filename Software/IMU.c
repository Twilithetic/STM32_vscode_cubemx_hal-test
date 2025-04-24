// IMU（Inertial Measurement Unit，惯性测量单元）
#include "IMU.h"

uint8_t ID = 0;

void IMU_Init(){
    ID = MPU6050_ReadReg(0x75);
    MPU6050_WriteReg(0x6B, 0x00); // 解除MPU6050的睡眠模式 才能对其他寄存器写入数据
    IMU_test();
}

uint8_t test_data = 0;
void IMU_test(){
    MPU6050_WriteReg(0x19, 0xAA); // 如果退出sleep 而且写入有效 那么 test_data应该是0xAA
    test_data = MPU6050_ReadReg(0x19);
}