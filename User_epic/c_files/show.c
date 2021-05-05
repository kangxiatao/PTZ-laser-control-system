#include "include.h"



/*
  全局变量
------------------------------------*/

/*-- 风力摆页面标志位  
 0 - 控制界面
 1 - 数据界面 
 2 - 设置界面 --*/
uint8_t Page=0;

/*--
  定义两个队列
  我也没办法啊，为了做界面
 --*/
Queue y1dataQueue;
Queue y2dataQueue;

/*--
  画任意边形设置数组
  最后一个用来储存自己的边数
  最大为六边形
 --*/
float Polygon[7][2];

/*
------------------------------------*/




/*********************************************************************
* 函 数 名：Display_Main 
* 函数功能：主要显示函数
* 入口参数：无
* 作    者：康夏涛（epic)
**********************************************************************/
void Display_Main(void)
{
	static u8 last_page=0;
	
	// 切换界面
	if(last_page != Page)
	{
		last_page = Page;
		
		Display_Init(); 
	}
	
	if(Page == 1)  // 数据界面
	{
		// 输入数据 波形显示
		if(Touch_Key[4].press)DispWave1(0, 170, 479, 429, Pitch.angle, 90);
		if(Touch_Key[5].press)DispWave2(0, 170, 479, 429, Roll.angle, 90);
//		if(Touch_Key[4].press)DispWave1(0, 170, 479, 429, angleTarget[1], 90);
//		if(Touch_Key[5].press)DispWave2(0, 170, 479, 429, angleTarget[0], 90);
        
        // 显示角速度信息
        POINT_COLOR=WHITE;
        if(Pitch.rate>=0)
        {
            LCD_ShowNum(188, 20, Pitch.rate, 6, 24);
        }
        else 
        {	
            LCD_ShowChar(188, 20, '-', 24, 0);
            LCD_ShowNum(200, 20, -Pitch.rate, 5, 24);
        }
        if(Roll.rate>=0)
        {
            LCD_ShowNum(188, 50, Roll.rate, 6, 24);
        }
        else 
        {
            LCD_ShowChar(188, 50, '-', 24, 0);
            LCD_ShowNum(200, 50, -Roll.rate, 5, 24);
        }
	}
	else if(Page == 0)  // 控制界面
	{
		static u8 last_task=66;
		
		// 任务切换进入
		if(last_task != Task_sign)
		{
			u8 *str;
			u8 i,j;
			
			last_task = Task_sign;
			
			// 清除上次显示
			POINT_COLOR=GRAY;
			LCD_Fill(186, 100, 186+108, 100+24, BLACK);
			
			// 任务切换显示
			switch(Task_sign)
			{
				case 0:str = (u8 *)"  ~^o^~  ";break;
				case 1:str = (u8 *)" ~ One ~ ";break;
				case 2:str = (u8 *)" ~ Two ~ ";break;
				case 3:str = (u8 *)"~ Three ~";break;
				case 4:str = (u8 *)"~ Four ~ ";break;
				case 5:str = (u8 *)"~ Five ~ ";break;
				case 6:str = (u8 *)" ~ Six ~ ";break;
				case 7:str = (u8 *)"~ Seven ~";break;
				case 8:str = (u8 *)"~ Eight ~";break;
				case 9:str = (u8 *)"~ Nine ~ ";break;
			}
			LCD_ShowString(186, 100, 108, 24, 24, str);
			
			for(i=0;i<7;i++)
				for(j=0;j<2;j++)
						Polygon[i][j]=0;
						
			if(Task_sign == 0 || Task_sign == 7 || Task_sign == 8)Display_Init();
		}
		
		// 显示摆动信息
		POINT_COLOR=WHITE;
		LCD_ShowNum(156, 630, Swing_radii, 3, 24);
		LCD_ShowNum(396, 630, Swing_angle, 3, 24);
		LCD_ShowNum(396, 655, Range_ratio, 3, 24);
	}
	else if(Page == 2)  // 设置界面
	{
		// 设置显示
		if(Setflag == 1)  // 修改半径
		{
			Swing_message(Swing_radii, Swing_angle, 1);
		}
		else if(Setflag == 2)  // 修改角度
		{
			Swing_message(Swing_radii, Swing_angle, 2);
		}
		else  // 无修改
		{
			Swing_message(Swing_radii, Swing_angle, 0);
		}
		
		// 显示摆动信息
		POINT_COLOR=WHITE;
		LCD_ShowNum(102, 475, Swing_radii, 3, 24);
		LCD_ShowNum(342, 475, Swing_angle, 3, 24);
		LCD_ShowNum(342, 500, Range_ratio, 3, 24);
	}		
	
	// 显示角度信息
	POINT_COLOR=WHITE;
	if(Pitch.angle>=0)
	{
		LCD_ShowNum(388, 20, Pitch.angle, 3, 24);
	}
	else 
	{	
		LCD_ShowChar(388, 20, '-', 24, 0);
		LCD_ShowNum(400, 20, -Pitch.angle, 2, 24);
	}
	if(Roll.angle>=0)
	{
		LCD_ShowNum(388, 50, Roll.angle, 3, 24);
	}
	else 
	{
		LCD_ShowChar(388, 50, '-', 24, 0);
		LCD_ShowNum(400, 50, -Roll.angle, 2, 24);
	}
    
    if(pitch_rate_PID.Integ>=0)
	{
		LCD_ShowNum(188, 20, pitch_rate_PID.Integ, 5, 24);
	}
	else 
	{	
		LCD_ShowChar(188, 20, '-', 24, 0);
		LCD_ShowNum(200, 20, -pitch_rate_PID.Integ, 4, 24);
	}
    
}



