#include "header.h"

/********��Ӧ������оƬ�Ĵ洢��ӳ��****************/
unsigned char xdata		TK01L 	_at_	0x500;
unsigned char xdata		TK01H 	_at_	0x501; 
unsigned char xdata		TK02L 	_at_	0x502; 
unsigned char xdata		TK02H 	_at_	0x503; 
unsigned char xdata		TK03L 	_at_	0x504;
unsigned char xdata		TK03H 	_at_	0x505; 
unsigned char xdata		TK04L 	_at_	0x506; 
unsigned char xdata		TK04H 	_at_	0x507; 
unsigned char xdata		TK05L 	_at_	0x508; 
unsigned char xdata		TK05H 	_at_	0x509; 
unsigned char xdata		TK06L 	_at_	0x50A;
unsigned char xdata		TK06H 	_at_	0x50B; 
unsigned char xdata		TK07L 	_at_	0x50C;
unsigned char xdata		TK07H 	_at_	0x50D; 
unsigned char xdata		TK08L 	_at_	0x50E;
unsigned char xdata		TK08H 	_at_	0x50F; 
unsigned char xdata		TK09L 	_at_	0x510;
unsigned char xdata		TK09H 	_at_	0x511; 
unsigned char xdata		TK10L 	_at_	0x512;
unsigned char xdata		TK10H 	_at_	0x513; 
unsigned char xdata		TK11L 	_at_	0x514;
unsigned char xdata		TK11H 	_at_	0x515; 
unsigned char xdata		TK12L 	_at_	0x516;
unsigned char xdata		TK12H 	_at_	0x517; 
unsigned char xdata		TK13L 	_at_	0x518; 
unsigned char xdata		TK13H 	_at_	0x519; 
unsigned char xdata		TK14L 	_at_	0x51A; 
unsigned char xdata		TK14H 	_at_	0x51B; 
unsigned char xdata		TK15L 	_at_	0x51C;
unsigned char xdata		TK15H 	_at_	0x51D; 
unsigned char xdata		TK16L 	_at_	0x51E; 
unsigned char xdata		TK16H 	_at_	0x51F; 
unsigned char xdata		TK17L 	_at_	0x520;
unsigned char xdata		TK17H 	_at_	0x521; 
unsigned char xdata		TK18L 	_at_	0x522;
unsigned char xdata		TK18H 	_at_	0x523; 
unsigned char xdata		TK19L 	_at_	0x524; 
unsigned char xdata		TK19H 	_at_	0x525; 
unsigned char xdata		TK20L 	_at_	0x526;
unsigned char xdata		TK20H 	_at_	0x527; 
unsigned char xdata		TK21L 	_at_	0x528; 
unsigned char xdata		TK21H 	_at_	0x529; 
unsigned char xdata		TK22L 	_at_	0x52A; 
unsigned char xdata		TK22H 	_at_	0x52B; 
unsigned char xdata		TK23L 	_at_	0x52C;
unsigned char xdata		TK23H 	_at_	0x52D; 
unsigned char xdata		TK24L 	_at_	0x52E;
unsigned char xdata		TK24H 	_at_	0x52F;

unsigned char xdata		t2msCnt = 0;			//2ms����ֵ
unsigned int  xdata		t500msCnt = 0;			//500ms����ֵ

		 bit			g_flg2ms = 0;			//2ms��ʱ��־λ
		 bit			g_flg500ms = 0;			//500ms��ʱ��־λ
		 
		 bit			flgPwoeroff = 0;		//�����־λ
unsigned char xdata		testCnt = 0;			//���β������ڼ���
unsigned char xdata		highCnt = 0;			//�ߵ�ƽ����

