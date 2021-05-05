/*！

 * @file       altitude.c
 * @brief      高度融合算法
 * @author     康夏涛（epic)
 * @version    v2.0
 * @date       2017-05-19
 
------------------------------------
*/
#include "altitude.h"
#include "control.h"
#include "stm32f10x_it.h" 
#include "ahrs.h"
#include "usart.h"
#include "ultrasonic.h"



float nav_z,nav_vz,nav_az;		//NED frame in earth
float z_est[3];	// estimate z  Vz  Az
static float w_z_baro=0.5f;
static float w_z_acc=20.0f;
static float w_acc_bias=0.05f;

/* acceleration in NED frame */
float accel_NED[3] = { 0.0f, 0.0f, -CONSTANTS_ONE_G };
/* store error when sensor updates, but correct on each time step to avoid jumps in estimated value */
float corr_acc[] = { 0.0f, 0.0f, 0.0f };	// N E D ,  m/s2
float acc_bias[] = { 0.0f, 0.0f, 0.0f };	// body frame ,
float corr_baro = 0.0f;					//m
//float accb[3]={0,0,0};


//Combine Filter to correct err
static void inertial_filter_predict(float dt, float x[3])
{
    x[0] += x[1] * dt + x[2] * dt * dt / 2.0f;
    x[1] += x[2] * dt;
}

static void inertial_filter_correct(float e, float dt, float x[3], int i, float w)
{
    float ewdt = e * w * dt;
    x[i] += ewdt;

    if (i == 0) {
        x[1] += w * ewdt;
        x[2] += w * w * ewdt / 3.0;

    } else if (i == 1) {
        x[2] += w * ewdt;
    }
}

//timeStamp in us. Thread should be executed every 2~20ms
//MS5611_Altitude  , should be in m. (can be fixed to abs, not relative). positive above ground
//accFilted  ,should be filted .
void AltitudeCombine(void)
{
    static uint32_t tPre=0;
    uint32_t t;
    float dt;

    /* accelerometer bias correction */
    float accel_bias_corr[3] = { 0.0f, 0.0f, 0.0f };
    uint8_t i,j;

    t=micros();
    dt = (tPre>0)?((t-tPre)/1000000.0f):0;
    tPre=t;

	if(!Ult_Pre)AHRS.alt = ult_height;
	
	if(ahrsUpdated&0x02)
	{
		corr_baro = 0 - AHRS.alt - z_est[0];

		AHRS.accb[0] -= acc_bias[0];
		AHRS.accb[1] -= acc_bias[1];
		AHRS.accb[2] -= acc_bias[2];

		for(i=0; i<3; i++)
		{
			accel_NED[i]=0.0f;
			for(j=0; j<3; j++)
			{
				accel_NED[i] += AHRS.DCMgb[j][i] * AHRS.accb[j];
			}
		}

		accel_NED[2] = -accel_NED[2];
		corr_acc[2] = accel_NED[2] + CONSTANTS_ONE_G - z_est[2];
		ahrsUpdated &= 0xfd;
	}

    //correct accelerometer bias every time step
    accel_bias_corr[2] -= corr_baro * w_z_baro * w_z_baro;

    //transform error vector from NED frame to body frame
	for (i = 0; i < 3; i++)
    {
        float c = 0.0f;

        for (j = 0; j < 3; j++) {
            c += AHRS.DCMgb[i][j] * accel_bias_corr[j];
        }

        acc_bias[i] += c * w_acc_bias * dt;		//accumulate bias
    }

    acc_bias[2] = -acc_bias[2];


    /* inertial filter prediction for altitude */
    inertial_filter_predict(dt, z_est);
    /* inertial filter correction for altitude */
    inertial_filter_correct(corr_baro, dt, z_est, 0, w_z_baro);	//0.5f
    inertial_filter_correct(corr_acc[2], dt, z_est, 2, w_z_acc); //20.0f

    nav_z=z_est[0];
    nav_vz=z_est[1];
	nav_az=z_est[2];
	
//	printf("%f - %f - %f\r\n",nav_z,nav_vz,AHRS.alt);
//	printf("%f - %f - %f\r\n",AHRS.accb[1],AHRS.accb[0],AHRS.accb[2]);
}


