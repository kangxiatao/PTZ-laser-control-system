/*��

 * @file       control.c
 * @brief      �����㷨
 * @author     �����Σ�epic)
 * @version    v1.0
 * @date       2019-08-04
 
------------------------------------
*/

#include "control.h"
#include "include.h"



// ����������ת���㵽����ľ���  ��λ����(CM)
#define	DISTANCE_GROUND		86.6
// �������� - (time/PERIOD)*2*pi
#define	PERIOD		320  // 1600ms 1600/5



/*
  ȫ�ֱ���
------------------------------------*/

/*-- ������־λ  
 0 - ����  
 1 - ���� --*/
uint8_t ARMED=0;

/*-- ���������ñ�־λ  
 0 - ������
 1 - ���ð뾶
 2 - ���ýǶ� --*/
uint8_t Setflag=0;

/*-- �����������־λ  
 ��ֵ��Ӧ���� --*/
uint8_t Task_sign=0;

/*-- ��ڹ켣�ٶ�
 ���������� ms*5���������� - (time/Period)*2*pi --*/
uint16_t Period=320;  // 1600ms 1600/5


/*-- ��������PID��� --*/
float Pose[2]={0};

/*-- ʵ������ --*/
MPU Pitch,Roll;
/*-- �����Ƕ� --*/
float angleTarget[2]={0};

/*-- �ڶ���������н� --*/
float Track_angle=0;
/*-- �ڶ��뾶 --*/
float Swing_radii=30;  // ��λ CM
/*-- �ڶ����� --*/
float Swing_angle=0;  // ��λ ��
/*-- �Ƕ���λ --*/
float Angle_phase=0;  // ��λ ��
/*-- ���ȱ������� --*/
float Range_ratio=0;  // ����

/*-- ����PID �ǶȻ��ͽ��ٶȻ� --*/
PID_Typedef pitch_angle_PID;		// pitch�ǶȻ�PID
PID_Typedef roll_angle_PID;   		// roll�ǶȻ�PID

PID_Typedef pitch_rate_PID;			//pitch���ٶȻ�PID
PID_Typedef roll_rate_PID;    		//roll���ٶȻ�PID

/*
------------------------------------*/





/*********************************************************************
* �� �� ����PID_Postion_Cal()
* �������ܣ�λ��ʽPID
* ��ڲ�����PID�ṹ�� , ����ֵ , ����ֵ
* ��    �ߣ������Σ�epic)
**********************************************************************/
static void PID_Postion_Cal(PID_Typedef * PID, float target, float measure)
{
	float termI;
	
	/*-- ��� = ����ֵ - ����ֵ --*/
	PID->Error = target - measure;

//	/*-- ��ǰ���΢�� --*/
//	PID->Deriv = (PID->Error - PID->PreError);
//	
//	/*-- �����ϴ���� --*/
//	PID->PreError = PID->Error;
	
	/*-- ����ֱ�ӶԲ���ֵ����΢�� (΢������) --*/
	PID->Deriv = (PID->PreError - measure);
	
	/*-- �������������ϴβ���ֵ (΢������) --*/
	PID->PreError = measure;
	
	/*-- �����Ż��� ����������� --*/
	if(ARMED){
		/*-- ���ֻ��� --*/
		termI = (PID->Integ) + (PID->Error);   
		/*-- �������� --*/
		if(termI > -PID->iLimit && termI < PID->iLimit && fabs(PID->Error) < 10)    
			PID->Integ=termI;
	}else{
		PID->Integ = 0;
	}
	
	/*-- PID��� = �������� + ���ֻ��� + ΢�ֻ��� --*/
	PID->Output = (PID->P * PID->Error) + (PID->I * PID->Integ) + (PID->D * PID->Deriv);
}


