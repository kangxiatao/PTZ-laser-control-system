#ifndef __INCLUDE_H
#define __INCLUDE_H

/*-- System --*/
#include "stm32f10x_it.h" 
#include "sys.h"
#include "clock.h"
#include "usart.h"
#include <math.h>

/*-- Hardware --*/
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ahrs.h"
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

/*-- User_Src --*/
#include "timer.h"
#include "pwm.h"  // motor 
#include "myusart.h"
#include "show.h"
#include "mygui.h"
#include "queue.h"

/*-- Control --*/
#include "control.h"
#include "filter.h"
#include "altitude.h"



#endif