/*-----------------------------------------------------------------------------
Description:		��ʱ��5�ж��ӳ���125us����һ���жϣ�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void IntTimer5(void) interrupt 3
{
	unsigned char checkSum = 0;
	unsigned char i =0,j =0;

	_push_(INSCON);								//ѹջ
    BANK1_SET;	
	TF5 = 0;   									//���Timer5�ж������־
	BANK0_SET;
	
	if(flgPwoeroff == 0)						//�޼�⵽����ʱ������ִ�г���
	{
		t2msCnt++;								//2ms��ʱ
		if(t2msCnt >= T2MS_OVER_CNT)
		{
			t2msCnt = 0;
			g_flg2ms = 1;						//2msʱ���־
		}
		
		t500msCnt++;
		if(t500msCnt >= T500MS_OVER_CNT)
		{
			t500msCnt = 0;
			g_flg500ms = 1;						//500msʱ���־
		}
		
		BuzzerDrive(); 							//125msִ��һ��	 4K������ ����������125��s ����
	}
	else										//�м�⵽����
	{
		testCnt++;
		if((LPDCON & BIT6) == BIT6)				//LPD��־λΪ�ߣ��ߵ�ƽ��1
		{
			highCnt++;
		}
		
		if(testCnt == 10)						//��������10*0.25 = 2.5ms
		{
			if(highCnt > 8)						//�⵽����ߵ�ƽ���ж�Ϊ�����
			{
				EA = 0;							//�����ж�
				InitWdt();						//�幷
				
				P4CR &= ~BIT2;					//������������벻������
				P4PCR &= ~BIT2;
				
				/*�ر��������*/
				P_COM1 = 1;
				P_COM2 = 1;
				P_COM3 = 1;
				P_COM4 = 1;
				P_SEG_A = 0;
				P_SEG_B = 0;
				P_SEG_C = 0;
				P_SEG_D = 0;
				P_SEG_E = 0;
				P_SEG_F = 0;
				P_SEG_G = 0;
				P_SEG_H = 0;
				
				P_TXD = 0;
				P_RXD = 0;

				checkSum = 0;
				
				if(g_pumpCalibrationFlag)
				{
					i = (unsigned char)((calibrationData.pumpNormalWorkAd & 0xff00) >> 8);
					j = (unsigned char)(calibrationData.pumpNormalWorkAd & 0x00ff);
					SectorWrite(0x0010,i,EEPROM_CRL);					//��¼��ǰ��ˮ�õ����궨ֵ
					SectorWrite(0x0011,j,EEPROM_CRL);
				}
				
				/*ԤԼ����������������״̬�±�������*/
				if((g_sysType == SysModeOrderWork) || (g_sysType == SysModeWork) || (g_sysType == SysModeWarm))
				{
					SectorWrite(0x0000,0x5A,EEPROM_CRL);				//����ͷ��
					checkSum += 0x5A;
					
					SectorWrite(0x0001,g_sysType,EEPROM_CRL);			//����ϵͳ��ǰ״̬
					checkSum += g_sysType;
					
					SectorWrite(0x0002,g_menuNumber,EEPROM_CRL);		//���浱ǰ�˵����
					checkSum += g_menuNumber;

					SectorWrite(0x0003,g_nowStepNum,EEPROM_CRL);		//���浱ǰ���ղ�����
					checkSum += g_nowStepNum;
					
					SectorWrite(0x0004,g_nowStepTechnology,EEPROM_CRL);	//���浱ǰ����Ĺ���
					checkSum += g_nowStepTechnology;
					
					SectorWrite(0x0005,g_nowStepworkTemp,EEPROM_CRL);	//���浱ǰ�����¶�
					checkSum += g_nowStepworkTemp;
				
					SectorWrite(0x0006,g_nowStepworkTime,EEPROM_CRL);	//���浱ǰ����ʱ��
					checkSum += g_nowStepworkTime;
					
					SectorWrite(0x0007,g_workTimeAll,EEPROM_CRL);		//���湤����ʱ��
					checkSum += g_workTimeAll;

					i = (unsigned char)((g_highSteamPowerCnt & 0xff00) >> 8);
					j = (unsigned char)(g_highSteamPowerCnt & 0x00ff);
					SectorWrite(0x0008,i,EEPROM_CRL);					//���浱ǰ�����߹���ʱ��ĸ߰�λ
					checkSum += i;
					SectorWrite(0x0009,j,EEPROM_CRL);					//���浱ǰ�����߹���ʱ��ĵͰ�λ
					checkSum += j;

					i = (unsigned char)((g_keepWarmTime & 0xff00) >> 8);
					j = (unsigned char)(g_keepWarmTime & 0x00ff);
					SectorWrite(0x000A,i,EEPROM_CRL);					//���浱ǰ����ʱ��ĸ߰�λ
					checkSum += i;
					SectorWrite(0x000B,j,EEPROM_CRL);					//���浱ǰ����ʱ��ĵͰ�λ
					checkSum += j;
					
					i = (unsigned char)((g_orderTime & 0xff00) >> 8);
					j = (unsigned char)(g_orderTime & 0x00ff);
					SectorWrite(0x000C,i,EEPROM_CRL);					//���浱ǰԤԼʱ��ĸ߰�λ
					checkSum += i;
					SectorWrite(0x000D,j,EEPROM_CRL);					//���浱ǰԤԼʱ��ĵͰ�λ
					checkSum += j;
					
					SectorWrite(0x000E,g_workOneMinCnt,EEPROM_CRL);		//���浱ǰ���ӵļ���ֵ
					checkSum += g_workOneMinCnt;
					
					SectorWrite(0x000F,checkSum,EEPROM_CRL);			//����У���
					
					while(1)											//��ѭ�������ٺĵ�
					{
						CORE_NTC_IO_OUT;								//IO����������ĵ�
					}
				}
			}
			
			testCnt = 0;
			highCnt = 0;
			flgPwoeroff = 0;					//������磬��������־
			
			IEN1 |= BIT6;						//���¿���LPD�ж�
		}
	}

	_pop_(INSCON);  							//��ջ
}

