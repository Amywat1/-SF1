#include "header.h"

enum _SYS_Type_     	g_sysType;              		//系统状态
enum _ERROR_Type_		g_enContinueErr;				//可继续工作报警类型
enum _WATER_YIELD_TYPE_	g_waterYieldType;				//水泵出水状态

unsigned char xdata		g_menuNumber;					//菜单编号

unsigned char xdata		g_zeroType;						//过零状态（有过零，无过零）
unsigned char xdata		g_pumpType;						//水泵状态（有新抽水，无新抽水）
		 bit			g_LidFlag;						//是否合盖标志

unsigned int  xdata		g_sysStandbyWaitCnt;			//待机等待时间计数
unsigned char xdata		g_sysSelectWaitCnt;				//设置待机等待时间计数
unsigned char xdata		g_sysOrderSetWaitCnt;			//预约设置等待时间计数
unsigned char xdata		g_sysSelfCheckWaitCnt;			//自检等待时间计数
unsigned char xdata		g_dispFlashCnt;					//显示闪烁计数
unsigned char xdata		g_adjWaitTimeCnt;				//时间/温度调节等待时间计数
unsigned int  xdata		g_cleanPumpTimeCnt;				//水泵清洁程序，步骤时间

unsigned char xdata		g_darkScreenTime;				//自检第十步全息时间
unsigned char xdata		g_pdWaitTime;					//掉电重新上电后的等待时间

unsigned char xdata		g_nowStepworkTemp;				//当前步骤工作温度
unsigned char xdata		g_nowStepworkTime;				//当前步骤工作时间
unsigned char xdata		g_workTimeAll;					//工作总时间
unsigned int  xdata		g_keepWarmTime;					//已保温时间
unsigned int  xdata		g_orderTime;					//预约时间（这么长时间后工作完成）
unsigned int  xdata		g_orderMinTime;					//最小预约时间
unsigned char xdata		g_workOneMinCnt;				//1min时间计时
unsigned int  xdata		g_highSteamPowerCnt;			//纯蒸工艺下高功率输出时间计时

unsigned char xdata		g_nowStepTechnology;			//当前步骤的工艺
unsigned char xdata		g_nowStepNum;					//当前工艺步骤数

		 bit			g_dispFlashFlag;				//显示闪烁标志位
		 bit			g_selfCheckEnFlag;				//自检允许标志
		 bit			g_timeAdjEnFlag;				//时间可调节标志
		 bit			g_tempAdjEnFlag;				//温度可调节标志
		 bit			g_orderEnFlag;					//预约有效标志
		 bit			g_timeAdjStateFlag;				//时间调节状态标志
		 bit			g_tempAdjStateFlag;				//温度调节状态标志
		 bit			g_pumpCalibrationFlag;			//水泵电流标定标志
		 bit			g_coolingFanDealyFlag;			//散热风扇延时停止标志
		 bit			g_steamLowPowerFlag;			//纯蒸的低功率标志
		 bit			g_quickPumpingFlag;				//水泵快抽标志（在缺水报警按“启动取消”键继续后快抽）
		 
		 bit			g_lidSelfCheckOkFlag;			//盖自检合格标志
		 bit			g_coreNtcSelfCheckOkFlag;		//腔体NTC电路自检合格标志
		 bit			g_selfCheckPassFlag;			//自检合格标志
		 bit			g_selfCheckOverFlag;			//显示板自检完毕标志
		 bit			g_pumpWorkFlag;					//整机自检水泵工作标志
		 bit			g_pumpDelayJudgeFlag;			//整机自检水泵延时判断标志

unsigned char xdata		g_selfCheckStep;				//自检步骤
unsigned char xdata		g_pidDispStep;					//PID步骤
unsigned char xdata		g_debugDispStep;				//Debug步骤
unsigned char xdata		g_cleanIncrustantStep;			//水垢清洁步骤

