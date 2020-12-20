#include "header.h"

enum _UATR_RECV_TYPE_	g_uartRecvType;			//串口接收状态
enum _UATR_SEND_TYPE_	g_uartSendType;			//串口发送状态

UART_DATA xdata			UartData;				//串口数据使用到的结构体 

bit 					g_firstRecvFlag;		//首次接收数据标志

/*-----------------------------------------------------------------------------
Description:		初始化串口IO
					电源板通讯：TXD：P4.0；RXD：P4.2
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitUartIo(void)
{
	BANK0_SET;										//指向Bank0

	/*电源板*/
	/*TXD输出，不带上拉，输出高电平*/
	P4CR |= BIT0;
	P4PCR &= (~BIT0);
	P_TXD = 1;
	
	/*RXD输入，不带上拉*/
	P4CR &= (~BIT2);
	P4PCR &= (~BIT2);
}

/*-----------------------------------------------------------------------------
Description:		初始化变量串口
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitUartVariable(void)
{
	g_uartRecvType = recvGoing;
	g_uartSendType = sendOver;

	InitCharArrayData(UartData.SendData_Buff,SEND_MAX);
	InitCharArrayData(UartData.SendDataTmp_Buff,SEND_MAX);
	InitCharArrayData(UartData.SendDataSave_Buff,SEND_MAX);
	InitCharArrayData(UartData.RecvData_Buff,RECV_MAX);
	InitCharArrayData(UartData.RecvDataTmp_Buff,RECV_MAX);
	InitCharArrayData(UartData.RecvDataSave_Buff,RECV_MAX);
	
	UartData.uartRevcCnt     = 0;
	UartData.uartSendCnt     = 0;
	UartData.uartRecvErrCnt  = 0;
	UartData.uartSendCrlCnt  = 0;
	UartData.uartRecvOverCnt = 0;
	
	g_firstRecvFlag			 = 0;
}

/*-----------------------------------------------------------------------------
Description:		初始化串口配置
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitUart(void)
{
	/*BANK1*/
	BANK1_SET;
	UART1CR = 0x24;							//TXD1映射到P4.0；RXD1映射到P4.2
	
	SCON1 = 0x50;							//0101 0000，方式1，8位异步，可变波特率，接收允许
	
											//24000000/16/9600 = 156.25
											//SBRT = 32768 - 156 = 32612 = 0x7F64
											//9600 = 24000000/(16*156+BFINE)
											//BFINE = 4，实际波特率为24000000/(16*156+4)=9600
	SBRTH1 = 0x7F;							//EUART波特率发生器计数器
	SBRTL1 = 0x64;
	SFINE1 = 0x04;
	
	SBRTH1 |= BIT7;							//开EUART1波特率
	
	BANK0_SET;
	
	UTOS |= BIT1;							//RXD1电平为TTL逻辑
	
	IEN2 |= BIT0;							//开EUART1中断
}

