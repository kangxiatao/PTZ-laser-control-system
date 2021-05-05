#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"


// PID�ṹ��
typedef struct
{
	float P;
	float I;
	float D;
	float Error;		//�������
	float PreError;		//�ϴ����
	float Integ;		//����
	float iLimit;		//��������
	float Deriv;		//΢��
	float Output;		//���
 
}PID_Typedef;

// ��̬
typedef struct
{
	float angle;
	short rate;
}MPU;

// 
//enum {ROLL,PITCH,YAW};



void EPIC_Init(void);  //  һϵ�г�ʼ��

void Moto_PwmRflash(uint16_t motor1,uint16_t motor2,uint16_t motor3,uint16_t motor4);  //  ����ռ�ձ�
void Motor_Control(float pose[]);  //  ���PWM��ֵ
void Target_Control(float *target);  //  �����޸�

void Key_control(void);  //  ��������

float Get_AngleTarget1(float swing_radii,float angle_phase,u16 time,u16 period);  // ����(�ڶ��뾶,��λ,ʱ��)�ó���Ӧ�Ƕ�
float Get_AngleTarget2(float swing_radii,float angle_phase,u16 time,u16 period);  // ����(�ڶ��뾶,��λ,ʱ��)�ó���Ӧ�Ƕ�

void System_Optimize(void);  // ϵͳ�Ż�

/*-- �õ�xy��ĽǶȺͽ��ٶ� --*/
u8 Get_dmp_data(float *pitch, float *roll, short *gyrox, short *gyroy);


/*
  ȫ�ֱ���
------------------------------------*/

/*-- ������־λ  0 - ����  1 - ���� --*/
extern uint8_t ARMED;
/*-- ���������ñ�־λ  
 0 - ������
 1 - ���ð뾶
 2 - ���ýǶ� --*/
extern uint8_t Setflag;
/*-- �����������־λ  
 ��ֵ��Ӧ���� --*/
extern uint8_t Task_sign;

/*-- ��ڹ켣�ٶȣ��������� ms�� --*/
extern uint16_t Period;

/*-- ��������PID��� --*/
extern float Pose[2];

/*-- ʵ������ --*/
extern MPU Pitch,Roll;
/*-- �����Ƕ� --*/
extern float angleTarget[2];

/*-- �ڶ���������н� --*/
extern float Track_angle;
/*-- �ڶ��뾶 --*/
extern float Swing_radii;
/*-- �ڶ����� --*/
extern float Swing_angle;
/*-- �Ƕ���λ --*/
extern float Angle_phase; 
/*-- ���ȱ������� --*/
extern float Range_ratio;

/*-- ����PID �ǶȻ��ͽ��ٶȻ� --*/
extern PID_Typedef pitch_angle_PID;		//pitch�ǶȻ�PID
extern PID_Typedef roll_angle_PID;   	//roll�ǶȻ�PID

extern PID_Typedef pitch_rate_PID;		//pitch���ٶȻ�PID
extern PID_Typedef roll_rate_PID;    	//roll���ٶȻ�PID

/*
------------------------------------*/

#endif