/*********************************************************************
* �� �� ����Balance_Control()
* �������ܣ�λ��ʽƽ�����
* ��ڲ�����PID�ṹ�� , �����Ƕ� , �Ƕ� , ���ٶ�
* ��    �ߣ������Σ�epic)
**********************************************************************/
static void Balance_Control(PID_Typedef * PID, float target, float angle, float rate)
{
	float termI;
    
	/*-- ��� = ����ֵ - ����ֵ --*/
	PID->Error = target - angle;
	
	/*-- ����ֱ���ý��ٶ� --*/
	PID->Deriv = rate;
    
    /*-- �����Ż��� ����������� --*/
	if(ARMED){
		/*-- ���ֻ��� --*/
		termI = (PID->Integ) + (PID->Error);   
		/*-- �������� --*/
		if(termI > -PID->iLimit && termI < PID->iLimit)    
			PID->Integ=termI;
	}else{
		PID->Integ = 0;
	}
	
	/*-- PID��� = �������� + ���ֻ��� + ΢�ֻ��� --*/
	PID->Output = (PID->P * PID->Error) + (PID->I * PID->Integ) + (PID->D * PID->Deriv);
}



/*********************************************************************
* �� �� ����TIM1_UP_IRQHandler(void)
* �������ܣ�����ϵͳ�����ж�
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)  // 5ms��ʱ�ж�
	{   
		TIM1->SR&=~(1<<0);  // ����жϱ�־λ
		
	/*-- System running LED --*/
		if(ARMED)LED0_Flash(100);
		else LED0=0;
		
	/*-- refresh data --*/  
//		Get_dmp_data(&Pitch.angle, &Roll.angle, &Pitch.rate, &Roll.rate);  // MPU6050
//		Get_AHRS_Data(&Pitch.angle, &Roll.angle, &Pitch.rate, &Roll.rate);  // ����uart3�жϷ�������
	 	
	/*-- ϵͳ�Ż� --*/
		System_Optimize();
		
	/*-- �����޸� �����й���ʵ��ȫ���ı������� --*/
		Target_Control(angleTarget);  
		
//	/*-- PID�⻷����(�Ƕȿ���) --*/
//		PID_Postion_Cal(&pitch_angle_PID, angleTarget[PITCH], Pitch.angle);  
//		PID_Postion_Cal(&roll_angle_PID, angleTarget[ROLL], Roll.angle);	
//		
//	/*-- PID�ڻ�����(���ٶȿ���) --*/
//		PID_Postion_Cal(&pitch_rate_PID, -pitch_angle_PID.Output, (float)(Pitch.rate)/10.0f);	
//		PID_Postion_Cal(&roll_rate_PID, -roll_angle_PID.Output, (float)(Roll.rate)/10.0f);

//	/*-- �ڻ�������� --*/
//		Pose[PITCH] = pitch_rate_PID.Output;
//		Pose[ROLL] = roll_rate_PID.Output;

    /*-- ƽ����� --*/
        Balance_Control(&pitch_rate_PID, angleTarget[PITCH], Pitch.angle, (float)(Pitch.rate)/1000.0f);
        Balance_Control(&roll_rate_PID, angleTarget[ROLL], Roll.angle, (float)(Roll.rate)/1000.0f);
		
	/*-- ���������� --*/
		Pose[PITCH] = -pitch_rate_PID.Output;
		Pose[ROLL] = -roll_rate_PID.Output;
		
	/*-- PWM ��� --*/
		Motor_Control(Pose);
	}
}


