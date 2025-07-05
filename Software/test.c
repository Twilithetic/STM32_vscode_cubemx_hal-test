#include "test.h"

void test_Init(){
    Build_in_LED_Init();
    AS5600_Init();
    HAL_TIM_Base_Start_IT(&htim4);
}

static char buffer_update[256];
void Build_in_SCI_log(){
    snprintf(buffer_update, sizeof(buffer_update),
    "\rmotor angle:%4ld sense_1:%05ld sense_2:%05ld sense_3:%05ld sense_D:%05ld sense_Q_Torque:%05d motor speed:%05d rad_Total:%05ld\n", 
        (int32_t)(FOC_measured_data.Machine_Rad * (180.0f / M_PI)), 
        (int32_t)(FOC_measured_data.current_a * 1000),
        (int32_t)(FOC_measured_data.current_b * 1000),
        (int32_t)(FOC_measured_data.current_c * 1000),
        // (int16_t)(FOC_Current_Vector_Show_IN_Abs_elec_rad_Torque_Vector.current_D * 1000),
        // (int16_t)(FOC_Current_Vector_Show_IN_Abs_elec_rad_Torque_Vector.current_Q_Torque * 1000),
        0,
        0,
        (int32_t)(FOC_measured_data.Machine_Rad_Speed * 1000),
        (int32_t)(FOC_measured_data.Machine_Rad_Total * 1000)
    );
    Build_in_SCI_Print(buffer_update);
    
}

void test(){
    Build_in_LED_Set();
    // Build_in_Delay_ms(500);
    // Build_in_LED_Clr();
    // Build_in_Delay_ms(500);
    Build_in_Delay_ms(40);
    FOC_Measure_Update();
    Build_in_SCI_log();
}


// 中断回调函数（用户代码区）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim4) {

    }
}
