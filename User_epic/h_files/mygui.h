#ifndef _mygui_h_
#define _mygui_h_

#include "sys.h"


//x,y:��ʼ����
//*p:���������׵�ַ
//mode: [7]:��������(1)��������(0) - [3]:Բ��(1)����(0)
//press:����(1)����(0) (����ֵ)
typedef struct 
{
	u16 x;
	u16 y;
	u8 *p;  
	u8 mode;  
	u8 press;
	
} KeyInfo;


// ���尴������
#define Key_Number 7

// �����Ͱ�����Ϣ
extern KeyInfo Touch_Key[Key_Number];


// �û�����API
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
void Touch_Key_Show(KeyInfo keyinfo, u8 number);  // ����������ʾ
void Touch_Key_Init(void);  // ������ʾ��ʼ��
u8 Set_Touch_Key(KeyInfo keyinfo, u8 press, u8 number);  // ���ð�����ʾ
u8 Scan_Touch_Key(KeyInfo keyinfo, u8 number);  // ��������ɨ��

void Display_4X4_Keypad(u16 x1, u16 y1, u16 x2, u16 y2, u8 size);  // 4X4�������
s8 Touch_Keyboard(u16 x1, u16 y1, u16 x2, u16 y2);






#endif
