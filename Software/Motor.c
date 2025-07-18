#include "Motor.h"
// .c维护的数据
struct Motor_typedef Motor= {7}; // 极对数：7
// 静态任务数组，每个任务包含任务函数、执行周期（毫秒）和上次运行时间（毫秒）
// 全局变量，用于存储任务数量
uint8_t task_num;
static Motor_Ctrl_t motor_task_list[] =
{
  {FOC_Measure_Update, &FOC_debug_us_data.measure_us_data, 1000, 0},
// //  {Key_Task, 10, 0},
//   {Oled_Task, 10, 0},
//   {Uart_Task, 10, 0},
//   {Mpu6050_Task, 1, 0},
// //  {Gray_Task, 10, 0},
//   {Motor_Task, 10, 0},
//   {Encoder_Task, 100, 0},
};

void Motor_Init(){
    task_num = sizeof(motor_task_list) / sizeof(Motor_Ctrl_t); // 数组大小 / 数组成员大小 = 数组元素个数
}

uint8_t is_debug = 1;
void Motor_Ctrl_task_run(){
  // 遍历任务数组中的所有任务
  for (uint8_t i = 0; i < task_num; i++)
  {
    // 获取当前的系统时间（毫秒）
    uint64_t now_us = Timestamp_us_Count();
    // 检查当前时间是否达到任务的执行时间
    if (Build_in_Elapsed_us_Compute(motor_task_list[i].last_start, now_us) >= motor_task_list[i].period_us)
    {
      // 更新任务的上次运行时间为当前时间
      motor_task_list[i].last_start = now_us;

      // 执行任务函数
      FOC_Debug_func_us(motor_task_list[i].task_func, motor_task_list[i].debug_data);
    }
  }

  if (is_debug) {FOC_Debug_UART();}

  
}




