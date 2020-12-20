#include "header.h"

LOAD_CRL_MESSAGE xdata			loadCrlData;				//负载控制的数据

		 bit					tempZoneFlag;				//温区标志
		 bit					tempTrendFlag;				//温度趋势标志
		 bit					g_corePreHeatFlag;			//中心温度预热标志
		 bit					g_corePreHeatDelayFlag;		//预热延时标志
		 bit					g_corePreHeatFirstFlag;		//首次预热标志
		 bit					highSteamFirstHeatFlag;		//110℃纯蒸首次加热标志
		 bit					g_steamPreHeatFlag;			//锅炉温度预热标志
		 
		 bit					tubeHeatFlag;				//卤素管加热标志
		 bit					fanOpenFlag;				//罩极风扇开启标志
		 bit					steamOpenFlag;				//锅炉开启标志
		 bit					sanReOpenFlag;				//电源板散热开启标志

/*空气炸功能预热步骤控温参数查询表*/
unsigned int code OnlyBakePreHeat_Table[35]=
{
	TEMP_LOW_028,	TEMP_LOW_030,	TEMP_LOW_032,	TEMP_LOW_035,	TEMP_LOW_039,	//30℃~50℃

	TEMP_LOW_043,	TEMP_LOW_045,	TEMP_LOW_049,	TEMP_LOW_053,	TEMP_LOW_057,	//55℃~75℃

	TEMP_LOW_062,	TEMP_LOW_067,	TEMP_LOW_072,	TEMP_LOW_077,	TEMP_LOW_082,	//80℃~100℃
									
	TEMP_LOW_087,	TEMP_LOW_092,	TEMP_LOW_097,	TEMP_LOW_102,	TEMP_LOW_107,	//105℃~125℃
	
	TEMP_LOW_112,	TEMP_LOW_117,	TEMP_LOW_122,	TEMP_LOW_127,	TEMP_LOW_132,	//130℃~150℃
	
	TEMP_HIG_137,	TEMP_HIG_142,	TEMP_HIG_147,	TEMP_HIG_152,	TEMP_HIG_158,	//155℃~175℃
	
	TEMP_HIG_163,	TEMP_HIG_167,	TEMP_HIG_171,	TEMP_HIG_174,	TEMP_HIG_178,	//180℃~200℃
};

/*空气炸功能区间控温参数查询表*/
unsigned int code OnlyBakeRegionHeat_Table[35][2]=
{
//		开启点			关闭点			开启点			关闭点			开启点			关闭点
	{TEMP_LOW_032,	TEMP_LOW_031},	{TEMP_LOW_034,	TEMP_LOW_033},	{TEMP_LOW_036,	TEMP_LOW_035},		//30℃~40℃
	
	{TEMP_LOW_039,	TEMP_LOW_037},	{TEMP_LOW_043,	TEMP_LOW_041},	{TEMP_LOW_047,	TEMP_LOW_045},		//45℃~55℃

	{TEMP_LOW_051,	TEMP_LOW_049},	{TEMP_LOW_056,	TEMP_LOW_054},	{TEMP_LOW_061,	TEMP_LOW_059},		//60℃~70℃
	
	{TEMP_LOW_065,	TEMP_LOW_063},	{TEMP_LOW_070,	TEMP_LOW_068},	{TEMP_LOW_075,	TEMP_LOW_073},		//75℃~85℃
	
	{TEMP_LOW_080,	TEMP_LOW_078},	{TEMP_LOW_085,	TEMP_LOW_083},	{TEMP_LOW_090,	TEMP_LOW_088},		//90℃~100℃
	
	{TEMP_LOW_094,	TEMP_LOW_092},	{TEMP_LOW_099,	TEMP_LOW_097},	{TEMP_LOW_104,	TEMP_LOW_102},		//105℃~115℃
	
	{TEMP_LOW_109,	TEMP_LOW_107},	{TEMP_LOW_114,	TEMP_LOW_112},	{TEMP_LOW_119,	TEMP_LOW_117},		//120℃~130℃
	
	{TEMP_LOW_123,	TEMP_LOW_121},	{TEMP_LOW_128,	TEMP_LOW_126},	{TEMP_HIG_133,	TEMP_HIG_131},		//135℃~145℃
	
	{TEMP_HIG_138,	TEMP_HIG_136},	{TEMP_HIG_143,	TEMP_HIG_141},	{TEMP_HIG_148,	TEMP_HIG_146},		//150℃~160℃
	
	{TEMP_HIG_153,	TEMP_HIG_151},	{TEMP_HIG_158,	TEMP_HIG_156},	{TEMP_HIG_163,	TEMP_HIG_161},		//165℃~175℃
	
	{TEMP_HIG_168,	TEMP_HIG_166},	{TEMP_HIG_172,	TEMP_HIG_170},	{TEMP_HIG_175,	TEMP_HIG_173},		//180℃~190℃
	
	{TEMP_HIG_178,	TEMP_HIG_176},	{TEMP_HIG_182,	TEMP_HIG_180},										//195℃~200℃
};

/*纯蒸功能预热步骤控温参数查询表*/
unsigned int code OnlySteamPreHeat_Table[7]=
{
	TEMP_LOW_071,	TEMP_LOW_074,	TEMP_LOW_077,	TEMP_LOW_080,	TEMP_LOW_080,	TEMP_LOW_060,	TEMP_LOW_060,	//80℃~110℃
};

/*纯蒸功能区间控温参数查询表*/
unsigned int code OnlySteamRegionHeat_Table[7][3]=
{
//		开启点			关闭点			功率档位
	TEMP_LOW_080,	TEMP_LOW_081,	PLATE_GEAR_6_20,	//80℃
	
	TEMP_LOW_083,	TEMP_LOW_084,	PLATE_GEAR_8_20,	//85℃
	
	TEMP_LOW_086,	TEMP_LOW_087,	PLATE_GEAR_10_20,	//90℃
	
	TEMP_LOW_091,	TEMP_LOW_092,	PLATE_GEAR_12_20,	//95℃
	
	TEMP_HIG_199,	TEMP_HIG_200,	PLATE_GEAR_14_20,	//100℃		一直喷蒸汽
	
	/*卤素管辅助加热挡*/
	TEMP_LOW_102,	TEMP_LOW_103,	PLATE_GEAR_14_20,	//105℃
	
	TEMP_LOW_102,	TEMP_LOW_103,	PLATE_GEAR_14_20,	//110℃
};

