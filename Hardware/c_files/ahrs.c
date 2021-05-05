/*��

 * @file       ahrs.c
 * @brief      ��̬����
 * @author     �����Σ�epic)
 * @version    v2.0
 * @date       2017-05-12
 
------------------------------------
*/
#include "ahrs.h"
#include "include.h"

/* -- ���д���¾� -- ��������2017-05-12�޸�
��д�ߣ�lisn3188
��ַ��www.chiplab7.com
����ʱ��: 2012-05-25
���ԣ� ���������ڵ���ʵ���ҵ�mini IMU����ɲ���
���ܣ�
  ʹ�ô����жϳ������mini IMU �����ݡ�
  ��ֲ�������Լ���д��Ӧ�Ĵ��ڳ�ʼ�����򣬲����������жϡ�
  ���ж��ӳ����� ��Ҫ����Ӧ�ĸĶ�

  Ϊ��ʹ�������������Ҫ��
  1.��ʼ����Ĵ��ڣ�����������ģ��Ĳ��������ó�һ�µ�
  2.�������ڵĽ����жϣ����ο����³����д�ж��ӳ���
  �ο� void USART3_IRQHandler(void) 
  3.���������ѭ�����У���ʱ�����ӳ���
  void USART_CommandRoute(void)
  �Լ���Ƿ��յ��µ�����֡
  ���е���̬���ݺ�ADCֵ������ USART_CommandRoute
  �н��и��¡�
  4.ʹ�� ����ֵ���Զ����£�
  
	float yaw;  		//ƫ����
	float pitch;		//����
	float roll; 		//��ת
	float alt;  		//�߶�
	float tempr;		//�¶�
	float press;		//��ѹ
	int16_t gyro[3];	//���ٶȼ�
	int16_t accel[3];	//������
	int16_t magnet[3];	//������
	------------------------------------
*/


#define b_uart_head  0x80  //�յ�A5 ͷ ��־λ
#define b_rx_over    0x40  //�յ�������֡��־
// USART Receiver buffer
#define RX_BUFFER_SIZE 100 //���ջ������ֽ���

void Decode_frame(unsigned char data);
volatile unsigned char rx_buffer[RX_BUFFER_SIZE]; //�������ݻ�����
volatile unsigned char rx_wr_index; //����дָ��
volatile unsigned char RC_Flag;  //����״̬��־�ֽ�

unsigned char ahrsUpdated;  //���ݸ��±�־λ 
unsigned char ahrsAdjust;  //����У׼��־λ


//��̬����
AHRS_Typedef AHRS={0};

float gyroOffset[3]; //���ٶ�ƫ����
float angleOffset[3]; //�Ƕ�ƫ����


// Read AHRS Data
void Get_AHRS_Data(float *pitch, float *roll, short *gyrox, short *gyroy)
{
	USART_CommandRoute();  // ��ȡAHRS��������
	
	if(ahrsUpdated)  // �����Ѿ�����
	{
		*pitch = -(AHRS.pitch - angleOffset[PITCH]);
		*roll  = (AHRS.roll - angleOffset[ROLL]);
		*gyrox = -(AHRS.gyro[PITCH] - gyroOffset[PITCH]);
		*gyroy = (AHRS.gyro[ROLL] - gyroOffset[ROLL]);
		
//		*pitch = -AHRS.pitch;
//		*roll  = AHRS.roll;
//		*gyrox = -AHRS.gyro[PITCH];
//		*gyroy = AHRS.gyro[ROLL];
	
		// �˴�Ӧ�и߼��˲�
		
		ahrsUpdated = 0;
	}
}


