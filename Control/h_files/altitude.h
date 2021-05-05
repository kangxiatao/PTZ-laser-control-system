#ifndef ALTITUDE_H_
#define ALTITUDE_H_

#include "sys.h"



extern float nav_z,nav_vz,nav_az;
extern float z_est[3];	// estimate z Vz  Az
extern uint8_t landed;


static void inertial_filter_predict(float dt, float x[3]);
static void inertial_filter_correct(float e, float dt, float x[3], int i, float w);

void AltitudeCombine(void);

#endif
