#include "header.h"

enum _UATR_RECV_TYPE_	g_uartRecvType;			//���ڽ���״̬
enum _UATR_SEND_TYPE_	g_uartSendType;			//���ڷ���״̬

UART_DATA xdata			UartData;				//��������ʹ�õ��Ľṹ�� 

bit 					g_firstRecvFlag;		//�״ν������ݱ�־

/*-----------------------------------------------------------------------------
Description:		��ʼ������IO
					��Դ��ͨѶ��TXD��P4.0��RXD��P4.2
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitUartIo(void)
{
	BANK0_SET;										//ָ��Bank0

	/*��Դ��*/
	/*TXD�������������������ߵ�ƽ*/
	P4CR |= BIT0;
	P4PCR &= (~BIT0);
	P_TXD = 1;
	
	/*RXD���룬��������*/
	P4CR &= (~BIT2);
	P4PCR &= (~BIT2);
}

/*-----------------------------------------------------------------------------
Description:		��ʼ����������
Input:				void
Return:				void
History:			��
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
Description:		��ʼ����������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitUart(void)
{
	/*BANK1*/
	BANK1_SET;
	UART1CR = 0x24;							//TXD1ӳ�䵽P4.0��RXD1ӳ�䵽P4.2
	
	SCON1 = 0x50;							//0101 0000����ʽ1��8λ�첽���ɱ䲨���ʣ���������
	
											//24000000/16/9600 = 156.25
											//SBRT = 32768 - 156 = 32612 = 0x7F64
											//9600 = 24000000/(16*156+BFINE)
											//BFINE = 4��ʵ�ʲ�����Ϊ24000000/(16*156+4)=9600
	SBRTH1 = 0x7F;							//EUART�����ʷ�����������
	SBRTL1 = 0x64;
	SFINE1 = 0x04;
	
	SBRTH1 |= BIT7;							//��EUART1������
	
	BANK0_SET;
	
	UTOS |= BIT1;							//RXD1��ƽΪTTL�߼�
	
	IEN2 |= BIT0;							//��EUART1�ж�
}

