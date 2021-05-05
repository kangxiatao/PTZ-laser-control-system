#ifndef __AHRS_H
#define __AHRS_H
#include "sys.h"



#define CONSTANTS_ONE_G		9.80665f		/*--  m/s^2  --*/

#define SENSOR_MAX_G	2.0f		//constant g		
#define SENSOR_MAX_W	2000.0f		//deg/s

// ��̬�ṹ��
typedef struct
{
	float yaw;  		//ƫ����
	float pitch;		//����
	float roll; 		//��ת
	float alt;  		//�߶�
	float tempr;		//�¶�
	float press;		//��ѹ
	int16_t accel[3];	//���ٶȼ�ADCԭʼֵ
	int16_t gyro[3];	//������ADCԭʼֵ
	int16_t magnet[3];	//������ADCԭʼֵ
	float accRaw[3];	//���ٶ�
	float gyroRaw[3];	//���ٶ�
	float accb[3];		//�˲�����ٶ�
	float accOffset[3]; //���ٶ�ƫ����
	float DCMgb[3][3];  //��������
	
}AHRS_Typedef;


enum {ROLL,PITCH,YAW};


extern AHRS_Typedef AHRS;

extern unsigned char ahrsUpdated;

extern float Velocity[3];


void Get_AHRS_Data(float *pitch, float *roll, short *gyrox, short *gyroy);
void AHRS_Data_Init(void);  // ���ڴ�whileǰ����У׼
void USART_CommandRoute(void);  // ��ȡAHRS��������

void Get_accOffset(void);
uint8_t IMU_Calibrate(void);

#endif

