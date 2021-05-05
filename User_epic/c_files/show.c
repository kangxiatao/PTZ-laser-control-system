#include "include.h"



/*
  ȫ�ֱ���
------------------------------------*/

/*-- ������ҳ���־λ  
 0 - ���ƽ���
 1 - ���ݽ��� 
 2 - ���ý��� --*/
uint8_t Page=0;

/*--
  ������������
  ��Ҳû�취����Ϊ��������
 --*/
Queue y1dataQueue;
Queue y2dataQueue;

/*--
  �����������������
  ���һ�����������Լ��ı���
  ���Ϊ������
 --*/
float Polygon[7][2];

/*
------------------------------------*/




/*********************************************************************
* �� �� ����Display_Main 
* �������ܣ���Ҫ��ʾ����
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Display_Main(void)
{
	static u8 last_page=0;
	
	// �л�����
	if(last_page != Page)
	{
		last_page = Page;
		
		Display_Init(); 
	}
	
	if(Page == 1)  // ���ݽ���
	{
		// �������� ������ʾ
		if(Touch_Key[4].press)DispWave1(0, 170, 479, 429, Pitch.angle, 90);
		if(Touch_Key[5].press)DispWave2(0, 170, 479, 429, Roll.angle, 90);
//		if(Touch_Key[4].press)DispWave1(0, 170, 479, 429, angleTarget[1], 90);
//		if(Touch_Key[5].press)DispWave2(0, 170, 479, 429, angleTarget[0], 90);
        
        // ��ʾ���ٶ���Ϣ
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
	else if(Page == 0)  // ���ƽ���
	{
		static u8 last_task=66;
		
		// �����л�����
		if(last_task != Task_sign)
		{
			u8 *str;
			u8 i,j;
			
			last_task = Task_sign;
			
			// ����ϴ���ʾ
			POINT_COLOR=GRAY;
			LCD_Fill(186, 100, 186+108, 100+24, BLACK);
			
			// �����л���ʾ
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
		
		// ��ʾ�ڶ���Ϣ
		POINT_COLOR=WHITE;
		LCD_ShowNum(156, 630, Swing_radii, 3, 24);
		LCD_ShowNum(396, 630, Swing_angle, 3, 24);
		LCD_ShowNum(396, 655, Range_ratio, 3, 24);
	}
	else if(Page == 2)  // ���ý���
	{
		// ������ʾ
		if(Setflag == 1)  // �޸İ뾶
		{
			Swing_message(Swing_radii, Swing_angle, 1);
		}
		else if(Setflag == 2)  // �޸ĽǶ�
		{
			Swing_message(Swing_radii, Swing_angle, 2);
		}
		else  // ���޸�
		{
			Swing_message(Swing_radii, Swing_angle, 0);
		}
		
		// ��ʾ�ڶ���Ϣ
		POINT_COLOR=WHITE;
		LCD_ShowNum(102, 475, Swing_radii, 3, 24);
		LCD_ShowNum(342, 475, Swing_angle, 3, 24);
		LCD_ShowNum(342, 500, Range_ratio, 3, 24);
	}		
	
	// ��ʾ�Ƕ���Ϣ
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
* �� �� ����Display_Init 
* �������ܣ���ʾ������ʼ��
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Display_Init(void)  // ��ʼ������
{	
	u8 i;
	
	LCD_Clear(BLACK);
	BACK_COLOR=BLACK;
	
/*-- ��ʼ���ж� --*/	
	
	InitQueue(&y1dataQueue);
	InitQueue(&y2dataQueue);

	// ���������� ���Сbug
	for (i = 0; i < MAXQUEUE; i++)
	{
		EnQueue(300, &y1dataQueue);  // ��ʼ��ֵ������Ҫ���벨����������
		EnQueue(300, &y2dataQueue);  // ��ʼ��ֵ������Ҫ���벨����������
	}
	
/*-- ����������ʼ�� --*/
	
//	Touch_Key_Init();  // ����������ʼ��
	
	// �л�������ʼ��
	Touch_Key_Show(Touch_Key[3],3);
	// ���ð�����ʼ��
	Touch_Key_Show(Touch_Key[6],6);
	
	if(Page == 0)  // ���ƽ���
	{
		u8 *str;
		
		// ���ư���
		for(i=0;i<3;i++)
		{
			Touch_Key_Show(Touch_Key[i],i);
		}
		
		POINT_COLOR=GRAY;
		DispFrame(0, 140, 479, 620);
		
		// ������ʾ
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
	
		// �ڶ�������ʾ��ʼ��
		LCD_ShowString(0, 630, 144, 24, 24, (u8 *)"Swing Radii:");
		LCD_ShowString(240, 630, 144, 24, 24, (u8 *)"Swing Angle:");
	}
	else if(Page == 1) // ���ݽ���
	{
		for(i=4;i<6;i++)
		{
			Touch_Key_Show(Touch_Key[i],i);
		}
		
		POINT_COLOR=YELLOW;
		DispFrame(0, 120, 479, 479);
	}
	else if(Page == 2) // ���ý���
	{
		// �ڶ�������ʾ��ʼ��
		POINT_COLOR=GRAY;
		LCD_ShowString(48, 200, 144, 24, 24, (u8 *)"Swing Radii:");
		LCD_ShowString(288, 200, 144, 24, 24, (u8 *)"Swing Angle:");
		
		POINT_COLOR=WHITE;
		Display_4X4_Keypad(0,540,479,798,24);
		
		Swing_message(Swing_radii, Swing_angle, 1);
	}
	
	// ������ʾ��ʼ��
	POINT_COLOR=GRAY;
	LCD_ShowString(316, 20, 144, 24, 24, (u8 *)"Pitch:    C'");
	LCD_ShowString(316, 50, 144, 24, 24, (u8 *)"Roll :    C'");
}





