#include "mygui.h"
#include "include.h"




// ���尴���Ͱ�����Ϣ
// x,y:��ʼ����
// *p:���������׵�ַ
// mode: [7]:��������(1)��������(0) - [3]:Բ��(1)����(0)
// press:����(1)����(0) (����ֵ)
KeyInfo Touch_Key[Key_Number] = {
//     x   y        str     mode press
	{380, 740, (u8*)"start", 0x88, 0},  // ������������
	{80 , 720, (u8*)" <<  ", 0x00, 0},  // <<
	{200, 720, (u8*)" >>  ", 0x00, 0},  // >>
	{20 , 20 , (u8*)"@tea ", 0x80, 0},  // �л�ҳ��
	{160, 560, (u8*)"pitch", 0x80, 0},  // pitch������ʾ����
	{260, 560, (u8*)"roll ", 0x80, 0},  // roll������ʾ����
	{20 , 70 , (u8*)"@set ", 0x80, 0},  // ���ð���
};


void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);  //��ˮƽ��
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);  //��ʵ��Բ
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);  //��һ������


/*********************************************************************
* �� �� ����Set_Touch_Key
* �������ܣ�����������ʾ����
* ��ڲ�����

 - keyinfo:	������Ϣ
 - press:	����(1)����(0)
 - number:	�ڼ�������

* ��    �ߣ������Σ�epic)
**********************************************************************/
u8 Set_Touch_Key(KeyInfo keyinfo, u8 press, u8 number)
{
	u16 x1,x2,y1,y2;
	static u8 last_press[Key_Number]={0};
	static u8 last_page=0;  // ����л�����bug
	
	x1=keyinfo.x;
	x2=65+x1;
	y1=keyinfo.y;
	y2=36+y1;
	
	if(keyinfo.mode&0x80)  // ��������
	{
		static u8 press_flag[Key_Number]={0};
		
		// ������������ⲿ�л�����bug
		if(last_page != Page)
		{
			last_page = Page;
			
			// ���ĳЩ����
			press_flag[0] = 0;
			press_flag[4] = 0;
			press_flag[5] = 0;
		}
		
		if(last_press[number] != press)
		{
			last_press[number] = press;
			
			if(press)press_flag[number] = !press_flag[number];
			
			POINT_COLOR=BLACK;
			
			if(keyinfo.mode&0x08)  // Բ��
			{
				gui_fill_circle(x1,y1,39,LGRAY);
				
				if(press_flag[number])gui_fill_circle(x1,y1,36,DARKBLUE);
				else gui_fill_circle(x1,y1,36,GRAY);
				
				LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
			}
			else  // ����
			{
				LCD_Fill(x1,y1,x2,y2,LGRAY);
				
				if(press_flag[number])LCD_Fill(x1+3,y1+3,x2-3,y2-3,BRRED);
				else LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
				
				LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
			}
		}
		
		return press_flag[number];
	}
	else  // ��������
	{
		if(last_press[number] != press)
		{
			last_press[number] = press;
			
			POINT_COLOR=BLACK;
			
			if(keyinfo.mode&0x08)  // Բ��
			{
				gui_fill_circle(x1,y1,39,LGRAY);
				
				if(press)gui_fill_circle(x1,y1,36,DARKBLUE);
				else gui_fill_circle(x1,y1,36,GRAY);
				
				LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
			}
			else   // ����
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
* �� �� ����Touch_Key_Show()
* �������ܣ�����������ʾ
* ��ڲ�����

 - keyinfo:	������Ϣ
 - number:	�ڼ�������

* ��    �ߣ������Σ�epic)
**********************************************************************/
void Touch_Key_Show(KeyInfo keyinfo, u8 number)
{
	u16 x1,x2,y1,y2;
	
	x1=keyinfo.x;
	x2=65+x1;
	y1=keyinfo.y;
	y2=36+y1;
	
	if(keyinfo.mode&0x80)  // ��������
	{
		POINT_COLOR=BLACK;
		
		if(keyinfo.mode&0x08)  // Բ��
		{
			gui_fill_circle(x1,y1,39,LGRAY);
			
			gui_fill_circle(x1,y1,36,GRAY);
			
			LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
		}
		else  // ����
		{
			LCD_Fill(x1,y1,x2,y2,LGRAY);
			
			LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
			
			LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
		}
	}
	else  // ��������
	{
		POINT_COLOR=BLACK;
		
		if(keyinfo.mode&0x08)  // Բ��
		{
			gui_fill_circle(x1,y1,39,LGRAY);
			
			gui_fill_circle(x1,y1,36,GRAY);
			
			LCD_ShowString(x1-30,y1-12,60,24,24,keyinfo.p);
		}
		else   // ����
		{
			LCD_Fill(x1,y1,x2,y2,LGRAY);
			
			LCD_Fill(x1+3,y1+3,x2-3,y2-3,GRAY);
			
			LCD_ShowString(x1+9,y1+6,48,24,24,keyinfo.p);
		}
	}
}



/*********************************************************************
* �� �� ����Touch_Key_init()
* �������ܣ�������ʾ��ʼ��
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Touch_Key_Init(void)
{
	u8 i;
	
	for(i=0;i<Key_Number;i++)
	{
		Set_Touch_Key(Touch_Key[i],1,i);  // ��װ�Ѿ�����һ�Σ����Сbug
		Set_Touch_Key(Touch_Key[i],0,i);
		Set_Touch_Key(Touch_Key[i],1,i);
	}
}


/*********************************************************************
* �� �� ����Scan_Touch_Key()
* �������ܣ���������ɨ��
* ��ڲ�����
 - keyinfo:	������Ϣ
 - number:	�ڼ�������
* �� �� ֵ�����ذ����Ƿ�����Ϣ
* ��    �ߣ������Σ�epic)
**********************************************************************/
u8 Scan_Touch_Key(KeyInfo keyinfo, u8 number)
{
	u8 press;

	tp_dev.scan(0);
	
	if((tp_dev.sta&0x80)==0x80)		//�а�������
	{
		if(keyinfo.mode&0x08){
			if( (tp_dev.x[0]>keyinfo.x-39)&&(tp_dev.x[0]<keyinfo.x+39)
				&&(tp_dev.y[0]>keyinfo.y-39)&&(tp_dev.y[0]<keyinfo.y+39) )	//��������
			{
				press = 1;
			}
			else press = 0;
		}
		else {
			if( (tp_dev.x[0]>keyinfo.x)&&(tp_dev.x[0]<keyinfo.x+65)
				&&(tp_dev.y[0]>keyinfo.y)&&(tp_dev.y[0]<keyinfo.y+36) )	//��������
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
�������ܣ���ˮƽ��
��ڲ�����x0,y0:���ꣻ
		  len:�߳���
		  color:��ɫ
���ز�������
*********************************************************************************/
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0) return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}


/********************************************************************************
�������ܣ���ʵ��Բ
��ڲ�����x0,y0:����
		  r:�뾶
		  color:��ɫ
���ز�������
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
�������ܣ���ʾ4*4���ּ���
��ڲ�����(x1,y1),(x2,y2):���̵ĶԽ�����
		  size���������ִ�С
���ز�������
�������ߣ�@FUYOU
*********************************************************************************/
void Display_4X4_Keypad(u16 x1, u16 y1, u16 x2, u16 y2, u8 size)
{
	u16 x, y;
	
	if((x1>x2)||(y1>y2)||(x1>lcddev.width)||(x2>lcddev.width)||(y1>lcddev.height)||(y2>lcddev.height)) return;
	
	x=(x2-x1)/4;
	y=(y2-y1)/4;
	
//	LCD_DrawRectangle(x1,y1,x2,y2);		//����
//	
//	/*������*/
//	LCD_DrawLine(x1+x,y1,x1+x,y2);
//	LCD_DrawLine(x1+x*2,y1,x1+x*2,y2);
//	LCD_DrawLine(x1+x*3,y1,x1+x*3,y2);
//	
//	/*������*/
//	LCD_DrawLine(x1,y1+y,x2,y1+y);
//	LCD_DrawLine(x1,y1+y*2,x2,y1+y*2);
//	LCD_DrawLine(x1,y1+y*3,x2,y1+y*3);
	
	/*��ʾ��������*/
	/*��һ��*/
	LCD_ShowChar(x1+(x/2-size/4),y1+(y/2-size/2),'1',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+(y/2-size/2),'2',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+(y/2-size/2),'3',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+(y/2-size/2),'C',size,1);
	
	/*�ڶ���*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y+(y/2-size/2),'4',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y+(y/2-size/2),'5',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y+(y/2-size/2),'6',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y+(y/2-size/2),'+',size,1);
	
	/*������*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y*2+(y/2-size/2),'7',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y*2+(y/2-size/2),'8',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*2+(y/2-size/2),'9',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y*2+(y/2-size/2),'-',size,1);
	
	/*������*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y*3+(y/2-size/2),'.',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y*3+(y/2-size/2),'0',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*3+(y/2-size/2),'<',size,1);
	LCD_ShowString(x1+x*3+(x/2-size/2),y1+y*3+(y/2-size/2),24,24,24,(u8*)"OK");
}


/********************************************************************************
�������ܣ�4*4��������,֧������
��ڲ�����(x1,y1),(x2,y2):���̵ĶԽ�����
���ز���������ֵ
�������ߣ�@FUYOU
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
	
	if((tp_dev.sta&0x80)==0x80)		//�а�������
	{
		if(((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x2))&&((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y2)))	//��������
		{
			if((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y1+y))	//��һ��
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
				else flag = 66;  // û�в���
			}
			else if((tp_dev.y[0]>y1+y)&&(tp_dev.y[0]<y1+y*2))	//�ڶ���
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
				else flag = 66;  // û�в���
			}
			else if((tp_dev.y[0]>y1+y*2)&&(tp_dev.y[0]<y1+y*3))	//������
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
				else flag = 66;  // û�в���
			}
			else if((tp_dev.y[0]>y1+y*3)&&(tp_dev.y[0]<y1+y*4))	//������
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
				else flag = 66;  // û�в���
			}
			else flag = 66;  // û�в���
			
			/*������ʾ��ʾ*/
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
			while((tp_dev.sta&0x80)==0x80)	//���ּ��
			{
				if(Continuous==0)		//����ģʽ
				{
					delay_ms(1);
					tp_dev.scan(0);
					t++;
					if(t>599)
					{
						Continuous=1;	//��������ģʽ
						break;
					}
				}
				else		//����ģʽ
				{	
					t++;
					if(t>120)flag = 66;  // ��װû�в���һ��
					break;
				}
			}
			
//			LCD_Fill(x1,y1,x2,y2,BLACK);
//			Display_4X4_Keypad(x1,y1,x2,y2,size);
			
			/*���������ʾ*/
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
		else flag = 66;  // û�в���
	}
	else 
	{
		Continuous = 0;
		flag = 66;  // û�в���
	}
	
	return flag;
}