/*********************************************************************
* �� �� ����EPIC_Init()
* �������ܣ�ʷʫ����ʼ��
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void EPIC_Init(void)
{
    pitch_rate_PID.P = 0.8;
    pitch_rate_PID.I = 0;
    pitch_rate_PID.D = 0.8;
    roll_rate_PID.P = 0.8;
    roll_rate_PID.I = 0;
    roll_rate_PID.D = 0.8;
    
    pitch_rate_PID.iLimit = 2000;
    roll_rate_PID.iLimit = 2000;
    
	AHRS_Data_Init();  // У׼����
}	



/*********************************************************************
* �� �� ����Target_Control 
* �������ܣ��޸� Pitch,Roll ��������
* ��ڲ�����Pitch,Roll ����ֵ��ַ
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Target_Control(float *target)
{
	static u16 time = 0;
	static u16 time_1 = 0;
	static u16 time_2 = 0;
	static float last_angle = 666;  // �ϴ��趨�ڽ�
	static float last_radii = 666;  // �ϴ��趨�뾶
	static float real_radii = 0;  // ��ʵ�뾶
	static float angle_x = 0.0; // �ڶ��Ƕ�x����
	static float angle_y = 0.0; // �ڶ��Ƕ�y����
	
	// ��߳���Ч��
	if(last_radii != Swing_radii)
	{
		last_radii = Swing_radii;
		
		// �뾶��ಹ��
		real_radii = (Swing_radii + 3) * 2;
	}
	if(last_angle != Swing_angle)
	{
		last_angle = Swing_angle;
		
		angle_x = cos(Swing_angle * 180.0 / 3.14159);	 //X�������
		angle_y = sin(Swing_angle * 180.0 / 3.14159);	 //X�������
	}
	
	switch(Task_sign)
	{
		case 0:
			// ������: 0110 - 666 - 0x6666
			target[PITCH] = 0;
			target[ROLL] = 0;
			break;
		case 1:
			// ��һ�ʣ�����һ������50cm��ֱ�ߣ�����뾶Ϊ�����뾶��
			target[PITCH] = Get_AngleTarget1(80, 0, time, Period);
			target[ROLL] = 0;
			break;
		case 2:
			// �ڶ��ʣ�����������30~60cm�������ֱ���趨�ڶ�����
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = 0;
			break;
		case 3:
			// �����ʣ��趨�ڶ����򣨵��ڷ��ȣ���Ӧ������x,y��
			target[PITCH] = angle_x * Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = angle_y * Get_AngleTarget2(real_radii, 0, time, Period);
			break;
		case 4:
			// �����ʣ��ϸ���
			target[PITCH] = 0;
			target[ROLL] = 0;
			break;
		case 5:
			// �����ʣ���Բ
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = Get_AngleTarget2(real_radii, 90, time, Period);
			break;
		case 6:
			// �����ʣ���������Բ
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = Get_AngleTarget2(real_radii, 270, time, Period);
			break;
		case 7:
			// �����ʣ���㻭
			if(Range_ratio<1)Range_ratio=1;
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period) / Range_ratio;
			target[ROLL] = Get_AngleTarget2(real_radii, Swing_angle, time_1, Period*Range_ratio);
		
			// ������ڳ�ͻ
			time_1 ++;
			if(time_1>Period*Range_ratio)time_1=0;
			break;
		case 8:
			// �ڰ��ʣ���㻭�����
			if(1)
			{
				static u8 cnt=0;
				
				target[PITCH] = Polygon[cnt][0];
				target[ROLL] = Polygon[cnt][1];
				
				time_2 ++;
				if(time_2>=100){
					time_2 = 0;
					cnt ++;
				}
			
				if(cnt > Polygon[6][0])
					cnt = 0;
			}
			break;
		case 9:
			// �ھ��ʣ����ֻ�
			target[PITCH] = Polygon[0][0];
			target[ROLL] = Polygon[0][1];
			break;
	}
	
	time++;
	if(time>Period)time=0;  // Period Ϊ��������
}



/*********************************************************************
* �� �� ����Motor_Control 
* �������ܣ��ĸ������PWM���
* ��ڲ�����PID�����������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Motor_Control(float pose[])
{
	/*-- ���PWM���� --*/
	uint16_t motor[4]={0};
	
	/*    ����Ϊ��    */
    motor[0] = Serve_MID_PWM + pose[PITCH];
    motor[1] = Level_MID_PWM + pose[ROLL];
    motor[2] = Serve_MID_PWM;
    motor[3] = Serve_MID_PWM;
	
  	if(ARMED) 
		Moto_PwmRflash(motor[0],motor[1],motor[2],motor[3]);   
	else                  
		Moto_PwmRflash(Serve_MID_PWM,Level_MID_PWM,Serve_MID_PWM,Serve_MID_PWM);
}