// ���ڴ�whileǰ����У׼
void AHRS_Data_Init(void)
{
	u8 cnt=0;
	
	ahrsAdjust = 0;  // ������ݿ�ʼУ׼
	
	while(1)
	{
		USART_CommandRoute();  // ��ȡAHRS��������
		
		if(ahrsUpdated)  // �����Ѿ�����
		{
			angleOffset[PITCH] += AHRS.pitch;
			angleOffset[ROLL]  += AHRS.roll;
			gyroOffset[PITCH] += AHRS.gyro[PITCH];
			gyroOffset[ROLL] += AHRS.gyro[ROLL];
			
			cnt ++;
			ahrsUpdated = 0;
		}
		
		if(cnt >= 20)
		{
			angleOffset[PITCH] /= cnt;
			angleOffset[ROLL] /= cnt;
			gyroOffset[PITCH] /= cnt;
			gyroOffset[ROLL] /= cnt;
			
			printf("pitch - %f\r\nroll - %f\r\ngyrox - %f\r\ngyroy - %f\r\n",
			angleOffset[PITCH], angleOffset[ROLL], gyroOffset[PITCH], gyroOffset[ROLL]);
			printf("\r\nOK - (0110)(0110)(0110) - \r\n");
			
			ahrsAdjust = 1;  // �������У׼���
			
			break;
		}
	}
}


//�ڽ�����һ֡IMU��̬����󣬵�������ӳ�����ȡ����̬����
void USART_Get_IMU(void)
{
	int16_t temp;
	
//	temp = 0;
//	temp = rx_buffer[2];
//	temp <<= 8;
//	temp |= rx_buffer[3];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.yaw=(float)temp / 10.0f; //ƫ����
	
	temp = 0;
	temp = rx_buffer[4];
	temp <<= 8;
	temp |= rx_buffer[5];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.pitch=(float)temp / 10.0f;//����
	
	temp = 0;
	temp = rx_buffer[6];
	temp <<= 8;
	temp |= rx_buffer[7];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.roll=(float)temp / 10.0f;//��ת
//	
//	temp = 0;
//	temp = rx_buffer[8];
//	temp <<= 8;
//	temp |= rx_buffer[9];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.alt=(float)temp / 10.0f;//�߶�
	
//	temp = 0;
//	temp = rx_buffer[10];
//	temp <<= 8;
//	temp |= rx_buffer[11];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.tempr=(float)temp / 10.0f;//�¶�
	
//	temp = 0;
//	temp = rx_buffer[12];
//	temp <<= 8;
//	temp |= rx_buffer[13];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.press=(float)temp * 10.0f;//��ѹ
	
}

//�ڽ���һ֡ReportMotion ���������ӳ�����ȡ��ADC����
void USART_Get_Motion(void)
{
	int16_t temp;
	
//	temp = 0;
//	temp = rx_buffer[2];
//	temp <<= 8;
//	temp |= rx_buffer[3];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.accel[ROLL]=temp;//���ٶȼ� X���ADCֵ
//	
//	temp = 0;
//	temp = rx_buffer[4];
//	temp <<= 8;
//	temp |= rx_buffer[5];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.accel[PITCH]=temp;//���ٶȼ� Y���ADCֵ
//	
//	temp = 0;
//	temp = rx_buffer[6];
//	temp <<= 8;
//	temp |= rx_buffer[7];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.accel[YAW]=temp;//���ٶȼ� Z���ADCֵ
	
	temp = 0;
	temp = rx_buffer[8];
	temp <<= 8;
	temp |= rx_buffer[9];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.gyro[ROLL]=temp;//������ X���ADCֵ
	
	temp = 0;
	temp = rx_buffer[10];
	temp <<= 8;
	temp |= rx_buffer[11];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.gyro[PITCH]=temp;//������ Y���ADCֵ
	
//	temp = 0;
//	temp = rx_buffer[12];
//	temp <<= 8;
//	temp |= rx_buffer[13];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.gyro[YAW]=temp;//������ Z���ADCֵ
	
//	temp = 0;
//	temp = rx_buffer[14];
//	temp <<= 8;
//	temp |= rx_buffer[15];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.magnet[0]=temp;//������ X���ADCֵ
//	
//	temp = 0;
//	temp = rx_buffer[16];
//	temp <<= 8;
//	temp |= rx_buffer[17];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.magnet[1]=temp;//������ Y���ADCֵ
//	
//	temp = 0;
//	temp = rx_buffer[18];
//	temp <<= 8;
//	temp |= rx_buffer[19];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.magnet[2]=temp;//������ Z���ADCֵ
}


