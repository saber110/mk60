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
//������ʵ�ֵĽӿں���  


void OLED_WRCMD(uint8 com);  
void OLED_WRDATA(uint8 dat);  
void OLED_Setxy(uint8 x, uint8 y);    //OLED ��������  
void OLED_Fill(uint8 bmp_dat);   //bmp_dat=0x00ȫ����bmp_dat=0xffȫ����  
void OLED_Init(void); //OLED��ʼ��    
void OLED_6x8(uint8 x,uint8 y,uint8 ch[]);//��ʾ6*8һ���׼ASCII�ַ���    ��ʾ�����꣨x,y����yΪҳ��Χ0��7  
void OLED_Cler_6x8(uint8 x,uint8 y,uint8 n);//���N��6*8�ַ�  
void OLED_8x16(uint8 x,uint8 y,uint8 ch[]);//��ʾ8*16һ���׼ASCII�ַ���   ��ʾ�����꣨x,y����yΪҳ��Χ0��7  
//void OLED_Cler_8x16(uint8 x,uint8 y,uint8 n); //���n��8*16�ַ�  
//void OLED_16x16(uint8 x,uint8 y,uint8 N);//��ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7  
//void OLED_Cler_16x16(uint8 x,uint8 y,uint8 n);//���n��16*16��  
void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 BMP[]);//��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*  
void OLED_Refresh_Gram(void);  
void OLED_DRAWPoint(uint8 x,uint8 y,uint8 t);  
void LCD_ZI(uint8 x,uint8 y,uint8 ch);
void LCD_SHUZI(uint8 x,uint8 y,int shu);
void DrawPixel(uint8 Pixel[]);
void LCD__change_num(unsigned char x, unsigned char y,float float_num);
extern void OLED_Display();
void OLED_DRAW(uint8 x,uint8 y,uint8 t);
void DrawLine(uint8 num);
void OLED_xiaoshu(uint8 x,uint8 y,float shu);//��ʾС������������С�����3λ��

//oled_menu
void Oled_Menu_init(void);    //OLED�˵���ʼ��
void Display_Page1(float *num);     //��ʾ��һҳ
void Display_Page2(float *num);     //��ʾ�ڶ�ҳ
void Display_Jinzhi1(float* num);   //��ʾ���Ƶ�һҳ
void Display_Jinzhi2(float* num);    //��ʾ���Ƶڶ�ҳ
void Menu_Change_Data(float* num,float* Jinzhi);   //�趨���꣬��������
float Set_jinzhi(float num);  //���Ľ���
void OLED_WRDATA_Yinyang(uint8 dat,uint8 type);// ʵ��������д��һ�ֽ�����
void OLED_Yinyangma_6x8 (uint8 x,uint8 y,uint8 type,uint8 ch[]);//6*8���������,tye=1���֣�type=0����
#endif
