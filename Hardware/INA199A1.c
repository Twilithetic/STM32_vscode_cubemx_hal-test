#include "INA199A1.h"


INA199A1_Data_typedef INA199A1_Data; 
INA199A1_Bias_Data_typedef INA199A1_Bias_Data;
uint16_t adcBuffer[3] = {0,0,0}; // 存储ADC原始数据的缓冲区
void INA199A1_Init() {
    // 定义累加变量（用long防止溢出）
    long sum_sense1 = 0, sum_sense2 = 0, sum_sense3 = 0;
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcBuffer, sizeof(adcBuffer) / sizeof(uint16_t));
    // 关闭中断或延时，确保初始化不受干扰（可选）
    // Interrupt_disableGlobal();  // 若使用中断，先关闭
    
    // 循环1000次采集原始数据（不减去偏差）
    uint16_t i = 0;
    for (i = 0; i < 1000; i++) {
        _Updata_INA199A_Data_no_bias();  // 新函数：不减去偏差的采集
        
        // 累加原始电流值（注意：这里用原始值，未减偏差）
        sum_sense1 += (long)(INA199A1_Data.Sense_1_current * 1000);  // 放大1000倍防小数丢失
        sum_sense2 += (long)(INA199A1_Data.Sense_2_current * 1000);
        sum_sense3 += (long)(INA199A1_Data.Sense_3_current * 1000);
        
        // 适当延时（若ADC采样过快，可加小延时避免干扰）
        // DELAY_US(10);
    }
    
    // 计算平均偏差（单位：A）
    INA199A1_Bias_Data.Sense_1_current_bias = (float)sum_sense1 / 1000 / 1000;
    INA199A1_Bias_Data.Sense_2_current_bias = (float)sum_sense2 / 1000 / 1000;
    INA199A1_Bias_Data.Sense_3_current_bias = (float)sum_sense3 / 1000 / 1000;
    
    // 重新启用中断（若之前关闭）
    // Interrupt_enableGlobal();
    
    // 打印偏差值（调试用）
    // SCI_printf("INA199A1 Bias: S1=%.4fA, S2=%.4fA, S3=%.4fA\r\n", 
    //           INA199A1_Bias_Data.Sense_1_current_bias,
    //           INA199A1_Bias_Data.Sense_2_current_bias,
    //           INA199A1_Bias_Data.Sense_3_current_bias);
}

// 修改原数据更新函数：减去偏差（保持原逻辑）
void INA199A_Update_Data() {

    _Updata_INA199A_Data_no_bias();  // 先采集原始数据
    // 减去偏差
    INA199A1_Data.Sense_3_current -= INA199A1_Bias_Data.Sense_3_current_bias;
    INA199A1_Data.Sense_2_current -= INA199A1_Bias_Data.Sense_2_current_bias;
    INA199A1_Data.Sense_1_current -= INA199A1_Bias_Data.Sense_1_current_bias;
}

// 新函数：采集原始电流数据（不减去偏差）
void _Updata_INA199A_Data_no_bias() {
    // 触发ADC转换
    
    
    // 读取原始数据（不减去偏差）
    INA199A1_Data.Sense_1_current = INA199A1_ADC_To_Current(adcBuffer[0]);
    INA199A1_Data.Sense_2_current = INA199A1_ADC_To_Current(adcBuffer[1]);
    INA199A1_Data.Sense_3_current = INA199A1_ADC_To_Current(adcBuffer[2]);
}

float INA199A1_ADC_To_Current(uint16_t adc_Value){
    //  🌼第一步：确定关键参数（来自文档哦～）
    //      增益（Gain）：INA199A1 的增益是 50 倍（V/V）
    //      ：“INA199x1: 50 V/V”
    //      分流电阻（Rshunt）：你用的是 20 毫欧，即 0.02Ω
    //  🌼 第二步：从 ADC 值算出实际电压差
    float INA199A1_Vout = (adc_Value / 4096.0) * 3.3; // 单片机读到的 ADC 值→输出电压 Vout
    float INA199A1_Vshunt = (INA199A1_Vout - INA199A1_VREF) / 50; // 计算分流电阻上的电压差 Vshunt
    //  🌼 第三步：电压差→电流值（关键公式！）
    //      电流I = Vshunt / Rshunt （Vshunt 单位是 V，Rshunt 单位是 Ω，结果单位是 A）
    float INA199A1_Current = INA199A1_Vshunt / INA199A1_Rshunt;

    return INA199A1_Current;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
    if(hadc == &hadc1){

    }
}