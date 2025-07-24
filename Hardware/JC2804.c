// #include "JC2804.h"

// void JC2804_Init(){
//     HAL_CAN_Start(&hcan);

//     // 配置接收过滤器（回环模式下也需允许接收自身发送的数据）
//     CAN_FilterTypeDef sFilterConfig;
//     sFilterConfig.FilterBank = 0;
//     sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
//     sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//     sFilterConfig.FilterIdHigh = 0x0000;   // 掩码模式，允许所有ID
//     sFilterConfig.FilterIdLow = 0x0000;
//     sFilterConfig.FilterMaskIdHigh = 0x0000;
//     sFilterConfig.FilterMaskIdLow = 0x0000;
//     sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
//     sFilterConfig.FilterActivation = ENABLE;
//     sFilterConfig.SlaveStartFilterBank = 14; // 单CAN时可忽略
//     HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

//     // 启动接收中断（回环模式下自收需使能中断或轮询）
//     HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);


// }
// CAN_TxHeaderTypeDef TxHeader;  // 发送报文的头部配置
// uint8_t TxData[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}; // 发送数据（最多8字节）
// uint32_t TxMailbox;  // 发送邮箱编号（硬件自动分配，无需手动设置）

// void test_message_TX(){
//     TxHeader.StdId = 0x123;         // 标准ID（0~0x7FF），如果是扩展帧用ExtId
//     TxHeader.ExtId = 0x00;          // 扩展ID（仅当IDE=CAN_ID_EXT时有效）
//     TxHeader.RTR = CAN_RTR_DATA;    // 类型：数据帧（CAN_RTR_DATA）或远程帧（CAN_RTR_REMOTE）
//     TxHeader.IDE = CAN_ID_STD;      // 标识符类型：标准帧（CAN_ID_STD）或扩展帧（CAN_ID_EXT）
//     TxHeader.DLC = 8;               // 数据长度：1~8字节
//     TxHeader.TransmitGlobalTime = DISABLE; // 是否记录发送时间戳（需时基支持，一般DISABLE）
//     // 1. 发送数据
//     TxHeader.StdId = 0x123; // 每次发送可修改ID或数据
//     TxData[0]++; // 模拟数据变化
//     if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) == HAL_OK) {
//         Build_in_UART_Print("CAN数据发送成功！\n");
//     } else {
//         Build_in_UART_Print("CAN发送失败！\n");
//     }
    
//     // 2. 轮询接收（回环模式下，发送后立即尝试接收）
//     CAN_RxHeaderTypeDef RxHeader;
//     uint8_t RxData[8];
//     if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
//         snprintf(UART1_TX_buffer, sizeof(UART1_TX_buffer), "收到CAN数据：ID=0x%03X，数据=", RxHeader.StdId);
//         Build_in_UART_Print(UART1_TX_buffer);
//         for (int i=0; i<RxHeader.DLC; i++) {
//             Build_in_UART_Print("0x%02X ", RxData[i]);
//         }
//         Build_in_UART_Print("\n");
//     }
// }

// // 接收中断回调（可选，替代轮询）
// void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
//     CAN_RxHeaderTypeDef RxHeader;
//     uint8_t RxData[8];
//     if (hcan->Instance == CAN1) {
//         HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
//         snprintf(UART1_TX_buffer, sizeof(UART1_TX_buffer), "收到CAN数据：ID=0x%03X，数据=", RxHeader.StdId);
//         Build_in_UART_Print(UART1_TX_buffer);
//         for (int i=0; i<RxHeader.DLC; i++) {
//             Build_in_UART_Print("0x%02X ", RxData[i]);
//         }
//         Build_in_UART_Print("\n");
//     }
// }
