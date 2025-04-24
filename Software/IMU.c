// IMU（Inertial Measurement Unit，惯性测量单元）
#include "IMU.h"

//一、核心原理四元数 \(q = [q_w, q_x, q_y, q_z]\) 表示绕单位轴 \(\vec{a} = [a_x, a_y, a_z]\) 旋转 \(\theta\) 角的旋转，初始为单位四元数 \([1, 0, 0, 0]\)。
// 通过以下步骤更新四元数：
// 陀螺仪预测：用角速度积分预测四元数变化。
// 加速度计修正：用重力向量误差修正四元数漂移（静态或低动态场景）。
#define DEG2RAD 3.1415926 / 180;

float gyro_bias[3] = {0.0f, 0.0f, 0.0f};  // 陀螺仪零偏（需预先校准）
float Accel_bias[3] = {0.0f, 0.0f, 0.0f};
float dt = 0.01f;                          // 采样时间（需根据实际频率计算）
float Kp = 0.1f, Ki = 0.001f;              // 互补滤波比例/积分系数
float e_int[3] = {0.0f, 0.0f, 0.0f};        // 积分误差累计

// 姿态 欧拉角或四元数
float roll = 0;
float pitch = 0;
float yaw = 0;
QuaternionTypedef Quaternion= {1.0f, 0.0f, 0.0f, 0.0f};  // 初始化为单位四元数;

// 运动 （x, y, z）单位向量的加速度方向 单位g
float ax = 0;
float ay = 0;
float az = 0;

void IMU_Init(){
// 陀螺仪零偏校准：
    // 在设备静止时采集一段时间数据，计算均值作为 gyro_bias。
    for (int i=0; i<1000; i++) {
        MPU6050_Update_Data();

        Accel_bias[0] += MPU6050_Data.Accel_x;
        Accel_bias[1] += MPU6050_Data.Accel_y;
        Accel_bias[2] += MPU6050_Data.Accel_z;

        gyro_bias[0] += MPU6050_Data.Gyro_x;
        gyro_bias[1] += MPU6050_Data.Gyro_y;
        gyro_bias[2] += MPU6050_Data.Gyro_z;
        HAL_Delay(1);
    }
    gyro_bias[0] /= 1000;
    gyro_bias[1] /= 1000;
    gyro_bias[2] /= 1000;

    Accel_bias[0] /= 1000;
    Accel_bias[1] /= 1000;
    Accel_bias[2] /= 1000;
    //IMU_test();
}

void IMU_Update() {
    MPU6050_Update_Data();
    // 1. 数据预处理
    // 加速度计量程 ±16g 时，比例因子为 2048 LSB/g
    ax = ( MPU6050_Data.Accel_x ) / 2048.0f;
    ay = ( MPU6050_Data.Accel_y ) / 2048.0f;
    az = ( MPU6050_Data.Accel_z ) / 2048.0f;
    // 陀螺仪量程 ±2000°/s 时，比例因子为 16.4 LSB/(°/s)
    float gx = (MPU6050_Data.Gyro_x - gyro_bias[0]) / 16.4f * DEG2RAD;
    float gy = (MPU6050_Data.Gyro_y - gyro_bias[1]) / 16.4f * DEG2RAD;
    float gz = (MPU6050_Data.Gyro_z - gyro_bias[2]) / 16.4f * DEG2RAD;

    // 2. 加速度计归一化
    float norm = sqrt(ax*ax + ay*ay + az*az);
    if (norm == 0) return;
    ax /= norm; ay /= norm; az /= norm; // 单位向量 加速度的方向

    // 3. 四元数预测与修正（Mahony滤波）
    float q_w = Quaternion.q_w, q_x = Quaternion.q_x, q_y = Quaternion.q_y, q_z = Quaternion.q_z;
    float gx_pred = 2.0f * (q_y * q_z - q_w * q_x);
    float gy_pred = 2.0f * (q_x * q_z + q_w * q_y);
    float gz_pred = 2.0f * (q_x * q_y - q_w * q_z);
    float ex = (ay * gz_pred - az * gy_pred);
    float ey = (az * gx_pred - ax * gz_pred);
    float ez = (ax * gy_pred - ay * gx_pred);

    gx += Kp * ex;
    gy += Kp * ey;
    gz += Kp * ez;
    e_int[0] += Ki * ex * dt;
    e_int[1] += Ki * ey * dt;
    e_int[2] += Ki * ez * dt;

    // 4. 四元数更新
    Quaternion.q_w += (-q_x * gx - q_y * gy - q_z * gz) * 0.5f * dt;
    Quaternion.q_x += (q_w * gx + q_y * gz - q_z * gy) * 0.5f * dt;
    Quaternion.q_y += (q_w * gy + q_z * gx - q_x * gz) * 0.5f * dt;
    Quaternion.q_z += (q_w * gz + q_x * gy - q_y * gx) * 0.5f * dt;
    norm = sqrt(Quaternion.q_w*Quaternion.q_w + Quaternion.q_x*Quaternion.q_x + Quaternion.q_y*Quaternion.q_y + Quaternion.q_z*Quaternion.q_z);
    Quaternion.q_w /= norm; Quaternion.q_x /= norm; Quaternion.q_y /= norm; Quaternion.q_z /= norm;

    // 四元数转欧拉角（Z-Y-X顺序，弧度）
    roll = atan2(2*(Quaternion.q_w*Quaternion.q_x + Quaternion.q_y*Quaternion.q_z), 1 - 2*(Quaternion.q_x*Quaternion.q_x + Quaternion.q_y*Quaternion.q_y));
    pitch = asin(2*(Quaternion.q_w*Quaternion.q_y - Quaternion.q_z*Quaternion.q_x));
    yaw = atan2(2*(Quaternion.q_w*Quaternion.q_z + Quaternion.q_x*Quaternion.q_y), 1 - 2*(Quaternion.q_y*Quaternion.q_y + Quaternion.q_z*Quaternion.q_z));
}



