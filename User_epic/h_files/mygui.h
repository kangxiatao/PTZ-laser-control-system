#ifndef _mygui_h_
#define _mygui_h_

#include "sys.h"


//x,y:起始坐标
//*p:开关名称首地址
//mode: [7]:自锁开关(1)独立按键(0) - [3]:圆形(1)矩形(0)
//press:按下(1)松手(0) (返回值)
typedef struct 
{
	u16 x;
	u16 y;
	u8 *p;  
	u8 mode;  
	u8 press;
	
} KeyInfo;


// 定义按键个数
#define Key_Number 7

// 按键和按键信息
extern KeyInfo Touch_Key[Key_Number];


// 用户调用API
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
void Touch_Key_Show(KeyInfo keyinfo, u8 number);  // 触摸按键显示
void Touch_Key_Init(void);  // 按键显示初始化
u8 Set_Touch_Key(KeyInfo keyinfo, u8 press, u8 number);  // 设置按键显示
u8 Scan_Touch_Key(KeyInfo keyinfo, u8 number);  // 触摸按键扫描

void Display_4X4_Keypad(u16 x1, u16 y1, u16 x2, u16 y2, u8 size);  // 4X4矩阵键盘
s8 Touch_Keyboard(u16 x1, u16 y1, u16 x2, u16 y2);






#endif