unsigned char code MenuDefaultValue_Table[6][22]={			//6个菜单的默认参数
//	(		工艺1	)	(		工艺2	)	(		工艺3	)	(		工艺4	)	（保温工艺）	 （温度调节范围）（时间调节范围）	(		调节是否有效		)
//	工艺	温度/时间	工艺	温度/时间	工艺	温度/时间	工艺	温度/时间	工艺	温度/时间		MIN	MAX		MIN	MAX			温度	时间	预约
	ONLY_BAKE,180,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		30,	200,	1,	60,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//自主空气炸
	
	BAKE_STEAM,180,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		100,200,	1,	60,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//自主蒸汽烤

	ONLY_STEAM,100,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		80,	110,	1,	60,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//自主原味蒸
																											            			
	ONLY_BAKE,180,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		170,190,	15,	25,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//菜单01
																											            			
	BAKE_STEAM,200,	5,	ONLY_BAKE,200,	11,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		180,200,	15,	25,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//菜单02
																											            			
	ONLY_STEAM,100,	28,	ONLY_STEAM,80,	8,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	ONLY_STEAM,80,	60,		100,100,	30,	50,		NO_ADJ,	EN_ADJ,	EN_ADJ,	//菜单03
};

/*-----------------------------------------------------------------------------
Description:		初始化系统状态变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitSysModeVariable(void)
{
	g_sysType				= SysModePowerUp;
	g_enContinueErr			= NoError;
	g_waterYieldType		= NormalType;

	g_menuNumber			= DEFAULT_MENU;
	
	g_zeroType				= STATE_YES;
	g_pumpType				= STATE_NO;								//默认水泵无抽水动作
	g_LidFlag 				= LID_OPEN;
	
	g_sysStandbyWaitCnt		= 0;
	g_sysSelectWaitCnt		= 0;
	g_sysOrderSetWaitCnt	= 0;
	g_sysSelfCheckWaitCnt	= 0;
	g_dispFlashCnt			= 0;
	g_adjWaitTimeCnt		= 0;
	g_cleanPumpTimeCnt		= 0;
	
	g_darkScreenTime		= 0;
	g_pdWaitTime 			= 0;

	g_nowStepworkTemp 		= 0;
	g_nowStepworkTime 		= 0;
	g_workTimeAll			= 0;
	g_keepWarmTime			= 0;
	g_orderTime				= 0;
	g_orderMinTime			= 0;
	g_workOneMinCnt 		= 0;
	g_highSteamPowerCnt		= 0;
	
	g_nowStepTechnology		= NULL_STEP;							//默认当前无工艺步骤
	g_nowStepNum			= 1;									//默认第一步工艺

	g_dispFlashFlag			= 0;
	g_selfCheckEnFlag		= 1;									//默认允许进入自检
	g_timeAdjEnFlag 		= 0;
	g_tempAdjEnFlag 		= 0;
	g_orderEnFlag 			= 0;
	g_timeAdjStateFlag 		= 0;
	g_tempAdjStateFlag 		= 1;									//默认温度调节状态
	g_pumpCalibrationFlag	= 0;
	g_coolingFanDealyFlag	= 0;
	g_steamLowPowerFlag		= 0;
	g_quickPumpingFlag		= 0;
	
	g_lidSelfCheckOkFlag	= 0;
	g_coreNtcSelfCheckOkFlag= 0;
	g_selfCheckPassFlag		= 0;
	g_selfCheckOverFlag		= 0;
	g_pumpWorkFlag			= 0;
	g_pumpDelayJudgeFlag	= 0;

	g_selfCheckStep			= 0;
	g_pidDispStep			= 0;
	g_debugDispStep			= 0;
	g_cleanIncrustantStep	= 0;
}

/*-----------------------------------------------------------------------------
Description:		系统状态控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SysModelCrl(void)
{
	unsigned char i = 0;
	
	static unsigned int  xdata	s_pdAdValue					= 0;
	static unsigned int  xdata	s_coolingFanDealyCnt		= 0;
	static unsigned int  xdata  s_lidOpenWorkCnt			= 0;		//工作和保温状态下开盖时间计时
	static unsigned int  xdata  s_sysEndWaitCnt				= 0;		//工作结束等待时间
	
	static 			bit 		s_selfCheckLidFlag			= 0;
	
	static			bit			s_pumpNoWorkWoringFlag		= 0;		//整机厂水泵自检报警已蜂鸣标志
	static unsigned char xdata  s_pumpTestWaitCnt			= 0;		//整机厂水泵自检等待时间

	static unsigned char xdata  s_powerUpCnt				= 0;		//上电时间计时
	static unsigned char xdata  s_powerUpFifteenSecondsCnt	= 0;		//上电15S计时

	static unsigned char xdata  s_keepWarmMaxTime			= 0;

	if(g_sysType == SysModePCBCheck)									//PCB自检状态时
	{
		if(g_selfCheckStep == 1)										//自检第一步记录盖状态
		{
			s_selfCheckLidFlag		= g_LidFlag;
			s_pdAdValue				= 0;
			
			g_lidSelfCheckOkFlag	= 0;								//第一步开始默认自检不合格
			g_coreNtcSelfCheckOkFlag= 0;
			g_selfCheckPassFlag		= 0;
			g_selfCheckOverFlag		= 0;								//标记为自检未完成
		}
		else if(g_selfCheckStep == 8)
		{
			if(g_lidSelfCheckOkFlag)									//若盖检测合格，检测腔体NTC值
			{
				if((measureData.coreAdHValue > SELF_CHECK_AD_HMAX) || (measureData.coreAdHValue < SELF_CHECK_AD_HMIN) || (measureData.coreAdLValue > SELF_CHECK_AD_LMAX) || (measureData.coreAdLValue < SELF_CHECK_AD_LMIN))
				{
					g_coreNtcSelfCheckOkFlag = 0;						//标记腔体NTC电路自检不合格
					g_selfCheckPassFlag = 0;							//标记自检不合格
					g_selfCheckOverFlag		= 1;						//标记为自检完成
					
					gU8_buzzerType = BUZ_JK_ALARM;						//发出报警蜂鸣
						
					g_sysType = SysModeError;
					measureData.errCode = 0x58;
				}
				else
				{
					g_coreNtcSelfCheckOkFlag = 1;						//标记腔体NTC电路自检合格

//					s_pdAdValue = AdScanValue();						//检掉电检测端口电压

					g_selfCheckPassFlag = 1;							//标记自检合格
					
					g_selfCheckOverFlag	= 1;							//标记为自检完成
				}
			}
			else
			{
				g_selfCheckPassFlag	= 0;								//标记自检不合格
				g_selfCheckOverFlag		= 1;							//标记为自检完成
				
				gU8_buzzerType = BUZ_JK_ALARM;							//发出报警蜂鸣
						
				g_sysType = SysModeError;
				measureData.errCode = 0x57;
			}
		}
		else															//第二步及以后
		{
			if(g_lidSelfCheckOkFlag == 0)								//若盖检测未合格则一直检测盖有无变化
			{
				if(g_LidFlag != s_selfCheckLidFlag)						//若盖有变化，则标记盖检测合格
				{
					g_lidSelfCheckOkFlag = 1;
				}
			}
		}
	}
	
	/*掉电重启延时计时*/
	if(g_pdWaitTime)													//当有掉电延迟时，进行延时倒计时
	{
		g_pdWaitTime--;
		
		if(g_pdWaitTime == 0)											//最后启动按键扫描
		{
			TKCON1 |= BIT5;    											//启动按键扫描
		}
	}
	
	/*显示闪烁控制*/
	g_dispFlashCnt++;
	if(g_dispFlashCnt >= DISP_FLASH_FRE)
	{
		g_dispFlashCnt  = 0;
		g_dispFlashFlag = ~g_dispFlashFlag;								//一定频率闪烁
	}
	
