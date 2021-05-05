/*��

 * @file       clock.c
 * @brief      ϵͳʱ��
 * @author     �����Σ�epic)
 * @version    v2.0
 * @date       2017-05-16
 
------------------------------------
*/
#include "clock.h"
#include "stm32f10x_it.h" 


char SysClock;       //����洢ϵͳʱ�ӱ�������λMHz


//����������ִ���������踴λ!�����������𴮿ڲ�����.
//������ʱ�ӼĴ�����λ
void MYRCC_DeInit(void)
{
    RCC->APB1RSTR = 0x00000000;//��λ����
    RCC->APB2RSTR = 0x00000000;

    RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.
    RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.
    RCC->APB1ENR = 0x00000000;
    RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION
    RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]
    RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
    RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP
    RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
    RCC->CIR = 0x00000000;     //�ر������ж�
}

/********************************************
           ʹ���ⲿ������Ϊϵͳʱ��Դ
���ܣ�
1.ʹ���ⲿHSEʱ��8M��ΪPLL����
2.PLL��Ƶϵ��PLLMUL<=9(ʵ�ʵ���16ʱ������������Ƶ�ⲿʱ��)
3.���������PLLMUL��PLL��Ƶϵ��
4.��ע���ٷ��ֲ���˵��ʹ��HSE��Ϊϵͳʱ��Դʱ����߿ɱ�Ƶ��72MHz������ʵ�ʿ��Ա�Ƶ��128Mϵͳ�����ȶ�
********************************************/
//ϵͳʱ�ӳ�ʼ������
//pll:ѡ��ı�Ƶ������2��ʼ�����ֵΪ16
//ʱ��ԴΪ�ⲿ����
//��ע������������8M����ʱ����ֻ��ʹ���ⲿ8M������Ϊʱ��Դ��
//      ���ڲ���HSI����ʹ���ҷ���û������������λ��ɶ�취û
char SystemClock_HSE(u8 PLL)
{
    unsigned char temp=0;
    MYRCC_DeInit();			//��λ������������
    RCC->CR|=1<<16;       	//�ⲿ����ʱ��ʹ��HSEON
    while(!(RCC->CR>>17));	//�ȴ��ⲿʱ�Ӿ���
    RCC->CFGR=0X00000400; 	//APB1=DIV2;APB2=DIV1;AHB=DIV1;
    PLL-=2;					//����2����λ
    RCC->CFGR|=PLL<<18;   	//����PLLֵ 2~16
    RCC->CFGR|=1<<16;	    //PLLSRC ON
    FLASH->ACR|=0x32;	    //FLASH 2����ʱ����
    RCC->CR|=0x01000000;  	//PLLON
    while(!(RCC->CR>>25));	//�ȴ�PLL����
    RCC->CFGR|=0x00000002;	//PLL��Ϊϵͳʱ��
    while(temp!=0x02)     	//�ȴ�PLL��Ϊϵͳʱ�����óɹ�
    {
        temp=RCC->CFGR>>2;
        temp&=0x03;
    }

    SysClock=(PLL+2)*8;
    return SysClock;
}

//ϵͳʱ�ӳ�ʼ��
void SystemClock_Init(void)
{
	SystemClock_HSE(9);  // ϵͳʱ�ӳ�ʼ����ʱ��Դ�ⲿ����HSEs  8*9=72MHz;
    SysTick_Config(SystemCoreClock/1000);  //SysTick����ϵͳtick��ʱ������ʼ�����жϣ�1ms
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