/**********************************************************************************
*	�� �� ��: LCD_DrawPoints
*	����˵��: ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ��
*	��    �Σ�
*			x, y     ����������
*			_usSize  �������С
*			wide	 : ƫ�ƶ�
*	�� �� ֵ: ��
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
*	�� �� ��: DispFrame
*	����˵��: ��ʾ���δ��ڵı߿�Ϳ̶���
*	��    �Σ�x1,y1,x2,y2
		ע�⣨x2 - x1 + 1��ҪΪ12�ı���
			��y2 - y1 + 1��ҪΪ8�ı���
		��ȻӰ���������
*	�� �� ֵ: ��
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
	
	/* ����һ��ʵ�߾��ο� */
	LCD_DrawRectangle(x1, y1, x2, y2);

	/* ���ƴ�ֱ�̶ȵ� */
//	for (x = 0; x < xx; x += tx)
//	{
//		for (y = 0; y < yy; y += 5)
//		{
//			LCD_DrawPoint(x, y1 + y);
//		}
//	}

	/* ����ˮƽ�̶ȵ� */
//	for (y = 0; y < yy; y += ty)
//	{
//		for (x = 0; x < xx; x += 5)
//		{
//			LCD_DrawPoint(x, y1 + y);
//		}
//	}

	/* ���ƴ�ֱ���Ŀ̶ȵ� */
	for (y = 0; y < yy; y += 5)
	{	 
		LCD_DrawPoint(((x1+x2)/2), y1 + y);
//		LCD_DrawPoint(((x1+x2)/2-1), y1 + y);
//		LCD_DrawPoint(((x1+x2)/2+1), y1 + y);
	}

	/* ����ˮƽ���Ŀ̶ȵ� */
	for (x = 0; x < xx; x += 5)
	{	 
		LCD_DrawPoint(x, ((y1+y2)/2));
//		LCD_DrawPoint(x, ((y1+y2)/2-1));
//		LCD_DrawPoint(x, ((y1+y2)/2+1));
	}
}



/*-- (���������������������ظ��Ĵ��룬������ʾ���ֲ���ʱ��ԭ��û�з�װ̫��) --*/

/**********************************************************************************
*	�� �� ��: DispWave 
*	����˵��: ���ڲ�����ʾ�� ��ֲҪ�Ѷ�����ж���ֲ��ȥ 
*	��    �Σ�
*			x1,y1	 : ��ʼ����
*			x2,y2    : ��������
*			data     ������
*			pp       ����ֵ

*	�� �� ֵ: ��
************************************************************************************/
void DispWave1(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp)
{
	static u8 first_f=1;  
	static u16 xdata[MAXQUEUE];
	u16 ty,dy;
	
	// ��һ�ν����ʼ��
	if(first_f == 1)
	{
		u16 i_flag;  
		
		for(i_flag=0;i_flag<MAXQUEUE;i_flag++)  // MAXQUEUE������
		{
			// ת��Ϊ����
			xdata[i_flag] = i_flag * (x2 - x1 + 1) / MAXQUEUE;
		}
		
		first_f = 0;
	}
	
	// ��ʾ���η�Χ
	ty = y2 - y1 + 1;  

	// ת��Ϊ����
	dy = ty * (data + pp / 2) / pp + y1;
	
	// ����
	POINT_COLOR=BLACK;
//	(�ҿ������׵�ַ���涼�Ǵ�ģ�������������ݲ�����������ַ)
//	LCD_DrawPoints(xdata, &(ydataQueue.front->item), MAXQUEUE, 0);  // ����ϴβ��� 
	LCD_DrawPoints1(xdata, &y1dataQueue, MAXQUEUE, 0);  // ����ϴβ��� 
	POINT_COLOR=YELLOW;  
	DispFrame(x1, y1-50, x2, y2+50);  // ���»��߿�Ϳ̶���
	
	// ��������
	DeQueue(&y1dataQueue);  // ɾ������
	EnQueue(dy, &y1dataQueue);
	
	// �������β���
	POINT_COLOR=RED;
//	LCD_DrawPoints(xdata, &(ydataQueue.front->item), MAXQUEUE, 0);  
	LCD_DrawPoints1(xdata, &y1dataQueue, MAXQUEUE, 0);
}
void DispWave2(u16 x1, u16 y1, u16 x2, u16 y2, int data, u16 pp)
{
	static u8 first_f=1;  
	static u16 xdata[MAXQUEUE];
	u16 ty,dy;
	
	// ��һ�ν����ʼ��
	if(first_f == 1)
	{
		u16 i_flag;  
		
		for(i_flag=0;i_flag<MAXQUEUE;i_flag++)  // MAXQUEUE������
		{
			// ת��Ϊ����
			xdata[i_flag] = i_flag * (x2 - x1 + 1) / MAXQUEUE;
		}
		
		first_f = 0;
	}
	
	// ��ʾ���η�Χ
	ty = y2 - y1 + 1; 

	// ת��Ϊ����
	dy = ty * (data + pp / 2) / pp + y1;
	
	// ����
	POINT_COLOR=BLACK;
	LCD_DrawPoints1(xdata, &y2dataQueue, MAXQUEUE, 0);  // ����ϴβ��� 
	POINT_COLOR=YELLOW;  
	DispFrame(x1, y1-50, x2, y2+50);  // ���»��߿�Ϳ̶���
	
	// ��������
	DeQueue(&y2dataQueue);  // ɾ������
	EnQueue(dy, &y2dataQueue);
	
	// �������β���
	POINT_COLOR=GBLUE;
	LCD_DrawPoints1(xdata, &y2dataQueue, MAXQUEUE, 0);
}


