#include "header.h"

MEASURE_DATA		xdata		measureData;					//������ر���
CALIBRATION_DATA	xdata		calibrationData;				//ˮ�ñ궨��ر���

unsigned int  xdata				g_adDataHSteam;					//��¯������ADֵ
unsigned int  xdata				g_adDataLSteam;					//��¯������ADֵ

unsigned int  xdata 			g_pumpCurrentAd;				//ˮ�õ�������ADֵ
unsigned char xdata 			g_pumpDelayJudgeCnt;			//���ǽ�ˮ�ܳ��ȣ��ӳ���ȱˮ

unsigned int  xdata				g_lastPumpNormalWorkAd;			//�ϴμ�¼��ˮ��������ˮ����ֵ
unsigned char xdata				judgeErrValue;					//ˮ��ȱˮ���ж���ֵ

		 bit					g_reachStopHeatFlag;			//�Ƿ񵽴��ǿ�ص��־
		 bit					g_reachRegionTempFlag;			//�Ƿ񵽴�������¶ȱ�־����ˮ�õ�����ʼ�궨ʱ��¯�¶ȸ���ǿ�ص���ֱ���ָ��������¶��ٿ�ʼ�궨��

		 bit					g_firstAdGetGlag;				//�ɼ����״�ǻ��ADֵ��־

