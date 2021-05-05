#ifndef __PWM_H
#define __PWM_H
#include "sys.h"


#define Serve_MIN_PWM  50 // ºó
#define Serve_MID_PWM  135
#define Serve_MAX_PWM  220 // Ç°

#define Level_MIN_PWM  50 // ÓÒ
#define Level_MID_PWM  150
#define Level_MAX_PWM  250 // ×ó


#define PWM1   TIM2->CCR1
#define PWM2   TIM2->CCR2
#define PWM3   TIM2->CCR3
#define PWM4   TIM2->CCR4


void TIM2_Init(u16 arr,u16 psc);

#endif
