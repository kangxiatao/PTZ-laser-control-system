/*！

 * @file       ahrs.c
 * @brief      姿态数据
 * @author     康夏涛（epic)
 * @version    v2.0
 * @date       2017-05-12
 
------------------------------------
*/
#include "ahrs.h"
#include "include.h"

/* -- 向编写者致敬 -- 康夏涛于2017-05-12修改
编写者：lisn3188
网址：www.chiplab7.com
初版时间: 2012-05-25
测试： 本程序已在第七实验室的mini IMU上完成测试
功能：
  使用串行中断程序接收mini IMU 的数据。
  移植者需在自己编写对应的串口初始化程序，并开启接收中断。
  在中断子程序中 需要做相应的改动

  为了使用这个程序，你需要：
  1.初始化你的串口，将波特率与模块的波特率设置成一致的
  2.开启串口的接收中断，并参考以下程序编写中断子程序
  参考 void USART3_IRQHandler(void) 
  3.在主程序的循环体中，定时调用子程序：
  void USART_CommandRoute(void)
  以检查是否收到新的数据帧
  所有的姿态数据和ADC值都会在 USART_CommandRoute
  中进行更新。
  4.使用 以下值会自动更新：
  
	float yaw;  		//偏航角
	float pitch;		//俯仰
	float roll; 		//滚转
	float alt;  		//高度
	float tempr;		//温度
	float press;		//气压
	int16_t gyro[3];	//加速度计
	int16_t accel[3];	//陀螺仪
	int16_t magnet[3];	//磁力计
	------------------------------------
*/


#define b_uart_head  0x80  //收到A5 头 标志位
#define b_rx_over    0x40  //收到完整的帧标志
// USART Receiver buffer
#define RX_BUFFER_SIZE 100 //接收缓冲区字节数

void Decode_frame(unsigned char data);
volatile unsigned char rx_buffer[RX_BUFFER_SIZE]; //接收数据缓冲区
volatile unsigned char rx_wr_index; //缓冲写指针
volatile unsigned char RC_Flag;  //接收状态标志字节

unsigned char ahrsUpdated;  //数据更新标志位 
unsigned char ahrsAdjust;  //数据校准标志位


//姿态数据
AHRS_Typedef AHRS={0};

float gyroOffset[3]; //角速度偏移量
float angleOffset[3]; //角度偏移量


// Read AHRS Data
void Get_AHRS_Data(float *pitch, float *roll, short *gyrox, short *gyroy)
{
	USART_CommandRoute();  // 读取AHRS所有数据
	
	if(ahrsUpdated)  // 数据已经更新
	{
		*pitch = -(AHRS.pitch - angleOffset[PITCH]);
		*roll  = (AHRS.roll - angleOffset[ROLL]);
		*gyrox = -(AHRS.gyro[PITCH] - gyroOffset[PITCH]);
		*gyroy = (AHRS.gyro[ROLL] - gyroOffset[ROLL]);
		
//		*pitch = -AHRS.pitch;
//		*roll  = AHRS.roll;
//		*gyrox = -AHRS.gyro[PITCH];
//		*gyroy = AHRS.gyro[ROLL];
	
		// 此处应有高级滤波
		
		ahrsUpdated = 0;
	}
}


// 放在大while前用于校准
void AHRS_Data_Init(void)
{
	u8 cnt=0;
	
	ahrsAdjust = 0;  // 标记数据开始校准
	
	while(1)
	{
		USART_CommandRoute();  // 读取AHRS所有数据
		
		if(ahrsUpdated)  // 数据已经更新
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
			
			ahrsAdjust = 1;  // 标记数据校准完毕
			
			break;
		}
	}
}


//在接收完一帧IMU姿态报告后，调用这个子程序来取出姿态数据
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
//	AHRS.yaw=(float)temp / 10.0f; //偏航角
	
	temp = 0;
	temp = rx_buffer[4];
	temp <<= 8;
	temp |= rx_buffer[5];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.pitch=(float)temp / 10.0f;//俯仰
	
	temp = 0;
	temp = rx_buffer[6];
	temp <<= 8;
	temp |= rx_buffer[7];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.roll=(float)temp / 10.0f;//滚转
//	
//	temp = 0;
//	temp = rx_buffer[8];
//	temp <<= 8;
//	temp |= rx_buffer[9];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.alt=(float)temp / 10.0f;//高度
	
//	temp = 0;
//	temp = rx_buffer[10];
//	temp <<= 8;
//	temp |= rx_buffer[11];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.tempr=(float)temp / 10.0f;//温度
	
//	temp = 0;
//	temp = rx_buffer[12];
//	temp <<= 8;
//	temp |= rx_buffer[13];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.press=(float)temp * 10.0f;//气压
	
}

