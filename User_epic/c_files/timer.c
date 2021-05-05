/*��

 * @file       timer.c
 * @brief      PID�������ڶ�ʱ��
 * @author     �����Σ�epic)
 * @version    v3.0
 * @date       2017-06-21
 
------------------------------------
*/

#include "timer.h"



void TIM1_Init(u16 arr,u16 psc)  
{  
	NVIC_InitTypeDef NVIC_InitStructure;  

	RCC->APB2ENR|=1<<11;//TIM1ʱ��ʹ��    
 	TIM1->ARR=arr;      //�趨�������Զ���װֵ   
	TIM1->PSC=psc;      //Ԥ��Ƶ��
	TIM1->DIER|=1<<0;   //��������ж�				
	TIM1->DIER|=1<<6;   //�������ж�	   
	TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��
	  
	//�������ȼ�  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);   
}