/*-----------------------------------------------------------------------------
Description:		��ʼ���������IO
					ǻ��NTC��AD�ڣ�P4.3��IO�ڣ�P4.4
					�Ǽ��ڣ�P5.0
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitMeasureIo(void)
{
	BANK0_SET;

	P4CR  &= (~(BIT3 | BIT4));						//AD�ں�IO�������벻���������ȼ�������
	P4PCR &= (~(BIT3 | BIT4));
	
	BANK1_SET;
	
	P5CR  &= ~BIT3;									//�Ǽ��IO�����������
	P5PCR |= BIT3;
	BANK0_SET;
}

/*-----------------------------------------------------------------------------
Description:		��ʼ������źű���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitMeasureVariable(void)
{
	unsigned char i;
	
	for(i=0;i<10;i++)
	{
		measureData.ntcAd_buff[i] = 0;
	}
	
	measureData.coreAdHValue = 0;
	measureData.coreAdLValue = 0;
	measureData.adcnt		 = 0;
	measureData.errCode		 = 0;
	
	calibrationData.smoothingCnt	 = 0;
	calibrationData.regionHeatCnt	 = 0;
	calibrationData.stopHeatCnt		 = 0;
	calibrationData.highTempTimeCnt  = 0;
	calibrationData.steamMinAd		 = 0x3FF;
	calibrationData.pumpWorkAdTemp	 = 0;
	for(i=0;i<5;i++)
	{
		calibrationData.pumpWorkAdBuff[i] = 0x3FF;
	}
	calibrationData.pumpNormalWorkAd = 0;
	
	g_adDataHSteam			= 0;
	g_adDataLSteam			= 0;

	g_pumpCurrentAd			= 0;
	g_pumpDelayJudgeCnt		= 0;
	
	g_lastPumpNormalWorkAd	= 0;
	judgeErrValue			= 0;
	
	g_reachStopHeatFlag		= 0;
	g_reachRegionTempFlag	= 0;
	
	g_firstAdGetGlag		= 0;
}

/*-----------------------------------------------------------------------------
Description:		��ʼ��AD�������¶ȼ�⣩
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitAd(void)
{
	unsigned int  PdAdc1 = 0,PdAdc2 = 0,PdAdc3 = 0;
	unsigned char PdCheckSum = 0,checkSumRead = 0;
	unsigned char PdCheckCode = 0;
	unsigned char PdSysType = 0,PdMenuNumber = 0,PdNowStepNum = 0,PdNowStepTechnology = 0,PdNowStepworkTemp = 0;
	unsigned char PdNowStepworkTime = 0,PdWorkTimeAll = 0,PdWorkOneMinCnt = 0;
	unsigned int  PdKeepWarmTime = 0,PdOrderTime = 0,PdHighSteamPowerCnt = 0;
	unsigned char i = 0,j = 0;
	
	bit PdTempReasonableFlag = 0;

	ADT    = 0x9f;									//ʱ������=1/24*24=1US  ����ʱ��=15*Tad=15US ת��ʱ��=14*1+15=19us��һ��ͨ����
	ADCON1 = 0x80;									//��ADCģ��
	ADCON2 = 0x00;
	SEQCON = 0x00;
	
	ADCH1  = 0x08;									//AN3��ģ�������
	ADCH2  = 0x00;
	
	SEQCHX = 0x03;									//ѡ��ͨ��3
	
	Delay1ms(20);									//�˿����������ӳ�
	
	PdAdc1 = AdScanValue();
	Delay1ms(1);									//��ʱ
	PdAdc2 = AdScanValue();
	Delay1ms(1);									//��ʱ
	PdAdc3 = AdScanValue();
	Delay1ms(1);									//��ʱ

	PdCheckSum = 0;
	
	PdCheckCode = SectorRead(0x0000,EEPROM_CRL);	//��ȡͷ��
	PdCheckSum += PdCheckCode;
	
	PdSysType	= SectorRead(0x0001,EEPROM_CRL);	//��ȡϵͳ״̬
	PdCheckSum += PdSysType;
	
	PdMenuNumber	= SectorRead(0x0002,EEPROM_CRL);//��ȡ�˵����
	PdCheckSum += PdMenuNumber;

	PdNowStepNum		= SectorRead(0x0003,EEPROM_CRL);	//��ȡ��ǰ���ղ�����
	PdCheckSum += PdNowStepNum;
	
	PdNowStepTechnology	= SectorRead(0x0004,EEPROM_CRL);	//��ȡ��ǰ����Ĺ���
	PdCheckSum += PdNowStepTechnology;
	
	PdNowStepworkTemp	= SectorRead(0x0005,EEPROM_CRL);	//��ȡ��ǰ�����¶�
	PdCheckSum += PdNowStepworkTemp;
	
	PdNowStepworkTime	= SectorRead(0x0006,EEPROM_CRL);	//��ȡ��ǰ����ʱ��
	PdCheckSum += PdNowStepworkTime;
	
	PdWorkTimeAll		= SectorRead(0x0007,EEPROM_CRL);	//��ȡ������ʱ��
	PdCheckSum += PdWorkTimeAll;

	i			= SectorRead(0x0008,EEPROM_CRL);
	PdCheckSum += i;
	j			= SectorRead(0x0009,EEPROM_CRL);
	PdCheckSum += j;
	PdHighSteamPowerCnt = ((unsigned int)(i) << 8) + (unsigned int)(j);		//��ȡ�����ĵ͹���ʱ��

	i			= SectorRead(0x000A,EEPROM_CRL);
	PdCheckSum += i;
	j			= SectorRead(0x000B,EEPROM_CRL);
	PdCheckSum += j;
	PdKeepWarmTime = ((unsigned int)(i) << 8) + (unsigned int)(j);			//��ȡ����ʱ��

	i			= SectorRead(0x000C,EEPROM_CRL);
	PdCheckSum += i;
	j			= SectorRead(0x000D,EEPROM_CRL);
	PdCheckSum += j;
	PdOrderTime = ((unsigned int)(i) << 8) + (unsigned int)(j);				//��ȡԤԼʱ��
	
	PdWorkOneMinCnt = SectorRead(0x000E,EEPROM_CRL);						//��ȡ���ӵļ���ֵ
	PdCheckSum += PdWorkOneMinCnt;
	
	checkSumRead	= SectorRead(0x000F,EEPROM_CRL);	//��ȡУ���
	
	/*�¶�����������ж�*/
 	if(PdNowStepTechnology == ONLY_BAKE)
	{
		if((PdNowStepworkTemp >= MenuDefaultValue_Table[MENU_1_NUM][ADJ_MIN_TEMP_NUM]) && (PdNowStepworkTemp <= MenuDefaultValue_Table[MENU_1_NUM][ADJ_MAX_TEMP_NUM]))
		{
			PdTempReasonableFlag = 1;
		}
		else
		{
			PdTempReasonableFlag = 0;
		}
	}
	else if(PdNowStepTechnology == BAKE_STEAM)
	{
		if((PdNowStepworkTemp >= MenuDefaultValue_Table[MENU_2_NUM][ADJ_MIN_TEMP_NUM]) && (PdNowStepworkTemp <= MenuDefaultValue_Table[MENU_2_NUM][ADJ_MAX_TEMP_NUM]))
		{
			PdTempReasonableFlag = 1;
		}
		else
		{
			PdTempReasonableFlag = 0;
		}
	}
	else if(PdNowStepTechnology == ONLY_STEAM)
	{
		if((PdNowStepworkTemp >= MenuDefaultValue_Table[MENU_3_NUM][ADJ_MIN_TEMP_NUM]) && (PdNowStepworkTemp <= MenuDefaultValue_Table[MENU_3_NUM][ADJ_MAX_TEMP_NUM]))
		{
			PdTempReasonableFlag = 1;
		}
		else
		{
			PdTempReasonableFlag = 0;
		}
	}
	else												//�޹���ʱĬ���¶Ⱥ�����ֹ����״̬ʱ�������Ĺ��գ�
	{
		PdTempReasonableFlag = 1;
	}
	
	/*��ȡ�ϴμ�¼��ˮ�ñ궨ֵ*/
	i = SectorRead(0x0010,EEPROM_CRL);
	j = SectorRead(0x0011,EEPROM_CRL);
	g_lastPumpNormalWorkAd = ((unsigned int)(i) << 8) + (unsigned int)(j);
	
	/*3��ad����ֵ��������ֵ��ͷ��Ϊ0x5A��У�����ȷ*/
	// if((PdAdc1 > PD_VALUE) && (PdAdc2 > PD_VALUE) && (PdAdc3 > PD_VALUE) && (PdCheckCode == 0x5A) && (PdCheckSum == checkSumRead))
	// {
	// 	/*������ԤԼ����������������״̬�µ���ģ����������*/
	// 	if((PdSysType == SysModeOrderWork) || (PdSysType == SysModeWork) || (PdSysType == SysModeWarm))
	// 	{
	// 		if(PdTempReasonableFlag)													//���¶��������
	// 		{
	// 			g_sysType			= PdSysType;
	// 			g_menuNumber		= PdMenuNumber;
	// 			g_nowStepNum		= PdNowStepNum;
	// 			g_nowStepTechnology	= PdNowStepTechnology;
	// 			g_nowStepworkTemp	= PdNowStepworkTemp;
	// 			g_nowStepworkTime	= PdNowStepworkTime;
	// 			g_workTimeAll		= PdWorkTimeAll;
	// 			g_highSteamPowerCnt	= PdHighSteamPowerCnt;
	// 			g_keepWarmTime		= PdKeepWarmTime;
	// 			g_orderTime			= PdOrderTime;
	// 			g_workOneMinCnt		= PdWorkOneMinCnt;
				
	// 			/*��һ��NTC��ֵ��ʹ���²������Ҳ�����*/
	// 			measureData.coreAdHValue = 0x014A;		//240��
	// 			measureData.coreAdLValue = 0x000F;		//240��
	// 			g_adDataHSteam			 = 0x0259;		//185��
	// 			g_adDataLSteam			 = 0x002D;		//185��
				
	// 			g_tempAdjEnFlag 	= MenuDefaultValue_Table[g_menuNumber][TEMP_ADJ_EN_NUM];	//�����¶ȿɵ���־
	// 			g_timeAdjEnFlag 	= MenuDefaultValue_Table[g_menuNumber][TIME_ADJ_EN_NUM];	//����ʱ��ɵ���־

	// 			/*����ֵ��ʹ������*/
	// 			g_LidFlag		= LID_CLOSE;
	// 			g_zeroType		= STATE_YES;
				
	// 			g_timeAdjStateFlag = 1;					//�ϵ�ָ�Ĭ����ʾʱ��
	// 			g_tempAdjStateFlag = 0;

	// 			g_pdWaitTime	= PD_WAIT_TIME;			//1S��ʹ�ö����ADֵ��֮����ʹ�ò�����ֵ
	// 		}
	// 	}
	// }
	
	SectorErase(0x0000,EEPROM_CRL);					//��������0����������̺������������ٴα��

	/*�������AD����GPIO����*/
	// ADCH1 &= ~BIT7;
	
	// /*������AD�ڲ������������*/
	// P3    |= BIT7;									//���������ĵ��ݳ��
	// P3CR  |= BIT7;
	// P3PCR &= ~BIT7;
	
	SEQCHX = 0x03;									//ѡ��ͨ��3
}

