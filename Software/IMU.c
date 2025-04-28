// IMU（Inertial Measurement Unit，惯性测量单元）
#include "IMU.h"



void IMU_Init(){
// 陀螺仪零偏校准：
    MPU6050_Init();
    SH1122_Init();
    //IMU_test();
}


void IMU_test(){
    MPU6050_Updata_DMP_Data();
    // accel proc
    // char buffer_accel[32]; // 定义一个足够大的缓冲区来存储格式化后的字符串
    // snprintf(buffer_accel, sizeof(buffer_accel), "accel:(%.2fg,%.2fg,%.2fg)", IMU_accel_x, IMU_accel_y, IMU_accel_z);
    // SH1122_ShowString(0,0,buffer_accel,ASCII_0816_FORMAT);
    // gyro proc
    char buffer_gyro[48]; // 定义一个足够大的缓冲区来存储格式化后的字符串
    snprintf(buffer_gyro, sizeof(buffer_gyro), "roll:%.2f pitch:%.2f yaw:%.2f", MPU6050_DMP_Data.roll, MPU6050_DMP_Data.pitch, MPU6050_DMP_Data.yaw);
    SH1122_ShowString(0,16,buffer_gyro,ASCII_0816_FORMAT);

    SH1122_Update();

}







