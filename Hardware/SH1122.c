#include "SH1122.h"
 
uint8_t DDRAM_mirror[64][128];

//******************************************************************************
//    函数说明：传输一个字节
//    入口数据：dat 数据
//    返回值：  无
//******************************************************************************
void SH1122_WR_Bus(uint8_t dat)
{
    uint8_t i;
    SH1122_CS_Clr();
    for (i = 0; i < 8; i++)
    {
        SH1122_SCL_Clr();
        if (dat & 0x80)
        {
            SH1122_SDA_Set();
        }
        else
        {
            SH1122_SDA_Clr();
        }
        SH1122_SCL_Set();
        dat <<= 1;
    }
    SH1122_CS_Set();
}

//******************************************************************************
//    函数说明：OLED写入一个指令
//    入口数据：reg 指令
//    返回值：  无
//******************************************************************************
void SH1122_WR_Code(uint8_t code)
{
    SH1122_DC_Clr();
    SH1122_WR_Bus(code);
    SH1122_DC_Set();
}

//******************************************************************************
//    函数说明：OLED写入一个数据
//    入口数据：dat 数据
//    返回值：  无
//******************************************************************************
void SH1122_WR_Data(uint8_t data)
{
    SH1122_WR_Bus(data);
}

//******************************************************************************
//    函数说明：OLED显示列的起始终止地址
//    入口数据：Column_double  列的起始地址
//              Row  列的终止地址
//    返回值：  无
//******************************************************************************
void SH1122_Locate(uint8_t Column_double, uint8_t Row)
{
    SH1122_WR_Code(0xB0);
    SH1122_WR_Code(Row);
    SH1122_WR_Code(((Column_double & 0xf0) >> 4) | 0x10); // 横着的第几个双像素 高位部分 0001 0 A6 A5 A4
    SH1122_WR_Code((Column_double & 0x0f));//横着的第几个双像素 低位部分 0000 A3 A2 A1 A10
    // column 会自动增加 1 在每次写入数据
}


void SH1122_Update(void){
    for (uint8_t i = 0; i < 64; i++)
    {
        SH1122_Locate(0, i);
        for (uint8_t j = 0; j < 128; j++)
        {
            SH1122_WR_Data(DDRAM_mirror[i][j]);
        }
    }
}

//******************************************************************************
//    函数说明：SH1122初始化
//    输入参数：无
//    返回值：  无
//******************************************************************************
void SH1122_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 GPIO 时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();  // 根据实际使用的 GPIO 端口修改

    // 配置 SH1122 引脚
    GPIO_InitStruct.Pin = SH1122_SCL_PIN | SH1122_SDA_PIN | SH1122_RES_PIN | SH1122_DC_PIN | SH1122_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SH1122_SCL_PORT, &GPIO_InitStruct);

    // 复位 SH1122
    SH1122_RES_Clr();
    HAL_Delay(10);
    SH1122_RES_Set();

    // 发送初始化命令
	SH1122_WR_Code(0xAE);//Set display off
	SH1122_WR_Code(0xB0); //Row address Mode Setting
	SH1122_WR_Code(0x00);
	SH1122_WR_Code(0x10); //Set Higher Column Address of display RAM
	SH1122_WR_Code(0x00); //Set Lower Column Address of display RAM
	SH1122_WR_Code(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency
	SH1122_WR_Code(0x50); //50 125hz
	SH1122_WR_Code(0xD9); //Set Discharge/Precharge Period
	SH1122_WR_Code(0x22);
	SH1122_WR_Code(0x40); //Set Display Start Line
	SH1122_WR_Code(0x81); //The Contrast Control Mode Set
	SH1122_WR_Code(0xFF);
	if(USE_HORIZONTAL)
	{
        SH1122_WR_Code(0xA1); //Set Segment Re-map
        SH1122_WR_Code(0xC8); //Set Common Output Scan Direction
        SH1122_WR_Code(0xD3); //Set Display Offset
        SH1122_WR_Code(0x20);
	}else
	{
        SH1122_WR_Code(0xA0); //Set Segment Re-map
        SH1122_WR_Code(0xC0); //Set Common Output Scan Direction
        SH1122_WR_Code(0xD3); //Set Display Offset
        SH1122_WR_Code(0x00);
	}
	SH1122_WR_Code(0xA4); //Set Entire Display OFF/ON
	SH1122_WR_Code(0xA6); //Set Normal/Reverse Display
	SH1122_WR_Code(0xA8); //Set Multiplex Ration
	SH1122_WR_Code(0x3F);
	SH1122_WR_Code(0xAD); //DC-DC Setting
	SH1122_WR_Code(0x80); //DC-DC is disable
	SH1122_WR_Code(0xDB); //Set VCOM Deselect Level
	SH1122_WR_Code(0x30);
	SH1122_WR_Code(0xDC); //Set VSEGM Level
	SH1122_WR_Code(0x30);
	SH1122_WR_Code(0x33); //Set Discharge VSL Level 1.8V
	SH1122_Clear();
	SH1122_WR_Code(0xAF); //Set Display On
}


//******************************************************************************
//    函数说明：OLED清屏显示
//    入口数据：无
//    返回值：  无
//******************************************************************************
void SH1122_Clear(void)
{
    uint16_t j, i;
    for (i = 0; i < 64; i++)
    {
        for (j = 0; j < 128; j++)
        {
            DDRAM_mirror[i][j] = 0x00;
        }
    }
}

