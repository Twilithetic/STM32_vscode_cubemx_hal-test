#include "test.h"

void test_Init(){
    Build_in_LED_Init();
    AS5600_Init();
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim3);
    __HAL_DBGMCU_FREEZE_TIM3(); // 断点不停止时钟
}

static char buffer_update[256];
uint8_t Build_in_SCI_log_ms_cnt;
void Build_in_SCI_log(){
    if (Build_in_SCI_log_ms_cnt) return;
    Build_in_SCI_log_ms_cnt = 1;
    snprintf(buffer_update, sizeof(buffer_update),
    "\rmotor angle:%4ld sense_1:%05ld sense_2:%05ld sense_3:%05ld sense_D:%05d sense_Q_Torque:%d motor speed:%ld rad_Total:%05ld dt_ms:%f\n", 
        (int32_t)(FOC_measured_data.Machine_Rad * (180.0f / M_PI)), 
        (int32_t)(FOC_measured_data.current_a * 1000),
        (int32_t)(FOC_measured_data.current_b * 1000),
        (int32_t)(FOC_measured_data.current_c * 1000),
        // (int16_t)(FOC_Current_Vector_Show_IN_Abs_elec_rad_Torque_Vector.current_D * 1000),
        // (int16_t)(FOC_Current_Vector_Show_IN_Abs_elec_rad_Torque_Vector.current_Q_Torque * 1000),
        0,
        0,
        (int32_t)(FOC_measured_data.Machine_Rad_Speed * 1000),
        (int32_t)(FOC_measured_data.Machine_Rad_Total * 1000),
        dt_t
    );
    Build_in_SCI_Print(buffer_update);
    
}
float dt_ms;
uint8_t is_log = 1;
void test(){
    uint16_t start = Timestamp_us_Count();
    // 主体
    //Build_in_Delay_ms(50);
    FOC_Measure_Update();
    //主体结束
    uint16_t now = Timestamp_us_Count();
    dt_ms = (Build_timestamp_us_Compute(start, now) / 1000.0f);
    if (is_log) {Build_in_SCI_log();}// 0.3ms
}


// 中断回调函数（用户代码区）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim4) {
        // AS5600更新周期控制
        if(AS5600_Rad_Updata_delay_ms_cnt++ == 1) {AS5600_Rad_Updata_delay_ms_cnt = 0;}
        if(AS5600_Speed_Updata_delay_ms_cnt++ == 10) {AS5600_Speed_Updata_delay_ms_cnt = 0;}
        // SCI Debug
        if(Build_in_SCI_log_ms_cnt++ == 50) {Build_in_SCI_log_ms_cnt = 0;}
        
    }
}
