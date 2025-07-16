#include "Motor.h"
// 静态任务数组，每个任务包含任务函数、执行周期（毫秒）和上次运行时间（毫秒）
// 全局变量，用于存储任务数量
uint8_t task_num;
static Motor_Ctrl_t scheduler_task[] =
{
  {Led_Task, 1, 0},
//  {Key_Task, 10, 0},
  {Oled_Task, 10, 0},
  {Uart_Task, 10, 0},
  {Mpu6050_Task, 1, 0},
//  {Gray_Task, 10, 0},
  {Motor_Task, 10, 0},
  {Encoder_Task, 100, 0},
};

void Motor_Init(){
    task_num = sizeof(scheduler_task) / sizeof(Motor_Ctrl_t); // 数组大小 / 数组成员大小 = 数组元素个数
}

void Motor_Control(){
  // 遍历任务数组中的所有任务
  for (uint8_t i = 0; i < task_num; i++)
  {
    // 获取当前的系统时间（毫秒）
    uint32_t now_us = Timestamp_us_Count();
    HAL_GetTick();
    // 检查当前时间是否达到任务的执行时间
    if (now_us >= scheduler_task[i].period_us + scheduler_task[i].last_run)
    {
      // 更新任务的上次运行时间为当前时间
      scheduler_task[i].last_run = now_us;

      // 执行任务函数
      scheduler_task[i].task_func();
    }
  }
}




