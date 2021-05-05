#include "mygui.h"
#include "include.h"




// 定义按键和按键信息
// x,y:起始坐标
// *p:开关名称首地址
// mode: [7]:自锁开关(1)独立按键(0) - [3]:圆形(1)矩形(0)
// press:按下(1)松手(0) (返回值)
KeyInfo Touch_Key[Key_Number] = {
//     x   y        str     mode press
	{380, 740, (u8*)"start", 0x88, 0},  // 锁定解锁开关
	{80 , 720, (u8*)" <<  ", 0x00, 0},  // <<
	{200, 720, (u8*)" >>  ", 0x00, 0},  // >>
	{20 , 20 , (u8*)"@tea ", 0x80, 0},  // 切换页面
	{160, 560, (u8*)"pitch", 0x80, 0},  // pitch数据显示开关
	{260, 560, (u8*)"roll ", 0x80, 0},  // roll数据显示开关
	{20 , 70 , (u8*)"@set ", 0x80, 0},  // 设置按键
};


void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);  //画水平线
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);  //画实心圆
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);  //画一条粗线


/*********************************************************************
* 函 数 名：Set_Touch_Key
* 函数功能：触摸按键显示设置
* 入口参数：

 - keyinfo:	按键信息
 - press:	按下(1)松手(0)
 - number:	第几个按键

* 作    者：康夏涛（epic)
**********************************************************************/
u8 Set_Touch_Key(KeyInfo keyinfo, u8 press, u8 number)
{
	u16 x1,x2,y1,y2;
	static u8 last_press[Key_Number]={0};
	static u8 last_page=0;  // 解决切换界面bug
	
	x1=keyinfo.x;
	x2=65+x1;
	y1=keyinfo.y;
	y2=36+y1;
	
	if(keyinfo.mode&0x80)  // 自锁开关
	{
		static u8 press_flag[Key_Number]={0};
		
		// 这里用来解决外部切换界面bug
		if(last_page != Page)
		{
			last_page = Page;
			
			// 清除某些数组
			press_flag[0] = 0;
			press_flag[4] = 0;
			press_flag[5] = 0;
		}
		
		if(last_press[number] != press)
		{
			last_press[number] = press;
			
			if(press)press_flag[number] = !press_flag[number];
			
			POINT_COLOR=BLACK;
			
			if(keyinfo.mode&0x08)  // 圆形
			{
				gui_fill_circle(x1,y1,39,LGRAY);
				
				if(press_flag[number])gui_fill_circle(x1,y1,36,DARKBLUE);
				else gui_fill_circle(x1,y1,36,GRAY);
				
				LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
			}
			else  // 矩形
			{
				LCD_Fill(x1,y1,x2,y2,LGRAY);
				
				if(press_flag[number])LCD_Fill(x1+3,y1+3,x2-3,y2-3,BRRED);
				else LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
				
				LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
			}
		}
		
		return press_flag[number];
	}
	else  // 独立按键
	{
		if(last_press[number] != press)
		{
			last_press[number] = press;
			
			POINT_COLOR=BLACK;
			
			if(keyinfo.mode&0x08)  // 圆形
			{
				gui_fill_circle(x1,y1,39,LGRAY);
				
				if(press)gui_fill_circle(x1,y1,36,DARKBLUE);
				else gui_fill_circle(x1,y1,36,GRAY);
				
				LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
			}
			else   // 矩形
			{
				LCD_Fill(x1,y1,x2,y2,LGRAY);
				
				if(press)LCD_Fill(x1+3,y1+3,x2-3,y2-3,BRRED);
				else LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
				
				LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
			}
		}
		
		return press;
	}
}


/*********************************************************************
* 函 数 名：Touch_Key_Show()
* 函数功能：触摸按键显示
* 入口参数：

 - keyinfo:	按键信息
 - number:	第几个按键

* 作    者：康夏涛（epic)
**********************************************************************/
void Touch_Key_Show(KeyInfo keyinfo, u8 number)
{
	u16 x1,x2,y1,y2;
	
	x1=keyinfo.x;
	x2=65+x1;
	y1=keyinfo.y;
	y2=36+y1;
	
	if(keyinfo.mode&0x80)  // 自锁开关
	{
		POINT_COLOR=BLACK;
		
		if(keyinfo.mode&0x08)  // 圆形
		{
			gui_fill_circle(x1,y1,39,LGRAY);
			
			gui_fill_circle(x1,y1,36,GRAY);
			
			LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
		}
		else  // 矩形
		{
			LCD_Fill(x1,y1,x2,y2,LGRAY);
			
			LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
			
			LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
		}
	}
	else  // 独立按键
	{
		POINT_COLOR=BLACK;
		
		if(keyinfo.mode&0x08)  // 圆形
		{
			gui_fill_circle(x1,y1,39,LGRAY);
			
			gui_fill_circle(x1,y1,36,GRAY);
			
			LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
		}
		else   // 矩形
		{
			LCD_Fill(x1,y1,x2,y2,LGRAY);
			
			LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
			
			LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
		}
	}
}