/*********************************************************************
* 函 数 名：Display_Init 
* 函数功能：显示函数初始化
* 入口参数：无
* 作    者：康夏涛（epic)
**********************************************************************/
void Display_Init(void)  // 初始化界面
{	
	u8 i;
	
	LCD_Clear(BLACK);
	BACK_COLOR=BLACK;
	
/*-- 初始化列队 --*/	
	
	InitQueue(&y1dataQueue);
	InitQueue(&y2dataQueue);

	// 先填满队列 解决小bug
	for (i = 0; i < MAXQUEUE; i++)
	{
		EnQueue(300, &y1dataQueue);  // 初始化值，这里要填入波形中心坐标
		EnQueue(300, &y2dataQueue);  // 初始化值，这里要填入波形中心坐标
	}
	
/*-- 触摸按键初始化 --*/
	
//	Touch_Key_Init();  // 触摸按键初始化
	
	// 切换按键初始化
	Touch_Key_Show(Touch_Key[3],3);
	// 设置按键初始化
	Touch_Key_Show(Touch_Key[6],6);
	
	if(Page == 0)  // 控制界面
	{
		u8 *str;
		
		// 控制按键
		for(i=0;i<3;i++)
		{
			Touch_Key_Show(Touch_Key[i],i);
		}
		
		POINT_COLOR=GRAY;
		DispFrame(0, 140, 479, 620);
		
		// 任务显示
//		Task_sign = 0;
		switch(Task_sign)
		{
			case 0:str = (u8 *)"  ~^o^~  ";break;
			case 1:str = (u8 *)" ~ One ~ ";break;
			case 2:str = (u8 *)" ~ Two ~ ";break;
			case 3:str = (u8 *)"~ Three ~";break;
			case 4:str = (u8 *)"~ Four ~ ";break;
			case 5:str = (u8 *)"~ Five ~ ";break;
			case 6:str = (u8 *)" ~ Six ~ ";break;
			case 7:str = (u8 *)"~ Seven ~";break;
			case 8:str = (u8 *)"~ Eight ~";break;
			case 9:str = (u8 *)"~ Nine ~ ";break;
		}
		LCD_ShowString(186, 100, 108, 24, 24, str);
	
		// 摆动参数显示初始化
		LCD_ShowString(0, 630, 144, 24, 24, (u8 *)"Swing Radii:");
		LCD_ShowString(240, 630, 144, 24, 24, (u8 *)"Swing Angle:");
	}
	else if(Page == 1) // 数据界面
	{
		for(i=4;i<6;i++)
		{
			Touch_Key_Show(Touch_Key[i],i);
		}
		
		POINT_COLOR=YELLOW;
		DispFrame(0, 120, 479, 479);
	}
	else if(Page == 2) // 设置界面
	{
		// 摆动参数显示初始化
		POINT_COLOR=GRAY;
		LCD_ShowString(48, 200, 144, 24, 24, (u8 *)"Swing Radii:");
		LCD_ShowString(288, 200, 144, 24, 24, (u8 *)"Swing Angle:");
		
		POINT_COLOR=WHITE;
		Display_4X4_Keypad(0,540,479,798,24);
		
		Swing_message(Swing_radii, Swing_angle, 1);
	}
	
	// 数据显示初始化
	POINT_COLOR=GRAY;
	LCD_ShowString(316, 20, 144, 24, 24, (u8 *)"Pitch:    C'");
	LCD_ShowString(316, 50, 144, 24, 24, (u8 *)"Roll :    C'");
}





