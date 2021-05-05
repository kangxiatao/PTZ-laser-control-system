/*！
DWWWWWWWEQHQEWWWWWWWWWWWWEQHHBBBBBQWWWWWWWWWWWWWWWEEEEWWWWWWWWWWWWEHBBHQEEWWWWWWWWEQHEEWWWWEEEEWWWWWWWWWEHBBQEWWWWWWWWWW
DWWWWWWEBBuFHEEEWWWWWWWWEHBBBBWCuYHEWWWWWWWWEEEWWEHBBHEWWWWWWWWWWEHHvCBBBBEWWWWWWEBBYWQWWWEHBHHEWWWWWWWQBBvvHHEWWWWWWWWW
DWWWEEQBB  qBBBHQWWWWWWWQX        WQWWWWWWEQBDBQEHB  BBEWWWWWWWWEHBL  Bv IHEWWWWEBB  qHEEQBB  BEWWWWWEQBE   HBBQEEWWWWWW
DWEHBBBBB  BWGoDBQWWWWWWHn  vDBI DBHEWWWWEHBB EBBBB  kFQWWWWWWEQBBBc  BnvNBEWWEQBBc FBBHBBBX XBBEWWWEHBF vBv kBBBBHQEWWW
DWQBL vv        nBEWWWWEBBBBBB  BBBBHEWWEBWCy  kB     vHEWWWWWQWJii       XEWWQBBk    BEJvv   uBBEEQBBc JBBk    JWBBHEWW
DEBo vFv  vXBBB vBEWWWEHBHkqy       oQWWQv    JBBv  BBBHEWWWWEHj      BBBBBQEEHv     CBI y  n  BBBBBE  BBDonWBy     yQWW
DEBY EBN   vBB  BBEWWWQv      v  JLLDEWEHHBBB GBBB  YvOBEWWWEQBBB     FBBBBBHQHyo  BBBBBB  BBB   Ek  zBBY    BBBBWIukEWW
DWQBi iN  jy   BBBEWWWQZkHBBBBBW BBBQEEHBBnWB uB   C  vBHEEEHBBv  Bv      vGHBHBB  B   W  BBB  Znk vBBBBHJ    BBBBBBQEWW
DWEHBE      vBB  HEWWWEQHHBBBBB  BHEWWEH  QBB Bv  FG IBBBBHBBI  yBBv qBo     LHBB  BOi   B   vBBBHBBBZ     vYDDBBEEEWWWW
DWWEQBBB  BBBEJ  HEWWWWWWEHG   vBHEWWWQn  iCvvEn         vDF  vBBBB  FBBBBBQEDEQBv BBv CBBvjEBHQEEEEHGvv  uFJ   HEWWWWWW
DWWWWEHBBy     GBHEWWWWWWWHv vQBHEWWWWEHBD vNzvvnWBBBkuv uEGCBBBHHB  BBQQHHHHEWEHBvcvYBBBHBBHQEWWWWWEBB   vvcCvvHEWWWWWW
DWWWWWWEHBBBBBBBQEWWWWWWWWEHBBHEEWWWWWWEHBBBBBBBBBBBBBBBBHQQQQEEWEHFBHEWWWWWWWWWEHBEBHQEEEEEEWWWWWWWWEBBBBBBBBBBQWWWWWWW
DWWWWWWWEEEQQQEEWWWWWWWWWWWEEEEWWWWWWWWWWEEEEEEEEEEWEEEEEEEWWWWWWWEQQEWWWWWWWWWWWWEEEEWWWWWWWWWWWWWWWWEQHHHQEEEEWWWWWWWW

-:) TEA-EPIC-USER -:)

 * @file       main.c
 * @brief      云台激光控制系统
 * @author     康夏涛（epic)
 * @version    v1.0
 * @date       2019-08-04
 
------------------------------------
*/

#include "include.h" 


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// set interrupt priority groups 
	
	SystemClock_Init();  		// System clock init
	cycleCounterInit();  		// System counter init
	
	uart_init(115200);		   	// usart1 init
	uart3_init(115200);		   	// usart3 init
	
	LED_Init();                	// led init
	KEY_Init();				   	// key init
	LCD_Init();					// lcd init
	
	tp_dev.init();				// touch screen init
	
//	MPU_Init();					// mpu6050 init
//	while(mpu_dmp_init())		// sure to succeed
//	{LED1_Flash(666);}LED1=1;
	
	TIM2_Init(1999,719);		// 频率 50hz
    Moto_PwmRflash(Serve_MID_PWM,Level_MID_PWM,Serve_MID_PWM,Serve_MID_PWM);  // 归中舵机
    delay_ms(200);
	
	TIM1_Init(49,7199);			// 5ms cycle time , interrupt service function in control.c
	
	EPIC_Init();				// EPIC init
	
	Display_Init();				// Display init
	
	while(1)
	{
		// human computer interaction
		Key_control();	
		Display_Main(); 
		USART_Adjust();  // usart to modify the PID parameters
		
		// 期望修改 （所有功能实现全靠改变期望）
//		Target_Control(angleTarget);  
	}
}



