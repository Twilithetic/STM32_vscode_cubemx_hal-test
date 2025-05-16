#include "test.h"

void test_Init(){
    AS5600_Configure_ZERO_Angle_in_Hardware();

}

void test(){
    char buffer[10];
    Updata_AS5600_Data();
    snprintf(buffer,20, "Angle: %.2f", AS5600_Data.Angle);
    SH1122_ShowString(0,0,buffer,ASCII_0816_FORMAT);
    snprintf(buffer,20, "AGC: %d", AS5600_Data.AGC);
    SH1122_ShowString(0,16,buffer,ASCII_0816_FORMAT);
    SH1122_Update();


}