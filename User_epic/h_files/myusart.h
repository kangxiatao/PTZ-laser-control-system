#ifndef __MYUSART_H
#define __MYUSART_H	 
#include "sys.h"

#define UartDataNum 17 
//��λ������ͨ����������������λ������һ�� ���ȣ�FreeCarsDataNum���Ѿ�֪���ģ�����ͨ����UartDataNum*2(��Ϊ������Int16�͵�). 



void USendOneByte(u8 date);		// ���ڷ���һ���ֽ�
void sendDataToScope(void);     // ���ݷ���Э��
void push(u8 chanel,u16 data);  // ����λ���������
void PC_data(void);				// ����λ������ŷ��������
void USART_Adjust(void);		// �������ݴ���

#endif
