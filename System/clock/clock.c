/*！

 * @file       clock.c
 * @brief      系统时钟
 * @author     康夏涛（epic)
 * @version    v2.0
 * @date       2017-05-16
 
------------------------------------
*/
#include "clock.h"
#include "stm32f10x_it.h" 


char SysClock;       //申请存储系统时钟变量，单位MHz


//不能在这里执行所有外设复位!否则至少引起串口不工作.
//把所有时钟寄存器复位
void MYRCC_DeInit(void)
{
    RCC->APB1RSTR = 0x00000000;//复位结束
    RCC->APB2RSTR = 0x00000000;

    RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.
    RCC->APB2ENR = 0x00000000; //外设时钟关闭.
    RCC->APB1ENR = 0x00000000;
    RCC->CR |= 0x00000001;     //使能内部高速时钟HSION
    RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]
    RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
    RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP
    RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
    RCC->CIR = 0x00000000;     //关闭所有中断
}

/********************************************
           使用外部晶体作为系统时钟源
功能：
1.使用外部HSE时钟8M作为PLL输入
2.PLL倍频系数PLLMUL<=9(实际到达16时，还能正常倍频外部时钟)
3.输入参数：PLLMUL，PLL倍频系数
4.备注：官方手册上说，使用HSE作为系统时钟源时，最高可倍频到72MHz，但是实际可以倍频到128M系统还算稳定
********************************************/
//系统时钟初始化函数
//pll:选择的倍频数，从2开始，最大值为16
//时钟源为外部晶振
//备注：当机身焊接了8M晶振时，就只能使用外部8M晶振作为时钟源，
//      用内部的HSI不好使，我反正没调出来，看各位有啥办法没
char SystemClock_HSE(u8 PLL)
{
    unsigned char temp=0;
    MYRCC_DeInit();			//复位并配置向量表
    RCC->CR|=1<<16;       	//外部高速时钟使能HSEON
    while(!(RCC->CR>>17));	//等待外部时钟就绪
    RCC->CFGR=0X00000400; 	//APB1=DIV2;APB2=DIV1;AHB=DIV1;
    PLL-=2;					//抵消2个单位
    RCC->CFGR|=PLL<<18;   	//设置PLL值 2~16
    RCC->CFGR|=1<<16;	    //PLLSRC ON
    FLASH->ACR|=0x32;	    //FLASH 2个延时周期
    RCC->CR|=0x01000000;  	//PLLON
    while(!(RCC->CR>>25));	//等待PLL锁定
    RCC->CFGR|=0x00000002;	//PLL作为系统时钟
    while(temp!=0x02)     	//等待PLL作为系统时钟设置成功
    {
        temp=RCC->CFGR>>2;
        temp&=0x03;
    }

    SysClock=(PLL+2)*8;
    return SysClock;
}

//系统时钟初始化
void SystemClock_Init(void)
{
	SystemClock_HSE(9);  // 系统时钟初始化，时钟源外部晶振HSEs  8*9=72MHz;
    SysTick_Config(SystemCoreClock/1000);  //SysTick开启系统tick定时器并初始化其中断，1ms
}


void delay_ms(uint16_t nms)
{
    uint32_t t0 = micros();
    while(micros() - t0 < nms * 1000);
}

void delay_us(uint16_t nus)
{
	uint32_t t0 = micros();
    while(micros() - t0 < nus);
}