//�����ж��ӳ����ǻ���STM32�ģ������ʹ�õĲ���STM32 ������Ӧ���޸�
//------------------------------------------------------
void USART3_IRQHandler(void) //�����ж��ӳ���
{
	unsigned char data;
		
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�Ƿ��յ�����?
	{
		data=USART_ReceiveData(USART3); //��ȡ���յ����ֽ�
			
		if((RC_Flag&b_rx_over) == 0){//ȷ�������Ѿ�������
			
			Decode_frame(data);	//�����֡����
		}
	}
	
	// У׼�����ڴ˴�������
	if(ahrsAdjust)
	{
		/*-- refresh data --*/
		Get_AHRS_Data(&Pitch.angle, &Roll.angle, &Pitch.rate, &Roll.rate);
	}
	
	/* Clear the USART3 Recive interrupt */
//	USART_ClearITPendingBit(USART3, USART_IT_RXNE); //���жϱ�־.
}


//--У�鵱ǰ���յ���һ֡�����Ƿ� ��֡У���ֽ�һ��
unsigned char Sum_check(void)
{ 
	unsigned char i;
	unsigned int checksum=0; 
	for(i=0;i<rx_buffer[0]-2;i++)
		checksum+=rx_buffer[i];
	if((checksum%256)==rx_buffer[rx_buffer[0]-2])
		return(0x01); //Checksum successful
	else
		return(0x00); //Checksum error
}



//--����ӳ�����Ҫ���������� ��ʱ����,�Լ�� �����Ƿ������һ֡����
void USART_CommandRoute(void)
{
	if(RC_Flag&b_rx_over){  //�Ѿ�������һ֡?
		RC_Flag&=~b_rx_over; //���־��
		if(Sum_check()){ 
			//У��ͨ��
			if(rx_buffer[1]==0xA1){ //USART_ReportIMU ������
				USART_Get_IMU();	//ȡ����
			}
			if(rx_buffer[1]==0xA2){ //USART_ReportMotion ������
				USART_Get_Motion();	 //ȡ����
			} 
			ahrsUpdated = 1;  //��������Ѹ���
		}//У���Ƿ�ͨ��?
	}
}

//�յ�һ���ֽڵ���һ�Ρ����յ����ֽ���Ϊ���롣
void Decode_frame(unsigned char data){
	if(data==0xa5) 
	{ 
		RC_Flag|=b_uart_head; //������յ�A5 ��λ֡ͷ��רλ
		rx_buffer[rx_wr_index++]=data; //��������ֽ�.
	}
	else if(data==0x5a)
	{ 
		if(RC_Flag&b_uart_head) //�����һ���ֽ���A5 ��ô�϶� �����֡��ʼ�ֽ�
		{ rx_wr_index=0;  //���� ������ָ��
			RC_Flag&=~b_rx_over; //���֡�Ÿոտ�ʼ��
		}
		else //��һ���ֽڲ���A5
			rx_buffer[rx_wr_index++]=data;
		RC_Flag&=~b_uart_head; //��֡ͷ��־
	}
	else
	{ rx_buffer[rx_wr_index++]=data;
	   RC_Flag&=~b_uart_head;
	   if(rx_wr_index==rx_buffer[0]) //�չ����ֽ���.
	   {  
		   RC_Flag|=b_rx_over; //��λ ����������һ֡����
//		   USART_CommandRoute(); //������ȡ���ݡ�
	   }
	}
	   
	if(rx_wr_index==RX_BUFFER_SIZE) //��ֹ���������
	rx_wr_index--;
}