/*********************************************************************
* 函 数 名：Touch_Key_init()
* 函数功能：按键显示初始化
* 入口参数：无
* 作    者：康夏涛（epic)
**********************************************************************/
void Touch_Key_Init(void)
{
	u8 i;
	
	for(i=0;i<Key_Number;i++)
	{
		Set_Touch_Key(Touch_Key[i],1,i);  // 假装已经按了一次，解决小bug
		Set_Touch_Key(Touch_Key[i],0,i);
		Set_Touch_Key(Touch_Key[i],1,i);
	}
}


/*********************************************************************
* 函 数 名：Scan_Touch_Key()
* 函数功能：触摸按键扫描
* 入口参数：
 - keyinfo:	按键信息
 - number:	第几个按键
* 返 回 值：返回按键是否按下信息
* 作    者：康夏涛（epic)
**********************************************************************/
u8 Scan_Touch_Key(KeyInfo keyinfo, u8 number)
{
	u8 press;

	tp_dev.scan(0);
	
	if((tp_dev.sta&0x80)==0x80)		//有按键按下
	{
		if(keyinfo.mode&0x08){
			if( (tp_dev.x[0]>keyinfo.x-39)&&(tp_dev.x[0]<keyinfo.x+39)
				&&(tp_dev.y[0]>keyinfo.y-39)&&(tp_dev.y[0]<keyinfo.y+39) )	//在区域内
			{
				press = 1;
			}
			else press = 0;
		}
		else {
			if( (tp_dev.x[0]>keyinfo.x)&&(tp_dev.x[0]<keyinfo.x+65)
				&&(tp_dev.y[0]>keyinfo.y)&&(tp_dev.y[0]<keyinfo.y+36) )	//在区域内
			{
				press = 1;
			}
			else press = 0;
		}
	}
	else press = 0;
	
	press = Set_Touch_Key(keyinfo, press, number);
	
	return press;
}



/********************************************************************************
函数功能：画水平线
入口参数：x0,y0:坐标；
		  len:线长度
		  color:颜色
返回参数：无
*********************************************************************************/
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0) return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}


/********************************************************************************
函数功能：画实心圆
入口参数：x0,y0:坐标
		  r:半径
		  color:颜色
返回参数：无
*********************************************************************************/
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  


