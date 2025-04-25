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
float IMU_roll = 0;
float IMU_pitch = 0;
float IMU_yaw = 0;


// 运动 （x, y, z）单位向量的加速度方向 单位g
float IMU_accel_x = 0;
float IMU_accel_y = 0;
float IMU_accel_z = 0;

void IMU_Init(){
// 陀螺仪零偏校准：
    MPU6050_Init();
    SH1122_Init();
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


void IMU_test(){
    IMU_Update();
    // accel proc
    char buffer_accel[32]; // 定义一个足够大的缓冲区来存储格式化后的字符串
    snprintf(buffer_accel, sizeof(buffer_accel), "accel:(%.2fg,%.2fg,%.2fg)", IMU_accel_x, IMU_accel_y, IMU_accel_z);
    SH1122_ShowString(0,0,buffer_accel,ASCII_0816_FORMAT);
    // gyro proc
    char buffer_gyro[48]; // 定义一个足够大的缓冲区来存储格式化后的字符串
    snprintf(buffer_gyro, sizeof(buffer_gyro), "roll:%.2f pitch:%.2f yaw:%.2f", IMU_roll, IMU_pitch, IMU_yaw);
    SH1122_ShowString(0,16,buffer_gyro,ASCII_0816_FORMAT);

    SH1122_Update();

}

void IMU_Update() {
    MPU6050_Update_Data();
    // 1. 数据预处理
    // 加速度计量程 ±16g 时，比例因子为 2048 LSB/g
    IMU_accel_x = ( MPU6050_Data.Accel_x ) / 2048.0f;
    IMU_accel_y = ( MPU6050_Data.Accel_y ) / 2048.0f;
    IMU_accel_z = ( MPU6050_Data.Accel_z ) / 2048.0f;
    // 陀螺仪量程 ±2000°/s 时，比例因子为 16.4 LSB/(°/s)
    float gx = (MPU6050_Data.Gyro_x - gyro_bias[0]) / 16.4f;
    float gy = (MPU6050_Data.Gyro_y - gyro_bias[1]) / 16.4f;
    float gz = (MPU6050_Data.Gyro_z - gyro_bias[2]) / 16.4f;

    // 2. 加速度计归一化
    float norm = sqrt(IMU_accel_x*IMU_accel_x + IMU_accel_y*IMU_accel_y + IMU_accel_z*IMU_accel_z);
    if (norm == 0) return;
    IMU_accel_x /= norm; IMU_accel_y /= norm; IMU_accel_z /= norm; // 单位向量 加速度的方向

    // 3. 四元数预测与修正（Mahony滤波）
    float q_w = Quaternion.q_w, q_x = Quaternion.q_x, q_y = Quaternion.q_y, q_z = Quaternion.q_z;
    float gx_pred = 2.0f * (q_y * q_z - q_w * q_x);
    float gy_pred = 2.0f * (q_x * q_z + q_w * q_y);
    float gz_pred = 2.0f * (q_x * q_y - q_w * q_z);
    float ex = (IMU_accel_y * gz_pred - IMU_accel_z * gy_pred);
    float ey = (IMU_accel_z * gx_pred - IMU_accel_x * gz_pred);
    float ez = (IMU_accel_x * gy_pred - IMU_accel_y * gx_pred);

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
    IMU_roll = atan2(2*(Quaternion.q_w*Quaternion.q_x + Quaternion.q_y*Quaternion.q_z), 1 - 2*(Quaternion.q_x*Quaternion.q_x + Quaternion.q_y*Quaternion.q_y));
    IMU_pitch = asin(2*(Quaternion.q_w*Quaternion.q_y - Quaternion.q_z*Quaternion.q_x));
    IMU_yaw = atan2(2*(Quaternion.q_w*Quaternion.q_z + Quaternion.q_x*Quaternion.q_y), 1 - 2*(Quaternion.q_y*Quaternion.q_y + Quaternion.q_z*Quaternion.q_z));
}


static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x03) {                   //返回0x03为MPU6050六轴，只要通过该if语句，就可以实现零偏自动校准
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);           //读取当前陀螺仪的状态
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);            //根据读取的状态进行校准

        mpu_get_accel_sens(&accel_sens);    //读取当前加速度计的状态
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);          //根据读取的状态进行校准
        printf("setting bias succesfully ......\r\n");
    }
}



