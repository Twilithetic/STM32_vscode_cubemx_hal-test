#include "Scheduler.h"
// .c维护的数据
struct Scheduler_typedef Motor= {7}; // 极对数：7
// 静态任务数组，每个任务包含任务函数、执行周期（毫秒）和上次运行时间（毫秒）
// 全局变量，用于存储任务数量
uint8_t task_num;
Scheduler_Task Scheduler_task_list[] =
{
  //{test_message_TX, &Debug_Data.measure_us_data, 100 * 1e3, 0},
  {Debug_UART_Print, NULL, 30 * 1e3, 0},
};

void Scheduler_Init(){
    task_num = sizeof(Scheduler_task_list) / sizeof(Scheduler_Task); // 数组大小 / 数组成员大小 = 数组元素个数
    Debug_Init();
}

void Scheduler_Start(){
  // 遍历任务数组中的所有任务
  for (uint8_t i = 0; i < task_num; i++)
  {
    // 获取当前的系统时间（毫秒）
    uint64_t now_us = Timestamp_us_Count();
    // 检查当前时间是否达到任务的执行时间
    if (Build_in_Elapsed_us_Compute(Scheduler_task_list[i].last_start, now_us) >= Scheduler_task_list[i].period_us)
    {
      // 更新任务的上次运行时间为当前时间
      Scheduler_task_list[i].last_start = now_us;

      // 执行任务函数
      Debug_func_us(Scheduler_task_list[i].task_func, Scheduler_task_list[i].period_us, Scheduler_task_list[i].debug_data);
    }
  }
}