/*-----------------------------------------------------------------------------
Description:		求数据的校验和
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
unsigned char TmpDataSum(unsigned char *buf,unsigned char len)
{
	unsigned char sum = 0;
	unsigned char i;
	
	for(i=0;i<len;i++)
	{
		sum += *buf;
		buf++;
	}
	
	return sum;
}

/*-----------------------------------------------------------------------------
Description:		
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TmpDataGive(unsigned char *recv,unsigned char *give,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
		*recv = *give;
		recv++;
		give++;
	}
}

/*-----------------------------------------------------------------------------
Description:		串口发送数据处理，共发送13个数据，每100ms刷新数据
					波特率为9600，一个数据为10位（1个起始位，8个数据位，1个停止位）
					发送13个数据时间为：13*10*（1/9600）= 13.54ms
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void UartSendDeal(void)
{
	static unsigned char snSend;
	
	if((measureData.coreAdHValue <= CORE_MAX_TEMP_AD))					//中心温度大于240℃停止所有加热
	{
		SEND_OUT_DATA1 &= ~BIT_HOT;
		SEND_OUT_DATA_STEAM = 0;
	}

	if(g_adDataHSteam <= STEAM_MAX_TEMP_AD)								//锅炉温度高于185℃关闭蒸发盘加热
	{
		SEND_OUT_DATA_STEAM = 0;	
	}
	
	UartData.uartSendCrlCnt++;
	if((UartData.uartSendCrlCnt >= 10) && (g_uartSendType == sendOver))	//10*10=100ms，每100ms刷新数据
	{
		UartData.uartSendCrlCnt = 0;
		
		TmpDataGive(UartData.SendDataTmp_Buff,UartData.SendDataSave_Buff,SEND_MAX);	//将保存的值赋值给发送数据的中间值
		
		snSend++;														//SN码
		if(snSend > 255)
		{
			snSend = 0;
		}
		
		UartData.SendDataTmp_Buff[0] = 0x55;							//头码
		UartData.SendDataTmp_Buff[1] = snSend;
		UartData.SendDataTmp_Buff[6] = UartData.SendDataTmp_Buff[2] ^ 0xFF;
		UartData.SendDataTmp_Buff[7] = UartData.SendDataTmp_Buff[3] ^ 0xFF;
		UartData.SendDataTmp_Buff[8] = UartData.SendDataTmp_Buff[4] ^ 0xFF;
		UartData.SendDataTmp_Buff[9] = UartData.SendDataTmp_Buff[5] ^ 0xFF;
		UartData.SendDataTmp_Buff[SEND_MAX-2] = TmpDataSum(UartData.SendDataTmp_Buff,SEND_MAX-2);//发送数据的校验和
		UartData.SendDataTmp_Buff[SEND_MAX-1] = UartData.SendDataTmp_Buff[SEND_MAX-2] ^ 0XFF;	//校验和反码
		
		TmpDataGive(UartData.SendData_Buff,UartData.SendDataTmp_Buff,SEND_MAX);					//将数据赋值给缓存区，准备发送
		
		g_uartSendType = sendGoing;										//串口正在发送状态
		UartData.uartSendCnt = 0;										//发送计数清零
		
		BANK1_SET;
		SBUF1 = UartData.SendData_Buff[0];								//发送第一位数据
		BANK0_SET;
	}
}

/*-----------------------------------------------------------------------------
Description:		串口接收数据处理
					波特率为9600，一个数据为10位（1个起始位，8个数据位，1个停止位）
					接收29个数据时间为：29*10*（1/9600）= 30.21ms
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void UartRecvDeal(void)
{
	unsigned char checksum,checkRvsCode;
	
	static unsigned int s_getPumpWorkAdFlag = 0;
	static 			bit	s_powerDownUpFlag = 0;

	UartData.uartRecvErrCnt++;
	UartData.uartRecvOverCnt++;
	if(UartData.uartRecvErrCnt >= 10)									//10*10=100ms
	{
		UartData.uartRecvErrCnt = 11;									//接收超时后，重新开始接收数据，防止串包
		UartData.uartRevcCnt = 0;
	}
	if(UartData.uartRecvOverCnt >= 300)									//10*300=3S	
	{
		UartData.uartRecvOverCnt = 301;									//超过3S未接收到完整数据帧，报错
	}
	if(g_uartRecvType == recvOver)										//一帧数据接收完成，校验存储
	{
		checksum = TmpDataSum(UartData.RecvDataTmp_Buff,RECV_MAX-2);	//接收数据的校验和
		checkRvsCode = checksum ^ 0XFF;									//接收数据和的反码	
	
		/*校验正确，存储数据，不正确重新接收*/
		if((checksum == UartData.RecvDataTmp_Buff[RECV_MAX-2]) && (checkRvsCode == UartData.RecvDataTmp_Buff[RECV_MAX-1]))
		{
			TmpDataGive(UartData.RecvDataSave_Buff,UartData.RecvDataTmp_Buff,RECV_MAX);
			UartData.uartRecvOverCnt= 0;
			
			g_firstRecvFlag = 1;										//首次接收数据标志，做串口报警使用
		}
		
		g_uartRecvType = recvGoing;
		
		if(g_pdWaitTime == 0)											//当无掉电延迟时，读取测量数据
		{
			g_adDataHSteam = (((RECV_STEAM_H_TEM_H8 & 0x00FF) << 8) + RECV_STEAM_H_TEM_L8);			//读取锅炉的温度，中心温度由控制板采集
			g_adDataLSteam = (((RECV_STEAM_L_TEM_H8 & 0x00FF) << 8) + RECV_STEAM_L_TEM_L8);

//			g_potType	= RECV_POT_TYPE;								//内锅状态读取
			g_zeroType	= RECV_ZERO_TYPE;								//过零状态读取
			g_pumpType	= RECV_PUMP_WORK_FLAG;							//水泵是否有新抽水读取
			
//			g_potType	= STATE_YES;
			
			if(g_pumpType == STATE_YES)									//若水泵有新一次抽水
			{
				if(s_getPumpWorkAdFlag == 0)							//未记录水泵工作电流时
				{
					g_pumpCurrentAd = (((RECV_PUMP_CURRENT_H8 & 0x00FF) << 8) + RECV_PUMP_CURRENT_L8);	//读取当前水泵电流值
					
					s_getPumpWorkAdFlag = 1;							//标记为已记录水泵工作电流
				}
			}
			else
			{
				s_getPumpWorkAdFlag = 0;
			}
			
//			if(s_powerDownUpFlag)										//若有重新上下电
//			{
//				if(g_potType == STATE_YES)								//重新检测是否有锅，只检测一次
//				{
//					g_potFlag = POT_OK;									//标记为有锅
//				}
//				else
//				{
//					g_potFlag = POT_NO;									//标记为缺锅
//				}
//				
//				s_powerDownUpFlag = 0;									//重新上下电已处理
//			}
		}
		else
		{
			/*赋一个NTC初值，使高温不加热且不报警*/
			g_adDataHSteam 	= 0x0259;									//185℃
			g_adDataLSteam 	= 0x002D;									//185℃
			
			/*赋初值，使不报警*/
//			g_potType		= STATE_YES;
//			g_potFlag		= POT_OK;
			g_zeroType		= STATE_YES;
			
			s_powerDownUpFlag = 1;										//表示短时掉电重新上电
		}
	}
}