/*-----------------------------------------------------------------------------
Description:		NTC��ADֵɨ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
unsigned int AdScanValue(void)
{
	unsigned int adValue_H = 0;
	unsigned int adValue_L = 0;	

	ADCON1 |= 0x01;
	while(ADCON1 & 0x01);
	_nop_();
	_nop_();
			
	adValue_H = (unsigned int)ADDXH;
	adValue_L = (unsigned int)((ADDXL >> 6) & 0x03);
	
	adValue_H = (adValue_H << 2) + adValue_L;
	return(adValue_H);
}

/*-----------------------------------------------------------------------------
Description:		NTC��AD�˲���ֵ
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void NtcAdScan(void)
{
	unsigned char i;
	unsigned int  ntcMaxValue = 0;
	unsigned int  ntcMinValue = 0;
	unsigned int  ntcSumValue = 0;					//10��10λADֵ���ֵС��int��
	
	/*�����¶�NTC���*/
	measureData.adcnt++;
	
	if(measureData.adcnt <= 5)						//NTC��IO��Ϊ���벻����������������
	{
	}
	
	else if(measureData.adcnt <= 15)				//�������ݲɼ�
	{
		measureData.ntcAd_buff[measureData.adcnt - 6] = AdScanValue();	//ADɨ��ֵ��ֵ

		if(measureData.adcnt == 15)
		{
			ntcMaxValue = measureData.ntcAd_buff[0];
			ntcMinValue = measureData.ntcAd_buff[0];
			ntcSumValue = 0;
			for(i=0;i<10;i++)
			{
				if(measureData.ntcAd_buff[i] > ntcMaxValue)
				{
					ntcMaxValue = measureData.ntcAd_buff[i];
				}
				if(measureData.ntcAd_buff[i] < ntcMinValue)
				{
					ntcMinValue = measureData.ntcAd_buff[i];
				}
				
				ntcSumValue = ntcSumValue + measureData.ntcAd_buff[i];
			}
		
			/*ȥ�����С��ƽ��*/
			measureData.coreAdLValue = ntcSumValue - ntcMaxValue - ntcMinValue;
			measureData.coreAdLValue = measureData.coreAdLValue >> 3;

			/*�л���������*/
			CORE_NTC_IO_OUT;						//�������������
			P_CORE_NTC_IO = 1;
		}
	}
	
	else if(measureData.adcnt <= 20)				//NTC��IO��Ϊ���������������������
	{
	}
	
	else if(measureData.adcnt <= 30)				//�������ݲɼ�
	{
		measureData.ntcAd_buff[measureData.adcnt-21] = AdScanValue();	//ADɨ��ֵ��ֵ
		
		if(measureData.adcnt == 30)
		{
			ntcMaxValue = measureData.ntcAd_buff[0];
			ntcMinValue = measureData.ntcAd_buff[0];
			ntcSumValue = 0;
			for(i=0;i<10;i++)
			{
				if(measureData.ntcAd_buff[i] > ntcMaxValue)
				{
					ntcMaxValue = measureData.ntcAd_buff[i];
				}
				if(measureData.ntcAd_buff[i] < ntcMinValue)
				{
					ntcMinValue = measureData.ntcAd_buff[i];
				}
				
				ntcSumValue = ntcSumValue + measureData.ntcAd_buff[i];
			}
		
			/*ȥ�����С��ƽ��*/
			measureData.coreAdHValue = ntcSumValue - ntcMaxValue - ntcMinValue;
			measureData.coreAdHValue = measureData.coreAdHValue >> 3;
			
			/*�л���������*/
			CORE_NTC_IO_IN;							//����������NTC��IO�������벻������
		}
	}
	
	if(measureData.adcnt >= 30)						//ָ��ˢ��
	{
 		measureData.adcnt = 0;
		
		if(g_firstAdGetGlag == 0)
		{
			g_firstAdGetGlag = 1;
			
			g_corePreHeatFlag		= 0;			//����״�AD�ɼ����ݺ������ж�Ԥ��
			g_corePreHeatDelayFlag	= 0;
			g_corePreHeatFirstFlag	= 1;
		}
	}

}

