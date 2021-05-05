#ifndef __FILTER_H
#define __FILTER_H

#include "sys.h"
#include <math.h>

#define M_PI_F 3.1415926

#define IMU_SAMPLE_RATE			100.0f
#define IMU_FILTER_CUTOFF_FREQ	30.0f

void LPF2pSetCutoffFreq_1(float sample_freq, float cutoff_freq);
float LPF2pApply_1(float sample);
void LPF2pSetCutoffFreq_2(float sample_freq, float cutoff_freq);
float LPF2pApply_2(float sample);
void LPF2pSetCutoffFreq_3(float sample_freq, float cutoff_freq);
float LPF2pApply_3(float sample);

#endif

