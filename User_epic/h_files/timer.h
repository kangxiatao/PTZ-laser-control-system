#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM1_Init(u16 arr,u16 psc);

extern volatile uint16_t time_ms,loop100HzCnt;
extern uint8_t  loop50HzFlag,loop10HzFlag;
 
#endif