/********************************************************************************
函数功能：显示4*4数字键盘
入口参数：(x1,y1),(x2,y2):键盘的对角坐标
		  size：键盘数字大小
返回参数：无
函数作者：@FUYOU
*********************************************************************************/
void Display_4X4_Keypad(u16 x1, u16 y1, u16 x2, u16 y2, u8 size)
{
	u16 x, y;
	
	if((x1>x2)||(y1>y2)||(x1>lcddev.width)||(x2>lcddev.width)||(y1>lcddev.height)||(y2>lcddev.height)) return;
	
	x=(x2-x1)/4;
	y=(y2-y1)/4;
	
//	LCD_DrawRectangle(x1,y1,x2,y2);		//画框
//	
//	/*画竖线*/
//	LCD_DrawLine(x1+x,y1,x1+x,y2);
//	LCD_DrawLine(x1+x*2,y1,x1+x*2,y2);
//	LCD_DrawLine(x1+x*3,y1,x1+x*3,y2);
//	
//	/*画横线*/
//	LCD_DrawLine(x1,y1+y,x2,y1+y);
//	LCD_DrawLine(x1,y1+y*2,x2,y1+y*2);
//	LCD_DrawLine(x1,y1+y*3,x2,y1+y*3);
	
	/*显示键盘内容*/
	/*第一行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+(y/2-size/2),'1',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+(y/2-size/2),'2',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+(y/2-size/2),'3',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+(y/2-size/2),'C',size,1);
	
	/*第二行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y+(y/2-size/2),'4',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y+(y/2-size/2),'5',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y+(y/2-size/2),'6',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y+(y/2-size/2),'+',size,1);
	
	/*第三行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y*2+(y/2-size/2),'7',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y*2+(y/2-size/2),'8',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*2+(y/2-size/2),'9',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y*2+(y/2-size/2),'-',size,1);
	
	/*第四行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y*3+(y/2-size/2),'.',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y*3+(y/2-size/2),'0',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*3+(y/2-size/2),'<',size,1);
	LCD_ShowString(x1+x*3+(x/2-size/2),y1+y*3+(y/2-size/2),24,24,24,(u8*)"OK");
}


/********************************************************************************
函数功能：4*4触摸按键,支持连按
入口参数：(x1,y1),(x2,y2):键盘的对角坐标
返回参数：按键值
函数作者：@FUYOU
*********************************************************************************/
s8 Touch_Keyboard(u16 x1, u16 y1, u16 x2, u16 y2)
{
	static u8 Continuous=0;
	u8 flag=0,size=24;
	u16 x, y, t;
	u16 KeyColor=LIGHTBLUE;
	u16 _keycolor_=BLACK;
	
	x=(x2-x1)/4;
	y=(y2-y1)/4;
	
	tp_dev.scan(0);
	
	if((tp_dev.sta&0x80)==0x80)		//有按键按下
	{
		if(((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x2))&&((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y2)))	//在区域内
		{
			if((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y1+y))	//第一行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 1;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 2;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 3;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 12;
				}
				else flag = 66;  // 没有操作
			}
			else if((tp_dev.y[0]>y1+y)&&(tp_dev.y[0]<y1+y*2))	//第二行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 4;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 5;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 6;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 13;
				}
				else flag = 66;  // 没有操作
			}
			else if((tp_dev.y[0]>y1+y*2)&&(tp_dev.y[0]<y1+y*3))	//第三行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 7;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 8;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 9;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 14;
				}
				else flag = 66;  // 没有操作
			}
			else if((tp_dev.y[0]>y1+y*3)&&(tp_dev.y[0]<y1+y*4))	//第四行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 10;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 0;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 11;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 15;
				}
				else flag = 66;  // 没有操作
			}
			else flag = 66;  // 没有操作
			
			/*按下显示提示*/
			switch(flag)
			{
				case 1:LCD_Fill(x1+1,y1+1,x1+x-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+(y/2-size/2),'1',size,1);
					break;
				case 2:LCD_Fill(x1+x+1,y1+1,x1+x*2-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+(y/2-size/2),'2',size,1);
					break;
				case 3:LCD_Fill(x1+x*2+1,y1+1,x1+x*3-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+(y/2-size/2),'3',size,1);
					break;
				case 12:LCD_Fill(x1+x*3+1,y1+1,x1+x*4-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+(y/2-size/2),'C',size,1);
					break;
				
				case 4:LCD_Fill(x1+1,y1+y+1,x1+x-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+y+(y/2-size/2),'4',size,1);
					break;
				case 5:LCD_Fill(x1+x+1,y1+y+1,x1+x*2-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y+(y/2-size/2),'5',size,1);
					break;
				case 6:LCD_Fill(x1+x*2+1,y1+y+1,x1+x*3-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y+(y/2-size/2),'6',size,1);
					break;
				case 13:LCD_Fill(x1+x*3+1,y1+y+1,x1+x*4-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y+(y/2-size/2),'+',size,1);
					break;
							
				case 7:LCD_Fill(x1+1,y1+y*2+1,x1+x-1,y1+y*3,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+y*2+(y/2-size/2),'7',size,1);
					break;
				case 8:LCD_Fill(x1+x+1,y1+y*2+1,x1+x*2-1,y1+y*3-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y*2+(y/2-size/2),'8',size,1);
					break;
				case 9:LCD_Fill(x1+x*2+1,y1+y*2+1,x1+x*3-1,y1+y*3-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*2+(y/2-size/2),'9',size,1);
					break;
				case 14:LCD_Fill(x1+x*3+1,y1+y*2+1,x1+x*4-1,y1+y*3-1,KeyColor);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y*2+(y/2-size/2),'-',size,1);
					break;
					
				case 10:LCD_Fill(x1+1,y1+y*3+1,x1+x-1,y1+y*4-1,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+y*3+(y/2-size/2),'.',size,1);
					break;
				case 0:LCD_Fill(x1+x+1,y1+y*3+1,x1+x*2-1,y1+y*4-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y*3+(y/2-size/2),'0',size,1);
					break;
				case 11:LCD_Fill(x1+x*2+1,y1+y*3+1,x1+x*3-1,y1+y*4-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*3+(y/2-size/2),'<',size,1);
					break;
				case 15:LCD_Fill(x1+x*3+1,y1+y*3+1,x1+x*4-1,y1+y*4-1,KeyColor);
					LCD_ShowString(x1+x*3+(x/2-size/2),y1+y*3+(y/2-size/2),24,24,24,(u8*)"OK");
					break;
			}
			
			t=0;
			while((tp_dev.sta&0x80)==0x80)	//松手检测
			{
				if(Continuous==0)		//正常模式
				{
					delay_ms(1);
					tp_dev.scan(0);
					t++;
					if(t>599)
					{
						Continuous=1;	//进入连按模式
						break;
					}
				}
				else		//连按模式
				{	
					t++;
					if(t>120)flag = 66;  // 假装没有操作一次
					break;
				}
			}
			
