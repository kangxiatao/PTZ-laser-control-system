/*！

 * @file       control.c
 * @brief      控制算法
 * @author     康夏涛（epic)
 * @version    v1.0
 * @date       2019-08-04
 
------------------------------------
*/

#include "control.h"
#include "include.h"



// 定义万向轴转动点到地面的距离  单位厘米(CM)
#define	DISTANCE_GROUND		86.6
// 单摆周期 - (time/PERIOD)*2*pi
#define	PERIOD		320  // 1600ms 1600/5



/*
  全局变量
------------------------------------*/

/*-- 锁定标志位  
 0 - 锁定  
 1 - 解锁 --*/
uint8_t ARMED=0;

/*-- 风力摆设置标志位  
 0 - 无设置
 1 - 设置半径
 2 - 设置角度 --*/
uint8_t Setflag=0;

/*-- 风力摆任务标志位  
 数值对应任务 --*/
uint8_t Task_sign=0;

/*-- 风摆轨迹速度
 （控制周期 ms*5）单摆周期 - (time/Period)*2*pi --*/
uint16_t Period=320;  // 1600ms 1600/5


/*-- 两个方向PID输出 --*/
float Pose[2]={0};

/*-- 实际数据 --*/
MPU Pitch,Roll;
/*-- 期望角度 --*/
float angleTarget[2]={0};

/*-- 摆动与地面最大夹角 --*/
float Track_angle=0;
/*-- 摆动半径 --*/
float Swing_radii=30;  // 单位 CM
/*-- 摆动方向 --*/
float Swing_angle=0;  // 单位 度
/*-- 角度相位 --*/
float Angle_phase=0;  // 相位 度
/*-- 幅度比例设置 --*/
float Range_ratio=0;  // 幅度

/*-- 串行PID 角度环和角速度环 --*/
PID_Typedef pitch_angle_PID;		// pitch角度环PID
PID_Typedef roll_angle_PID;   		// roll角度环PID

PID_Typedef pitch_rate_PID;			//pitch角速度环PID
PID_Typedef roll_rate_PID;    		//roll角速度环PID

/*
------------------------------------*/





/*********************************************************************
* 函 数 名：PID_Postion_Cal()
* 函数功能：位置式PID
* 入口参数：PID结构体 , 期望值 , 测量值
* 作    者：康夏涛（epic)
**********************************************************************/
static void PID_Postion_Cal(PID_Typedef * PID, float target, float measure)
{
	float termI;
	
	/*-- 误差 = 期望值 - 测量值 --*/
	PID->Error = target - measure;

//	/*-- 当前误差微分 --*/
//	PID->Deriv = (PID->Error - PID->PreError);
//	
//	/*-- 储存上次误差 --*/
//	PID->PreError = PID->Error;
	
	/*-- 这里直接对测量值进行微分 (微分先行) --*/
	PID->Deriv = (PID->PreError - measure);
	
	/*-- 这里用来储存上次测量值 (微分先行) --*/
	PID->PreError = measure;
	
	/*-- 解锁才积分 锁定清除积分 --*/
	if(ARMED){
		/*-- 积分环节 --*/
		termI = (PID->Integ) + (PID->Error);   
		/*-- 积分条件 --*/
		if(termI > -PID->iLimit && termI < PID->iLimit && fabs(PID->Error) < 10)    
			PID->Integ=termI;
	}else{
		PID->Integ = 0;
	}
	
	/*-- PID输出 = 比例环节 + 积分环节 + 微分环节 --*/
	PID->Output = (PID->P * PID->Error) + (PID->I * PID->Integ) + (PID->D * PID->Deriv);
}


