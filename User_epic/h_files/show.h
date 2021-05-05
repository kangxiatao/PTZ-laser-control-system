#ifndef __SHOW_H
#define __SHOW_H

#include "sys.h"

/*
  全局变量
------------------------------------*/

/*-- 风力摆页面标志位  
 0 - 控制界面
 1 - 数据界面 --*/
extern uint8_t Page;

/*--
  画任意边形设置数组
  最后一个用来储存自己的边数
  最大为六边形
 --*/
extern float Polygon[7][2];
/*
------------------------------------*/


void Display_Main(void);
void Display_Init(void);  // 初始化界面
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t wide);
void DispFrame(u16 x1, u16 y1, u16 x2, u16 y2);

void DispWave1(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp);  // Pitch
void DispWave2(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp);  // Roll

void Swing_message(float radii, float angle, u8 mode);  // 用于设置摆动半径显示


#endif

