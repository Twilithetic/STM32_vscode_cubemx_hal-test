#ifndef __SH1122_H
#define __SH1122_H 

#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
#include "SH1122_Fonts.h"
extern uint8_t DDRAM_mirror[64][128];  //256 X 64 Display Data RAM mirror 每个像素有4位灰度D3 D2 D1 D0 横着两个像素的灰度用一个字节传输


// 定义SH1122引脚和端口
#define SH1122_SCL_PORT GPIOA
#define SH1122_SCL_PIN GPIO_PIN_12
#define SH1122_SDA_PORT GPIOA
#define SH1122_SDA_PIN GPIO_PIN_11
#define SH1122_RES_PORT GPIOA
#define SH1122_RES_PIN GPIO_PIN_10
#define SH1122_DC_PORT GPIOA
#define SH1122_DC_PIN GPIO_PIN_9
#define SH1122_CS_PORT GPIOA
#define SH1122_CS_PIN GPIO_PIN_8

#define USE_HORIZONTAL 0  // 显示方向设置 0:正常显示，1:旋转180度显示


#define SH1122_SCL_Clr()  HAL_GPIO_WritePin(SH1122_SCL_PORT, SH1122_SCL_PIN, GPIO_PIN_RESET)
#define SH1122_SCL_Set()  HAL_GPIO_WritePin(SH1122_SCL_PORT, SH1122_SCL_PIN, GPIO_PIN_SET)
#define SH1122_SDA_Clr()  HAL_GPIO_WritePin(SH1122_SDA_PORT, SH1122_SDA_PIN, GPIO_PIN_RESET)
#define SH1122_SDA_Set()  HAL_GPIO_WritePin(SH1122_SDA_PORT, SH1122_SDA_PIN, GPIO_PIN_SET)
#define SH1122_RES_Clr()  HAL_GPIO_WritePin(SH1122_RES_PORT, SH1122_RES_PIN, GPIO_PIN_RESET)
#define SH1122_RES_Set()  HAL_GPIO_WritePin(SH1122_RES_PORT, SH1122_RES_PIN, GPIO_PIN_SET)
#define SH1122_DC_Clr()   HAL_GPIO_WritePin(SH1122_DC_PORT, SH1122_DC_PIN, GPIO_PIN_RESET)
#define SH1122_DC_Set()   HAL_GPIO_WritePin(SH1122_DC_PORT, SH1122_DC_PIN, GPIO_PIN_SET)
#define SH1122_CS_Clr()   HAL_GPIO_WritePin(SH1122_CS_PORT, SH1122_CS_PIN, GPIO_PIN_RESET)
#define SH1122_CS_Set()   HAL_GPIO_WritePin(SH1122_CS_PORT, SH1122_CS_PIN, GPIO_PIN_SET)






// 显示功能函数声明
void SH1122_WR_Code(uint8_t reg); // 写一个指令
void SH1122_WR_Data(uint8_t dat); // 写一个数据
void SH1122_Locate(uint8_t Column_double, uint8_t Row); // 设置起始地址函数
void SH1122_Update(void);
void SH1122_Init(void);
void SH1122_Clear(void); // 清屏函数
void SH1122_ShowMonoImage(uint8_t Column, uint8_t Row, uint16_t Width, uint8_t Height, const uint8_t Image[]);
void SH1122_ShowGrayImage(uint8_t Column, uint8_t Row, uint16_t Width, uint8_t Height, const uint8_t Image[]);
void SH1122_ShowChar(uint8_t Column, uint8_t Row, uint8_t Character, CharFormat format);
void SH1122_ShowString(uint8_t Column, uint8_t Row, char* dp_String, CharFormat format);

#endif