/**********************************************************************************
*	函 数 名: LCD_DrawPoints
*	功能说明: 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。
*	形    参：
*			x, y     ：坐标数组
*			_usSize  ：数组大小
*			wide	 : 偏移度
*	返 回 值: 无
************************************************************************************/
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t wide)
{
	uint16_t i;
	
	for (i = 0; i < _usSize - 1; i++)
	{
		LCD_DrawLine(x[i] + wide, y[i], x[i + 1] + wide, y[i + 1]);
	}
}
void LCD_DrawPoints1(uint16_t *x, const Queue *pQueue, uint16_t _usSize, uint16_t wide)
{
	uint16_t i;
	uint16_t _queue;
	Node *temp;
	temp = pQueue->front;
	
	for (i = 0; i < _usSize - 1; i++)
	{
		_queue = temp->item;
		temp = temp->next;
		LCD_DrawLine(x[i] + wide, _queue, x[i + 1] + wide, temp->item);
	}
}


/*
************************************************
*	函 数 名: DispFrame
*	功能说明: 显示波形窗口的边框和刻度线
*	形    参：x1,y1,x2,y2
		注意（x2 - x1 + 1）要为12的倍数
			（y2 - y1 + 1）要为8的倍数
		不然影响界面美观
*	返 回 值: 无
************************************************
*/
void DispFrame(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 x,y;
//	u16 tx,ty;
	u16 xx,yy;
	
	xx = x2 - x1 + 1;
	yy = y2 - y1 + 1;
//	tx = xx / 12;
//	ty = yy / 8;
	
	/* 绘制一个实线矩形框 */
	LCD_DrawRectangle(x1, y1, x2, y2);

	/* 绘制垂直刻度点 */
//	for (x = 0; x < xx; x += tx)
//	{
//		for (y = 0; y < yy; y += 5)
//		{
//			LCD_DrawPoint(x, y1 + y);
//		}
//	}

	/* 绘制水平刻度点 */
//	for (y = 0; y < yy; y += ty)
//	{
//		for (x = 0; x < xx; x += 5)
//		{
//			LCD_DrawPoint(x, y1 + y);
//		}
//	}

	/* 绘制垂直中心刻度点 */
	for (y = 0; y < yy; y += 5)
	{	 
		LCD_DrawPoint(((x1+x2)/2), y1 + y);
//		LCD_DrawPoint(((x1+x2)/2-1), y1 + y);
//		LCD_DrawPoint(((x1+x2)/2+1), y1 + y);
	}

	/* 绘制水平中心刻度点 */
	for (x = 0; x < xx; x += 5)
	{	 
		LCD_DrawPoint(x, ((y1+y2)/2));
//		LCD_DrawPoint(x, ((y1+y2)/2-1));
//		LCD_DrawPoint(x, ((y1+y2)/2+1));
	}
}



/*-- (这里有两个函数，不是重复的代码，代表显示两种波，时间原因，没有封装太好) --*/

/**********************************************************************************
*	函 数 名: DispWave 
*	功能说明: 用于波形显示。 移植要把定义的列队移植过去 
*	形    参：
*			x1,y1	 : 起始坐标
*			x2,y2    : 结束坐标
*			data     ：数据
*			pp       ：峰值

*	返 回 值: 无
************************************************************************************/
void DispWave1(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp)
{
	static u8 first_f=1;  
	static u16 xdata[MAXQUEUE];
	u16 ty,dy;
	
	// 第一次进入初始化
	if(first_f == 1)
	{
		u16 i_flag;  
		
		for(i_flag=0;i_flag<MAXQUEUE;i_flag++)  // MAXQUEUE个数据
		{
			// 转换为坐标
			xdata[i_flag] = i_flag * (x2 - x1 + 1) / MAXQUEUE;
		}
		
		first_f = 0;
	}
	
	// 表示波形范围
	ty = y2 - y1 + 1;  

	// 转换为坐标
	dy = ty * (data + pp / 2) / pp + y1;
	
	// 清屏
	POINT_COLOR=BLACK;
//	(我靠，传首地址后面都是错的，后来传入的数据并不是连续地址)
//	LCD_DrawPoints(xdata, &(ydataQueue.front->item), MAXQUEUE, 0);  // 清除上次波形 
	LCD_DrawPoints1(xdata, &y1dataQueue, MAXQUEUE, 0);  // 清除上次波形 
	POINT_COLOR=YELLOW;  
	DispFrame(x1, y1-50, x2, y2+50);  // 重新画边框和刻度线
	
	// 传人数据
	DeQueue(&y1dataQueue);  // 删除首相
	EnQueue(dy, &y1dataQueue);
	
	// 画出本次波形
	POINT_COLOR=RED;
//	LCD_DrawPoints(xdata, &(ydataQueue.front->item), MAXQUEUE, 0);  
	LCD_DrawPoints1(xdata, &y1dataQueue, MAXQUEUE, 0);
}
void DispWave2(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp)
{
	static u8 first_f=1;  
	static u16 xdata[MAXQUEUE];
	u16 ty,dy;
	
	// 第一次进入初始化
	if(first_f == 1)
	{
		u16 i_flag;  
		
		for(i_flag=0;i_flag<MAXQUEUE;i_flag++)  // MAXQUEUE个数据
		{
			// 转换为坐标
			xdata[i_flag] = i_flag * (x2 - x1 + 1) / MAXQUEUE;
		}
		
		first_f = 0;
	}
	
	// 表示波形范围
	ty = y2 - y1 + 1; 

	// 转换为坐标
	dy = ty * (data + pp / 2) / pp + y1;
	
	// 清屏
	POINT_COLOR=BLACK;
	LCD_DrawPoints1(xdata, &y2dataQueue, MAXQUEUE, 0);  // 清除上次波形 
	POINT_COLOR=YELLOW;  
	DispFrame(x1, y1-50, x2, y2+50);  // 重新画边框和刻度线
	
	// 传人数据
	DeQueue(&y2dataQueue);  // 删除首相
	EnQueue(dy, &y2dataQueue);
	
	// 画出本次波形
	POINT_COLOR=GBLUE;
	LCD_DrawPoints1(xdata, &y2dataQueue, MAXQUEUE, 0);
}