/*-----------------------------------------------------------------------------
Description:		�����ݵ�У���
Input:				void
Return:				void
History:			��
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
History:			��
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
Description:		���ڷ������ݴ���������13�����ݣ�ÿ100msˢ������
					������Ϊ9600��һ������Ϊ10λ��1����ʼλ��8������λ��1��ֹͣλ��
					����13������ʱ��Ϊ��13*10*��1/9600��= 13.54ms
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void UartSendDeal(void)
{
	static unsigned char snSend;
	
	if((measureData.coreAdHValue <= CORE_MAX_TEMP_AD))					//�����¶ȴ���240��ֹͣ���м���
	{
		SEND_OUT_DATA1 &= ~BIT_HOT;
		SEND_OUT_DATA_STEAM = 0;
	}

	if(g_adDataHSteam <= STEAM_MAX_TEMP_AD)								//��¯�¶ȸ���185��ر������̼���
	{
		SEND_OUT_DATA_STEAM = 0;	
	}
	
	UartData.uartSendCrlCnt++;
	if((UartData.uartSendCrlCnt >= 10) && (g_uartSendType == sendOver))	//10*10=100ms��ÿ100msˢ������
	{
		UartData.uartSendCrlCnt = 0;
		
		TmpDataGive(UartData.SendDataTmp_Buff,UartData.SendDataSave_Buff,SEND_MAX);	//�������ֵ��ֵ���������ݵ��м�ֵ
		
		snSend++;														//SN��
		if(snSend > 255)
		{
			snSend = 0;
		}
		
		UartData.SendDataTmp_Buff[0] = 0x55;							//ͷ��
		UartData.SendDataTmp_Buff[1] = snSend;
		UartData.SendDataTmp_Buff[6] = UartData.SendDataTmp_Buff[2] ^ 0xFF;
		UartData.SendDataTmp_Buff[7] = UartData.SendDataTmp_Buff[3] ^ 0xFF;
		UartData.SendDataTmp_Buff[8] = UartData.SendDataTmp_Buff[4] ^ 0xFF;
		UartData.SendDataTmp_Buff[9] = UartData.SendDataTmp_Buff[5] ^ 0xFF;
		UartData.SendDataTmp_Buff[SEND_MAX-2] = TmpDataSum(UartData.SendDataTmp_Buff,SEND_MAX-2);//�������ݵ�У���
		UartData.SendDataTmp_Buff[SEND_MAX-1] = UartData.SendDataTmp_Buff[SEND_MAX-2] ^ 0XFF;	//У��ͷ���
		
		TmpDataGive(UartData.SendData_Buff,UartData.SendDataTmp_Buff,SEND_MAX);					//�����ݸ�ֵ����������׼������
		
		g_uartSendType = sendGoing;										//�������ڷ���״̬
		UartData.uartSendCnt = 0;										//���ͼ�������
		
		BANK1_SET;
		SBUF1 = UartData.SendData_Buff[0];								//���͵�һλ����
		BANK0_SET;
	}
}

/*-----------------------------------------------------------------------------
Description:		���ڽ������ݴ���
					������Ϊ9600��һ������Ϊ10λ��1����ʼλ��8������λ��1��ֹͣλ��
					����29������ʱ��Ϊ��29*10*��1/9600��= 30.21ms
Input:				void
Return:				void
History:			��
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
		UartData.uartRecvErrCnt = 11;									//���ճ�ʱ�����¿�ʼ�������ݣ���ֹ����
		UartData.uartRevcCnt = 0;
	}
	if(UartData.uartRecvOverCnt >= 300)									//10*300=3S	
	{
		UartData.uartRecvOverCnt = 301;									//����3Sδ���յ���������֡������
	}
	if(g_uartRecvType == recvOver)										//һ֡���ݽ�����ɣ�У��洢
	{
		checksum = TmpDataSum(UartData.RecvDataTmp_Buff,RECV_MAX-2);	//�������ݵ�У���
		checkRvsCode = checksum ^ 0XFF;									//�������ݺ͵ķ���	
	
		/*У����ȷ���洢���ݣ�����ȷ���½���*/
		if((checksum == UartData.RecvDataTmp_Buff[RECV_MAX-2]) && (checkRvsCode == UartData.RecvDataTmp_Buff[RECV_MAX-1]))
		{
			TmpDataGive(UartData.RecvDataSave_Buff,UartData.RecvDataTmp_Buff,RECV_MAX);
			UartData.uartRecvOverCnt= 0;
			
			g_firstRecvFlag = 1;										//�״ν������ݱ�־�������ڱ���ʹ��
		}
		
		g_uartRecvType = recvGoing;
		
		if(g_pdWaitTime == 0)											//���޵����ӳ�ʱ����ȡ��������
		{
			g_adDataHSteam = (((RECV_STEAM_H_TEM_H8 & 0x00FF) << 8) + RECV_STEAM_H_TEM_L8);			//��ȡ��¯���¶ȣ������¶��ɿ��ư�ɼ�
			g_adDataLSteam = (((RECV_STEAM_L_TEM_H8 & 0x00FF) << 8) + RECV_STEAM_L_TEM_L8);

//			g_potType	= RECV_POT_TYPE;								//�ڹ�״̬��ȡ
			g_zeroType	= RECV_ZERO_TYPE;								//����״̬��ȡ
			g_pumpType	= RECV_PUMP_WORK_FLAG;							//ˮ���Ƿ����³�ˮ��ȡ
			
//			g_potType	= STATE_YES;
			
			if(g_pumpType == STATE_YES)									//��ˮ������һ�γ�ˮ
			{
				if(s_getPumpWorkAdFlag == 0)							//δ��¼ˮ�ù�������ʱ
				{
					g_pumpCurrentAd = (((RECV_PUMP_CURRENT_H8 & 0x00FF) << 8) + RECV_PUMP_CURRENT_L8);	//��ȡ��ǰˮ�õ���ֵ
					
					s_getPumpWorkAdFlag = 1;							//���Ϊ�Ѽ�¼ˮ�ù�������
				}
			}
			else
			{
				s_getPumpWorkAdFlag = 0;
			}
			
//			if(s_powerDownUpFlag)										//�����������µ�
//			{
//				if(g_potType == STATE_YES)								//���¼���Ƿ��й���ֻ���һ��
//				{
//					g_potFlag = POT_OK;									//���Ϊ�й�
//				}
//				else
//				{
//					g_potFlag = POT_NO;									//���Ϊȱ��
//				}
//				
//				s_powerDownUpFlag = 0;									//�������µ��Ѵ���
//			}
		}
		else
		{
			/*��һ��NTC��ֵ��ʹ���²������Ҳ�����*/
			g_adDataHSteam 	= 0x0259;									//185��
			g_adDataLSteam 	= 0x002D;									//185��
			
			/*����ֵ��ʹ������*/
//			g_potType		= STATE_YES;
//			g_potFlag		= POT_OK;
			g_zeroType		= STATE_YES;
			
			s_powerDownUpFlag = 1;										//��ʾ��ʱ���������ϵ�
		}
	}
}