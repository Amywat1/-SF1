#include "header.h"

MEASURE_DATA		xdata		measureData;					//测量相关变量
CALIBRATION_DATA	xdata		calibrationData;				//水泵标定相关变量

unsigned int  xdata				g_adDataHSteam;					//锅炉高温区AD值
unsigned int  xdata				g_adDataLSteam;					//锅炉低温区AD值

unsigned int  xdata 			g_pumpCurrentAd;				//水泵电流采样AD值
unsigned char xdata 			g_pumpDelayJudgeCnt;			//考虑进水管长度，延迟判缺水

unsigned int  xdata				g_lastPumpNormalWorkAd;			//上次记录的水泵正常抽水电流值
unsigned char xdata				judgeErrValue;					//水泵缺水的判定阈值

		 bit					g_reachStopHeatFlag;			//是否到达过强关点标志
		 bit					g_reachRegionTempFlag;			//是否到达过区间温度标志（若水泵电流开始标定时锅炉温度高于强关点则直到恢复到区间温度再开始标定）

		 bit					g_firstAdGetGlag;				//采集到首次腔体AD值标志

/*-----------------------------------------------------------------------------
Description:		初始化检测引脚IO
					腔体NTC的AD口：P4.3；IO口：P4.4
					盖检测口：P5.0
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitMeasureIo(void)
{
	BANK0_SET;

	P4CR  &= (~(BIT3 | BIT4));						//AD口和IO口做输入不带上拉，先检测低温区
	P4PCR &= (~(BIT3 | BIT4));
	
	BANK1_SET;
	
	P5CR  &= ~BIT3;									//盖检测IO做输入带上拉
	P5PCR |= BIT3;
	BANK0_SET;
}

/*-----------------------------------------------------------------------------
Description:		初始化检测信号变量
Input:				void
Return:				void
History:			无
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
Description:		初始化AD（中心温度检测）
Input:				void
Return:				void
History:			无
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

	ADT    = 0x9f;									//时钟周期=1/24*24=1US  采样时间=15*Tad=15US 转换时间=14*1+15=19us（一个通道）
	ADCON1 = 0x80;									//打开ADC模块
	ADCON2 = 0x00;
	SEQCON = 0x00;
	
	ADCH1  = 0x08;									//AN3做模拟输入口
	ADCH2  = 0x00;
	
	SEQCHX = 0x03;									//选择通道3
	
	Delay1ms(20);									//端口配置完做延迟
	
	PdAdc1 = AdScanValue();
	Delay1ms(1);									//延时
	PdAdc2 = AdScanValue();
	Delay1ms(1);									//延时
	PdAdc3 = AdScanValue();
	Delay1ms(1);									//延时

	PdCheckSum = 0;
	
	PdCheckCode = SectorRead(0x0000,EEPROM_CRL);	//读取头码
	PdCheckSum += PdCheckCode;
	
	PdSysType	= SectorRead(0x0001,EEPROM_CRL);	//读取系统状态
	PdCheckSum += PdSysType;
	
	PdMenuNumber	= SectorRead(0x0002,EEPROM_CRL);//读取菜单编号
	PdCheckSum += PdMenuNumber;

	PdNowStepNum		= SectorRead(0x0003,EEPROM_CRL);	//读取当前工艺步骤数
	PdCheckSum += PdNowStepNum;
	
	PdNowStepTechnology	= SectorRead(0x0004,EEPROM_CRL);	//读取当前步骤的工艺
	PdCheckSum += PdNowStepTechnology;
	
	PdNowStepworkTemp	= SectorRead(0x0005,EEPROM_CRL);	//读取当前步骤温度
	PdCheckSum += PdNowStepworkTemp;
	
	PdNowStepworkTime	= SectorRead(0x0006,EEPROM_CRL);	//读取当前步骤时间
	PdCheckSum += PdNowStepworkTime;
	
	PdWorkTimeAll		= SectorRead(0x0007,EEPROM_CRL);	//读取工作总时间
	PdCheckSum += PdWorkTimeAll;

	i			= SectorRead(0x0008,EEPROM_CRL);
	PdCheckSum += i;
	j			= SectorRead(0x0009,EEPROM_CRL);
	PdCheckSum += j;
	PdHighSteamPowerCnt = ((unsigned int)(i) << 8) + (unsigned int)(j);		//读取纯蒸的低功率时间

	i			= SectorRead(0x000A,EEPROM_CRL);
	PdCheckSum += i;
	j			= SectorRead(0x000B,EEPROM_CRL);
	PdCheckSum += j;
	PdKeepWarmTime = ((unsigned int)(i) << 8) + (unsigned int)(j);			//读取保温时间

	i			= SectorRead(0x000C,EEPROM_CRL);
	PdCheckSum += i;
	j			= SectorRead(0x000D,EEPROM_CRL);
	PdCheckSum += j;
	PdOrderTime = ((unsigned int)(i) << 8) + (unsigned int)(j);				//读取预约时间
	
	PdWorkOneMinCnt = SectorRead(0x000E,EEPROM_CRL);						//读取分钟的计数值
	PdCheckSum += PdWorkOneMinCnt;
	
	checkSumRead	= SectorRead(0x000F,EEPROM_CRL);	//读取校验和
	
	/*温度区间合理性判断*/
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
	else												//无工艺时默认温度合理（防止工作状态时不工作的工艺）
	{
		PdTempReasonableFlag = 1;
	}
	
	/*读取上次记录的水泵标定值*/
	i = SectorRead(0x0010,EEPROM_CRL);
	j = SectorRead(0x0011,EEPROM_CRL);
	g_lastPumpNormalWorkAd = ((unsigned int)(i) << 8) + (unsigned int)(j);
	
	/*3次ad采样值都大于阈值，头码为0x5A，校验和正确*/
	// if((PdAdc1 > PD_VALUE) && (PdAdc2 > PD_VALUE) && (PdAdc3 > PD_VALUE) && (PdCheckCode == 0x5A) && (PdCheckSum == checkSumRead))
	// {
	// 	/*若是在预约工作、工作、保温状态下掉电的，则继续工作*/
	// 	if((PdSysType == SysModeOrderWork) || (PdSysType == SysModeWork) || (PdSysType == SysModeWarm))
	// 	{
	// 		if(PdTempReasonableFlag)													//若温度区间合理
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
				
	// 			/*赋一个NTC初值，使高温不加热且不报警*/
	// 			measureData.coreAdHValue = 0x014A;		//240℃
	// 			measureData.coreAdLValue = 0x000F;		//240℃
	// 			g_adDataHSteam			 = 0x0259;		//185℃
	// 			g_adDataLSteam			 = 0x002D;		//185℃
				
	// 			g_tempAdjEnFlag 	= MenuDefaultValue_Table[g_menuNumber][TEMP_ADJ_EN_NUM];	//加载温度可调标志
	// 			g_timeAdjEnFlag 	= MenuDefaultValue_Table[g_menuNumber][TIME_ADJ_EN_NUM];	//加载时间可调标志

	// 			/*赋初值，使不报警*/
	// 			g_LidFlag		= LID_CLOSE;
	// 			g_zeroType		= STATE_YES;
				
	// 			g_timeAdjStateFlag = 1;					//上电恢复默认显示时间
	// 			g_tempAdjStateFlag = 0;

	// 			g_pdWaitTime	= PD_WAIT_TIME;			//1S内使用定义的AD值，之后再使用采样的值
	// 		}
	// 	}
	// }
	
	SectorErase(0x0000,EEPROM_CRL);					//擦除扇区0，扇区被编程后，需擦除后才能再次编程

	/*掉电检测的AD口做GPIO引脚*/
	// ADCH1 &= ~BIT7;
	
	// /*掉电检测AD口不带上拉输出高*/
	// P3    |= BIT7;									//给掉电记忆的电容充电
	// P3CR  |= BIT7;
	// P3PCR &= ~BIT7;
	
	SEQCHX = 0x03;									//选择通道3
}

