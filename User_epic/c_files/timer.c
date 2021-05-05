/*！

 * @file       timer.c
 * @brief      PID控制周期定时器
 * @author     康夏涛（epic)
 * @version    v3.0
 * @date       2017-06-21
 
------------------------------------
*/

#include "timer.h"



void TIM1_Init(u16 arr,u16 psc)  
{  
	NVIC_InitTypeDef NVIC_InitStructure;  

	RCC->APB2ENR|=1<<11;//TIM1时钟使能    
 	TIM1->ARR=arr;      //设定计数器自动重装值   
	TIM1->PSC=psc;      //预分频器
	TIM1->DIER|=1<<0;   //允许更新中断				
	TIM1->DIER|=1<<6;   //允许触发中断	   
	TIM1->CR1|=0x01;    //使能定时器
	  
	//设置优先级  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);   
}





