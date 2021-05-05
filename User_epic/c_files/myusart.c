/*��

 * @file       myusart.c
 * @brief      ����Э��������޸�
 * @author     �����Σ�epic)
 * @version    v2.0
 * @date       2017-05-14
 
------------------------------------
*/

#include "myusart.h"
#include "usart.h"
#include "include.h"

u8 uSendBuf[UartDataNum*2]={0};
u8 FreeCarsDataNum=UartDataNum*2;

void USendOneByte(u8 date)
{
	USART1->DR=date;
	while((USART1->SR&0X40)==0);
}

void sendDataToScope(void)  //  ���ݷ���Э��
{
	s8 i,sum=0;
	USendOneByte(251);
	USendOneByte(109);
	USendOneByte(37);
	sum+=(251);
	sum+=(109);
	sum+=(37);
	for(i=0;i<FreeCarsDataNum;i++) 
	{ 
		USendOneByte(uSendBuf[i]); 
		sum+=uSendBuf[i];
	}
	USendOneByte(sum);  
}

/*
  ����λ��ĳ��ͨ���������������
  chanel��ͨ�� 
  data��  ���� 16λ
*/ 
void push(u8 chanel,u16 data) 
{ 
	uSendBuf[chanel*2]=data/256; 
	uSendBuf[chanel*2+1]=data%256; 
}

void PC_data(void)  // ����λ������ŷ��������
{
	push(0,10);
	push(1,10);
	push(2,10);
	push(3,10);
	sendDataToScope();  // ���ݷ���Э��
}

/*
  �������ݴ���

  -->> �����ʽ 
				
  �޸�PID������
	-- PR0.00 -- 
	ǰ��λ��ʾҪ�޸ĵı���
	����ΪҪ�޸ĵ�ֵ

  �鿴���ݣ�
    -- C --
*/
void USART_Adjust(void)  
{
	if(USART_RX_STA&0x8000)
	{				
		u8 i;
		for(i=2; i<=6; i++ )  // �����ַ�ת����
		{
			USART_RX_BUF[i] = USART_RX_BUF[i] - 0x30;
		}
		if(USART_RX_BUF[0] == 'P')
		{
			if(USART_RX_BUF[1] == 'P') 
			{
				pitch_angle_PID.P = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[3]*10 + USART_RX_BUF[4] );
				roll_angle_PID.P = pitch_angle_PID.P;
                printf("\r\npitch_angle_PID -P:%4.2f\r\n",pitch_angle_PID.P);
			}
			else if(USART_RX_BUF[1] == 'X') 
			{
				pitch_rate_PID.P = (float)( USART_RX_BUF[2]*1000 + USART_RX_BUF[4]*100 + USART_RX_BUF[5]*10 + USART_RX_BUF[6]) / 1000;
				roll_rate_PID.P = pitch_rate_PID.P;
                printf("\r\npitch_rate_PID -P:%1.3f\r\n",pitch_rate_PID.P);
			}
		}
		else if(USART_RX_BUF[0] == 'I')
		{
			if(USART_RX_BUF[1] == 'P') 
			{
				pitch_angle_PID.I = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5]) / 100;
				roll_angle_PID.I = pitch_angle_PID.I;
                printf("\r\npitch_angle_PID -I:%4.2f\r\n",pitch_angle_PID.I);
			}
			else if(USART_RX_BUF[1] == 'X') 
			{
				pitch_rate_PID.I = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5]) / 100;
				roll_rate_PID.I = pitch_rate_PID.I;
                printf("\r\npitch_rate_PID -I:%4.2f\r\n",pitch_rate_PID.I);
			}
		}
		else if(USART_RX_BUF[0] == 'D')
		{
			if(USART_RX_BUF[1] == 'P') 
			{
				pitch_angle_PID.D = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[3]*10 + USART_RX_BUF[4]);
				roll_angle_PID.D = pitch_angle_PID.D;
                printf("\r\npitch_angle_PID -D:%4.2f\r\n",pitch_angle_PID.D);
			}
			else if(USART_RX_BUF[1] == 'X') 
			{
				pitch_rate_PID.D = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5]) / 100;
				roll_rate_PID.D = pitch_rate_PID.D;
                printf("\r\npitch_rate_PID -D:%4.2f\r\n",pitch_rate_PID.D);
			}
		}
		else if(USART_RX_BUF[0] == 'C')  // ���ݲ鿴
		{
			printf("\r\n");
			printf("ϵͳһ�й������� - (0110)(0110)(0110) - \r\n");
		}
		USART_RX_STA=0;
	}
}