/*蒸汽烤功能区间控温参数查询表*/
unsigned int code SteamBakeRegionHeat_Table[21]=
{
	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	//100℃~120℃
	
	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	//125℃~145℃
	
	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	//150℃~170℃

	PLATE_GEAR_6_20,	PLATE_GEAR_6_20,	PLATE_GEAR_6_20,	PLATE_GEAR_6_20,	PLATE_GEAR_8_20,	//175℃~195℃
	
	PLATE_GEAR_8_20,																					//200℃
};


/*-----------------------------------------------------------------------------
Description:		初始化负载IO（控制板的负载只有散热风扇）预留
					LED灯：			P2.4
					摄像头风扇：	P2.5
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitLoadIo(void)
{
	BANK0_SET;
	
	P2CR |= (BIT4 | BIT5);											//输出，不带上拉，输出低
	P2PCR &= (~(BIT4 | BIT5));
	P2 &= (~(BIT4 | BIT5));
}

/*-----------------------------------------------------------------------------
Description:		初始化负载变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitLoadVariable(void)
{
	loadCrlData.tubeHeatTempOn	= 0x3ff;
	loadCrlData.tubeHeatTempOff = 0x3ff;
	loadCrlData.steamHeatTempOn = 0x3ff;
	loadCrlData.steamHeatTempOff= 0x3ff;
	loadCrlData.corepreHeatTemp = 0x3ff;
	
	loadCrlData.coreNowAd		= 0;
	loadCrlData.coreLastAd		= 0;
	loadCrlData.SteamLastAd 	= 0;
	
	loadCrlData.preHeatDelayCnt = 0;
	loadCrlData.heatJudgeCycCnt = HEAT_JUDGE_TIME - 1;		//启动后立即判温度
	loadCrlData.pumpJudgeCycCnt = 0;
	loadCrlData.forceSubWaterCnt= 0;
	
	loadCrlData.plateHeatGear 	= 0;
	loadCrlData.pumpFreGear		= 0;
	
	tempZoneFlag	= 0;
	tempTrendFlag	= 1;									//启动时希望温度上升
	
	g_corePreHeatFlag		= 0;
	g_corePreHeatDelayFlag	= 0;
	g_corePreHeatFirstFlag	= 1;
	highSteamFirstHeatFlag	= 1;							//启动时默认高温纯蒸首次加热完成（若启动检测到温度低会重新清零首次加热）
	g_steamPreHeatFlag		= 0;
	
	tubeHeatFlag			= 0;
	fanOpenFlag				= 0;
	steamOpenFlag			= 0;
	sanReOpenFlag			= 0;
}

/*-----------------------------------------------------------------------------
Description:		加载控温参数
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void LoadHeatCrlData(void)
{
	if(g_nowStepTechnology == ONLY_BAKE)					//空气炸工艺
	{
		loadCrlData.corepreHeatTemp		= (OnlyBakePreHeat_Table[(g_nowStepworkTemp - 30)/5]);
		loadCrlData.tubeHeatTempOn  	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 30)/5][0]);
		loadCrlData.tubeHeatTempOff 	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 30)/5][1]);
		loadCrlData.plateHeatGear		= 0;
	}
	else if(g_nowStepTechnology == ONLY_STEAM)				//原味蒸工艺
	{
		loadCrlData.corepreHeatTemp 	= (OnlySteamPreHeat_Table[(g_nowStepworkTemp - 80)/5]);
		loadCrlData.steamHeatTempOn 	= (OnlySteamRegionHeat_Table[(g_nowStepworkTemp - 80)/5][0]);
		loadCrlData.steamHeatTempOff	= (OnlySteamRegionHeat_Table[(g_nowStepworkTemp - 80)/5][1]);
		
		if(measureData.coreAdLValue >= loadCrlData.corepreHeatTemp)				//中心温度低于预热点，锅炉全功率加热
		{
			
			/*小批试产E06问题更改程序20191027*/
			if(g_pumpCalibrationFlag == 0)				//水泵电流未标定前
			{
				if(g_reachStopHeatFlag == 0)			//未到达强关点时，锅炉高功率加热
				{
					loadCrlData.plateHeatGear = PLATE_GEAR_MAX;
				}
				else
				{
					loadCrlData.plateHeatGear = 0;
				}
			}
			else
			{
				loadCrlData.plateHeatGear	= PLATE_GEAR_14_20;
			}
			
		}
		else												//中心温度高于预热点
		{
			if((g_adDataHSteam > STEAM_PRE_HEAT_AD) || (g_pumpCalibrationFlag == 0))	//锅炉温度低于预热点或水泵电流未标定时，锅炉全功率加热
			{
				
				/*小批试产E06问题更改程序20191027*/
				if(g_pumpCalibrationFlag == 0)				//水泵电流未标定前
				{
					if(g_reachStopHeatFlag == 0)			//未到达强关点时，锅炉高功率加热
					{
						loadCrlData.plateHeatGear = PLATE_GEAR_MAX;
					}
					else
					{
						loadCrlData.plateHeatGear = 0;
					}
				}
				else
				{
					loadCrlData.plateHeatGear	= PLATE_GEAR_14_20;
				}
				
			}
			else											//温度高于锅炉预热点且水泵已标定时，锅炉按设定功率加热
			{
				if((g_nowStepworkTemp >= 100) && (g_steamLowPowerFlag == 1))	//若当前菜单纯蒸温度≥100℃，且锅炉低功率输出标志为1
				{
					loadCrlData.plateHeatGear = PLATE_GEAR_10_20;				//锅炉按50%功率输出
				}
				else										//纯蒸100℃以下或高功率输出时间未超过最大值，则按设定功率输出
				{
					loadCrlData.plateHeatGear = (OnlySteamRegionHeat_Table[(g_nowStepworkTemp - 80)/5][2]);
				}
			}
		}
	}
	else if(g_nowStepTechnology == BAKE_STEAM)				//蒸汽烤工艺
	{
		loadCrlData.corepreHeatTemp 	= (OnlyBakePreHeat_Table[(g_nowStepworkTemp - 100)/5 + 14]);
		loadCrlData.tubeHeatTempOn  	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 100)/5 + 14][0]);
		loadCrlData.tubeHeatTempOff 	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 100)/5 + 14][1]);
		
		if((g_adDataHSteam > STEAM_PRE_HEAT_AD) || (g_pumpCalibrationFlag == 0))	//锅炉温度低于预热点或水泵电流未标定时，锅炉全功率加热
		{
			
			/*小批试产E06问题更改程序20191027*/
			if(g_pumpCalibrationFlag == 0)				//水泵电流未标定前
			{
				if(g_reachStopHeatFlag == 0)			//未到达强关点时，锅炉高功率加热
				{
					loadCrlData.plateHeatGear = PLATE_GEAR_MAX;
				}
				else
				{
					loadCrlData.plateHeatGear = 0;
				}
			}
			else
			{
				loadCrlData.plateHeatGear	= PLATE_GEAR_16_20;
			}
		}
		else												//温度高于锅炉预热点或水泵电流已标定时，锅炉按设定功率加热
		{
			loadCrlData.plateHeatGear	= (SteamBakeRegionHeat_Table[(g_nowStepworkTemp - 100)/5]);
		}
	}
	else													//无工艺状态
	{
		loadCrlData.corepreHeatTemp	= 0x3ff;
		loadCrlData.tubeHeatTempOn	= 0x3ff;
		loadCrlData.tubeHeatTempOff	= 0x3ff;
		loadCrlData.plateHeatGear	= 0;
	}
}

