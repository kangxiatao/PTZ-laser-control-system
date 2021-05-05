#ifndef __MYUSART_H
#define __MYUSART_H	 
#include "sys.h"

#define UartDataNum 17 
//上位机数据通道数量，必须与上位机设置一样 首先，FreeCarsDataNum是已经知道的，他是通道数UartDataNum*2(因为数据是Int16型的). 



void USendOneByte(u8 date);		// 串口发送一个字节
void sendDataToScope(void);     // 数据发送协议
void push(u8 chanel,u16 data);  // 向上位机填充数据
void PC_data(void);				// 给上位机发送欧拉角数据
void USART_Adjust(void);		// 串口数据处理

#endif