/*********************************************************************
* 函 数 名：Balance_Control()
* 函数功能：位置式平衡控制
* 入口参数：PID结构体 , 期望角度 , 角度 , 角速度
* 作    者：康夏涛（epic)
**********************************************************************/
static void Balance_Control(PID_Typedef * PID, float target, float angle, float rate)
{
	float termI;
    
	/*-- 误差 = 期望值 - 测量值 --*/
	PID->Error = target - angle;
	
	/*-- 这里直接用角速度 --*/
	PID->Deriv = rate;
    
    /*-- 解锁才积分 锁定清除积分 --*/
	if(ARMED){
		/*-- 积分环节 --*/
		termI = (PID->Integ) + (PID->Error);   
		/*-- 积分条件 --*/
		if(termI > -PID->iLimit && termI < PID->iLimit)    
			PID->Integ=termI;
	}else{
		PID->Integ = 0;
	}
	
	/*-- PID输出 = 比例环节 + 积分环节 + 微分环节 --*/
	PID->Output = (PID->P * PID->Error) + (PID->I * PID->Integ) + (PID->D * PID->Deriv);
}



/*********************************************************************
* 函 数 名：TIM1_UP_IRQHandler(void)
* 函数功能：控制系统调节中断
* 入口参数：无
* 作    者：康夏涛（epic)
**********************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)  // 5ms定时中断
	{   
		TIM1->SR&=~(1<<0);  // 清除中断标志位
		
	/*-- System running LED --*/
		if(ARMED)LED0_Flash(100);
		else LED0=0;
		
	/*-- refresh data --*/  
//		Get_dmp_data(&Pitch.angle, &Roll.angle, &Pitch.rate, &Roll.rate);  // MPU6050
//		Get_AHRS_Data(&Pitch.angle, &Roll.angle, &Pitch.rate, &Roll.rate);  // 放在uart3中断服务函数里
	 	
	/*-- 系统优化 --*/
		System_Optimize();
		
	/*-- 期望修改 （所有功能实现全靠改变期望） --*/
		Target_Control(angleTarget);  
		
//	/*-- PID外环控制(角度控制) --*/
//		PID_Postion_Cal(&pitch_angle_PID, angleTarget[PITCH], Pitch.angle);  
//		PID_Postion_Cal(&roll_angle_PID, angleTarget[ROLL], Roll.angle);	
//		
//	/*-- PID内环控制(角速度控制) --*/
//		PID_Postion_Cal(&pitch_rate_PID, -pitch_angle_PID.Output, (float)(Pitch.rate)/10.0f);	
//		PID_Postion_Cal(&roll_rate_PID, -roll_angle_PID.Output, (float)(Roll.rate)/10.0f);

//	/*-- 内环控制输出 --*/
//		Pose[PITCH] = pitch_rate_PID.Output;
//		Pose[ROLL] = roll_rate_PID.Output;

    /*-- 平衡控制 --*/
        Balance_Control(&pitch_rate_PID, angleTarget[PITCH], Pitch.angle, (float)(Pitch.rate)/1000.0f);
        Balance_Control(&roll_rate_PID, angleTarget[ROLL], Roll.angle, (float)(Roll.rate)/1000.0f);
		
	/*-- 舵机控制输出 --*/
		Pose[PITCH] = -pitch_rate_PID.Output;
		Pose[ROLL] = -roll_rate_PID.Output;
		
	/*-- PWM 输出 --*/
		Motor_Control(Pose);
	}
}