/*-----------------------------------------------------------------------------
Description:		�����źż��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void LidScan(void)
{
	unsigned char i = 0;
	static unsigned char s_lidOpenCnt = 0;			//���Ǽ�����ֵ
	static unsigned char s_lidCloseCnt = 0;			//�ϸǼ�����ֵ
	static bit			 s_lastLidState = LID_OPEN;	//�ϴεĸ�״̬
	static bit			 s_lidReCloseFlag = 0;		//���ºϸǱ�־

	static bit  s_ledOpenTemp;		//LED��״̬�ݴ�
	static bit  s_ledDealFlag = 0;
	
	BANK1_SET;
	if(s_lastLidState != P_LID)							//������״̬�����ı䣬�����¼�����״̬
	{
		s_lidOpenCnt 	= 0;
		s_lidCloseCnt 	= 0;
	}

	if(P_LID == LID_OPEN)   
	{
		s_lastLidState = LID_OPEN;
		
		s_lidOpenCnt++;
		if(s_lidOpenCnt >= 15)						//15�������������ж�Ϊ����
		{
			s_lidOpenCnt = 15;
			g_LidFlag = LID_OPEN;

			if(s_ledDealFlag == 0)
			{
				s_ledOpenTemp = g_LedOpenFlag;			//LED��״̬�ݴ�
				s_ledDealFlag = 1;
			}
			
			g_LedOpenFlag = 0;		//����ʱLED�ƹر�
		}
	}
	else
	{
		s_lastLidState = LID_CLOSE;
		
		s_lidCloseCnt++;
		if(s_lidCloseCnt >= 15)						//15�������ϸ����ж�Ϊ�ϸ�
		{
			s_lidCloseCnt = 15;
			
			if(g_sysType != SysModePowerUp)			//��ֹ�����ϵ��ᱨ��
			{
				if(g_LidFlag == LID_OPEN)			//���ϴ�Ϊ����
				{
					s_lidReCloseFlag = 1;
					
					gU8_buzzerType = BUZ_JK_MUTE;	//����ֹͣ����
				}
			}
			
			g_LidFlag = LID_CLOSE;

			if(s_lidReCloseFlag)					//�������ºϸ�
			{
				if(g_enContinueErr)					//���пɼ�����������
				{
					if(g_waterYieldType == LackWater)
					{
						g_quickPumpingFlag	= 1;	//ȡ��ȱˮ��������ٳ�ˮ
					}

					g_enContinueErr		= NoError;	//�������������¼��
					g_waterYieldType	= NormalType;	//���Ϊ������ˮ
					g_pumpDelayJudgeCnt	= 0;
					
					if(g_pumpCalibrationFlag == 0)	//��δ�궨�ɹ�
					{
						InitPumpCalibrationData();	//��ʼ������ˮ�ñ궨����
					}
				}		
				s_lidReCloseFlag = 0;				//���ºϸ��Ѵ���

				s_ledDealFlag = 0;
				g_LedOpenFlag = s_ledOpenTemp;	//���¸�ֵԭ�ȵ�LED��״̬
			}
		}
	}

	BANK0_SET;
}

/*-----------------------------------------------------------------------------
Description:		ˮ�ñ궨��ز�����ʼ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitPumpCalibrationData(void)
{
	unsigned char i;
	
	calibrationData.smoothingCnt		= 0;					//�������б궨����
	calibrationData.regionHeatCnt		= 0;
	calibrationData.stopHeatCnt			= 0;
	calibrationData.highTempTimeCnt 	= 0;
	calibrationData.steamMinAd			= 0x3FF;
	calibrationData.pumpWorkAdTemp		= 0;
	for(i=0;i<5;i++)
	{
		calibrationData.pumpWorkAdBuff[i] = 0x3FF;
	}
	calibrationData.pumpNormalWorkAd 	= 0;
	
	g_reachStopHeatFlag		= 0;								//���Ϊδ����ǿ�ص�
	g_reachRegionTempFlag	= 0;								//���Ϊδ���������¶�
}

/*-----------------------------------------------------------------------------
Description:		��������ˮ�õ���ֵ�궨����
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void PumpCurrentCalibration(void)
{
	unsigned int	pumpWorkAdSum = 0;
	unsigned char	i = 0;

	static unsigned int xdata s_steamPreHeatCnt = 0;
	static bit		s_newPumpWorkFlag = 0;

	if(g_reachStopHeatFlag == 0)								//��¯�¶�δ�����ǿ�ص�
	{
		if(g_adDataHSteam > STEAM_PRE_HEAT_AD)					//��¯�¶ȵ���Ԥ���¶ȵ�ʱ
		{
			s_steamPreHeatCnt++;
			if(s_steamPreHeatCnt > 6000)						//����Ԥ����ͣ����1min��10msʱ��
			{
				g_sysType = SysModeError;						//�궨ʧ��ֱ��ת�뱨��״̬
				measureData.errCode = 0x06;
				gU8_buzzerType = BUZ_JK_POINT;					//�����ɻָ���������

				g_pumpCalibrationFlag	= 0;					//���Ϊ�궨δ���
			}
		
			InitPumpCalibrationData();							//��ʼ������ˮ�ñ궨����
		}
		else if(g_adDataHSteam > STEAM_MAX_TEMP_AD)				//��¯�¶ȴ���Ԥ�ȵ㣬С��ǿ�ƹرյ�
		{
			s_steamPreHeatCnt		= 0;						//Ԥ������������
			
			g_reachStopHeatFlag		= 0;						//���Ϊδ����ǿ�ص�
			
			/*С���Բ�E06������ĳ���20191027*/
			calibrationData.regionHeatCnt++;
			if(calibrationData.regionHeatCnt >= 100)			//ͣ���������¶�1S����
			{
				g_reachRegionTempFlag = 1;						//���Ϊ���������¶�
			}
		
			if(g_pumpType == STATE_YES)							//��ˮ����һ���³�ˮ����¼ˮ�õ�ADֵ
			{
				if(s_newPumpWorkFlag == 0)						//���³�ˮ��־������ֵ��δ��¼ʱ
				{
					calibrationData.pumpWorkAdTemp = g_pumpCurrentAd;
					
					/*������С��5��ADֵ*/
					if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[0])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[0]λ��������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdBuff[0];
						calibrationData.pumpWorkAdBuff[0] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[1])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[1]λ��[0]λ���䣬������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[2])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[2]λ��[0]��[1]λ���䣬������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[3])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[3]λ��[0]��[1]��[2]λ���䣬������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[4])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[4]λ��[0]��[1]��[2]��[3]λ����
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdTemp;
					}
					
					s_newPumpWorkFlag = 1;						//�ô�ˮ�ó�ˮ��ֵ�Ѽ�¼
				}
			}
			else
			{
				s_newPumpWorkFlag = 0;
			}
			
			if(calibrationData.regionHeatCnt >= REGION_JUDEG_TIME)			//����¯�¶ȳ�ʱ��ͣ����������������
			{
				if((calibrationData.pumpWorkAdBuff[4] - calibrationData.pumpWorkAdBuff[0]) < 15)	//������5����С�����������Сֵ��15������ʱ��Ϊ������Ч
				{
					pumpWorkAdSum = 0;
					for(i=1;i<5;i++)
					{
						pumpWorkAdSum = pumpWorkAdSum + calibrationData.pumpWorkAdBuff[i];			//ȥ��С����ֵ��4�������
					}
					calibrationData.pumpNormalWorkAd = pumpWorkAdSum >> 2;	//��¼��ǰˮ�õ���ֵ��4����С����ֵ�ľ�ֵ
				
					g_pumpCalibrationFlag = 1;					//���Ϊ�궨���
					
					g_pumpDelayJudgeCnt	= 0;
					g_steamPreHeatFlag 	= 0;					//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
				}
				else
				{
//					g_waterYieldType = LackWater;				//���Ϊȱˮ
					
					g_sysType = SysModeError;					//�궨ʧ��ֱ��ת�뱨��״̬
					measureData.errCode = 0x06;
					gU8_buzzerType = BUZ_JK_POINT;				//�����ɻָ���������
					
					InitPumpCalibrationData();					//��ʼ������ˮ�ñ궨����
					g_pumpCalibrationFlag	= 0;				//���Ϊ�궨δ���
				}
			}
		}
		else													//��¯�¶ȴ���ǿ�ƹرյ�
		{
			s_steamPreHeatCnt		= 0;						//Ԥ������������
		
			if(g_reachRegionTempFlag)							//�ڵ���������¶�֮ǰ����ʹ�¶ȵ���ǿ�ص�Ҳ������ǿ�ص�궨����
			{
				g_reachStopHeatFlag = 1;						//���Ϊ����ǿ�ص�
				
				calibrationData.smoothingCnt	= 0;			//��5����Сˮ�õ�������AD���������¸�ֵ
				calibrationData.regionHeatCnt	= 0;
				calibrationData.stopHeatCnt		= 0;
				calibrationData.highTempTimeCnt = 0;
				calibrationData.steamMinAd		= 0x3FF;
				calibrationData.pumpWorkAdTemp	= 0;
				calibrationData.pumpNormalWorkAd= 0;
			}
			else												//��һ��ʼ�궨ʱ��¯�¶Ⱦ͸���ǿ�ص��¶�
			{
				calibrationData.highTempTimeCnt++;
				if(calibrationData.highTempTimeCnt >= HIGH_TEMP_TIME_MAX)		//�¶���ǿ�ص�����һ��ʱ����δ���£����ж�Ϊ�궨ʧ��
				{
//					g_waterYieldType = LackWater;				//���Ϊȱˮ
					
					g_sysType = SysModeError;					//�궨ʧ��ֱ��ת�뱨��״̬
					measureData.errCode = 0x06;
					gU8_buzzerType = BUZ_JK_POINT;				//�����ɻָ���������
					
					InitPumpCalibrationData();					//��ʼ������ˮ�ñ궨����
					g_pumpCalibrationFlag	= 0;				//���Ϊ�궨δ���
				}
			}
		}
	}
	else														//��¯�¶ȵ����ǿ�ص㣨�ǳ�ʼ�궨����ǿ�ص㣩
	{
		calibrationData.stopHeatCnt++;
		if(calibrationData.stopHeatCnt <= STOP_HEAT_JUDGE_TIME)	//���ж�ʱ����
		{
			if(g_adDataHSteam < calibrationData.steamMinAd)		//�ж�ʱ����һֱˢ��СADֵ
			{
				calibrationData.steamMinAd = g_adDataHSteam;
			}
			
			if(g_pumpType == STATE_YES)							//��ˮ����һ���³�ˮ����¼ˮ�õ�ADֵ
			{
				if(s_newPumpWorkFlag == 0)						//���³�ˮ��־������ֵ��δ��¼ʱ
				{
					calibrationData.pumpWorkAdTemp = g_pumpCurrentAd;
					
					/*������С��5��ADֵ*/
					if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[0])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[0]λ��������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdBuff[0];
						calibrationData.pumpWorkAdBuff[0] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[1])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[1]λ��[0]λ���䣬������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[2])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[2]λ��[0]��[1]λ���䣬������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[3])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[3]λ��[0]��[1]��[2]λ���䣬������λ����һλ
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[4])
					{	//Ĭ������[0]<[1]<[2]<[3]<[4]������С��Сʱ����ǰADռ�ݵ�[4]λ��[0]��[1]��[2]��[3]λ����
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdTemp;
					}
					
					s_newPumpWorkFlag = 1;						//�ô�ˮ�ó�ˮ��ֵ�Ѽ�¼
				}
			}
			else
			{
				s_newPumpWorkFlag = 0;
			}
			
			if(g_adDataHSteam > calibrationData.steamMinAd)		//����ǰ��¯�¶ȵ��ڼ�⵽����СADֵ��˵����¯�ѽ��£�
			{
				if((g_adDataHSteam - calibrationData.steamMinAd) > 150)			//�ж�ʱ���ڣ����´ﵽһ��ֵ�����ж�Ϊ������ˮ����¼��ǰˮ�õ���ֵ����Ϊ֮���ȱˮ�ж���׼
				{
					calibrationData.smoothingCnt++;
					if(calibrationData.smoothingCnt >= 30)		//��������ж��¶��½�ֵ�ϴ�10msʱ����g_adDataHSteamֵ100msˢ��һ�Σ�
					{
						if((calibrationData.pumpWorkAdBuff[4] - calibrationData.pumpWorkAdBuff[0]) < 15)	//������5����С�����������Сֵ��15������ʱ��Ϊ������Ч
						{
							pumpWorkAdSum = 0;
							for(i=1;i<5;i++)
							{
								pumpWorkAdSum = pumpWorkAdSum + calibrationData.pumpWorkAdBuff[i];			//ȥ��С����ֵ��4�������
							}
							calibrationData.pumpNormalWorkAd = pumpWorkAdSum >> 2;	//��¼��ǰˮ�õ���ֵ��4����С����ֵ�ľ�ֵ
						
							g_pumpCalibrationFlag = 1;			//���Ϊ�궨���
							
							g_pumpDelayJudgeCnt	= 0;
							g_steamPreHeatFlag 	= 0;			//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
						}
						else
						{
//							g_waterYieldType = LackWater;		//���Ϊȱˮ
							
							g_sysType = SysModeError;			//�궨ʧ��ֱ��ת�뱨��״̬
							measureData.errCode = 0x06;
							gU8_buzzerType = BUZ_JK_POINT;		//�����ɻָ���������
			
							InitPumpCalibrationData();			//��ʼ������ˮ�ñ궨����
							g_pumpCalibrationFlag	= 0;		//���Ϊ�궨δ���
						}
					}
				}
				else
				{
					calibrationData.smoothingCnt = 0;
				}
			}
		}
		else													//�ж�ʱ����������ֵδ�ﵽָ��ֵ�����ж�Ϊȱˮ
		{
//			g_waterYieldType = LackWater;						//���Ϊȱˮ
			
			g_sysType = SysModeError;							//�궨ʧ��ֱ��ת�뱨��״̬
			measureData.errCode = 0x06;
			gU8_buzzerType = BUZ_JK_POINT;						//�����ɻָ���������
			
			InitPumpCalibrationData();							//��ʼ������ˮ�ñ궨����
			g_pumpCalibrationFlag	= 0;						//���Ϊ�궨δ���
		}
	}
	
	if(g_pumpCalibrationFlag)									//���궨���
	{
		/*����¼�ı궨ֵ��С���������Ϊ�궨�������±궨*/
		if((calibrationData.pumpNormalWorkAd <= EN_ERROR_VALUE) || (calibrationData.pumpNormalWorkAd >= (0x03FF - EN_ERROR_VALUE)))
		{
//			g_waterYieldType = LackWater;						//���Ϊȱˮ
			
			g_sysType = SysModeError;							//�궨ʧ��ֱ��ת�뱨��״̬
			measureData.errCode = 0x06;
			gU8_buzzerType = BUZ_JK_POINT;						//�����ɻָ���������
			
			InitPumpCalibrationData();							//��ʼ������ˮ�ñ궨����
			g_pumpCalibrationFlag	= 0;						//���Ϊ�궨δ���
		}
		else
		{
			judgeErrValue = EN_ERROR_VALUE;
			
			/*���ϴε�ˮ�ó�ˮ����ֵ���������䣬�Աȵ�ǰ�궨��ֵ���ϴα궨��ֵ������ǰ�궨ֵ�����ϴα궨ֵһ����Χ�����Сȱˮ�ж���ֵ*/
			if((g_lastPumpNormalWorkAd > EN_ERROR_VALUE) && (g_lastPumpNormalWorkAd < (0x03FF - EN_ERROR_VALUE)))
			{
				if(calibrationData.pumpNormalWorkAd > g_lastPumpNormalWorkAd)
				{
					if(((calibrationData.pumpNormalWorkAd - g_lastPumpNormalWorkAd) > 25))
					{
						judgeErrValue = EN_LOW_ERROR_VALUE;
					}
				}
			}
		}
	}			
}

