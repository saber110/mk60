#include "common.h"
#ifndef __oled_H__  
#define __oled_H__  

#define OLED_DC     PTC19
#define OLED_SDA    PTC17
#define OLED_SCL    PTC16
#define OLED_RST    PTC18
  
#define XLevelL     0x00  
#define XLevelH     0x10  
#define XLevel      ((XLevelH&0x0F)*16+XLevelL)  
#define Max_Column  128  
#define Max_Row     64  
#define Brightness  0xCF   
#define X_WIDTH 128  
#define Y_WIDTH 64  
//本构件实现的接口函数  


void OLED_WRCMD(uint8 com);  
void OLED_WRDATA(uint8 dat);  
void OLED_Setxy(uint8 x, uint8 y);    //OLED 设置坐标  
void OLED_Fill(uint8 bmp_dat);   //bmp_dat=0x00全屏灭，bmp_dat=0xff全屏亮  
void OLED_Init(void); //OLED初始化    
void OLED_6x8(uint8 x,uint8 y,uint8 ch[]);//显示6*8一组标准ASCII字符串    显示的坐标（x,y），y为页范围0～7  
void OLED_Cler_6x8(uint8 x,uint8 y,uint8 n);//清除N个6*8字符  
void OLED_8x16(uint8 x,uint8 y,uint8 ch[]);//显示8*16一组标准ASCII字符串   显示的坐标（x,y），y为页范围0～7  
//void OLED_Cler_8x16(uint8 x,uint8 y,uint8 n); //清除n个8*16字符  
//void OLED_16x16(uint8 x,uint8 y,uint8 N);//显示16*16点阵  显示的坐标（x,y），y为页范围0～7  
//void OLED_Cler_16x16(uint8 x,uint8 y,uint8 n);//清除n个16*16字  
void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 BMP[]);//显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*  
void OLED_Refresh_Gram(void);  
void OLED_DRAWPoint(uint8 x,uint8 y,uint8 t);  
void LCD_ZI(uint8 x,uint8 y,uint8 ch);
void LCD_SHUZI(uint8 x,uint8 y,int shu);
void DrawPixel(uint8 Pixel[]);
void LCD__change_num(unsigned char x, unsigned char y,float float_num);
extern void OLED_Display();
void OLED_DRAW(uint8 x,uint8 y,uint8 t);
void DrawLine(uint8 num);
void OLED_xiaoshu(uint8 x,uint8 y,float shu);//显示小数（含负数，小数点后3位）

//oled_menu
void Oled_Menu_init(void);    //OLED菜单初始化
void Display_Page1(float *num);     //显示第一页
void Display_Page2(float *num);     //显示第二页
void Menu_Change_Data(float *num);  //设定坐标，更改数据
float Set_jinzhi(float num);  //更改进制
void OLED_WRDATA_Yinyang(uint8 dat,uint8 type);// 实现正反码写入一字节数据
void OLED_Yinyangma_6x8 (uint8 x,uint8 y,uint8 type,uint8 ch[]);//6*8阴阳码输出,tye=1亮字，type=0亮底
#endif