//******************************************************************************
//    函数说明：显示单色图片
//    输入参数：Column,Row :起始坐标（像素）
//              Width 图片宽度
//              Height  图片高度
//              Image[] 图片 传输的每个字节会在 坐标处 以 横的方向传输 高位在前 每位代表亮/灭 会自动column += 1 例如    D7 D6 D5 D4 D3 D2 D1 D0 ||| D7 D6.....
//    返回值：  无
//******************************************************************************
void SH1122_ShowMonoImage(uint8_t Column, uint8_t Row, uint16_t Width, uint8_t Height, const uint8_t Image[])
{
    uint8_t Image_BytePixel_Data = 0; //在处理的字节，里面包含横着的8个像素的亮灭信息
    uint8_t DoublePixel_Data = 0;
    for (uint8_t i = 0; i < Height; i++) // 在图片的 第 i 行
    {  
        for (uint8_t j = 0; j < (Width + 8 - 1) / 8; j++) // 在 图片的第 i 行 第 width / 8 * j 列，行是横着的 列是竖着的 像素的宽度是Width用来表示一行的像素是 Width / 8 个 (Width + 8 - 1) / 8 是 Width / 8的向上取整
        {   
            Image_BytePixel_Data = Image[(Width + 8 - 1) / 8 * i + j];
            for (int8_t k = 3; k >= 0 ; k--)//对Image_BytePixel_Data的每个位进行处理，适合我们这个灰度系统 一个字节的高低4位是左右两个像素的灰度信息
            {   // 0x03 = 0000 0011 一下处理两个 k = 3时 处理1100 0000 填入 3 - k 的序位置 
                
                if(( Image_BytePixel_Data & (0x03 << (k * 2) ) ) >> (k * 2) == 0x03) DoublePixel_Data = 0xff; // 0x03 两个都亮
                else if (( Image_BytePixel_Data & (0x03 << (k * 2) ) ) >> (k * 2) == 0x02) DoublePixel_Data = 0xf0;// 0x02 左边亮
                else if (( Image_BytePixel_Data & (0x03 << (k * 2) ) ) >> (k * 2) == 0x01) DoublePixel_Data = 0x0f;// 0x01 右边亮
                else if (( Image_BytePixel_Data & (0x03 << (k * 2) ) ) >> (k * 2) == 0x00) DoublePixel_Data = 0x00;// 0x00 都不亮
                DDRAM_mirror[Row + i][Column / 2 + j * 4 + (3 - k)]  = DoublePixel_Data; // Column是256x64中的像素 /2是DDRAM要的 j * 4 是 第 j 个字节要填DDRAM的开始地方由于image没有灰度

            }
        }
        
    }
    
	
}

//******************************************************************************
//    函数说明：显示单色图片
//    输入参数：Column,Row :起始坐标（像素）
//              Width 图片宽度
//              Height  图片高度
//              Image[] 图片 传输的每个字节会在 坐标处 以 横的方向传输 高位在前 每4位代表一个像素的灰度信息 0xff是最亮 会自动column += 1 例如    D7 D6 D5 D4 D3 D2 D1 D0 ||| D7 D6.....
//    返回值：  无
//******************************************************************************
void SH1122_ShowGrayImage(uint8_t Column, uint8_t Row, uint16_t Width, uint8_t Height, const uint8_t Image[])
{
    uint8_t DoublePixel_Data = 0; //在处理的字节，里面包含横着的2个像素的灰度信息
    for (uint8_t i = 0; i < Height; i++) 
    {  
        for (uint8_t j = 0; j < (Width / 2); j++) // 找到要处理的字节 Width是像素的宽度，而一个字节是两个像素的灰度信息，所以横着的像素数量是 Width / 2 
        {   
            DoublePixel_Data = Image[(Width / 2) * i + j];
            DDRAM_mirror[Row + i][Column / 2 + j]  = DoublePixel_Data; 
        }
        
    }
    
	
}

//******************************************************************************
//    函数说明：OLED显示字符函数 
//    入口数据：Column,Row :起始坐标（像素）
//              Character   要写入的字符
//              format 格式 例如 ASCII_0816_FORMAT
//    返回值：  无
//******************************************************************************
void SH1122_ShowChar(uint8_t Column, uint8_t Row, uint8_t Character, CharFormat format)
{
    switch (format)
    {
        case ASCII_0816_FORMAT:
            for (uint8_t i = 0; i < 95; i++) {
                if (ASCII_0816[i].Index[0] == Character) {
                    SH1122_ShowMonoImage(Column, Row, 8, 16, ASCII_0816[i].Msk);
                    return                             ;
                }
            }
            break;
        
        default:
            break;
    }
}


//******************************************************************************
//    函数说明：OLED显示字符串
//    入口数据：Column,Row :起始坐标（像素）
//              *dp_String   要写入的字符
//              format 格式 例如 ASCII_0816_FORMAT
//    返回值：  无
//******************************************************************************
void SH1122_ShowString(uint8_t Column, uint8_t Row, char* dp_String, CharFormat format)
{
    while (*dp_String != '\0')
    {
        SH1122_ShowChar(Column, Row, *dp_String, format);
        dp_String++;
        switch (format)
        {
            case ASCII_0816_FORMAT:
                Column += 8;
                break;
            
            default:
                break;
        }
        
    }
}