/*********************************************************************
* 函 数 名：EPIC_Init()
* 函数功能：史诗级初始化
* 入口参数：无
* 作    者：康夏涛（epic)
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
    
	AHRS_Data_Init();  // 校准数据
}	



/*********************************************************************
* 函 数 名：Target_Control 
* 函数功能：修改 Pitch,Roll 方向期望
* 入口参数：Pitch,Roll 期望值地址
* 作    者：康夏涛（epic)
**********************************************************************/
void Target_Control(float *target)
{
	static u16 time = 0;
	static u16 time_1 = 0;
	static u16 time_2 = 0;
	static float last_angle = 666;  // 上次设定摆角
	static float last_radii = 666;  // 上次设定半径
	static float real_radii = 0;  // 真实半径
	static float angle_x = 0.0; // 摆动角度x分量
	static float angle_y = 0.0; // 摆动角度y分量
	
	// 提高程序效率
	if(last_radii != Swing_radii)
	{
		last_radii = Swing_radii;
		
		// 半径差距补偿
		real_radii = (Swing_radii + 3) * 2;
	}
	if(last_angle != Swing_angle)
	{
		last_angle = Swing_angle;
		
		angle_x = cos(Swing_angle * 180.0 / 3.14159);	 //X方向分量
		angle_y = sin(Swing_angle * 180.0 / 3.14159);	 //X方向分量
	}
	
	switch(Task_sign)
	{
		case 0:
			// 第零问: 0110 - 666 - 0x6666
			target[PITCH] = 0;
			target[ROLL] = 0;
			break;
		case 1:
			// 第一问：画出一条大与50cm的直线（输入半径为补偿半径）
			target[PITCH] = Get_AngleTarget1(80, 0, time, Period);
			target[ROLL] = 0;
			break;
		case 2:
			// 第二问：画出长度在30~60cm间可设置直线设定摆动方向
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = 0;
			break;
		case 3:
			// 第三问：设定摆动方向（调节幅度，对应分量给x,y）
			target[PITCH] = angle_x * Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = angle_y * Get_AngleTarget2(real_radii, 0, time, Period);
			break;
		case 4:
			// 第四问：老哥稳
			target[PITCH] = 0;
			target[ROLL] = 0;
			break;
		case 5:
			// 第五问：画圆
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = Get_AngleTarget2(real_radii, 90, time, Period);
			break;
		case 6:
			// 第六问：换个方向画圆
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period);
			target[ROLL] = Get_AngleTarget2(real_radii, 270, time, Period);
			break;
		case 7:
			// 第七问：随便画
			if(Range_ratio<1)Range_ratio=1;
			target[PITCH] = Get_AngleTarget1(real_radii, 0, time, Period) / Range_ratio;
			target[ROLL] = Get_AngleTarget2(real_radii, Swing_angle, time_1, Period*Range_ratio);
		
			// 解决周期冲突
			time_1 ++;
			if(time_1>Period*Range_ratio)time_1=0;
			break;
		case 8:
			// 第八问：随便画多边形
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
			// 第九问：随手画
			target[PITCH] = Polygon[0][0];
			target[ROLL] = Polygon[0][1];
			break;
	}
	
	time++;
	if(time>Period)time=0;  // Period 为控制周期
}