/*-----------------------------------------------------------------------------
Description:		ȱˮ�жϣ���ˮ�õ����жϣ�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void LackWaterScan(void)
{
	if(g_pumpCurrentAd > (calibrationData.pumpNormalWorkAd + judgeErrValue))				//��ˮ�õ���ADֵ���ڱ궨ֵһ����Χ����ȱˮ����
	{
		g_waterYieldType = MabyLackWater;						//���Ϊ����ȱˮ״̬�������ٳ�һ��
		
		g_pumpDelayJudgeCnt++;
		if(g_pumpDelayJudgeCnt >= 10)							//����10���ж�Ϊ�ճ飬����Ϊȱˮ
		{
			g_pumpDelayJudgeCnt = 0;
			
			g_waterYieldType = LackWater;						//���Ϊȱˮ
		}
	}
	else
	{
		g_waterYieldType = NormalType;							//���Ϊ������ˮ
		
		g_pumpDelayJudgeCnt	= 0;
	}
}

/*-----------------------------------------------------------------------------
Description:		�������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void ErrorCheck(void)
{
	static unsigned char xdata s_zeroMissCnt = 0;

	if((g_sysType != SysModeError) && (g_sysType != SysModePowerUp))
	{
		if(g_firstRecvFlag == 0)								//��δ���յ��״����ݣ���ֻ����ͨѶ���
		{
			if(UartData.uartRecvOverCnt == 301)					//����3Sδ���յ���������֡������
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//������������
			
				g_sysType = SysModeError;
				measureData.errCode = 0x07;
			}
			else
			{
				//
			}
		}
		else
		{
			if(measureData.coreAdLValue >= NTC_OPEN_DATA)		//ǻ�����������-40���ж�Ϊ��·����
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//������������
			
				g_sysType = SysModeError;
				measureData.errCode = 0x01;
			}
			else if(measureData.coreAdHValue <= NTC_SHORT_DATA)	//ǻ�����������270���ж�Ϊ��·����
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//������������
			
				g_sysType = SysModeError;
				measureData.errCode = 0x02;
			}
			else if(UartData.uartRecvOverCnt == 301)			//����3Sδ���յ���������֡������
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//������������
			
				g_sysType = SysModeError;
				measureData.errCode = 0x07;
			}
			else
			{
				if((g_sysType == SysModeOrderWork) || (g_sysType == SysModeWork) || (g_sysType == SysModeWarm))//������ԤԼ����������״̬ʱ
				{
					if((g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						if(g_adDataLSteam >= NTC_OPEN_DATA)			//��¯����������-40���ж�Ϊ��·����
						{
							gU8_buzzerType = BUZ_JK_ALARM;			//������������
						
							g_sysType = SysModeError;
							measureData.errCode = 0x03;
						}
						else if(g_adDataHSteam <= NTC_SHORT_DATA)	//��¯����������270���ж�Ϊ��·����
						{
							gU8_buzzerType = BUZ_JK_ALARM;			//������������
						
							g_sysType = SysModeError;
							measureData.errCode = 0x04;
						}
						else
						{
							if(g_zeroType == STATE_NO)				//�������쳣ʱ
							{
								s_zeroMissCnt++;
								if(s_zeroMissCnt >= 200)			//��������ʱ������2Sδ��⵽�����򱨾�
								{
									s_zeroMissCnt = 201;

									gU8_buzzerType = BUZ_JK_ALARM;	//������������
								
									g_sysType = SysModeError;
									measureData.errCode = 0x05;
								}
							}
							else
							{
								s_zeroMissCnt = 0;
							}
						}
					}
				}
			}
		}
	}

	/*�����Լ�״̬�£���¯NTC�쳣ֱ�ӱ���*/
	if(g_sysType == SysModeMachineCheck)								//�����Լ�״̬�£���¯/�����쳣ֱ�ӱ���
	{
		if(g_adDataLSteam >= NTC_OPEN_DATA)								//��¯����������-40���ж�Ϊ��·����
		{
			gU8_buzzerType = BUZ_JK_ALARM;								//������������
		
			g_sysType = SysModeError;
			measureData.errCode = 0x03;
		}
		else if(g_adDataHSteam <= NTC_SHORT_DATA)						//��¯����������270���ж�Ϊ��·����
		{
			gU8_buzzerType = BUZ_JK_ALARM;								//������������
		
			g_sysType = SysModeError;
			measureData.errCode = 0x04;
		}
		else
		{
			if(g_zeroType == STATE_NO)									//�������쳣ʱ
			{
				s_zeroMissCnt++;
				if(s_zeroMissCnt > 200)									//��������ʱ������2Sδ��⵽�����򱨾�
				{
					gU8_buzzerType = BUZ_JK_ALARM;						//������������
				
					g_sysType = SysModeError;
					measureData.errCode = 0x05;
				}
			}
			else
			{
				s_zeroMissCnt = 0;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		���ư�ļ���ź�ɨ�裨�����¶Ⱥ͸Ǽ�⣩
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void MeasureDataScan(void)
{
	unsigned char i = 0;
	static bit s_waterScanDealFlag = 0;					//ȱˮ��⴦���־

	if(g_pdWaitTime == 0)								//���޵����ӳ�ʱ
	{
		LidScan();										//�Ǽ��
//		g_LidFlag = LID_CLOSE;
		
		if(SEQCHX == 0x03)								//��ͨ��ѡ�����NTC���ͨ��
		{
			NtcAdScan();								//�����¶�NTCɨ��
		}
		
		if(g_pumpCalibrationFlag)						//��ˮ�õ����ѱ궨���
		{
			if(g_pumpType == STATE_YES)					//��ˮ������һ�γ�ˮ
			{
				if(s_waterScanDealFlag == 0)			//�ô�ȱˮ���δ����
				{
					LackWaterScan();					//ȱˮ���
					
					s_waterScanDealFlag = 1;			//�ô�ȱˮ����Ѵ���
				}
			}
			else
			{
				s_waterScanDealFlag = 0;				//���Ϊȱˮ���δ����
			}
		}
		else											//δ�궨ʱ��������������б궨
		{
			if(g_sysType == SysModeWork)
			{
				if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))
				{
					/*�޿ɻָ��������ϸǡ���ȱˮ��*/
					if((g_LidFlag == LID_CLOSE)  && (g_waterYieldType == NormalType))
					{
						PumpCurrentCalibration();
					}
					else
					{
						InitPumpCalibrationData();		//��ʼ������ˮ�ñ궨����
					}
				}
				else									//Ϊ�궨ǰ��������״̬�Ǵ������գ������ñ궨����
				{
					InitPumpCalibrationData();			//��ʼ������ˮ�ñ궨����
				}
			}
			else										//δ�궨ǰ�����ǹ���״̬�����ñ궨��ز���
			{
				InitPumpCalibrationData();				//��ʼ������ˮ�ñ궨����
			}
		}
	}
	else
	{
		/*��һ��NTC��ֵ��ʹ���²������Ҳ�����*/
		measureData.coreAdHValue = 0x014A;				//240��
		measureData.coreAdLValue = 0x000F;				//240��
		
		/*����ֵ��ʹ������*/
		g_LidFlag = LID_CLOSE;
	}
}