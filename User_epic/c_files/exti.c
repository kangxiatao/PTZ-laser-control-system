#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


 
 
//外部中断初始化函数
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO

	KEY_Init();//初始化按键

    //GPIOC.5 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	

    //GPIOA.15	  
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	

    //GPIOA.0	  
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		


 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);  	 
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
  	NVIC_Init(&NVIC_InitStructure); 
 
 
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure); 
 
}

 
void EXTI0_IRQHandler(void)
{
	delay_ms(50);    //消抖
	if(WK_UP==1)
	{	  
		
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(50);   //消抖			 
	if(KEY0==0)	
	{
		
	}
 	EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}


void EXTI15_10_IRQHandler(void)
{
	delay_ms(50);    //消抖			 
	if(KEY1==0)	
	{
		
	}
	EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}
