#ifndef __INA199A1_H
#define __INA199A1_H
#include "Build_in-Led.h"


#define INA199A1_Rshunt = 0.02; // 0.02Ω
#define INA199A1_VREF = 1.65; // 1.65V

extern float INA199A1_Rshunt; // 0.02Ω
extern float INA199A1_VREF; // 1.65V

typedef struct 
{       
    float Sense_1_current; // A
    float Sense_2_current;
    float Sense_3_current;

} INA199A1_Data_typedef;

typedef struct
{
    float Sense_1_current_bias;
    float Sense_2_current_bias;
    float Sense_3_current_bias;
} INA199A1_Bias_Data_typedef;

extern INA199A1_Data_typedef INA199A1_Data; // 显式初始化
extern INA199A1_Bias_Data_typedef INA199A1_Bias_Data;

void INA199A1_Init();
void INA199A_Update_Data();
void _Updata_INA199A_Data_no_bias();



#endif // !__INA199A1_H