/*-----------------------------------------------------------------------------
Description:		LPD�ж��ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void IntLPD(void) interrupt 13
{
	_push_(INSCON);
    BANK0_SET;

	if((LPDCON & BIT4) == BIT4)					//���жϹ���
	{
		LPDCON &= ~BIT4;						//����ж������־
		
		IEN1 &= ~BIT6;							//ȡ��LPD�ж�

		flgPwoeroff = 1;						//�����־��1
		
		/*���жϺ�����˲�����ͨ����ʱ������ƽ���ж��Ƿ��������*/
		testCnt = 0;							//�������ڿ�ʼ����
		highCnt = 0;							//�ߵ�ƽ��������
	}
	else
	{
		LPDCON &= ~BIT4;						//����ж������־
	}

	_pop_(INSCON);  							//��ջ
}

/*-----------------------------------------------------------------------------
Description:		�����ж��ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void IntUart1(void) interrupt 15
{
	unsigned char uartRecvTmp;
	unsigned char i;
	
	_push_(INSCON);								//ѹջ
    BANK1_SET;
	
	if(RI1)
	{
		RI1 = 0;								//��������жϱ�־
		
		UartData.uartRecvErrCnt = 0;			//���ճ�ʱ ����		
		uartRecvTmp = SBUF1;
		if(UartData.uartRevcCnt == 0)
		{
			if(uartRecvTmp == 0x55)				//ͷ����ȷ
			{
				UartData.RecvData_Buff[0] = uartRecvTmp;
				UartData.uartRevcCnt = 1;
			}
		}
		else if(UartData.uartRevcCnt <= RECV_MAX)
		{
			UartData.RecvData_Buff[UartData.uartRevcCnt] = uartRecvTmp;
			UartData.uartRevcCnt++;
			if(UartData.uartRevcCnt >= RECV_MAX)//һ֡���ݽ������
			{
				/*�˴�������TmpDataGive()�����ᱨ����ԭ������ִ��ʱ�����ִ�иú����������ж���ִ�иó���,��������*/
				for(i=0;i<RECV_MAX;i++)
				{
					UartData.RecvDataTmp_Buff[i] = UartData.RecvData_Buff[i];	//�����յ�ֵ���뻺����
				}
				
				UartData.uartRevcCnt = 0;
				g_uartRecvType = recvOver;		//���ڽ���״̬Ϊ���״̬
			}
		}
	}
	else if(TI1)								//�����ж�
	{
		TI1 = 0;								//���㷢���жϱ�־	
		
		UartData.uartSendCnt++;
		if(UartData.uartSendCnt >= SEND_MAX)
		{
			UartData.uartSendCnt = SEND_MAX;
			g_uartSendType = sendOver;			//һ֡���ݷ������
		}
		else
		{
			SBUF1 = UartData.SendData_Buff[UartData.uartSendCnt];
		}
	}
	
	BANK0_SET;
	_pop_(INSCON);  							//��ջ
}

/*-----------------------------------------------------------------------------
Description:		���������ж��ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void IntTouchKey(void)	interrupt 1
{
	_push_(INSCON);
    BANK0_SET;

	if(TKF0 & BIT4)
	{
		TKF0 = 0;
		g_scanDataTk[0] = ((unsigned int)(TK01H << 8)) | ((unsigned int)TK01L);
		g_scanDataTk[1] = ((unsigned int)(TK02H << 8)) | ((unsigned int)TK02L);
		g_scanDataTk[2] = ((unsigned int)(TK03H << 8)) | ((unsigned int)TK03L);
		g_scanDataTk[3] = ((unsigned int)(TK04H << 8)) | ((unsigned int)TK04L);
		g_scanDataTk[4] = ((unsigned int)(TK05H << 8)) | ((unsigned int)TK05L);
		g_scanDataTk[5] = ((unsigned int)(TK06H << 8)) | ((unsigned int)TK06L);
		g_scanDataTk[6] = ((unsigned int)(TK07H << 8)) | ((unsigned int)TK07L);
		
		g_keyScanEndFlag = 1;
	}
	else
	{
		if(TKF0 & BIT6)           	//���������Ӧ�ü�С�Ŵ�ϵ���Ĵ���
		{
			TKF0 = 0;
            g_keyScanEndFlag = 0;
			Delay1us(10);			//��ʱ10us
			TKCON1 |= BIT5;    		//��������ɨ��
		}
		else if(TKF0&BIT5)          //�����źŴ�����ʱ10us����������ɨ��
		{
			TKF0 = 0;
            g_keyScanEndFlag = 0;
			Delay1us(10);			//��ʱ10us
			TKCON1 |= BIT5;    		//��������ɨ��
		}
		else if(TKF0&BIT3)      	//��������ɨ�������Ӧ��СC1ֵ
		{
			TKF0 = 0;
            g_keyScanEndFlag = 0;
			Delay1us(10);			//��ʱ10us
			TKCON1 |= BIT5;    		//��������ɨ��
		}
	}
	
	_pop_(INSCON);  				//��ջ
}