/*-----------------------------------------------------------------------------
Description:		NTC的AD值扫描
Input:				void
Return:				void
History:			无
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
Description:		NTC的AD滤波赋值
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void NtcAdScan(void)
{
	unsigned char i;
	unsigned int  ntcMaxValue = 0;
	unsigned int  ntcMinValue = 0;
	unsigned int  ntcSumValue = 0;					//10个10位AD值求和值小于int型
	
	/*中心温度NTC检测*/
	measureData.adcnt++;
	
	if(measureData.adcnt <= 5)						//NTC的IO口为输入不带上拉，检测低温区
	{
	}
	
	else if(measureData.adcnt <= 15)				//低温数据采集
	{
		measureData.ntcAd_buff[measureData.adcnt - 6] = AdScanValue();	//AD扫描值赋值

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
		
			/*去最大最小求平均*/
			measureData.coreAdLValue = ntcSumValue - ntcMaxValue - ntcMinValue;
			measureData.coreAdLValue = measureData.coreAdLValue >> 3;

			/*切换到高温区*/
			CORE_NTC_IO_OUT;						//不带上拉输出高
			P_CORE_NTC_IO = 1;
		}
	}
	
	else if(measureData.adcnt <= 20)				//NTC的IO口为输出不带上拉，检测高温区
	{
	}
	
	else if(measureData.adcnt <= 30)				//高温数据采集
	{
		measureData.ntcAd_buff[measureData.adcnt-21] = AdScanValue();	//AD扫描值赋值
		
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
		
			/*去最大最小求平均*/
			measureData.coreAdHValue = ntcSumValue - ntcMaxValue - ntcMinValue;
			measureData.coreAdHValue = measureData.coreAdHValue >> 3;
			
			/*切换到低温区*/
			CORE_NTC_IO_IN;							//检测低温区，NTC的IO口做输入不带上拉
		}
	}
	
	if(measureData.adcnt >= 30)						//指针刷新
	{
 		measureData.adcnt = 0;
		
		if(g_firstAdGetGlag == 0)
		{
			g_firstAdGetGlag = 1;
			
			g_corePreHeatFlag		= 0;			//获得首次AD采集数据后重新判断预热
			g_corePreHeatDelayFlag	= 0;
			g_corePreHeatFirstFlag	= 1;
		}
	}

}