//	ErrorCheck();														//报警检测
	
	if(g_flg500ms)														//500ms时间进一次
	{
		g_flg500ms = 0;

		/*上电15S计时*/
		if(g_selfCheckEnFlag == 1)										//若自检允许
		{
			s_powerUpFifteenSecondsCnt++;
			if(s_powerUpFifteenSecondsCnt >= POWER_UP_FIFTEEN_SECONDS)
			{
				g_selfCheckEnFlag = 0;									//禁止进入自检
				s_powerUpFifteenSecondsCnt = POWER_UP_FIFTEEN_SECONDS;
			}
		}
		
		/*温度/时间等待时间计时*/
		if(g_sysType == SysModeSelect)									//设置待机状态下常态显示温度
		{
			if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
			{
				if(g_tempAdjStateFlag == EN_ADJ)						//一键通菜单默认显示时间
				{
					g_adjWaitTimeCnt++;
					if(g_adjWaitTimeCnt >= ADJ_WAIT_TIME)				//调节等待时间到，切换回常态显示
					{
						g_adjWaitTimeCnt = 0;
						
						g_timeAdjStateFlag = EN_ADJ;
						g_tempAdjStateFlag = NO_ADJ;
					}
				}
			}
			else														//自主菜单默认显示温度
			{
				if(g_timeAdjStateFlag == EN_ADJ)
				{
					g_adjWaitTimeCnt++;
					if(g_adjWaitTimeCnt >= ADJ_WAIT_TIME)				//调节等待时间到，切换回常态显示
					{
						g_adjWaitTimeCnt = 0;
						
						g_timeAdjStateFlag = NO_ADJ;
						g_tempAdjStateFlag = EN_ADJ;
					}
				}
			}
		}
		else if(g_sysType == SysModeWork)								//工作状态下常态显示时间
		{
			if(g_tempAdjStateFlag == EN_ADJ)
			{
				g_adjWaitTimeCnt++;
				if(g_adjWaitTimeCnt >= ADJ_WAIT_TIME)					//调节等待时间到，切换回常态显示
				{
					g_adjWaitTimeCnt = 0;
					
					g_timeAdjStateFlag = EN_ADJ;
					g_tempAdjStateFlag = NO_ADJ;
				}
			}
		}
		else
		{
			//其它状态不做变换
		}
		
		
		/*工作结束后散热风扇延时关闭计数*/
		if(g_coolingFanDealyFlag)										//若散热延时关闭中
		{
			s_coolingFanDealyCnt++;
			if(s_coolingFanDealyCnt > COOLING_DELAY_TIME)
			{
				g_coolingFanDealyFlag = 0;
				s_coolingFanDealyCnt  = 0;
			}
		}
		else
		{
			s_coolingFanDealyCnt = 0;
		}
		
		/*纯蒸模式下初始阶段高功率输出计时*/
		if((g_sysType == SysModeWork) && (g_nowStepTechnology == ONLY_STEAM) && (g_nowStepworkTemp >= 100))	//若当前工作状态为100℃及以上纯蒸
		{
			/*无可恢复报警（合盖、正常不缺水）时计时*/
			if((g_LidFlag == LID_CLOSE) && (g_waterYieldType == NormalType))
			{
				if(g_steamLowPowerFlag == 0)							//若低功率输出标志位为0
				{
					g_highSteamPowerCnt++;
					if(g_highSteamPowerCnt >= HIGH_POWER_MAX_TIME)		//纯蒸高功率输出时间超过允许最大时间
					{
						g_steamLowPowerFlag = 1;
						g_highSteamPowerCnt = HIGH_POWER_MAX_TIME;
					}
				}
			}
		}
		
		/*工作或保温状态下若开盖超过5min则退回到待机状态*/
		if((g_sysType == SysModeWork) || (g_sysType == SysModeWarm))
		{
			if(g_LidFlag == LID_OPEN)
			{
				s_lidOpenWorkCnt++;
				if(s_lidOpenWorkCnt >= LID_OPEN_WORK_TIME)
				{
					g_sysType  = SysModeStandby;						//回到待机状态
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;					//标记为正常抽水
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零

					gU8_buzzerType = BUZ_JK_FINISH;						//发出工作结束蜂鸣
				}
			}
			else
			{
				s_lidOpenWorkCnt = 0;
			}
		}
		else
		{
			s_lidOpenWorkCnt = 0;
		}
		
		/*不同系统状态下控制*/
		switch(g_sysType)
		{
			case SysModePowerUp:										//上电状态
				if(s_powerUpCnt < POWER_UP_TIME)
				{
					s_powerUpCnt++;
				}
				else
				{
					g_sysType	= SysModeStandby;						//上电完成转到待机转态
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
					
					TKCON1 |= BIT5;    									//启动按键扫描
				}
				break;
				
			case SysModeStandby:										//待机状态
				/* g_sysStandbyWaitCnt++;
				if(g_sysStandbyWaitCnt >= SYS_STANDBY_WAIT_TIME)		//待机等待超过5分钟，转到黑屏状态
				{
					g_sysType = SysModeDarkScreen;
				} */
				break;
				
			case SysModeSelect:											//设置待机状态
				g_sysSelectWaitCnt++;
				if(g_sysSelectWaitCnt >= SYS_SELECT_WAIT_TIME)			//设置待机等待超过2分钟，转到待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
				}
				break;
			
			case SysModeOrderSet:										//预约设置状态
				g_sysOrderSetWaitCnt++;
				if(g_sysOrderSetWaitCnt >= SYS_ORDERSET_WAIT_TIME)		//预约等待时间超过2min，进入待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
				}
				break;

			case SysModeOrderWork:										//预约工作状态
				if(g_LidFlag == LID_OPEN)
				{
					if(g_enContinueErr == NoError)
					{
						g_enContinueErr = LidOpenErr;					//赋值为开盖报警
				
						gU8_buzzerType = BUZ_JK_POINT;					//发出可恢复报警蜂鸣
					}
				}
			
				if(g_orderTime > g_workTimeAll)							//预约时间大于工作时间
				{
					g_workOneMinCnt++;
					if(g_workOneMinCnt >= ONE_MINUTE_CNT)				//1min计时
					{
						g_workOneMinCnt = 0;
						
						g_orderTime--;
						
						/*此处直接转状态，防止500ms后才转*/
						if(g_orderTime <= g_workTimeAll)				//到达开始工作时间
						{
							g_sysType 			= SysModeWork;			//转到工作状态
							
							g_enContinueErr		= NoError;				//清可恢复报警
							g_waterYieldType 	= NormalType;			//默认正常抽水

							g_nowStepNum		= 1;					//开始第一步工艺

							g_timeAdjStateFlag	= 1;					//工作默认显示时间
							g_tempAdjStateFlag	= 0;

							g_highSteamPowerCnt	= 0;					//纯蒸工艺下高功率输出时间计时清零
							g_steamLowPowerFlag	= 0;					//纯蒸低功率输出标志清零
							
							g_coolingFanDealyFlag = 0;					//散热风扇默认不延时停止
							
							g_pumpDelayJudgeCnt	= 0;
							
							if(g_pumpCalibrationFlag == 0)				//若未标定成功
							{
								InitPumpCalibrationData();				//初始化所有水泵标定参数
							}
						}
					}
				}
				else													//预约时间等于开始工作时间时
				{
					g_sysType 			= SysModeWork;					//转到工作状态

					g_enContinueErr		= NoError;						//清可恢复报警
					g_waterYieldType 	= NormalType;					//默认正常抽水
					
					g_nowStepNum		= 1;							//开始第一步工艺

					g_timeAdjStateFlag	= 1;							//工作默认显示时间
					g_tempAdjStateFlag	= 0;

					g_highSteamPowerCnt	= 0;							//纯蒸工艺下高功率输出时间计时清零
					g_steamLowPowerFlag	= 0;							//纯蒸低功率输出标志清零
					
					g_coolingFanDealyFlag = 0;							//散热风扇默认不延时停止

					g_pumpDelayJudgeCnt	= 0;
					
					if(g_pumpCalibrationFlag == 0)						//若未标定成功
					{
						InitPumpCalibrationData();						//初始化所有水泵标定参数
					}
				}
				break;
				
			case SysModeWork:											//工作状态
				if(g_LidFlag == LID_CLOSE)								//合盖状态下，进入工作倒计时
				{
					if(g_waterYieldType == LackWater)					//缺水时，不进行倒计时
					{
						if(g_enContinueErr == NoError)					//若之前无报警
						{
							g_enContinueErr = LowWaterErr;				//赋值为缺水报警
						
							gU8_buzzerType = BUZ_JK_POINT;				//发出可恢复报警蜂鸣
						}
					}
					else
					{
						if(g_workTimeAll > 0)							//剩余工作时间大于零
						{
							g_workOneMinCnt++;
							if(g_workOneMinCnt >= ONE_MINUTE_CNT)		//1min计时
							{
								g_workOneMinCnt = 0;
								
								g_workTimeAll--;						//总时间减1
										
								/*此处直接转状态，防止500ms后才转*/
								if(g_workTimeAll == 0)					//工作时间结束
								{
									if(g_menuNumber == MENU_6_NUM)
									{
										g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][KEEP_WARM_TEMP_NUM];	//加载保温温度
										g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_WARM_NUM];//加载保温工艺
										
										g_sysType 		= SysModeWarm;	//转到保温状态
										g_keepWarmTime	= 0;			//保温时间正计时开始
										g_workOneMinCnt = 0;

										//切换工艺时，若切换到带蒸工艺，重新赋抽水档位初始值
										if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))
										{
											g_steamPreHeatFlag 	= 0;	//重新判定锅炉是否预热（重新赋值抽水默认值）
										}
									}
									else
									{
										g_sysType = SysModeEnd;			//转到工作结束状态
										s_sysEndWaitCnt = 0;			//工作结束等待时间清零
									}

									g_coolingFanDealyFlag = 1;				//散热风扇延时关闭标志置一
									s_coolingFanDealyCnt  = 0;				//散热风扇延时关闭计数清零
									
									gU8_buzzerType = BUZ_JK_FINISH;			//发出结束蜂鸣
								}
								else
								{
									if(g_nowStepworkTime)					//若当前工艺步骤剩余时间大于零
									{
										g_nowStepworkTime--;
										if(g_nowStepworkTime == 0)			//工艺时间结束后，跳转到下一个工艺
										{
											if(g_nowStepNum == 1)			//若是第一步工艺结束
											{
												g_nowStepNum = 2;			//转入第二步工艺

												g_nowStepworkTime		= MenuDefaultValue_Table[g_menuNumber][STEP_2_TIME_NUM];//加载第二步工艺的默认时间

												if(g_nowStepworkTime)		//若工艺时间不为零，则变更工艺和工艺温度
												{
													if(g_menuNumber == MENU_5_NUM)		//烤鸡的第二步工艺温度同第一步工艺温度
													{
														//工艺温度不变更
													}
													else
													{
														g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][STEP_2_TEMP_NUM];
													}

													g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_STEP_2_NUM];	//加载第二步工艺
												}
											}
											else if(g_nowStepNum == 2)		//若是第二步工艺结束
											{
												g_nowStepNum = 3;			//转入第三步工艺
												
												g_nowStepworkTime		= MenuDefaultValue_Table[g_menuNumber][STEP_3_TIME_NUM];//加载第三步工艺的默认时间

												if(g_nowStepworkTime)		//若工艺时间不为零，则变更工艺和工艺温度
												{
													g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][STEP_3_TEMP_NUM];
													g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_STEP_3_NUM];	//加载第三步工艺
												}
											}
											else if(g_nowStepNum == 3)			//若是第三步工艺结束
											{
												g_nowStepNum = 4;				//转入第四步工艺

												g_nowStepworkTime		= MenuDefaultValue_Table[g_menuNumber][STEP_4_TIME_NUM];//加载第四步工艺的默认时间

												if(g_nowStepworkTime)		//若工艺时间不为零，则变更工艺和工艺温度
												{
													g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][STEP_4_TEMP_NUM];
													g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_STEP_4_NUM];	//加载第四步工艺
												}
											}
											
											g_corePreHeatFlag		= 0;		//切换工艺后重新判断预热
											g_corePreHeatDelayFlag	= 0;
											g_corePreHeatFirstFlag	= 1;

											//切换工艺时，若切换到带蒸工艺，重新赋抽水档位初始值
											if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))
											{
												g_steamPreHeatFlag 	= 0;		//重新判定锅炉是否预热（重新赋值抽水默认值）
											}
										}
									}
								}
							}
						}
					}
				}
				else
				{
					if(g_enContinueErr == NoError)
					{
						g_enContinueErr = LidOpenErr;					//赋值为开盖报警
				
						gU8_buzzerType = BUZ_JK_POINT;					//发出可恢复报警蜂鸣
					}
				}
				break;
			
			case SysModeWarm:											//保温状态
				if(g_LidFlag == LID_CLOSE)								//合盖状态下，进入保温正计时
				{
					if(g_waterYieldType == LackWater)					//缺水时，不进行倒计时
					{
						if(g_enContinueErr == NoError)					//若之前无报警
						{
							g_enContinueErr = LowWaterErr;				//赋值为缺水报警
						
							gU8_buzzerType = BUZ_JK_POINT;				//发出可恢复报警蜂鸣
						}
					}
					else
					{
						g_workOneMinCnt++;
						if(g_workOneMinCnt >= ONE_MINUTE_CNT)			//1min计时
						{
							g_workOneMinCnt = 0;
							
							if(g_menuNumber == MENU_6_NUM)
							{
								s_keepWarmMaxTime	= MenuDefaultValue_Table[g_menuNumber][KEEP_WARM_TIME_NUM];
							}
							else
							{
								s_keepWarmMaxTime	= 0;
							}

							g_keepWarmTime++;
							if(g_keepWarmTime >= s_keepWarmMaxTime)		//保温时间到
							{
								g_sysType = SysModeEnd;					//转到工作结束状态
								s_sysEndWaitCnt = 0;					//工作结束等待时间清零
								
								gU8_buzzerType = BUZ_JK_FINISH;			//发出结束蜂鸣
							}
						}
					}
				}
				else
				{
					if(g_enContinueErr == NoError)
					{
						g_enContinueErr = LidOpenErr;					//赋值为开盖报警
				
						gU8_buzzerType = BUZ_JK_POINT;					//发出可恢复报警蜂鸣
					}
				}
				break;
			
			case SysModeEnd:
				s_sysEndWaitCnt++;
				if(s_sysEndWaitCnt >= SYS_END_WAIT_TIME)				//工作结束等待超过5分钟，转到待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
				}
				break;
				
			case SysModeDarkScreen:
				//关所有输出
				break;
				
			case SysModeError:
				//关所有输出
				break;

			case SysModePCBCheck:										//PCB自检状态
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt >= SYS_SELFCHECK_WAIT_TIME)	//自检等待超过15S，转到待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
					
					g_selfCheckStep	= 0;
					
					/*掉电检测的AD口做GPIO引脚*/
					ADCH1 &= ~BIT7;
					
					/*掉电检测AD口不带上拉输出高*/
					P3    |= BIT7;										//给掉电记忆的电容充电
					P3CR  |= BIT7;
					P3PCR &= ~BIT7;
					
					SEQCHX = 0x02;										//选择通道2
				}
				break;
				
			case SysModeMachineCheck:									//整机自检状态
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt >= SYS_SELFCHECK_WAIT_TIME)	//自检等待超过15S，转到待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单

					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
					
					g_selfCheckStep	= 0;
				}
				
				if(g_selfCheckStep == 11)
				{
					s_pumpTestWaitCnt++;								//若检测水泵异常，则延时1S报警，若正常则不使用该延时标志
					if(s_pumpTestWaitCnt >= 2)
					{
						g_pumpDelayJudgeFlag = 1;
						
						s_pumpTestWaitCnt = 2;							//防止溢出
					}
					else
					{
						g_pumpDelayJudgeFlag = 0;
					}
				
					if((g_pumpCurrentAd < 0x3F0) && (g_pumpCurrentAd > 0x100))		//若水泵电流AD值小于0x3F0，大于0x100则认为水泵工作，水泵工作标志位置1
					{
						g_pumpWorkFlag = 1;

						s_pumpNoWorkWoringFlag = 0;
					}
					else
					{
						g_pumpWorkFlag = 0;
						
						if(g_pumpDelayJudgeFlag)						//若延时时间已到
						{
							if(s_pumpNoWorkWoringFlag == 0)				//若未蜂鸣，则发出蜂鸣
							{
								gU8_buzzerType = BUZ_JK_POINT;			//发出可恢复报警蜂鸣
								
								s_pumpNoWorkWoringFlag = 1;				//标记为已蜂鸣
							}
						}
					}
				}
				else if(g_selfCheckStep == 13)
				{
					if(g_darkScreenTime)								//自检第10步黑屏倒计时
					{
						g_darkScreenTime--;
						if(g_darkScreenTime == 0)
						{
							g_selfCheckStep = 1;						//返回第一步
							
							gU8_buzzerType = BUZ_JK_PREHEATING;			//连续发出蜂鸣
						}
					}
				}
				else
				{
					g_darkScreenTime 		= 0;
					s_pumpNoWorkWoringFlag 	= 0;
					s_pumpTestWaitCnt 		= 0;
					g_pumpDelayJudgeFlag 	= 0;
				}
				break;
				
			case SysModeDebug:
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt >= SYS_SELFCHECK_WAIT_TIME)	//自检等待超过15S，转到待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
					
					g_debugDispStep = 0;
				}
				break;
			
			case SysModePID:
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt <= PID_STEP1_TIME)
				{
					g_pidDispStep = 1;									//2S内显示第一步
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP2_TIME)
				{
					g_pidDispStep = 2;									//4S内显示第二步
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP3_TIME)
				{
					g_pidDispStep = 3;									//6S内显示第二步
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP4_TIME)
				{
					g_pidDispStep = 4;									//8S内显示第二步
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP5_TIME)
				{
					g_pidDispStep = 5;									//10S内显示第二步
				}
				else
				{
					g_pidDispStep = 6;									//12S后显示第三步
				}

				if(g_sysSelfCheckWaitCnt >= SYS_PID_WAIT_TIME)			//PID等待超过12S，转到待机状态
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//默认菜单
					
					g_sysStandbyWaitCnt = 0;							//待机等待时间清零
					
					g_pidDispStep = 0;
				}
				break;
			
			case SysModeClean:											//水垢清洁程序
				if(g_cleanIncrustantStep == 1)							//清洁第一步
				{
					g_cleanPumpTimeCnt++;
					if(g_cleanPumpTimeCnt >= CLEAN_TIME_STEP_1)			//步骤一时间到
					{
						g_cleanIncrustantStep	= 2;					//进入步骤二
						
						g_cleanPumpTimeCnt		= 0;					//计时清零
					}
				}
				else if(g_cleanIncrustantStep == 2)						//清洁第二步
				{
					g_cleanPumpTimeCnt++;
					if(g_cleanPumpTimeCnt >= CLEAN_TIME_STEP_2)			//步骤二时间到
					{
						if(g_cleanPumpTimeCnt == CLEAN_TIME_STEP_2)
						{
							gU8_buzzerType = BUZ_JK_FINISH;				//发出结束蜂鸣
						}
						
						g_cleanPumpTimeCnt	= CLEAN_TIME_STEP_2 + 1;	//循环等待继续按键
					}
				}
				
				else
				{
					g_cleanPumpTimeCnt++;
					if(g_cleanPumpTimeCnt >= CLEAN_TIME_STEP_3)			//步骤三时间到
					{
						g_sysType = SysModeEnd;							//转到工作结束状态
						s_sysEndWaitCnt = 0;							//工作结束等待时间清零
						
						gU8_buzzerType = BUZ_JK_FINISH;					//发出结束蜂鸣
					}
				}
				break;
		}
	}
}