/**********************************************************************************
*	函 数 名: Swing_message 
*	功能说明: 用于设置摆动半径显示
*	形    参：
*			radii	 : 摆动半径
*			angle    : 摆动角度
*			mode     ：无(0) 半径设置(1) 角度设置(2)

*	返 回 值: 无
************************************************************************************/
void Swing_message(float radii, float angle, u8 mode)
{
	u16 x1=0,y1=230,x2=479,y2=470;  // 这里为设置界面的对角坐标
	u16 xx1,yy1,xx2,yy2,dx,dy,dr,length;  
	float sin_angle,cos_angle;
	
	// 储存上次传入数据
	static float last_radii=66, last_angle=66;
	static u8 last_mode=66;
	
	// 解决小bug 没话说
	angle += 1;
	if(radii >= 50)radii=50;
	if(angle >= 180)angle=180;
	
	if(last_radii != radii || last_angle != angle || last_mode != mode)
	{
		last_radii = radii;
		last_angle = angle;
		last_mode = mode;
		
		// 中心点
		xx1 = (x2-x1+1)/4+x1;
		xx2 = xx1+(x2-x1+1)/2;
		yy1 = (y2-y1)/2+y1;
		yy2 = yy1;
		
		// 1/2角度的sin值
		sin_angle = sinf(angle*3.1415926/180.0/2.0);
		cos_angle = cosf(angle*3.1415926/180.0/2.0);
		
		dr = ((y2-y1+1)/4)*radii/50;  // 坐标半径
		
		length = sin_angle*dr*2;  // 得出第三边长（其他两边是圆的半径）
		
		dy = length*sin_angle;  // 得出y轴偏移
		dx = length*cos_angle;  // 得出x轴偏移
		
		// 无设置
		if(mode == 0){
			
			LCD_Fill(x1,y1,(x2-x1)/2+x1,y2,BLACK);  // 清除上次显示
			LCD_Fill((x2-x1)/2+x1+1,y1,x2,y2,BLACK);  // 清除上次显示
			
			POINT_COLOR=BLACK;
			LCD_DrawRectangle(x1,y1,(x2-x1)/2+x1,y2);  // 半径设置位置
			LCD_DrawRectangle((x2-x1)/2+x1+1,y1,x2,y2);  // 角度设置位置
		}
		// 半径设置
		else if(mode == 1){
			
			LCD_Fill(x1,y1,(x2-x1)/2+x1,y2,BLACK);  // 清除上次显示
			
			POINT_COLOR=YELLOW;
			LCD_DrawRectangle(x1,y1,(x2-x1)/2+x1,y2);  // 半径设置位置
			POINT_COLOR=BLACK;
			LCD_DrawRectangle((x2-x1)/2+x1+1,y1,x2,y2);  // 角度设置位置
		}
		// 角度设置
		else if(mode == 2){
			
			LCD_Fill((x2-x1)/2+x1+1,y1,x2,y2,BLACK);  // 清除上次显示
			
			POINT_COLOR=BLACK;
			LCD_DrawRectangle(x1,y1,(x2-x1)/2+x1,y2);  // 半径设置位置
			POINT_COLOR=YELLOW;
			LCD_DrawRectangle((x2-x1)/2+x1+1,y1,x2,y2);  // 角度设置位置
		}
		
		POINT_COLOR=GRAY;
		LCD_DrawLine(xx1,yy1-dr,xx1,yy1+dr);  // 半径设置位置
		LCD_DrawLine(xx2-dx,yy2+dr-dy,xx2+dx,yy2-dr+dy);  // 角度设置位置
	}
}

