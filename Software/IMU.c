// IMU（Inertial Measurement Unit，惯性测量单元）
#include "IMU.h"


void IMU_Init(){


   
    //IMU_test();
}

uint8_t test_data = 0;
void IMU_test(){
    MPU6050_WriteReg(0x19, 0xAA); // 如果退出sleep 而且写入有效 那么 test_data应该是0xAA
    test_data = MPU6050_ReadReg(0x19);
}
MPU6050_Data_typedef test_data2;
void IMU_test2(){
    MPU6050_Update_Data();
}

uint8_t dataReg;
MPU6050_Data_typedef data;
uint8_t DataH, DataL;
void IMU_test3(){

    dataReg = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);

    MPU6050_Start();
    MPU6050_SendByte(0xD0);
    MPU6050_ReceiveAck();
    // 设置读取的开始地址 0x3B
    MPU6050_SendByte(MPU6050_ACCEL_ZOUT_H);
    MPU6050_ReceiveAck();

    MPU6050_SCL_Clr();// Start要额外拉低一次 完成MPU6050_ReceiveAck的时序， MPU6050_ReceiveAck在CLK置1后就不动了而start又不会把先CLK置0，其他的MPU6050_SendByte也是在完成后CLK置1的
    delay_us(1);
    MPU6050_Start();
    MPU6050_SendByte(0xD1);
    MPU6050_ReceiveAck();

    DataH = MPU6050_ReceiveByte();
    MPU6050_SendAck_Continue();
    DataL = MPU6050_ReceiveByte();
    MPU6050_SendAck_Done();
    data.Accel_z = (DataH << 8) | DataL;

    MPU6050_Stop();

}


MPU6050_Data_typedef IMU_Updata_Data() {


    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    data.Accel_x = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    data.Accel_y = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    data.Accel_z = (DataH << 8) | DataL;

    return data;
}