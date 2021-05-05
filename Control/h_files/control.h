#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"


// PID结构体
typedef struct
{
	float P;
	float I;
	float D;
	float Error;		//本次误差
	float PreError;		//上次误差
	float Integ;		//积分
	float iLimit;		//积分上限
	float Deriv;		//微分
	float Output;		//输出
 
}PID_Typedef;

// 姿态
typedef struct
{
	float angle;
	short rate;
}MPU;

// 
//enum {ROLL,PITCH,YAW};



void EPIC_Init(void);  //  一系列初始化

void Moto_PwmRflash(uint16_t motor1,uint16_t motor2,uint16_t motor3,uint16_t motor4);  //  更新占空比
void Motor_Control(float pose[]);  //  电机PWM赋值
void Target_Control(float *target);  //  期望修改

void Key_control(void);  //  按键控制

float Get_AngleTarget1(float swing_radii,float angle_phase,u16 time,u16 period);  // 根据(摆动半径,相位,时间)得出对应角度
float Get_AngleTarget2(float swing_radii,float angle_phase,u16 time,u16 period);  // 根据(摆动半径,相位,时间)得出对应角度

void System_Optimize(void);  // 系统优化

/*-- 得到xy轴的角度和角速度 --*/
u8 Get_dmp_data(float *pitch, float *roll, short *gyrox, short *gyroy);


/*
  全局变量
------------------------------------*/

/*-- 锁定标志位  0 - 锁定  1 - 解锁 --*/
extern uint8_t ARMED;
/*-- 风力摆设置标志位  
 0 - 无设置
 1 - 设置半径
 2 - 设置角度 --*/
extern uint8_t Setflag;
/*-- 风力摆任务标志位  
 数值对应任务 --*/
extern uint8_t Task_sign;

/*-- 风摆轨迹速度（控制周期 ms） --*/
extern uint16_t Period;

/*-- 两个方向PID输出 --*/
extern float Pose[2];

/*-- 实际数据 --*/
extern MPU Pitch,Roll;
/*-- 期望角度 --*/
extern float angleTarget[2];

/*-- 摆动与地面最大夹角 --*/
extern float Track_angle;
/*-- 摆动半径 --*/
extern float Swing_radii;
/*-- 摆动方向 --*/
extern float Swing_angle;
/*-- 角度相位 --*/
extern float Angle_phase; 
/*-- 幅度比例设置 --*/
extern float Range_ratio;

/*-- 串行PID 角度环和角速度环 --*/
extern PID_Typedef pitch_angle_PID;		//pitch角度环PID
extern PID_Typedef roll_angle_PID;   	//roll角度环PID

extern PID_Typedef pitch_rate_PID;		//pitch角速度环PID
extern PID_Typedef roll_rate_PID;    	//roll角速度环PID

/*
------------------------------------*/

#endif