//			LCD_Fill(x1,y1,x2,y2,BLACK);
//			Display_4X4_Keypad(x1,y1,x2,y2,size);
			
			/*清除触摸显示*/
			switch(flag)
			{
				case 1:LCD_Fill(x1+1,y1+1,x1+x-1,y1+y-1,_keycolor_);
					LCD_ShowChar(x1+(x/2-size/4),y1+(y/2-size/2),'1',size,1);
					break;
				case 2:LCD_Fill(x1+x+1,y1+1,x1+x*2-1,y1+y-1,_keycolor_);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+(y/2-size/2),'2',size,1);
					break;
				case 3:LCD_Fill(x1+x*2+1,y1+1,x1+x*3-1,y1+y-1,_keycolor_);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+(y/2-size/2),'3',size,1);
					break;
				case 12:LCD_Fill(x1+x*3+1,y1+1,x1+x*4-1,y1+y-1,_keycolor_);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+(y/2-size/2),'C',size,1);
					break;
				
				case 4:LCD_Fill(x1+1,y1+y+1,x1+x-1,y1+y*2-1,_keycolor_);
					LCD_ShowChar(x1+(x/2-size/4),y1+y+(y/2-size/2),'4',size,1);
					break;
				case 5:LCD_Fill(x1+x+1,y1+y+1,x1+x*2-1,y1+y*2-1,_keycolor_);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y+(y/2-size/2),'5',size,1);
					break;
				case 6:LCD_Fill(x1+x*2+1,y1+y+1,x1+x*3-1,y1+y*2-1,_keycolor_);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y+(y/2-size/2),'6',size,1);
					break;
				case 13:LCD_Fill(x1+x*3+1,y1+y+1,x1+x*4-1,y1+y*2-1,_keycolor_);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y+(y/2-size/2),'+',size,1);
					break;
							
				case 7:LCD_Fill(x1+1,y1+y*2+1,x1+x-1,y1+y*3,_keycolor_);
					LCD_ShowChar(x1+(x/2-size/4),y1+y*2+(y/2-size/2),'7',size,1);
					break;
				case 8:LCD_Fill(x1+x+1,y1+y*2+1,x1+x*2-1,y1+y*3-1,_keycolor_);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y*2+(y/2-size/2),'8',size,1);
					break;
				case 9:LCD_Fill(x1+x*2+1,y1+y*2+1,x1+x*3-1,y1+y*3-1,_keycolor_);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*2+(y/2-size/2),'9',size,1);
					break;
				case 14:LCD_Fill(x1+x*3+1,y1+y*2+1,x1+x*4-1,y1+y*3-1,_keycolor_);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y*2+(y/2-size/2),'-',size,1);
					break;
					
				case 10:LCD_Fill(x1+1,y1+y*3+1,x1+x-1,y1+y*4-1,_keycolor_);
					LCD_ShowChar(x1+(x/2-size/4),y1+y*3+(y/2-size/2),'.',size,1);
					break;
				case 0:LCD_Fill(x1+x+1,y1+y*3+1,x1+x*2-1,y1+y*4-1,_keycolor_);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y*3+(y/2-size/2),'0',size,1);
					break;
				case 11:LCD_Fill(x1+x*2+1,y1+y*3+1,x1+x*3-1,y1+y*4-1,_keycolor_);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*3+(y/2-size/2),'<',size,1);
					break;
				case 15:LCD_Fill(x1+x*3+1,y1+y*3+1,x1+x*4-1,y1+y*4-1,_keycolor_);
					LCD_ShowString(x1+x*3+(x/2-size/2),y1+y*3+(y/2-size/2),24,24,24,(u8*)"OK");
					break;
			}
		}
		else flag = 66;  // 没有操作
	}
	else 
	{
		Continuous = 0;
		flag = 66;  // 没有操作
	}
	
	return flag;
}