/*-----------------------------------------------------------------------------
Description:		锅盖信号检测
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void LidScan(void)
{
	unsigned char i = 0;
	static unsigned char s_lidOpenCnt = 0;			//开盖检测计数值
	static unsigned char s_lidCloseCnt = 0;			//合盖检测计数值
	static bit			 s_lastLidState = LID_OPEN;	//上次的盖状态
	static bit			 s_lidReCloseFlag = 0;		//重新合盖标志

	static bit  s_ledOpenTemp;		//LED灯状态暂存
	static bit  s_ledDealFlag = 0;
	
	BANK1_SET;
	if(s_lastLidState != P_LID)							//若锅盖状态发生改变，则重新检测锅盖状态
	{
		s_lidOpenCnt 	= 0;
		s_lidCloseCnt 	= 0;
	}

	if(P_LID == LID_OPEN)   
	{
		s_lastLidState = LID_OPEN;
		
		s_lidOpenCnt++;
		if(s_lidOpenCnt >= 15)						//15次连续开盖则判断为开盖
		{
			s_lidOpenCnt = 15;
			g_LidFlag = LID_OPEN;

			if(s_ledDealFlag == 0)
			{
				s_ledOpenTemp = g_LedOpenFlag;			//LED灯状态暂存
				s_ledDealFlag = 1;
			}
			
			g_LedOpenFlag = 0;		//开盖时LED灯关闭
		}
	}
	else
	{
		s_lastLidState = LID_CLOSE;
		
		s_lidCloseCnt++;
		if(s_lidCloseCnt >= 15)						//15次连续合盖则判断为合盖
		{
			s_lidCloseCnt = 15;
			
			if(g_sysType != SysModePowerUp)			//防止掉电上电后会报警
			{
				if(g_LidFlag == LID_OPEN)			//若上次为开盖
				{
					s_lidReCloseFlag = 1;
					
					gU8_buzzerType = BUZ_JK_MUTE;	//立即停止蜂鸣
				}
			}
			
			g_LidFlag = LID_CLOSE;

			if(s_lidReCloseFlag)					//若有重新合盖
			{
				if(g_enContinueErr)					//若有可继续工作报警
				{
					if(g_waterYieldType == LackWater)
					{
						g_quickPumpingFlag	= 1;	//取消缺水报警后快速抽水
					}

					g_enContinueErr		= NoError;	//消除报警，重新检测
					g_waterYieldType	= NormalType;	//标记为正常抽水
					g_pumpDelayJudgeCnt	= 0;
					
					if(g_pumpCalibrationFlag == 0)	//若未标定成功
					{
						InitPumpCalibrationData();	//初始化所有水泵标定参数
					}
				}		
				s_lidReCloseFlag = 0;				//重新合盖已处理

				s_ledDealFlag = 0;
				g_LedOpenFlag = s_ledOpenTemp;	//重新赋值原先的LED灯状态
			}
		}
	}

	BANK0_SET;
}

/*-----------------------------------------------------------------------------
Description:		水泵标定相关参数初始化
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitPumpCalibrationData(void)
{
	unsigned char i;
	
	calibrationData.smoothingCnt		= 0;					//重置所有标定变量
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
	
	g_reachStopHeatFlag		= 0;								//标记为未到达强关点
	g_reachRegionTempFlag	= 0;								//标记为未到达区间温度
}

/*-----------------------------------------------------------------------------
Description:		带蒸功能水泵电流值标定程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void PumpCurrentCalibration(void)
{
	unsigned int	pumpWorkAdSum = 0;
	unsigned char	i = 0;

	static unsigned int xdata s_steamPreHeatCnt = 0;
	static bit		s_newPumpWorkFlag = 0;

	if(g_reachStopHeatFlag == 0)								//锅炉温度未到达过强关点
	{
		if(g_adDataHSteam > STEAM_PRE_HEAT_AD)					//锅炉温度低于预热温度点时
		{
			s_steamPreHeatCnt++;
			if(s_steamPreHeatCnt > 6000)						//若在预热区停留了1min，10ms时基
			{
				g_sysType = SysModeError;						//标定失败直接转入报警状态
				measureData.errCode = 0x06;
				gU8_buzzerType = BUZ_JK_POINT;					//发出可恢复报警蜂鸣

				g_pumpCalibrationFlag	= 0;					//标记为标定未完成
			}
		
			InitPumpCalibrationData();							//初始化所有水泵标定参数
		}
		else if(g_adDataHSteam > STEAM_MAX_TEMP_AD)				//锅炉温度大于预热点，小于强制关闭点
		{
			s_steamPreHeatCnt		= 0;						//预热区计数清零
			
			g_reachStopHeatFlag		= 0;						//标记为未到达强关点
			
			/*小批试产E06问题更改程序20191027*/
			calibrationData.regionHeatCnt++;
			if(calibrationData.regionHeatCnt >= 100)			//停留在区间温度1S以上
			{
				g_reachRegionTempFlag = 1;						//标记为到达区间温度
			}
		
			if(g_pumpType == STATE_YES)							//若水泵有一次新抽水，记录水泵的AD值
			{
				if(s_newPumpWorkFlag == 0)						//有新抽水标志，但数值还未记录时
				{
					calibrationData.pumpWorkAdTemp = g_pumpCurrentAd;
					
					/*保留最小的5个AD值*/
					if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[0])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[0]位，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdBuff[0];
						calibrationData.pumpWorkAdBuff[0] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[1])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[1]位，[0]位不变，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[2])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[2]位，[0]、[1]位不变，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[3])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[3]位，[0]、[1]、[2]位不变，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[4])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[4]位，[0]、[1]、[2]、[3]位不变
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdTemp;
					}
					
					s_newPumpWorkFlag = 1;						//该次水泵抽水数值已记录
				}
			}
			else
			{
				s_newPumpWorkFlag = 0;
			}
			
			if(calibrationData.regionHeatCnt >= REGION_JUDEG_TIME)			//若锅炉温度长时间停留在正常控温区间
			{
				if((calibrationData.pumpWorkAdBuff[4] - calibrationData.pumpWorkAdBuff[0]) < 15)	//采样的5个最小数组最大与最小值差15个以内时认为数据有效
				{
					pumpWorkAdSum = 0;
					for(i=1;i<5;i++)
					{
						pumpWorkAdSum = pumpWorkAdSum + calibrationData.pumpWorkAdBuff[i];			//去最小采样值的4个数组和
					}
					calibrationData.pumpNormalWorkAd = pumpWorkAdSum >> 2;	//记录当前水泵电流值，4个较小采样值的均值
				
					g_pumpCalibrationFlag = 1;					//标记为标定完成
					
					g_pumpDelayJudgeCnt	= 0;
					g_steamPreHeatFlag 	= 0;					//重新判定锅炉是否预热（重新赋值抽水默认值）
				}
				else
				{
//					g_waterYieldType = LackWater;				//标记为缺水
					
					g_sysType = SysModeError;					//标定失败直接转入报警状态
					measureData.errCode = 0x06;
					gU8_buzzerType = BUZ_JK_POINT;				//发出可恢复报警蜂鸣
					
					InitPumpCalibrationData();					//初始化所有水泵标定参数
					g_pumpCalibrationFlag	= 0;				//标记为标定未完成
				}
			}
		}
		else													//锅炉温度大于强制关闭点
		{
			s_steamPreHeatCnt		= 0;						//预热区计数清零
		
			if(g_reachRegionTempFlag)							//在到达过区间温度之前，即使温度到达强关点也不进入强关点标定区间
			{
				g_reachStopHeatFlag = 1;						//标记为到达强关点
				
				calibrationData.smoothingCnt	= 0;			//除5个最小水泵电流采样AD，其余重新赋值
				calibrationData.regionHeatCnt	= 0;
				calibrationData.stopHeatCnt		= 0;
				calibrationData.highTempTimeCnt = 0;
				calibrationData.steamMinAd		= 0x3FF;
				calibrationData.pumpWorkAdTemp	= 0;
				calibrationData.pumpNormalWorkAd= 0;
			}
			else												//若一开始标定时锅炉温度就高于强关点温度
			{
				calibrationData.highTempTimeCnt++;
				if(calibrationData.highTempTimeCnt >= HIGH_TEMP_TIME_MAX)		//温度在强关点以上一定时间内未回温，则判定为标定失败
				{
//					g_waterYieldType = LackWater;				//标记为缺水
					
					g_sysType = SysModeError;					//标定失败直接转入报警状态
					measureData.errCode = 0x06;
					gU8_buzzerType = BUZ_JK_POINT;				//发出可恢复报警蜂鸣
					
					InitPumpCalibrationData();					//初始化所有水泵标定参数
					g_pumpCalibrationFlag	= 0;				//标记为标定未完成
				}
			}
		}
	}
	else														//锅炉温度到达过强关点（非初始标定就在强关点）
	{
		calibrationData.stopHeatCnt++;
		if(calibrationData.stopHeatCnt <= STOP_HEAT_JUDGE_TIME)	//在判定时间内
		{
			if(g_adDataHSteam < calibrationData.steamMinAd)		//判定时间内一直刷最小AD值
			{
				calibrationData.steamMinAd = g_adDataHSteam;
			}
			
			if(g_pumpType == STATE_YES)							//若水泵有一次新抽水，记录水泵的AD值
			{
				if(s_newPumpWorkFlag == 0)						//有新抽水标志，但数值还未记录时
				{
					calibrationData.pumpWorkAdTemp = g_pumpCurrentAd;
					
					/*保留最小的5个AD值*/
					if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[0])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[0]位，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdBuff[0];
						calibrationData.pumpWorkAdBuff[0] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[1])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[1]位，[0]位不变，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdBuff[1];
						calibrationData.pumpWorkAdBuff[1] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[2])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[2]位，[0]、[1]位不变，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdBuff[2];
						calibrationData.pumpWorkAdBuff[2] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[3])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[3]位，[0]、[1]、[2]位不变，其它排位后移一位
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdBuff[3];
						calibrationData.pumpWorkAdBuff[3] = calibrationData.pumpWorkAdTemp;
					}
					else if(calibrationData.pumpWorkAdTemp <= calibrationData.pumpWorkAdBuff[4])
					{	//默认数组[0]<[1]<[2]<[3]<[4]。比最小的小时，当前AD占据第[4]位，[0]、[1]、[2]、[3]位不变
						calibrationData.pumpWorkAdBuff[4] = calibrationData.pumpWorkAdTemp;
					}
					
					s_newPumpWorkFlag = 1;						//该次水泵抽水数值已记录
				}
			}
			else
			{
				s_newPumpWorkFlag = 0;
			}
			
			if(g_adDataHSteam > calibrationData.steamMinAd)		//若当前锅炉温度低于检测到的最小AD值（说明锅炉已降温）
			{
				if((g_adDataHSteam - calibrationData.steamMinAd) > 150)			//判定时间内，降温达到一定值，则判定为抽入了水，记录当前水泵电流值，作为之后的缺水判定标准
				{
					calibrationData.smoothingCnt++;
					if(calibrationData.smoothingCnt >= 30)		//连续多次判定温度下降值较大（10ms时基，g_adDataHSteam值100ms刷新一次）
					{
						if((calibrationData.pumpWorkAdBuff[4] - calibrationData.pumpWorkAdBuff[0]) < 15)	//采样的5个最小数组最大与最小值差15个以内时认为数据有效
						{
							pumpWorkAdSum = 0;
							for(i=1;i<5;i++)
							{
								pumpWorkAdSum = pumpWorkAdSum + calibrationData.pumpWorkAdBuff[i];			//去最小采样值的4个数组和
							}
							calibrationData.pumpNormalWorkAd = pumpWorkAdSum >> 2;	//记录当前水泵电流值，4个较小采样值的均值
						
							g_pumpCalibrationFlag = 1;			//标记为标定完成
							
							g_pumpDelayJudgeCnt	= 0;
							g_steamPreHeatFlag 	= 0;			//重新判定锅炉是否预热（重新赋值抽水默认值）
						}
						else
						{
//							g_waterYieldType = LackWater;		//标记为缺水
							
							g_sysType = SysModeError;			//标定失败直接转入报警状态
							measureData.errCode = 0x06;
							gU8_buzzerType = BUZ_JK_POINT;		//发出可恢复报警蜂鸣
			
							InitPumpCalibrationData();			//初始化所有水泵标定参数
							g_pumpCalibrationFlag	= 0;		//标记为标定未完成
						}
					}
				}
				else
				{
					calibrationData.smoothingCnt = 0;
				}
			}
		}
		else													//判定时间内若降温值未达到指定值，则判定为缺水
		{
//			g_waterYieldType = LackWater;						//标记为缺水
			
			g_sysType = SysModeError;							//标定失败直接转入报警状态
			measureData.errCode = 0x06;
			gU8_buzzerType = BUZ_JK_POINT;						//发出可恢复报警蜂鸣
			
			InitPumpCalibrationData();							//初始化所有水泵标定参数
			g_pumpCalibrationFlag	= 0;						//标记为标定未完成
		}
	}
	
	if(g_pumpCalibrationFlag)									//若标定完成
	{
		/*若记录的标定值过小或过大，则认为标定错误，重新标定*/
		if((calibrationData.pumpNormalWorkAd <= EN_ERROR_VALUE) || (calibrationData.pumpNormalWorkAd >= (0x03FF - EN_ERROR_VALUE)))
		{
//			g_waterYieldType = LackWater;						//标记为缺水
			
			g_sysType = SysModeError;							//标定失败直接转入报警状态
			measureData.errCode = 0x06;
			gU8_buzzerType = BUZ_JK_POINT;						//发出可恢复报警蜂鸣
			
			InitPumpCalibrationData();							//初始化所有水泵标定参数
			g_pumpCalibrationFlag	= 0;						//标记为标定未完成
		}
		else
		{
			judgeErrValue = EN_ERROR_VALUE;
			
			/*若上次的水泵抽水电流值在正常区间，对比当前标定的值与上次标定的值，若当前标定值大于上次标定值一定范围，则减小缺水判定阈值*/
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
Description:		缺水判断（测水泵电流判断）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void LackWaterScan(void)
{
	if(g_pumpCurrentAd > (calibrationData.pumpNormalWorkAd + judgeErrValue))				//若水泵电流AD值大于标定值一定范围，则报缺水报警
	{
		g_waterYieldType = MabyLackWater;						//标记为可能缺水状态，快速再抽一次
		
		g_pumpDelayJudgeCnt++;
		if(g_pumpDelayJudgeCnt >= 10)							//连续10次判定为空抽，则标记为缺水
		{
			g_pumpDelayJudgeCnt = 0;
			
			g_waterYieldType = LackWater;						//标记为缺水
		}
	}
	else
	{
		g_waterYieldType = NormalType;							//标记为正常抽水
		
		g_pumpDelayJudgeCnt	= 0;
	}
}

/*-----------------------------------------------------------------------------
Description:		报警检测
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void ErrorCheck(void)
{
	static unsigned char xdata s_zeroMissCnt = 0;

	if((g_sysType != SysModeError) && (g_sysType != SysModePowerUp))
	{
		if(g_firstRecvFlag == 0)								//若未接收到首次数据，则只进行通讯检测
		{
			if(UartData.uartRecvOverCnt == 301)					//超过3S未接收到完整数据帧，报错
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//发出报警蜂鸣
			
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
			if(measureData.coreAdLValue >= NTC_OPEN_DATA)		//腔体低温区低于-40℃判定为开路报警
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//发出报警蜂鸣
			
				g_sysType = SysModeError;
				measureData.errCode = 0x01;
			}
			else if(measureData.coreAdHValue <= NTC_SHORT_DATA)	//腔体高温区大于270℃判定为短路报警
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//发出报警蜂鸣
			
				g_sysType = SysModeError;
				measureData.errCode = 0x02;
			}
			else if(UartData.uartRecvOverCnt == 301)			//超过3S未接收到完整数据帧，报错
			{
				gU8_buzzerType = BUZ_JK_ALARM;					//发出报警蜂鸣
			
				g_sysType = SysModeError;
				measureData.errCode = 0x07;
			}
			else
			{
				if((g_sysType == SysModeOrderWork) || (g_sysType == SysModeWork) || (g_sysType == SysModeWarm))//工作、预约工作、保温状态时
				{
					if((g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						if(g_adDataLSteam >= NTC_OPEN_DATA)			//锅炉低温区低于-40℃判定为开路报警
						{
							gU8_buzzerType = BUZ_JK_ALARM;			//发出报警蜂鸣
						
							g_sysType = SysModeError;
							measureData.errCode = 0x03;
						}
						else if(g_adDataHSteam <= NTC_SHORT_DATA)	//锅炉高温区大于270℃判定为短路报警
						{
							gU8_buzzerType = BUZ_JK_ALARM;			//发出报警蜂鸣
						
							g_sysType = SysModeError;
							measureData.errCode = 0x04;
						}
						else
						{
							if(g_zeroType == STATE_NO)				//过零检测异常时
							{
								s_zeroMissCnt++;
								if(s_zeroMissCnt >= 200)			//过零检测延时，连续2S未检测到过零则报警
								{
									s_zeroMissCnt = 201;

									gU8_buzzerType = BUZ_JK_ALARM;	//发出报警蜂鸣
								
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

	/*整机自检状态下，锅炉NTC异常直接报警*/
	if(g_sysType == SysModeMachineCheck)								//整机自检状态下，锅炉/过零异常直接报警
	{
		if(g_adDataLSteam >= NTC_OPEN_DATA)								//锅炉低温区低于-40℃判定为开路报警
		{
			gU8_buzzerType = BUZ_JK_ALARM;								//发出报警蜂鸣
		
			g_sysType = SysModeError;
			measureData.errCode = 0x03;
		}
		else if(g_adDataHSteam <= NTC_SHORT_DATA)						//锅炉高温区大于270℃判定为短路报警
		{
			gU8_buzzerType = BUZ_JK_ALARM;								//发出报警蜂鸣
		
			g_sysType = SysModeError;
			measureData.errCode = 0x04;
		}
		else
		{
			if(g_zeroType == STATE_NO)									//过零检测异常时
			{
				s_zeroMissCnt++;
				if(s_zeroMissCnt > 200)									//过零检测延时，连续2S未检测到过零则报警
				{
					gU8_buzzerType = BUZ_JK_ALARM;						//发出报警蜂鸣
				
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
Description:		控制板的检测信号扫描（中心温度和盖检测）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void MeasureDataScan(void)
{
	unsigned char i = 0;
	static bit s_waterScanDealFlag = 0;					//缺水检测处理标志

	if(g_pdWaitTime == 0)								//当无掉电延迟时
	{
		LidScan();										//盖检测
//		g_LidFlag = LID_CLOSE;
		
		if(SEQCHX == 0x03)								//若通道选择的是NTC检测通道
		{
			NtcAdScan();								//中心温度NTC扫描
		}
		
		if(g_pumpCalibrationFlag)						//若水泵电流已标定完成
		{
			if(g_pumpType == STATE_YES)					//若水泵有新一次抽水
			{
				if(s_waterScanDealFlag == 0)			//该次缺水检测未处理
				{
					LackWaterScan();					//缺水检测
					
					s_waterScanDealFlag = 1;			//该次缺水检测已处理
				}
			}
			else
			{
				s_waterScanDealFlag = 0;				//标记为缺水检测未处理
			}
		}
		else											//未标定时若带蒸工艺则进行标定
		{
			if(g_sysType == SysModeWork)
			{
				if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))
				{
					/*无可恢复报警（合盖、不缺水）*/
					if((g_LidFlag == LID_CLOSE)  && (g_waterYieldType == NormalType))
					{
						PumpCurrentCalibration();
					}
					else
					{
						InitPumpCalibrationData();		//初始化所有水泵标定参数
					}
				}
				else									//为标定前，若工作状态非带蒸工艺，则重置标定参数
				{
					InitPumpCalibrationData();			//初始化所有水泵标定参数
				}
			}
			else										//未标定前，若非工作状态，重置标定相关参数
			{
				InitPumpCalibrationData();				//初始化所有水泵标定参数
			}
		}
	}
	else
	{
		/*赋一个NTC初值，使高温不加热且不报警*/
		measureData.coreAdHValue = 0x014A;				//240℃
		measureData.coreAdLValue = 0x000F;				//240℃
		
		/*赋初值，使不报警*/
		g_LidFlag = LID_CLOSE;
	}
}