/*********************************************************************
* �� �� ����Moto_PwmRflash
* �������ܣ�����ռ�ձ�
* ��ڲ�������·���PWM
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Moto_PwmRflash(uint16_t motor1,uint16_t motor2,uint16_t motor3,uint16_t motor4)
{
	/*-- 7.2������ --*/
    if(motor1 < Serve_MIN_PWM) motor1 = Serve_MIN_PWM;
    else if(motor1 > Serve_MAX_PWM) motor1 = Serve_MAX_PWM;
    if(motor2 < Level_MIN_PWM) motor2 = Level_MIN_PWM;
    else if(motor2 > Level_MAX_PWM) motor2 = Level_MAX_PWM;
	
	PWM1 = motor1;
	PWM2 = motor2;
	PWM3 = motor3;
	PWM4 = motor4;
}


/*********************************************************************
* �� �� ����Key_control(void)
* �������ܣ���������
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void Key_control(void)
{
	u16 i=0;
	u8 key;
	static u8 table=0;  // ���ڽ����л�
	
	key = KEY_Scan(0);
	
	// ������ɨ��
	Touch_Key[3].press = Scan_Touch_Key(Touch_Key[3], 3);  // �л�ҳ��
	Touch_Key[6].press = Scan_Touch_Key(Touch_Key[6], 6);  // ���ð���
	
	// �����л�
	if(key == KEY1_PRES) {
		
		table = !table;
	}
	
	// �����л�
	if(Touch_Key[3].press || table) Page = 1;
	else if(Touch_Key[6].press) Page = 2;
	else Page = 0;
	
	// ��ͬ���水������
	if(Page == 1) {
		
//		ARMED = 0;  // ����ȷ����ȫ
		Setflag = 0;  // ������ñ�־
		
		Touch_Key[4].press = Scan_Touch_Key(Touch_Key[4], 4);  // pitch������ʾ����
		Touch_Key[5].press = Scan_Touch_Key(Touch_Key[5], 5);  // roll������ʾ����
	}
	else if(Page == 0) {
		
		Setflag = 0;  // ������ñ�־
		
		// ������ɨ��
		Touch_Key[0].press = Scan_Touch_Key(Touch_Key[0], 0);  // ������������
		Touch_Key[1].press = Scan_Touch_Key(Touch_Key[1], 1);  // <<
		Touch_Key[2].press = Scan_Touch_Key(Touch_Key[2], 2);  // >>
		
		// ������������
		if(Touch_Key[0].press) ARMED = 1;
		else ARMED = 0;
		
		// ����ѡ�񰴼� << K2
		if(Touch_Key[1].press || key == KEY2_PRES)
		{
			if(Task_sign <= 0)Task_sign = 9;
			else Task_sign --;
			
			// ���������ּ��
			i = 0;
			while(Touch_Key[1].press)
			{
				i++;
				if(i>200)break;
				Touch_Key[1].press = Scan_Touch_Key(Touch_Key[1], 1);
				delay_ms(1);
			}
		}
		// ����ѡ�񰴼� >> K0
		else if(Touch_Key[2].press || key == KEY0_PRES)
		{
			if(Task_sign >= 9)Task_sign = 0;
			else Task_sign ++;
			
			// ���������ּ��
			i = 0;
			while(Touch_Key[2].press)
			{
				i++;
				if(i>200)break;
				Touch_Key[2].press = Scan_Touch_Key(Touch_Key[2], 2);
				delay_ms(2);
			}
		}
		
		// ��Ӷ�����������
		if(Task_sign == 8)
		{
			static u8 tp_cnt=0;
			
			if((tp_dev.sta&0x80)==0x80)		// �д���
			{
				if(((tp_dev.x[0]>0)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>140)&&(tp_dev.y[0]<620)))	//��������
				{
					// Ϊ�˷��㣬ֱ�Ӱ���ʾ��������
					static u16 tp_x[6],tp_y[6];
					
					//ȷ�� �����������һ����
					if(((tp_dev.x[0]>440)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>580)&&(tp_dev.y[0]<620)))	
					{
						POINT_COLOR=RED;
						LCD_DrawLine(tp_x[0],tp_y[0],tp_x[(u8)(Polygon[6][0])],tp_y[(u8)(Polygon[6][0])]);
					}
					else
					{
						gui_fill_circle(tp_dev.x[0],tp_dev.y[0],6,RED);
						POINT_COLOR=RED;
						if(tp_cnt>0)LCD_DrawLine(tp_x[tp_cnt-1],tp_y[tp_cnt-1],tp_dev.x[0],tp_dev.y[0]);
						tp_x[tp_cnt] = tp_dev.x[0];
						tp_y[tp_cnt] = tp_dev.y[0];
						
						// ������ת���������Ƕ�
						Polygon[tp_cnt][0] = (tp_dev.x[0] - 240) * 50 / 240;
						Polygon[tp_cnt][1] = (tp_dev.y[0] - 240 - 140) * 50 / 240;
						
						tp_cnt ++;
						if(tp_cnt>6)
						{
							u8 i,j;
							
							tp_cnt=0;
							
							for(i=0;i<7;i++)
								for(j=0;j<2;j++)
									Polygon[i][j]=0;
							
							Display_Init();
						}
						Polygon[6][0]=tp_cnt-1;
					}
					
					while((tp_dev.sta&0x80)==0x80)	//���ּ��
					{
						tp_dev.scan(0);
					}
				}
			}
		}
		else if(Task_sign == 9)
		{
			if(((tp_dev.x[0]>0)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>140)&&(tp_dev.y[0]<620)))	//��������
			{
				LCD_DrawPoint(tp_dev.x[0],tp_dev.y[0]);
				LCD_DrawPoint(tp_dev.x[0]+1,tp_dev.y[0]+1);
				LCD_DrawPoint(tp_dev.x[0]-1,tp_dev.y[0]-1);
				
				// ������ת���������Ƕ�
				Polygon[0][0] = (tp_dev.x[0] - 240) * 50 / 240;
				Polygon[0][1] = (tp_dev.y[0] - 240 - 140) * 50 / 240;
			}
			else
			{
				Polygon[0][0] = 0;
				Polygon[0][1] = 0;
			}
		}
		
	}
	else if(Page == 2) {
		
		s8 touch_key;
		
		ARMED = 0;  // ����ȷ����ȫ
		
//		tp_dev.scan(0);  // ��ʵ��������ɨ��
		
		touch_key = Touch_Keyboard(0,540,479,798);  // 4X4�������
		
		// ������� (��ʱδ��װ)
		if(touch_key != 66)
		{
			u8 i;
			static u8 digits=0;  // λ��
			static u8 num[3]={0};
			static u16 tab_value;  // ������ʾ�޸ĵ�ֵ
			
			if(touch_key == 15)  // ȷ������
			{
				// ��ֵ
				if(Setflag == 1)  // �޸İ뾶
				{
					Swing_radii = tab_value;
				}
				else if(Setflag == 2)  // �޸ĽǶ�
				{
					Swing_angle = tab_value;
				}
				else  // �޸ķ��ȱ���
				{
					Range_ratio = tab_value;
					if(Range_ratio>10)Range_ratio=10;
				}
				
				digits = 0;
				tab_value = 0;
				for(i=0;i<3;i++)num[i]=0;
			}
			else if(touch_key == 12)  // ���
			{
				digits = 0;
				tab_value = 0;
				for(i=0;i<3;i++)num[i]=0;
			}
			else
			{
				if((touch_key <= 9) && (touch_key >= 0))
				{
					num[digits] = touch_key;
					
					digits++;
				}
			}
			
			if(digits>3)
			{	
				digits=0;
				tab_value=0;
				for(i=0;i<3;i++)num[i]=0;
			}
			
//			for(i=0;i<digits;i++)
//			{
//				tab_value += num[i]*pow(10,digits-1-i);
//			}
			switch(digits)
			{
				case 1:tab_value=num[0];break;
				case 2:tab_value=num[0]*10+num[1];break;
				case 3:tab_value=num[0]*100+num[1]*10+num[2];break;
			}
			
			POINT_COLOR=YELLOW;
			LCD_ShowNum(222, 475, tab_value, 3, 24);
		}
		
		// ѡ���޸� (�뾶 �Ƕ�)
		if((tp_dev.sta&0x80)==0x80)		// �а�������
		{
			if(((tp_dev.x[0]>0)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>200)&&(tp_dev.y[0]<450)))	//��������
			{
				if((tp_dev.x[0]>0)&&(tp_dev.x[0]<240))	// �޸İ뾶
				{
					Setflag = 1;
				}
				else if((tp_dev.x[0]>=240)&&(tp_dev.x[0]<439))	// �޸ĽǶ� 
				{
					Setflag = 2;
				}
			}
			else if((tp_dev.x[0]>120)&&(tp_dev.x[0]<480)&&((tp_dev.y[0]>0)&&(tp_dev.y[0]<200)))
			{
				Setflag = 0;
			}
		}
	}
}



/*********************************************************************
* �� �� ����Get_AngleTarget
* �������ܣ�����(�ڶ��뾶,��λ,ʱ��)�ó���Ӧ�Ƕ�
* ��ڲ�����(�ڶ��뾶,��λ,ʱ��,��������)
* ��    �ߣ������Σ�epic)
**********************************************************************/
float Get_AngleTarget1(float swing_radii, float angle_phase, u16 time, u16 period)
{
	static float last_radii=666,last_phase=666;
	static float track_angle=0;  // ����ڶ����Ƕ�
	static float phase=0;  // ��λ (����)
	float angle;
	
	// �����ð뾶�б仯ʱ
	if(last_radii != swing_radii)
	{
		last_radii = swing_radii;

		// �ó��ڶ����Ƕ�
		track_angle = atan(swing_radii/DISTANCE_GROUND) * 180.0 / 3.14159;
	}
	// ��������λ�б仯ʱ
	if(last_phase != angle_phase)
	{
		last_phase = angle_phase;
		
		// �ó���λ (����)
		phase = angle_phase * 3.14159 / 180;
	}
	
/*------------------------------------
		y = A * sin(x + z)
		y - �ó��Ƕ�ֵ
		A - ����ڶ����Ƕ�  (track_angle)
		x - (time / period) * 2 * Pi  
		z - ��λ��  (angle_phase)
------------------------------------*/
	
	angle = track_angle * sin(time * 6.28318 / period + phase);
	
	return angle;
}
float Get_AngleTarget2(float swing_radii, float angle_phase, u16 time, u16 period)
{
	static float last_radii=666,last_phase=666;
	static float track_angle=0;  // ����ڶ����Ƕ�
	static float phase=0;  // ��λ (����)
	float angle;
	
	// �����ð뾶�б仯ʱ
	if(last_radii != swing_radii)
	{
		last_radii = swing_radii;

		// �ó��ڶ����Ƕ�
		track_angle = atan(swing_radii/DISTANCE_GROUND) * 180.0 / 3.14159;
	}
	// ��������λ�б仯ʱ
	if(last_phase != angle_phase)
	{
		last_phase = angle_phase;
		
		// �ó���λ (����)
		phase = angle_phase * 3.14159 / 180;
	}
	
/*------------------------------------
		y = A * sin(x + z)
		y - �ó��Ƕ�ֵ
		A - ����ڶ����Ƕ�  (track_angle)
		x - (time / period) * 2 * Pi  
		z - ��λ��  (angle_phase)
------------------------------------*/
	
	angle = track_angle * sin(time * 6.28318 / period + phase);
	
	return angle;
}


