#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	


void LED_Init(void);//��ʼ��
void LED0_Flash(u16 time);//��˸����
void LED1_Flash(u16 time);//��˸����
		 				    
#endif
