#ifndef __OLEDFONT_H
#define __OLEDFONT_H
#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改
// 注意！！ 传输的每个字节会在 坐标处 以 横的方向传输 高位在前 每位代表亮/灭 会自动column += 1 例如 D7 D6 D5 D4 D3 D2 D1 D0 ||| D7 D6.....

typedef enum {
    ASCII_0816_FORMAT,
} CharFormat;

typedef struct {
    uint16_t Index[1];
    uint8_t Msk[32];
} ASCII_0816Def;

typedef struct {
    uint16_t Index[1];
    uint8_t Msk[32];
} UTF_ChineseDef;


extern const ASCII_0816Def ASCII_0816[];
extern const UTF_ChineseDef UTF_Chinese[];
#endif
