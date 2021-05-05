#include "pwm.h"


void TIM2_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); // 
	//使能GPIO外设时钟使能
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
    //输出TIM2 PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM_CH1-4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Period = arr;  //72000000/(arr+1)/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	TIM_ARRPreloadConfig(TIM2, ENABLE);  //使能TIMx在ARR上的预装载寄存器


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//    
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//   
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);//    
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);//     
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
	TIM_CtrlPWMOutputs(TIM2, ENABLE);  //MOE 主输出使能
}

	

