#ifndef __SHOW_H
#define __SHOW_H

#include "sys.h"

/*
  ȫ�ֱ���
------------------------------------*/

/*-- ������ҳ���־λ  
 0 - ���ƽ���
 1 - ���ݽ��� --*/
extern uint8_t Page;

/*--
  �����������������
  ���һ�����������Լ��ı���
  ���Ϊ������
 --*/
extern float Polygon[7][2];
/*
------------------------------------*/


void Display_Main(void);
void Display_Init(void);  // ��ʼ������
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t wide);
void DispFrame(u16 x1, u16 y1, u16 x2, u16 y2);

void DispWave1(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp);  // Pitch
void DispWave2(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp);  // Roll

void Swing_message(float radii, float angle, u8 mode);  // �������ðڶ��뾶��ʾ


#endif