/*-----------------------------------------------------------------------------
Description:		卤素管间歇加热控制
Input:				funCnt：风扇提前开启时间
					val：	负载开启时间
					T:		负载开断判断周期
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TubeInterimHeat(unsigned char funCnt, unsigned char val, unsigned char T)//（风扇/卤素管开启时间，总周期）
{
	static unsigned char s_cnt = 0;

	if((val == 0) || (T == 0))
	{
		s_cnt = 0;
		
		tubeHeatFlag = CLOSE_LOAD;							//关卤素管
		fanOpenFlag  = CLOSE_LOAD;							//关罩极风扇
	}
	else
	{
		s_cnt++;
		if(s_cnt <= funCnt)									//加热前先开风扇，2s时基
		{
			fanOpenFlag  = OPEN_LOAD;						//开罩极风扇
		}
		else if(s_cnt <= (val + funCnt))
		{
			tubeHeatFlag = OPEN_LOAD;						//开卤素管
			fanOpenFlag  = OPEN_LOAD;						//开罩极风扇
		}
		else
		{
			tubeHeatFlag = CLOSE_LOAD;						//关卤素管
			fanOpenFlag  = CLOSE_LOAD;						//关罩极风扇
			
			if(s_cnt >= T)									//计数周期结束循环
			{
				s_cnt = 0;
				
				if(highSteamFirstHeatFlag == 0)				//若是首次周期结束，首次标志位置一
				{
					highSteamFirstHeatFlag = 1;
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		所有输出关闭
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void CloseAllOutPut(void)
{
	/*标志位和档位清零*/
	tubeHeatFlag	= 0;
	fanOpenFlag		= 0;
	steamOpenFlag	= 0;
	sanReOpenFlag	= 0;
	loadCrlData.plateHeatGear 	= 0;
	loadCrlData.pumpFreGear		= 0;

	/*发送数据清零*/
	SEND_OUT_DATA1		= 0;								//关闭所有输出
	SEND_OUT_DATA_STEAM = 0;
	SEND_OUT_DATA_PUMP	= 0;
}