/**********************************************************************************
*	�� �� ��: Swing_message 
*	����˵��: �������ðڶ��뾶��ʾ
*	��    �Σ�
*			radii	 : �ڶ��뾶
*			angle    : �ڶ��Ƕ�
*			mode     ����(0) �뾶����(1) �Ƕ�����(2)

*	�� �� ֵ: ��
************************************************************************************/
void Swing_message(float radii, float angle, u8 mode)
{
	u16 x1=0,y1=230,x2=479,y2=470;  // ����Ϊ���ý���ĶԽ�����
	u16 xx1,yy1,xx2,yy2,dx,dy,dr,length;  
	float sin_angle,cos_angle;
	
	// �����ϴδ�������
	static float last_radii=66, last_angle=66;
	static u8 last_mode=66;
	
	// ���Сbug û��˵
	angle += 1;
	if(radii >= 50)radii=50;
	if(angle >= 180)angle=180;
	
	if(last_radii != radii || last_angle != angle || last_mode != mode)
	{
		last_radii = radii;
		last_angle = angle;
		last_mode = mode;
		
		// ���ĵ�
		xx1 = (x2-x1+1)/4+x1;
		xx2 = xx1+(x2-x1+1)/2;
		yy1 = (y2-y1)/2+y1;
		yy2 = yy1;
		
		// 1/2�Ƕȵ�sinֵ
		sin_angle = sinf(angle*3.1415926/180.0/2.0);
		cos_angle = cosf(angle*3.1415926/180.0/2.0);
		
		dr = ((y2-y1+1)/4)*radii/50;  // ����뾶
		
		length = sin_angle*dr*2;  // �ó������߳�������������Բ�İ뾶��
		
		dy = length*sin_angle;  // �ó�y��ƫ��
		dx = length*cos_angle;  // �ó�x��ƫ��
		
		// ������
		if(mode == 0){
			
			LCD_Fill(x1,y1,(x2-x1)/2+x1,y2,BLACK);  // ����ϴ���ʾ
			LCD_Fill((x2-x1)/2+x1+1,y1,x2,y2,BLACK);  // ����ϴ���ʾ
			
			POINT_COLOR=BLACK;
			LCD_DrawRectangle(x1,y1,(x2-x1)/2+x1,y2);  // �뾶����λ��
			LCD_DrawRectangle((x2-x1)/2+x1+1,y1,x2,y2);  // �Ƕ�����λ��
		}
		// �뾶����
		else if(mode == 1){
			
			LCD_Fill(x1,y1,(x2-x1)/2+x1,y2,BLACK);  // ����ϴ���ʾ
			
			POINT_COLOR=YELLOW;
			LCD_DrawRectangle(x1,y1,(x2-x1)/2+x1,y2);  // �뾶����λ��
			POINT_COLOR=BLACK;
			LCD_DrawRectangle((x2-x1)/2+x1+1,y1,x2,y2);  // �Ƕ�����λ��
		}
		// �Ƕ�����
		else if(mode == 2){
			
			LCD_Fill((x2-x1)/2+x1+1,y1,x2,y2,BLACK);  // ����ϴ���ʾ
			
			POINT_COLOR=BLACK;
			LCD_DrawRectangle(x1,y1,(x2-x1)/2+x1,y2);  // �뾶����λ��
			POINT_COLOR=YELLOW;
			LCD_DrawRectangle((x2-x1)/2+x1+1,y1,x2,y2);  // �Ƕ�����λ��
		}
		
		POINT_COLOR=GRAY;
		LCD_DrawLine(xx1,yy1-dr,xx1,yy1+dr);  // �뾶����λ��
		LCD_DrawLine(xx2-dx,yy2+dr-dy,xx2+dx,yy2-dr+dy);  // �Ƕ�����λ��
	}
}

