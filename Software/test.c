#include "test.h"

void test_Init(){
    Build_in_LED_Init();
    AS5600_Init();
}

static char buffer_update[128];
void test_log(){
    // snprintf(buffer, sizeof(buffer),
    // "\rmotor angle:%4d sense_1:%05d sense_2:%05d sense_3:%05d sense_D:%05d sense_Q_Torque:%05d motor speed:%05d rad_Total:%05d\n", 
    //     (int16_t)(FOC_measured_data.Machine_Rad * (180.0f / M_PI)), 
    //     (int16_t)(FOC_measured_data.current_a * 1000),
    //     (int16_t)(FOC_measured_data.current_b * 1000),
    //     (int16_t)(FOC_measured_data.current_c * 1000),
    //     (int16_t)(FOC_Current_Vector_Show_IN_Abs_elec_rad_Torque_Vector.current_D * 1000),
    //     (int16_t)(FOC_Current_Vector_Show_IN_Abs_elec_rad_Torque_Vector.current_Q_Torque * 1000),
    //     (int16_t)(FOC_measured_data.Machine_Rad_Speed * 1000),
    //     (int16_t)(FOC_measured_data.Machine_Rad_Total * 1000)
    // );
    snprintf(buffer_update, sizeof(buffer_update), "\rmotor angle:%d\n", (AS5600_Data.Angle_Raw));
    Build_in_SCI_Print(buffer_update);
    
}

void test(){
    Build_in_LED_Set();
    Build_in_Delay_ms(500);
    Build_in_LED_Clr();
    Build_in_Delay_ms(500);
    
    Updata_AS5600_Data();
    test_log();
}