/*********************************************************************
* 函 数 名：Motor_Control 
* 函数功能：四个电机的PWM输出
* 入口参数：PID两个方向输出
* 作    者：康夏涛（epic)
**********************************************************************/
void Motor_Control(float pose[])
{
	/*-- 电机PWM数组 --*/
	uint16_t motor[4]={0};
	
	/*    上右为正    */
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
* 函 数 名：Moto_PwmRflash
* 函数功能：更新占空比
* 入口参数：四路电机PWM
* 作    者：康夏涛（epic)
**********************************************************************/
void Moto_PwmRflash(uint16_t motor1,uint16_t motor2,uint16_t motor3,uint16_t motor4)
{
	/*-- 7.2伏供电 --*/
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
* 函 数 名：Key_control(void)
* 函数功能：按键控制
* 入口参数：无
* 作    者：康夏涛（epic)
**********************************************************************/
void Key_control(void)
{
	u16 i=0;
	u8 key;
	static u8 table=0;  // 用于界面切换
	
	key = KEY_Scan(0);
	
	// 触摸屏扫描
	Touch_Key[3].press = Scan_Touch_Key(Touch_Key[3], 3);  // 切换页面
	Touch_Key[6].press = Scan_Touch_Key(Touch_Key[6], 6);  // 设置按键
	
	// 界面切换
	if(key == KEY1_PRES) {
		
		table = !table;
	}
	
	// 界面切换
	if(Touch_Key[3].press || table) Page = 1;
	else if(Touch_Key[6].press) Page = 2;
	else Page = 0;
	
	// 不同界面按键处理
	if(Page == 1) {
		
//		ARMED = 0;  // 锁定确保安全
		Setflag = 0;  // 清除设置标志
		
		Touch_Key[4].press = Scan_Touch_Key(Touch_Key[4], 4);  // pitch数据显示开关
		Touch_Key[5].press = Scan_Touch_Key(Touch_Key[5], 5);  // roll数据显示开关
	}
	else if(Page == 0) {
		
		Setflag = 0;  // 清除设置标志
		
		// 触摸屏扫描
		Touch_Key[0].press = Scan_Touch_Key(Touch_Key[0], 0);  // 锁定解锁开关
		Touch_Key[1].press = Scan_Touch_Key(Touch_Key[1], 1);  // <<
		Touch_Key[2].press = Scan_Touch_Key(Touch_Key[2], 2);  // >>
		
		// 锁定解锁开关
		if(Touch_Key[0].press) ARMED = 1;
		else ARMED = 0;
		
		// 任务选择按键 << K2
		if(Touch_Key[1].press || key == KEY2_PRES)
		{
			if(Task_sign <= 0)Task_sign = 9;
			else Task_sign --;
			
			// 触摸屏松手检测
			i = 0;
			while(Touch_Key[1].press)
			{
				i++;
				if(i>200)break;
				Touch_Key[1].press = Scan_Touch_Key(Touch_Key[1], 1);
				delay_ms(1);
			}
		}
		// 任务选择按键 >> K0
		else if(Touch_Key[2].press || key == KEY0_PRES)
		{
			if(Task_sign >= 9)Task_sign = 0;
			else Task_sign ++;
			
			// 触摸屏松手检测
			i = 0;
			while(Touch_Key[2].press)
			{
				i++;
				if(i>200)break;
				Touch_Key[2].press = Scan_Touch_Key(Touch_Key[2], 2);
				delay_ms(2);
			}
		}
		
		// 添加多边形任务控制
		if(Task_sign == 8)
		{
			static u8 tp_cnt=0;
			
			if((tp_dev.sta&0x80)==0x80)		// 有触摸
			{
				if(((tp_dev.x[0]>0)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>140)&&(tp_dev.y[0]<620)))	//在区域内
				{
					// 为了方便，直接把显示加在这里
					static u16 tp_x[6],tp_y[6];
					
					//确定 用于连上最后一根线
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
						
						// 把输入转换成期望角度
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
					
					while((tp_dev.sta&0x80)==0x80)	//松手检测
					{
						tp_dev.scan(0);
					}
				}
			}
		}
		else if(Task_sign == 9)
		{
			if(((tp_dev.x[0]>0)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>140)&&(tp_dev.y[0]<620)))	//在区域内
			{
				LCD_DrawPoint(tp_dev.x[0],tp_dev.y[0]);
				LCD_DrawPoint(tp_dev.x[0]+1,tp_dev.y[0]+1);
				LCD_DrawPoint(tp_dev.x[0]-1,tp_dev.y[0]-1);
				
				// 把输入转换成期望角度
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
		
		ARMED = 0;  // 锁定确保安全
		
//		tp_dev.scan(0);  // 其实到处都有扫描
		
		touch_key = Touch_Keyboard(0,540,479,798);  // 4X4矩阵键盘
		
		// 矩阵键盘 (暂时未封装)
		if(touch_key != 66)
		{
			u8 i;
			static u8 digits=0;  // 位数
			static u8 num[3]={0};
			static u16 tab_value;  // 用来显示修改的值
			
			if(touch_key == 15)  // 确定按键
			{
				// 赋值
				if(Setflag == 1)  // 修改半径
				{
					Swing_radii = tab_value;
				}
				else if(Setflag == 2)  // 修改角度
				{
					Swing_angle = tab_value;
				}
				else  // 修改幅度比例
				{
					Range_ratio = tab_value;
					if(Range_ratio>10)Range_ratio=10;
				}
				
				digits = 0;
				tab_value = 0;
				for(i=0;i<3;i++)num[i]=0;
			}
			else if(touch_key == 12)  // 清除
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
		
		// 选择修改 (半径 角度)
		if((tp_dev.sta&0x80)==0x80)		// 有按键按下
		{
			if(((tp_dev.x[0]>0)&&(tp_dev.x[0]<480))&&((tp_dev.y[0]>200)&&(tp_dev.y[0]<450)))	//在区域内
			{
				if((tp_dev.x[0]>0)&&(tp_dev.x[0]<240))	// 修改半径
				{
					Setflag = 1;
				}
				else if((tp_dev.x[0]>=240)&&(tp_dev.x[0]<439))	// 修改角度 
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
* 函 数 名：Get_AngleTarget
* 函数功能：根据(摆动半径,相位,时间)得出对应角度
* 入口参数：(摆动半径,相位,时间,控制周期)
* 作    者：康夏涛（epic)
**********************************************************************/
float Get_AngleTarget1(float swing_radii, float angle_phase, u16 time, u16 period)
{
	static float last_radii=666,last_phase=666;
	static float track_angle=0;  // 需求摆动最大角度
	static float phase=0;  // 相位 (弧度)
	float angle;
	
	// 当设置半径有变化时
	if(last_radii != swing_radii)
	{
		last_radii = swing_radii;

		// 得出摆动最大角度
		track_angle = atan(swing_radii/DISTANCE_GROUND) * 180.0 / 3.14159;
	}
	// 当设置相位有变化时
	if(last_phase != angle_phase)
	{
		last_phase = angle_phase;
		
		// 得出相位 (弧度)
		phase = angle_phase * 3.14159 / 180;
	}
	
/*------------------------------------
		y = A * sin(x + z)
		y - 得出角度值
		A - 需求摆动最大角度  (track_angle)
		x - (time / period) * 2 * Pi  
		z - 相位差  (angle_phase)
------------------------------------*/
	
	angle = track_angle * sin(time * 6.28318 / period + phase);
	
	return angle;
}
float Get_AngleTarget2(float swing_radii, float angle_phase, u16 time, u16 period)
{
	static float last_radii=666,last_phase=666;
	static float track_angle=0;  // 需求摆动最大角度
	static float phase=0;  // 相位 (弧度)
	float angle;
	
	// 当设置半径有变化时
	if(last_radii != swing_radii)
	{
		last_radii = swing_radii;

		// 得出摆动最大角度
		track_angle = atan(swing_radii/DISTANCE_GROUND) * 180.0 / 3.14159;
	}
	// 当设置相位有变化时
	if(last_phase != angle_phase)
	{
		last_phase = angle_phase;
		
		// 得出相位 (弧度)
		phase = angle_phase * 3.14159 / 180;
	}
	
/*------------------------------------
		y = A * sin(x + z)
		y - 得出角度值
		A - 需求摆动最大角度  (track_angle)
		x - (time / period) * 2 * Pi  
		z - 相位差  (angle_phase)
------------------------------------*/
	
	angle = track_angle * sin(time * 6.28318 / period + phase);
	
	return angle;
}


/*********************************************************************
* 函 数 名：System_Optimize(void)
* 函数功能：系统优化
* 入口参数：无
* 作    者：康夏涛（epic)
**********************************************************************/
void System_Optimize(void)
{
    if(Task_sign == 4)
    {
        /*------------------------------------------------------
		对应PID参数设置（单环）
		rate : 0.8 0.05 0.1
		个人感觉积分补偿效果可以，但是不适合这个系统
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
//		对应PID参数设置（串行）
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
		对应PID参数设置（单环）
		rate : 0.8 0 0.1
-------------------------------------------------------*/
		pitch_rate_PID.P = 0.8;
		pitch_rate_PID.I = 0;
		pitch_rate_PID.D = 0.8;
		roll_rate_PID.P = 0.8;
		roll_rate_PID.I = 0;
		roll_rate_PID.D = 0.8;
		
		/*-- 实际角度为0超过一定时时间，让输出为0 --*/
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
		
		// 大于45度时不调节
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
		对应PID参数设置（单环）
		rate : 0.8 0 0.1
		个人感觉积分补偿效果可以，但是不适合这个系统
-------------------------------------------------------*/
		pitch_rate_PID.P = 0.8;
		pitch_rate_PID.I = 0;
		pitch_rate_PID.D = 0.8;
		roll_rate_PID.P = 0.8;
		roll_rate_PID.I = 0;
		roll_rate_PID.D = 0.8;
	}
}



