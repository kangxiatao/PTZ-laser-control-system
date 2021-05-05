#ifndef __AHRS_H
#define __AHRS_H
#include "sys.h"



#define CONSTANTS_ONE_G		9.80665f		/*--  m/s^2  --*/

#define SENSOR_MAX_G	2.0f		//constant g		
#define SENSOR_MAX_W	2000.0f		//deg/s

// 姿态结构体
typedef struct
{
	float yaw;  		//偏航角
	float pitch;		//俯仰
	float roll; 		//滚转
	float alt;  		//高度
	float tempr;		//温度
	float press;		//气压
	int16_t accel[3];	//加速度计ADC原始值
	int16_t gyro[3];	//陀螺仪ADC原始值
	int16_t magnet[3];	//磁力计ADC原始值
	float accRaw[3];	//加速度
	float gyroRaw[3];	//角速度
	float accb[3];		//滤波后加速度
	float accOffset[3]; //加速度偏移量
	float DCMgb[3][3];  //地理坐标
	
}AHRS_Typedef;


enum {ROLL,PITCH,YAW};


extern AHRS_Typedef AHRS;

extern unsigned char ahrsUpdated;

extern float Velocity[3];


void Get_AHRS_Data(float *pitch, float *roll, short *gyrox, short *gyroy);
void AHRS_Data_Init(void);  // 放在大while前用于校准
void USART_CommandRoute(void);  // 读取AHRS所有数据

void Get_accOffset(void);
uint8_t IMU_Calibrate(void);

#endif