/*-----------------------------------------------------------------------------
Description:		加热控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void HeatCrl(void)
{
	static unsigned char s_time250msCnt = 0;				//250ms计时

	LoadHeatCrlData();										//加载控温参数（水泵电流标定完成后及时调整抽水档位）

	s_time250msCnt++;
	if(s_time250msCnt >= 25)								//10ms时基，250ms判断一次
	{
		s_time250msCnt = 0;

		if(g_nowStepTechnology == ONLY_BAKE)				//空气炸工艺
		{
			OnlyBakeHeatCrl();								//空气炸温度控制
		}
		else if(g_nowStepTechnology == ONLY_STEAM)			//纯蒸工艺
		{
			OnlySteamHeatCrl();								//纯蒸温度控制
		
			/*小批试产E06问题更改程序20191027*/
			if(g_pumpCalibrationFlag == 0)					//水泵电流未标定前
			{
				if(g_reachStopHeatFlag == 0)				//未到达强关点时，锅炉开启
				{
					steamOpenFlag = OPEN_LOAD;				//开启锅炉
				}
				else										//到达强关点后，锅炉关闭
				{
					steamOpenFlag = CLOSE_LOAD;
				}
			}
			
		}
		else if(g_nowStepTechnology == BAKE_STEAM)			//蒸汽烤工艺
		{
			SteamBakeHeatCrl();								//蒸烤温度控制
			
			/*小批试产E06问题更改程序20191027*/
			if(g_pumpCalibrationFlag == 0)					//水泵电流未标定前
			{
				if(g_reachStopHeatFlag == 0)				//未到达强关点时，锅炉开启
				{
					steamOpenFlag = OPEN_LOAD;				//开启锅炉
				}
				else										//到达强关点后，锅炉关闭
				{
					steamOpenFlag = CLOSE_LOAD;
				}
			}
			
		}
		else												//无工艺状态
		{
			CloseAllOutPut();								//关闭所有输出
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		空气炸加热控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void OnlyBakeHeatCrl(void)
{
	if(g_corePreHeatFlag == 0)								//还未预热，进行预热
	{
		OnlyBakePreHeatCrl();								//执行空气炸的预热子程序
	}
	else													//预热完成后进入控温阶段
	{
		OnlyBakeRegionHeatCrl();							//执行空气炸区间控温子程序
	}
	
	if(g_firstAdGetGlag)									//若得到了首次腔体AD采样值
	{
		fanOpenFlag = OPEN_LOAD;							//空气炸功能全程开罩极风扇
	}
	else
	{
		fanOpenFlag = CLOSE_LOAD;							//掉电期间关闭罩极风扇
	}
	steamOpenFlag	= CLOSE_LOAD;							//全程锅炉关闭
}

/*-----------------------------------------------------------------------------
Description:		纯蒸加热控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void OnlySteamHeatCrl(void)
{
	if(g_corePreHeatFlag == 0)								//还未预热，进行预热
	{
		OnlySteamPreHeatCrl();								//执行纯蒸的预热子程序
		
		tubeHeatFlag = CLOSE_LOAD;							//预热阶段发热管关闭
		fanOpenFlag	 = CLOSE_LOAD;							//预热阶段罩极电机风扇关闭
	}
	else													//预热完成后进入控温阶段
	{
		OnlySteamRegionHeatCrl();							//执行纯蒸区间控温子程序
	}
}

/*-----------------------------------------------------------------------------
Description:		蒸汽烤加热控制（控温方式同空气烤功能，额外增加不同功率的纯蒸功能）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SteamBakeHeatCrl(void)
{
	if(g_corePreHeatFlag == 0)								//还未预热，进行预热
	{
		OnlyBakePreHeatCrl();								//执行空气炸的预热子程序
	}
	else													//预热完成后进入控温阶段
	{
		OnlyBakeRegionHeatCrl();							//执行空气炸区间控温子程序
	}

	if(g_firstAdGetGlag)									//若得到了首次腔体AD采样值
	{
		fanOpenFlag = OPEN_LOAD;							//全程开罩极风扇
	}
	else
	{
		fanOpenFlag = CLOSE_LOAD;							//掉电期间关闭罩极风扇
	}

	steamOpenFlag	= OPEN_LOAD;							//全程开启锅炉
}

/*-----------------------------------------------------------------------------
Description:		空气炸预热子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void OnlyBakePreHeatCrl(void)
{
	unsigned int tubePreHeatTempBuf;
	
	static unsigned char s_onTimeCnt = 0;					//预热间歇加热的启动时间计数
	static bit 			 s_tubeHeatState = 0;				//卤素管加热状态

	if(g_corePreHeatDelayFlag == 1)							//预热延时中（不加热）
	{
		tubeHeatFlag = CLOSE_LOAD;							//关卤素管
		
		loadCrlData.preHeatDelayCnt++;
		if(loadCrlData.preHeatDelayCnt >= PRE_DEALY_TIME)	//预热延迟40*250ms=10s
		{
			g_corePreHeatFlag				= 1;			//预热及预热延时完成
			g_corePreHeatDelayFlag			= 0;			//不在预热延时状态（预热延时结束）
			g_corePreHeatFirstFlag			= 1;			//完成首次预热
			loadCrlData.preHeatDelayCnt		= 0;
			
			tempTrendFlag = 1;								//预热完温度上升趋势
		}
	}
	else													//不在预热延时状态
	{
		if((loadCrlData.corepreHeatTemp & 0x8000) == 0x8000)	//处于高温区
		{
			tubePreHeatTempBuf = loadCrlData.corepreHeatTemp & 0x7FFF;
			
			loadCrlData.coreNowAd = measureData.coreAdHValue;	//赋值高温区AD
			
			if(tempZoneFlag == 0)							//查看是否存在温区切换状态
			{
				loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//若上次处于低温区，将现在的AD值赋值给上次的值
			}
			tempZoneFlag = 1;
		}
		else												//处于低温区
		{
			tubePreHeatTempBuf = loadCrlData.corepreHeatTemp;
			
			loadCrlData.coreNowAd = measureData.coreAdLValue;	//赋值低温区AD
			
			if(tempZoneFlag == 1)							//查看是否存在温区切换状态
			{
				loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//若上次处于低温区，将现在的AD值赋值给上次的值
			}
			tempZoneFlag = 0;
		}

		if(g_corePreHeatFirstFlag == 1)						//非首次预热
		{
			if(loadCrlData.coreNowAd > tubePreHeatTempBuf)	//当前温度低于预热点温度
			{
				tubeHeatFlag = OPEN_LOAD;					//开启卤素管
				
				g_corePreHeatFlag			= 0;
				g_corePreHeatDelayFlag		= 0;
				g_corePreHeatFirstFlag		= 0;			//重新进行首次预热
				loadCrlData.preHeatDelayCnt	= 0;
			}
			else											//当前温度达到，跳过预热延迟
			{
				tubeHeatFlag = CLOSE_LOAD;					//关卤素管
			
				g_corePreHeatFlag			= 1;			//直接结束预热阶段
				g_corePreHeatDelayFlag		= 0;
				g_corePreHeatFirstFlag		= 1;
				loadCrlData.preHeatDelayCnt	= 0;
				
				tempTrendFlag = 1;							//预热完温度上升趋势
			}
		}
		else												//首次预热
		{
			if(loadCrlData.coreNowAd > tubePreHeatTempBuf)	//当前温度低于预热温度
			{
				tubeHeatFlag = OPEN_LOAD;					//开卤素管
				
				g_corePreHeatFlag			= 0;
				g_corePreHeatDelayFlag		= 0;
				g_corePreHeatFirstFlag		= 0;
				loadCrlData.preHeatDelayCnt	= 0;
			}
			else
			{
				tubeHeatFlag = CLOSE_LOAD;				//关卤素管
				
				g_corePreHeatFlag			= 0;
				g_corePreHeatDelayFlag		= 1;
				g_corePreHeatFirstFlag		= 0;
				loadCrlData.preHeatDelayCnt	= 0;
			}
		}
		
		/*针对冲温过高问题，预热阶段使用间歇加热*/
		if(measureData.coreAdHValue < (TEMP_HIG_140 & 0x7FFF))	//预热阶段温度高于150℃后，采用间歇加热（此处需考虑热烘除菌功能的全功率时间）
		{
			if(tubeHeatFlag == OPEN_LOAD)						//若卤素管开启
			{
				s_onTimeCnt++;
				if(s_onTimeCnt <= 12)							//250ms时基，按周期加热一段时间停一段时间
				{
					s_tubeHeatState = OPEN_LOAD;
				}
				else
				{
					s_tubeHeatState = CLOSE_LOAD;
				}
				
				if(s_onTimeCnt >= 24)
				{
					s_onTimeCnt = 0;
				}
			}
			else												//若禁止卤素管加热，则周期计时清零
			{
				s_onTimeCnt		= 0;
				s_tubeHeatState = CLOSE_LOAD;
			}
			
			if(s_tubeHeatState == OPEN_LOAD)					//若卤素管状态判定为开启，则开卤素管
			{
				tubeHeatFlag = OPEN_LOAD;
			}
			else												//若卤素管状态判定为关闭，则关卤素管
			{
				tubeHeatFlag = CLOSE_LOAD;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		空气炸控温阶段子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void OnlyBakeRegionHeatCrl(void)
{
	unsigned int tubeHeatTempOnBuf;
	unsigned int tubeHeatTempOffBuf;
	
	unsigned char judgeFre = 0;

	if((loadCrlData.tubeHeatTempOn & 0x8000) == 0x8000)		//处于高温区
	{
		tubeHeatTempOnBuf	= loadCrlData.tubeHeatTempOn & 0x7FFF;
		tubeHeatTempOffBuf	= loadCrlData.tubeHeatTempOff & 0x7FFF;
		
		loadCrlData.coreNowAd = measureData.coreAdHValue;	//赋值高温区AD
		
		if(tempZoneFlag == 0)								//查看是否存在温区切换状态
		{
			loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//若上次处于低温区，将现在的AD值赋值给上次的值
		}
		tempZoneFlag = 1;
	}
	else													//处于低温区
	{
		tubeHeatTempOnBuf	= loadCrlData.tubeHeatTempOn;
		tubeHeatTempOffBuf	= loadCrlData.tubeHeatTempOff;
		
		loadCrlData.coreNowAd = measureData.coreAdLValue;	//赋值低温区AD
		
		if(tempZoneFlag == 1)								//查看是否存在温区切换状态
		{
			loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//若上次处于低温区，将现在的AD值赋值给上次的值
		}
		tempZoneFlag = 0;
	}

	/*区间控温*/
	if(g_nowStepworkTemp < 100)
	{
		judgeFre = HEAT_JUDGE_TIME;
	}
	else
	{
		judgeFre = HEAT_JUDGE_TIME_LONG;
	}
	
	loadCrlData.heatJudgeCycCnt++;
	if(loadCrlData.heatJudgeCycCnt >= judgeFre)				//加热每周期判断一次
	{
		loadCrlData.heatJudgeCycCnt = 0;

		/*	此处控温程序与下面一段控温程序等效
		if(loadCrlData.coreNowAd >= tubeHeatTempOffBuf )	//当前温度比关闭点还低
		{
			tubeHeatFlag = OPEN_LOAD;						//开卤素管
			tempTrendFlag = 1;								//温度上升状态
		}
		else if(loadCrlData.coreNowAd <= tubeHeatTempOnBuf)	//当前温度比开启点还高
		{
			tubeHeatFlag = CLOSE_LOAD;						//关卤素管
			tempTrendFlag = 0;								//温度下降状态
		}
		else												//中间温区
		{
			if(tempTrendFlag)								//温度为上升状态
			{
				if(loadCrlData.coreNowAd < tubeHeatTempOffBuf)	//温度到达关闭点
				{
					tubeHeatFlag = CLOSE_LOAD;				//关卤素管
					tempTrendFlag = 0;						//温度下降状态
				}
			}
			else											//温度为下降状态
			{
				if(loadCrlData.coreNowAd > tubeHeatTempOnBuf)	//温度低于开启点
				{
					tubeHeatFlag = OPEN_LOAD;	//开卤素管
					tempTrendFlag = 1;						//温度上升状态
				}
			}
		}
		
		loadData.coreLastAd = loadData.coreNowAd;			//更新上次AD值
		*/
		
		if(loadCrlData.coreNowAd > (tubeHeatTempOffBuf + 3))//当前温度比关闭点还低
		{
			tubeHeatFlag = OPEN_LOAD;						//开卤素管
			tempTrendFlag = 1;								//温度上升状态
		}
		else if(loadCrlData.coreNowAd < (tubeHeatTempOnBuf - 3))	//当前温度比开启点还高
		{
			tubeHeatFlag = CLOSE_LOAD;						//关卤素管
			tempTrendFlag = 0;								//温度下降状态
		}
		else												//中间温区
		{
			if(tempTrendFlag)								//温度为上升状态
			{
				if(loadCrlData.coreNowAd >= loadCrlData.coreLastAd)	//温度下降或不变
				{
					tubeHeatFlag = OPEN_LOAD;				//开卤素管
					tempTrendFlag = 1;						//温度上升状态
				}
				else										//温度上升
				{
					if(loadCrlData.coreNowAd <= tubeHeatTempOffBuf)	//温度到达关闭点
					{
						tubeHeatFlag = CLOSE_LOAD;			//关卤素管
						tempTrendFlag = 0;					//温度下降状态
					}
					else									//温度未到达关闭点
					{
						tubeHeatFlag = OPEN_LOAD;			//开卤素管
						tempTrendFlag = 1;					//温度上升状态
					}
				}
			}
			else											//温度为下降状态
			{
				if(loadCrlData.coreNowAd >= loadCrlData.coreLastAd)	//温度下降或不变
				{
					if(loadCrlData.coreNowAd >= tubeHeatTempOnBuf)	//温度到达开启点
					{
						tubeHeatFlag = OPEN_LOAD;			//开卤素管
						tempTrendFlag = 1;					//温度上升状态
					}
					else									//温度大于关闭点
					{
						tubeHeatFlag = CLOSE_LOAD;			//关卤素管
						tempTrendFlag = 0;					//温度下降状态
					}
				}
				else										//温度上升
				{
					tubeHeatFlag = CLOSE_LOAD;				//关卤素管
					tempTrendFlag = 0;						//温度下降状态
				}
			}
		}
		
		loadCrlData.coreLastAd = loadCrlData.coreNowAd;		//更新上次AD值
	}
}

/*-----------------------------------------------------------------------------
Description:		纯蒸预热子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void OnlySteamPreHeatCrl(void)
{
	if(measureData.coreAdLValue < loadCrlData.corepreHeatTemp)	//到达预热温度点
	{
		g_corePreHeatFlag = 1;								//标记预热完成

		g_steamPreHeatFlag = 0;								//重新判定锅炉是否预热（重新赋值抽水默认值）
	}
	else
	{
		highSteamFirstHeatFlag = 0;							//清零高温纯蒸首次加热标志（重新进行高温纯蒸首次加热）
		
		steamOpenFlag = OPEN_LOAD;							//开启锅炉
	}
}

/*-----------------------------------------------------------------------------
Description:		纯蒸控温阶段子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void OnlySteamRegionHeatCrl(void)
{
	unsigned int steamHeatTempOnBuf;
	unsigned int steamHeatTempOffBuf;
	static	 bit tubeInterimHeatFlag = 0;

	if((loadCrlData.steamHeatTempOn & 0x8000) == 0x8000)	//处于高温区
	{
		steamHeatTempOnBuf	= loadCrlData.steamHeatTempOn & 0x7FFF;
		steamHeatTempOffBuf	= loadCrlData.steamHeatTempOff & 0x7FFF;
		
		loadCrlData.coreNowAd = measureData.coreAdHValue;	//赋值高温区AD
	}
	else													//处于低温区
	{
		steamHeatTempOnBuf	= loadCrlData.steamHeatTempOn;
		steamHeatTempOffBuf	= loadCrlData.steamHeatTempOff;
		
		loadCrlData.coreNowAd = measureData.coreAdLValue;	//赋值低温区AD
	}
	
	/*区间控温*/
	loadCrlData.heatJudgeCycCnt++;
	if(loadCrlData.heatJudgeCycCnt >= HEAT_JUDGE_TIME)		//加热2S判断一次
	{
		loadCrlData.heatJudgeCycCnt = 0;
		
		if(g_nowStepworkTemp > 100)							//纯蒸功能设定温度高于100℃时，卤素管辅助加热，一直出蒸汽
		{
			if(loadCrlData.coreNowAd > steamHeatTempOnBuf)	//当前温度比开启点还低
			{
				tubeInterimHeatFlag = 1;					//开卤素管间歇加热
				tempTrendFlag = 1;							//温度上升状态
			}
			else if(loadCrlData.coreNowAd < steamHeatTempOffBuf)//当前温度比关闭点还高
			{
				tubeInterimHeatFlag = 0;					//关卤素管间歇加热
				tempTrendFlag = 0;							//温度下降状态
			}
			else
			{
				if(tempTrendFlag)
				{
					tubeInterimHeatFlag = 1;				//开卤素管间歇加热
					tempTrendFlag = 1;						//温度上升状态
				}
				else
				{
					tubeInterimHeatFlag = 0;				//关卤素管间歇加热
					tempTrendFlag = 0;						//温度下降状态
				}
			}
			
			if(tubeInterimHeatFlag)							//卤素管需要开启间歇加热
			{
				if(highSteamFirstHeatFlag == 0)				//未进行首次加热
				{
					TubeInterimHeat(5,3,15);				//赋值风扇提前时间，加热时间和周期，2s时基
				}
				else
				{
					if(g_nowStepworkTemp == 105)			//控温温度为105℃时
					{
						TubeInterimHeat(5,1,50);			//赋值风扇提前时间，加热时间和周期，2s时基
					}
					else if(g_nowStepworkTemp == 110)
					{
						TubeInterimHeat(5,1,20);			//赋值风扇提前时间，加热时间和周期，2s时基
					}
					else
					{
						TubeInterimHeat(0,0,0);				//赋值风扇提前时间，加热时间和周期都为0（卤素管不加热）
					}
				}
			}
			else
			{
				TubeInterimHeat(0,0,0);						//赋值风扇提前时间，加热时间和周期都为0（卤素管不加热）
			}
			
			steamOpenFlag = OPEN_LOAD;						//一直开启锅炉
		}
		else												//纯蒸功能80℃~100℃，只出蒸汽控温
		{
			if(loadCrlData.coreNowAd > steamHeatTempOnBuf)	//当前温度比开启点还低
			{
				steamOpenFlag = OPEN_LOAD;					//开启锅炉
				tempTrendFlag = 1;							//温度上升状态
			}
			else if(loadCrlData.coreNowAd < steamHeatTempOffBuf)//当前温度比关闭点还高
			{
				steamOpenFlag = CLOSE_LOAD;					//关闭锅炉
				tempTrendFlag = 0;							//温度下降状态
			}
			else
			{
				if(tempTrendFlag)
				{
					steamOpenFlag = OPEN_LOAD;				//开启锅炉
					tempTrendFlag = 1;						//温度上升状态
				}
				else
				{
					steamOpenFlag = CLOSE_LOAD;				//关闭锅炉
					tempTrendFlag = 0;						//温度下降状态
				}
			}
			
			tubeHeatFlag	= CLOSE_LOAD;					//100℃及以下纯蒸发热管关闭
			fanOpenFlag		= CLOSE_LOAD;					//100℃及以下纯蒸罩极电机风扇关闭
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		水泵加水控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void PumpCrl(void)
{
	unsigned char xdata defrentAd = 0;						//AD差值
	unsigned char xdata changeGear = 0; 					//抽水调整的档位

	/*预热阶段*/
	if(g_steamPreHeatFlag == 0)								//未预热时
	{
		if(g_adDataHSteam > STEAM_PRE_HEAT_AD)				//温度低于预热点，不加水
		{
			loadCrlData.pumpFreGear = 0;
		}
		else												//温度高于预热点
		{	
			g_steamPreHeatFlag = 1;							//结束预热阶段
			
			if(g_pumpCalibrationFlag == 0)					//水泵电流未标定前，水泵固定抽水频率
			{
				if(g_reachStopHeatFlag == 0)				//未到达强关点时，抽水频率慢
				{
					loadCrlData.pumpFreGear = 60;
				}
				else										//水泵标定到达强关点后抽水频率快
				{
					loadCrlData.pumpFreGear = 25;
				}
			}
			else
			{
				if(loadCrlData.plateHeatGear > PLATE_HIGH_GEAR)	//根据锅炉功率档位选择抽水量（数值越大，抽水频率越小）
				{
					loadCrlData.pumpFreGear = 55;
				}
				else if(loadCrlData.plateHeatGear > PLATE_MID_GEAR)
				{
					loadCrlData.pumpFreGear = 80;
				}
				else if(loadCrlData.plateHeatGear > PLATE_LOW_GEAR)
				{
					loadCrlData.pumpFreGear = 110;
				}
				else if(loadCrlData.plateHeatGear > 3)
				{
					loadCrlData.pumpFreGear = 190;
				}
				else
				{
					loadCrlData.pumpFreGear = 240;
				}
			}
		}
	}
	
	/*抽水频率自动调节阶段*/
	else
	{
		/*小批试产E06问题更改程序20191027*/
		if(g_pumpCalibrationFlag == 0)							//水泵电流未标定前，水泵固定抽水频率
		{
			if(g_reachStopHeatFlag == 0)						//未到达强关点时，抽水频率慢
			{
				loadCrlData.pumpFreGear = 60;
			}
			else												//水泵标定到达强关点后抽水频率快
			{
				loadCrlData.pumpFreGear = 25;
			}
		}
		else
		{
			loadCrlData.pumpJudgeCycCnt++;

			if(loadCrlData.pumpJudgeCycCnt >= PUMP_JUDGE_TIME)	//以一定周期判断升温/降温值
			{
				loadCrlData.pumpJudgeCycCnt = 0;

				loadCrlData.forceSubWaterCnt++;
				if(loadCrlData.forceSubWaterCnt >= 2)			//两个判断周期，强制减少抽水量
				{
					loadCrlData.forceSubWaterCnt = 0;
					if(loadCrlData.pumpFreGear < PUMP_GEAR_MAX)
					{
						loadCrlData.pumpFreGear++;				//强制减少抽水量
					}
					else
					{
						loadCrlData.pumpFreGear = PUMP_GEAR_MAX;
					}
				}

				if(g_adDataHSteam < STEAM_MAX_TEMP_AD)			//锅炉温度高于强制关闭温度
				{
					if(loadCrlData.pumpFreGear > (PUMP_GEAR_MIN + 5))		//保证不小于最小抽水档位（档位越小，抽水越多）
					{
						loadCrlData.pumpFreGear = loadCrlData.pumpFreGear - 5;		//大幅加快抽水频率
					}
					else
					{
						loadCrlData.pumpFreGear = PUMP_GEAR_MIN;
					}
				}

				else if(g_adDataHSteam > STEAM_LOW_TEMP_AD)		//锅炉温度较低时
				{
					if(loadCrlData.pumpFreGear < (PUMP_GEAR_MAX - 8))	//保证不大于最大抽水档位
					{
						loadCrlData.pumpFreGear = loadCrlData.pumpFreGear + 8;		//大幅减少抽水频率
					}
					else
					{
						loadCrlData.pumpFreGear = PUMP_GEAR_MAX;
					}
				}

				else											//若当前盘温处在正常控制温度区间，则按变化率微调
				{
					if(loadCrlData.SteamLastAd != 0)			//首次不进行判断
					{
						/*降温则微调减慢抽水频率*/
						if(g_adDataHSteam > loadCrlData.SteamLastAd)		//当前降温
						{
							defrentAd = g_adDataHSteam - loadCrlData.SteamLastAd;
							changeGear = defrentAd/4;
							if(changeGear > 2)
							{
								changeGear = 2;					//限制抽水减慢速度
							}
							
							if(loadCrlData.pumpFreGear < (PUMP_GEAR_MAX - changeGear))//保证大于最大抽水档位
							{
								loadCrlData.pumpFreGear = loadCrlData.pumpFreGear + changeGear;	//抽水频率减慢
							}
							else
							{
								loadCrlData.pumpFreGear = PUMP_GEAR_MAX;
							}
						}
			
						/*升温则微调加快抽水频率*/
						else if(g_adDataHSteam < loadCrlData.SteamLastAd)	//当前升温
						{
							defrentAd = loadCrlData.SteamLastAd - g_adDataHSteam;
							changeGear = defrentAd/4;
							if(changeGear >= 3)
							{
								changeGear = 3;					//限制抽水加快速度（加快速度较快，让锅炉温度不至于过高）
							}
							
							if(loadCrlData.pumpFreGear > (PUMP_GEAR_MIN + changeGear))//保证不小于最小抽水档位
							{
								loadCrlData.pumpFreGear = loadCrlData.pumpFreGear - changeGear;	//抽水频率加快
							}
							else
							{
								loadCrlData.pumpFreGear = PUMP_GEAR_MIN;
							}
						}
					}
				}

				loadCrlData.SteamLastAd = g_adDataHSteam;		//刷新锅炉上次AD值
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		负载的启动信息传递给发送数据
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void LoadMessageDeliver(void)
{
	if(tubeHeatFlag)										//卤素管允许开启则赋值开启信息，否则赋值关闭信息
	{
		SEND_OUT_DATA1 |= BIT_HOT;
	}
	else
	{
		SEND_OUT_DATA1 &= ~BIT_HOT;
	}
	
	if(fanOpenFlag)											//罩极风扇允许开启则赋值开启信息，否则赋值关闭信息
	{
		SEND_OUT_DATA1 |= BIT_FAN;
	}
	else
	{
		SEND_OUT_DATA1 &= ~BIT_FAN;
	}
	
	if(sanReOpenFlag)										//电源板散热风扇允许开启则赋值开启信息，否则赋值关闭信息
	{
		SEND_OUT_DATA1 |= BIT_SANRE;
	}
	else
	{
		SEND_OUT_DATA1 &= ~BIT_SANRE;
	}
	
	if(steamOpenFlag)										//锅炉允许开启则赋值锅炉的加热档位，否则赋值关闭信息
	{
		SEND_OUT_DATA_STEAM = loadCrlData.plateHeatGear;
	}
	else
	{
		SEND_OUT_DATA_STEAM = 0;
	}

	SEND_OUT_DATA_PUMP = loadCrlData.pumpFreGear;			//赋值水泵的抽水频率信息
}

/*-----------------------------------------------------------------------------
Description:		所有输出负载控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void LoadCrl(void)
{
	static unsigned char s_pumpFreGeartemp		= 0;				//腔体温度过高关闭水泵时，水泵档位缓存值
	static unsigned char s_maybeLackPumpTemp	= 0;				//可能缺水时，水泵档位缓存值
	static bit			 s_maybeLackWaterFlag 	= 0;				//可能缺水标志

	/*工作或保温状态下*/
	if((g_sysType == SysModeWork) || (g_sysType == SysModeWarm))
	{
		/*无可恢复报警（合盖、不缺水）时正常启动工作*/
		if((g_LidFlag == LID_CLOSE) && (g_waterYieldType == NormalType))
		{
			/*加热控制*/
			HeatCrl();
			
			if(s_maybeLackWaterFlag)							//若之前有可能缺水报警
			{
				loadCrlData.pumpFreGear = s_maybeLackPumpTemp;	//赋值原先抽水档位

				g_quickPumpingFlag		= 0;					//快抽标志清零
			}
			s_maybeLackWaterFlag = 0;							//可能缺水标志清零
			
			/*水泵档位控制*/
			if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))	//带蒸工艺时
			{
				/*小批试产E06问题更改程序20191027*/
				if(g_pumpCalibrationFlag == 0)					//水泵电流未标定前，水泵固定抽水频率
				{
					PumpCrl();
				}
				else
				{
					if(steamOpenFlag)							//若锅炉为启动状态（锅炉温度过高强制停止仍有效）
					{
						//锅炉开启时，若预热完成且水泵为关闭状态（到达关闭点关闭后重新回到开启点的状态），则赋值水泵档位缓存值
						if((g_steamPreHeatFlag == 1) && (loadCrlData.pumpFreGear == 0))
						{
							loadCrlData.pumpFreGear = s_pumpFreGeartemp;
						}
						
						PumpCrl();								//水泵加水量调节
					}
					else										//锅炉不允许启动状态（带蒸功能中心温度过高）
					{
						if(loadCrlData.pumpFreGear != 0)		//锅炉为关闭状态，若水泵档位不为零
						{
							if(g_adDataHSteam > STEAM_MAX_TEMP_AD)	//锅炉温度低于强制关闭温度时，先暂存，再清零
							{
								s_pumpFreGeartemp = loadCrlData.pumpFreGear;
								loadCrlData.pumpFreGear = 0;
							}
							else
							{
								//若当前锅炉不允许启动，水泵档位不为零，锅炉温度高于强关点温度，则抽水直到温度低于强关点
							}
						}
						else
						{
							loadCrlData.pumpFreGear = 0;
						}
					}
				}
			}
			else
			{
				loadCrlData.pumpFreGear = 0;
			}
		}
		else													//出现可恢复报警时，先记录水泵当前档位再清零
		{
			if(g_waterYieldType == MabyLackWater)				//若是可能缺水报警
			{
				if(s_maybeLackWaterFlag == 0)					//若之前无可能缺水报警
				{
					s_maybeLackPumpTemp = loadCrlData.pumpFreGear;	//暂存当前抽水档位
				}
				s_maybeLackWaterFlag = 1;						//可能缺水标志置一
			
				if(g_quickPumpingFlag)
				{
					loadCrlData.pumpFreGear = QUICK_PUMPING;	//取消缺水报警后，快速抽水
				}
				else
				{
					loadCrlData.pumpFreGear = s_maybeLackPumpTemp;	//工作过程中缺水，按原来速度抽水
				}
				
				/*可能缺水状态下若合盖则继续工作*/
				if(g_LidFlag == LID_CLOSE)
				{
					HeatCrl();									//继续加热控制
				}
				else
				{
					InitLoadVariable();							//初始化负载的所有变量（除搅拌）
				
					s_pumpFreGeartemp 		= 0;
					s_maybeLackPumpTemp 	= 0;
					s_maybeLackWaterFlag 	= 0;				//可能缺水标志清零
				}
			}
			else
			{
				InitLoadVariable();								//初始化负载的所有变量
				
				s_pumpFreGeartemp 		= 0;
				s_maybeLackPumpTemp 	= 0;
				s_maybeLackWaterFlag 	= 0;					//可能缺水标志清零
			}
		}
		
		sanReOpenFlag = OPEN_LOAD;								//开启电源板散热风扇
	}
	else if(g_sysType == SysModeMachineCheck)					//整机自检状态时
	{
		switch(g_selfCheckStep)
		{
			case 1:
				tubeHeatFlag	= OPEN_LOAD;					//负载全功率开启（除水泵）
				fanOpenFlag		= OPEN_LOAD;
				steamOpenFlag	= OPEN_LOAD;
				sanReOpenFlag	= OPEN_LOAD;
				loadCrlData.plateHeatGear = PLATE_GEAR_MAX;
			break;
			
			case 2:
				CloseAllOutPut();								//关闭所有输出
			break;
			
			case 3:
				CloseAllOutPut();								//关闭所有输出
			break;
			
			case 4:
				CloseAllOutPut();								//关闭所有输出
			break;

			case 5:
				tubeHeatFlag = OPEN_LOAD;						//开卤素管
			break;
			
			case 6:
				tubeHeatFlag = OPEN_LOAD;						//开卤素管
			break;

			case 7:
				CloseAllOutPut();								//关闭所有输出
				steamOpenFlag = OPEN_LOAD;						//开锅炉
				loadCrlData.plateHeatGear = PLATE_GEAR_MAX;		//全功率加热
			break;

			case 8:
				CloseAllOutPut();								//关闭所有输出
				steamOpenFlag = OPEN_LOAD;						//开锅炉
				loadCrlData.plateHeatGear = PLATE_GEAR_MAX;		//全功率加热
			break;
			
			case 9:
				CloseAllOutPut();								//关闭所有输出
				fanOpenFlag 	= OPEN_LOAD;					//开罩极电机风扇
			break;
			
			case 10:
				CloseAllOutPut();								//关闭所有输出
				sanReOpenFlag = OPEN_LOAD;						//开电源板散热风扇
			break;
			
			case 11:
				CloseAllOutPut();								//关闭所有输出
				loadCrlData.pumpFreGear = PUMP_CHECK_GEAR;		//水泵按抽0.1S停1S的频率持续抽水
			break;
			
			case 12:
				CloseAllOutPut();								//关闭所有输出
			break;
			
			case 13:
				//关闭所有输出
			break;
		}
	}
	
	else if(g_sysType == SysModeClean)							//水垢清洁状态时
	{
		CloseAllOutPut();										//关闭所有输出
		
		if((g_cleanIncrustantStep == 2) && (g_cleanPumpTimeCnt == CLEAN_TIME_STEP_2 + 1))		//第二步结束状态时
		{
			CloseAllOutPut();									//关闭所有输出
		}
		else
		{
			if(g_adDataHSteam > 0x03E0)							//锅炉温度控制在75℃				
			{
				steamOpenFlag = OPEN_LOAD;						//开锅炉
				loadCrlData.plateHeatGear = PLATE_GEAR_10_20;	//半功率加热
			}
			
			if(g_cleanIncrustantStep == 1)						//清洁第一步
			{
				loadCrlData.pumpFreGear = 40;					//第一步慢抽
			}

			else if(g_cleanIncrustantStep == 2)					//清洁第二步
			{
				loadCrlData.pumpFreGear = 9;					//第二步持续抽
			}
			
			else if(g_cleanIncrustantStep == 3)					//清洁第二步
			{
				loadCrlData.pumpFreGear = 9;					//第三步持续抽
			}
		}
	}
	
	else
	{
		InitLoadVariable();										//初始化负载的所有变量

		s_pumpFreGeartemp 		= 0;
		s_maybeLackPumpTemp 	= 0;
		s_maybeLackWaterFlag 	= 0;
	}

	if(g_coolingFanDealyFlag)									//若散热风扇延时关闭中
	{
		sanReOpenFlag = OPEN_LOAD;								//开启电源板散热风扇
	}

	if(g_sysType == SysModeWork)								//工作状态下，开LED灯和摄像头散热风扇
	{
		P_CAMERA_FAN = 1;
		P_CAMERA_LED = 1;
	}
	else
	{
		P_CAMERA_FAN = 0;
		P_CAMERA_LED = 0;
	}

	/*赋值负载相关的发送信息*/
	LoadMessageDeliver();
}