//在接收一帧ReportMotion 后调用这个子程序来取出ADC数据
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
//	AHRS.accel[ROLL]=temp;//加速度计 X轴的ADC值
//	
//	temp = 0;
//	temp = rx_buffer[4];
//	temp <<= 8;
//	temp |= rx_buffer[5];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.accel[PITCH]=temp;//加速度计 Y轴的ADC值
//	
//	temp = 0;
//	temp = rx_buffer[6];
//	temp <<= 8;
//	temp |= rx_buffer[7];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.accel[YAW]=temp;//加速度计 Z轴的ADC值
	
	temp = 0;
	temp = rx_buffer[8];
	temp <<= 8;
	temp |= rx_buffer[9];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.gyro[ROLL]=temp;//陀螺仪 X轴的ADC值
	
	temp = 0;
	temp = rx_buffer[10];
	temp <<= 8;
	temp |= rx_buffer[11];
	if(temp&0x8000){
		temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	AHRS.gyro[PITCH]=temp;//陀螺仪 Y轴的ADC值
	
//	temp = 0;
//	temp = rx_buffer[12];
//	temp <<= 8;
//	temp |= rx_buffer[13];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.gyro[YAW]=temp;//陀螺仪 Z轴的ADC值
	
//	temp = 0;
//	temp = rx_buffer[14];
//	temp <<= 8;
//	temp |= rx_buffer[15];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.magnet[0]=temp;//磁力计 X轴的ADC值
//	
//	temp = 0;
//	temp = rx_buffer[16];
//	temp <<= 8;
//	temp |= rx_buffer[17];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.magnet[1]=temp;//磁力计 Y轴的ADC值
//	
//	temp = 0;
//	temp = rx_buffer[18];
//	temp <<= 8;
//	temp |= rx_buffer[19];
//	if(temp&0x8000){
//		temp = 0-(temp&0x7fff);
//	}else temp = (temp&0x7fff);
//	AHRS.magnet[2]=temp;//磁力计 Z轴的ADC值
}


//以下中断子程序是基于STM32的，如果你使用的不是STM32 请做相应的修改
//------------------------------------------------------
void USART3_IRQHandler(void) //接收中断子程序
{
	unsigned char data;
		
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //是否收到数据?
	{
		data=USART_ReceiveData(USART3); //读取接收到的字节
			
		if((RC_Flag&b_rx_over) == 0){//确保数据已经处理完
			
			Decode_frame(data);	//进入解帧程序
		}
	}
	
	// 校准过才在此处度数据
	if(ahrsAdjust)
	{
		/*-- refresh data --*/
		Get_AHRS_Data(&Pitch.angle, &Roll.angle, &Pitch.rate, &Roll.rate);
	}
	
	/* Clear the USART3 Recive interrupt */
//	USART_ClearITPendingBit(USART3, USART_IT_RXNE); //清中断标志.
}


//--校验当前接收到的一帧数据是否 与帧校验字节一致
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



//--这个子程序需要在主程序中 定时调用,以检查 串口是否接收完一帧数据
void USART_CommandRoute(void)
{
	if(RC_Flag&b_rx_over){  //已经接收完一帧?
		RC_Flag&=~b_rx_over; //清标志先
		if(Sum_check()){ 
			//校验通过
			if(rx_buffer[1]==0xA1){ //USART_ReportIMU 的数据
				USART_Get_IMU();	//取数据
			}
			if(rx_buffer[1]==0xA2){ //USART_ReportMotion 的数据
				USART_Get_Motion();	 //取数据
			} 
			ahrsUpdated = 1;  //标记数据已更新
		}//校验是否通过?
	}
}

//收到一个字节调用一次。把收到的字节做为输入。
void Decode_frame(unsigned char data){
	if(data==0xa5) 
	{ 
		RC_Flag|=b_uart_head; //如果接收到A5 置位帧头标专位
		rx_buffer[rx_wr_index++]=data; //保存这个字节.
	}
	else if(data==0x5a)
	{ 
		if(RC_Flag&b_uart_head) //如果上一个字节是A5 那么认定 这个是帧起始字节
		{ rx_wr_index=0;  //重置 缓冲区指针
			RC_Flag&=~b_rx_over; //这个帧才刚刚开始收
		}
		else //上一个字节不是A5
			rx_buffer[rx_wr_index++]=data;
		RC_Flag&=~b_uart_head; //清帧头标志
	}
	else
	{ rx_buffer[rx_wr_index++]=data;
	   RC_Flag&=~b_uart_head;
	   if(rx_wr_index==rx_buffer[0]) //收够了字节数.
	   {  
		   RC_Flag|=b_rx_over; //置位 接收完整的一帧数据
//		   USART_CommandRoute(); //立即提取数据。
	   }
	}
	   
	if(rx_wr_index==RX_BUFFER_SIZE) //防止缓冲区溢出
	rx_wr_index--;
}