/*********************************************************************
* �� �� ����System_Optimize(void)
* �������ܣ�ϵͳ�Ż�
* ��ڲ�������
* ��    �ߣ������Σ�epic)
**********************************************************************/
void System_Optimize(void)
{
    if(Task_sign == 4)
    {
        /*------------------------------------------------------
		��ӦPID�������ã�������
		rate : 0.8 0.05 0.1
		���˸о����ֲ���Ч�����ԣ����ǲ��ʺ����ϵͳ
-------------------------------------------------------*/
		pitch_rate_PID.P = 0.8;
		pitch_rate_PID.I = 0.06;
		pitch_rate_PID.D = 0.8;
		roll_rate_PID.P = 0.8;
		roll_rate_PID.I = 0.06;
		roll_rate_PID.D = 0.8;
    }
	else if(Task_sign == 0)
	{
		static u8 time=0;
	
///*------------------------------------------------------
//		��ӦPID�������ã����У�
//		rate : 36 0 8
//		angle : 60
//-------------------------------------------------------*/
//		pitch_angle_PID.P  = 60;
//		pitch_angle_PID.I  = 0; 		
//		pitch_angle_PID.D  = 0;
//		roll_angle_PID.P  = 60;
//		roll_angle_PID.I  = 0;	
//		roll_angle_PID.D  = 0;
//		
//		pitch_rate_PID.P = 36;
//		pitch_rate_PID.I = 0;
//		pitch_rate_PID.D = 8;
//		roll_rate_PID.P = 36;
//		roll_rate_PID.I = 0;
//		roll_rate_PID.D = 8;
        
/*------------------------------------------------------
		��ӦPID�������ã�������
		rate : 0.8 0 0.1
-------------------------------------------------------*/
		pitch_rate_PID.P = 0.8;
		pitch_rate_PID.I = 0;
		pitch_rate_PID.D = 0.8;
		roll_rate_PID.P = 0.8;
		roll_rate_PID.I = 0;
		roll_rate_PID.D = 0.8;
		
		/*-- ʵ�ʽǶ�Ϊ0����һ��ʱʱ�䣬�����Ϊ0 --*/
		if(fabsf(Pitch.angle) < 2.0f && fabsf(Roll.angle) < 2.0f) {
			
			time ++;
			
			if(time > 40)  // 200ms
			{	
//				Pitch.angle = 0;
//				Roll.angle = 0;
//				Pitch.rate = 0;
//				Roll.rate = 0;
			}
		}
		else time = 0;
		
		// ����45��ʱ������
//		if(fabsf(Pitch.angle) > 45.0f || fabsf(Roll.angle) > 45.0f) {	
//		
//			Pitch.angle = 0;
//			Roll.angle = 0;
//			Pitch.rate = 0;
//			Roll.rate = 0;
//		}
	}
	else
	{
/*------------------------------------------------------
		��ӦPID�������ã�������
		rate : 0.8 0 0.1
		���˸о����ֲ���Ч�����ԣ����ǲ��ʺ����ϵͳ
-------------------------------------------------------*/
		pitch_rate_PID.P = 0.8;
		pitch_rate_PID.I = 0;
		pitch_rate_PID.D = 0.8;
		roll_rate_PID.P = 0.8;
		roll_rate_PID.I = 0;
		roll_rate_PID.D = 0.8;
	}
}



