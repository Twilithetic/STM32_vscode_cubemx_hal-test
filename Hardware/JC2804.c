#include "JC2804.h"
#include "stm32f1xx_hal_can.h"

JC2804_Data JC2804_Data_list[] =
{
  {1, 90.0f},
  {2, 90.0f}
};


CAN_TxHeaderTypeDef TxHeader;  // 发送报文的头部配置
uint8_t TxData[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}; // 发送数据（最多8字节）
uint32_t TxMailbox;  // 发送邮箱编号（硬件自动分配，无需手动设置）

void JC2804_Init(){
    HAL_CAN_Start(&hcan);

    // 配置接收过滤器（回环模式下也需允许接收自身发送的数据）
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;   // 掩码模式，允许所有ID
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14; // 单CAN时可忽略
    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

    // 启动接收中断（回环模式下自收需使能中断或轮询）
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);


}

void JC2804_pull_data(){
    for (uint8_t i = 0; i < sizeof(JC2804_Data_list) / sizeof(JC2804_Data); i++)
    {
        set_Abs_Point(JC2804_Data_list[i].id, JC2804_Data_list[i].angle);
    }
    
}

void set_Abs_Point(uint8_t id, float angle) {
    // 1. 配置CAN发送头
    TxHeader.StdId = 0x600 + id;         // 标准ID（例：id=1 → 0x601）
    TxHeader.ExtId = 0x00;              // 扩展ID无效，设为0
    TxHeader.RTR = CAN_RTR_DATA;        // 数据帧类型
    TxHeader.IDE = CAN_ID_STD;          // 标准帧格式
    TxHeader.DLC = 8;                   // 固定8字节数据
    TxHeader.TransmitGlobalTime = DISABLE; // 关闭时间戳

    // 2. 处理角度值（放大100倍，处理精度）
    uint32_t scaled_angle = (uint32_t)round(angle * 100.0f); // 四舍五入避免误差

    // 3. 填充CAN数据域
    uint8_t TxData[8] = {0}; // 初始化为0
    TxData[0] = 0x23;        // 命令字（固定）
    TxData[1] = 0x00;        // 寄存器地址0x0023的高字节
    TxData[2] = 0x23;        // 寄存器地址0x0023的低字节
    TxData[3] = 0x00;        // 保留位（固定填0）
    
    // 大端序拆分32位角度值
    TxData[4] = (scaled_angle >> 24) & 0xFF; // 最高位字节
    TxData[5] = (scaled_angle >> 16) & 0xFF; // 次高位字节
    TxData[6] = (scaled_angle >> 8)  & 0xFF; // 次低位字节
    TxData[7] =  scaled_angle        & 0xFF; // 最低位字节

    // 4. 发送CAN数据
    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}

void test_JC2804_loopback(){

    // 1. 发送数据
    TxHeader.StdId = 0x123; // 每次发送可修改ID或数据
    TxData[0]++; // 模拟数据变化
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) == HAL_OK) {
        Build_in_UART_Printf("CAN数据发送成功！\n");
    } else {
        Build_in_UART_Print("CAN发送失败！\n");
    }
    
    // 2. 轮询接收（回环模式下，发送后立即尝试接收）
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
        Build_in_UART_Printf("收到CAN数据：ID=0x%03X，数据=", RxHeader.StdId);
        for (int i=0; i<RxHeader.DLC; i++) {
            Build_in_UART_Printf("0x%02X ", RxData[i]);
        }
        Build_in_UART_Print("\n");
    }
}

float test_data[] = {90, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; 
uint8_t cnt;
void test_PID(){
    if (cnt >= (sizeof(test_data) / sizeof(float)) - 1){cnt = 0;}
    
    JC2804_Data_list[0].angle = test_data[cnt];
    cnt++;
}
// 接收中断回调（可选，替代轮询）
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    if (hcan->Instance == CAN1) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
        // Build_in_UART_Printf("收到CAN数据：ID=0x%03X, 数据=0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", 
        //         RxHeader.StdId,
        //         RxData[0],
        //         RxData[1],
        //         RxData[2],
        //         RxData[3],
        //         RxData[4],
        //         RxData[5],
        //         RxData[6],
        //         RxData[7]);
    }
}
