#include "header.h" 

unsigned int  				g_scanDataTk[VALID_TOUCHKEY_NUMBER];				//触摸按键的扫描值
unsigned long xdata			adSumTk[VALID_TOUCHKEY_NUMBER];						//按键AD数值累加和
unsigned int  xdata			adAvgTk[VALID_TOUCHKEY_NUMBER];						//按键AD均值
unsigned int  xdata			adRefTk[VALID_TOUCHKEY_NUMBER];						//触摸按键基准值
unsigned char xdata			refCntKey[VALID_TOUCHKEY_NUMBER];					//触摸按键基准数据刷新时间计数
		 
unsigned char xdata			tkAddCnt;						//按键AD数值累加和次数

unsigned int				keyBuf;							//按键值缓存
unsigned int				keyValue;						//按键值
unsigned int				keyCnt;							//按键计时

		 bit				g_keyScanEndFlag;				//按键扫描完成标志
		 bit				keyReleaseFlag;					//按键抬起标志位
		 bit				g_adjPressFlag;					//加减键按下标志
		 bit				keyOnOffLongPressFlag;			//启动取消键长按标志
		 bit				keyOrderLongPressFlag;			//预约键长按标志

unsigned int code			TouchKey_Valid_Threshould_Table[VALID_TOUCHKEY_NUMBER] = {
//	温度-		启动/取消	时间+		预约	时间-	功能	温度+
	750,		1100,		900,		750,	750,	750,	950,
}; 

/*-----------------------------------------------------------------------------
Description:		初始触摸按键 IO
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitTouchKeyIo(void)
{
	BANK0_SET;

	P0CR &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6));			//输入
	P0PCR &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6));			//无上拉

	P1CR &= (~BIT7);			//输入
	P1PCR &= (~BIT7);			//无上拉
}

/*-----------------------------------------------------------------------------
Description:		初始化触摸按键变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitTouchKeyVariable(void)
{
	unsigned char i = 0;

	for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
	{
		g_scanDataTk[i]	= 0;
		adSumTk[i]		= 0;
		adAvgTk[i]		= 0;
		adRefTk[i]		= 0;
		refCntKey[i] 	= 0;
	}
	
	tkAddCnt 	= 0;
	keyCnt		= 0;
	keyBuf 		= KEY_NULL;
	keyValue 	= KEY_NULL;
	
	g_keyScanEndFlag 		= 0;
	keyReleaseFlag 			= 0;
	keyOnOffLongPressFlag 	= 0;
	g_adjPressFlag 			= 0;
}

/*-----------------------------------------------------------------------------
Description:		初始化触摸按键配置
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitTouchKey(void)
{
	BANK0_SET;
	
	TKCON1 = 0x81;							//允许触摸按键工作，选择充电次数来作为数据参数，按键采样3次输出数据
	TKST = 0x0C;							//触摸开关频率=Fosc/0X06=24/12=2M
	TKRANDOM |= BIT5;        			 	//比较器滞回使能（解决门限电压附近抖动问题）
	TKF0 = 0;								//所有中断标志位清零
	TKDIV01	= 0;                    	 	//设置放大系数
	TKDIV02	= 0;
	TKDIV03	= 0xAA;
	TKDIV04	= 0x00;
	
	P0SS |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);			//P0.1~P0.6作为触摸口
	P1SS |= BIT7;							//P1.7作为触摸口
	
	TKVREF = 0xA7;                     	 	//Vref=1.0v   去抖时间32Tsys  OP输出电压3.0V 放电时间延时640Tsy
	TKU1 = 0x80;							//按键扫描顺序控制
	TKU2 = 0x7E;
	
	ETK = 1;								//允许触摸按键中断
}

/*-----------------------------------------------------------------------------
Description:		触摸按键AD值扫描
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TouchKeyAdScan(void)
{
	unsigned long s_tkAdBuf;							//按键AD数值累加和缓存
	unsigned char i = 0;
	
	if(g_keyScanEndFlag == 1)
	{
		for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
		{
			s_tkAdBuf = (unsigned long)g_scanDataTk[i];
			adSumTk[i] += s_tkAdBuf;
		}
		
	   	tkAddCnt++;
		if(tkAddCnt >= 4)								//按键值求平均
		{
		   tkAddCnt = 0;
		   
			for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
			{
				adAvgTk[i] = adSumTk[i] >> 2;
				adSumTk[i] = 0;
			}
		}
		
		if(adRefTk[0] == 0)								//初始按键基准值为0，第一次扫描后进行基准值赋值
		{
			if(adAvgTk[0] != 0)
			{
				for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
				{
					adRefTk[i] = adAvgTk[i];
				}
			}
		}
		g_keyScanEndFlag = 0;							//按键扫描处理完成，重新开始扫描
		
		TKF0 = 0;										//触摸中断寄存器清零
		TKCON1 |= BIT5;    								//启动按键扫描
	}
}

/*-----------------------------------------------------------------------------
Description:		按键扫描，同时刷新按键基准
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TouchKeyScan(void)
{
	unsigned char i = 0;

	if(adRefTk[0] != 0)								//首次数据还未出现不扫描按键	
	{
		keyBuf  = 0; 								//键值缓存为0，无按键

		for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
		{
			if(adAvgTk[i] >= adRefTk[i])
			{
				if((adAvgTk[i] - adRefTk[i]) >= TouchKey_Valid_Threshould_Table[i])	
				{
					if((adAvgTk[i] - adRefTk[i]) < VALID_THRESHOLD_MAX)		//限幅滤波（差值小于最大限制有效）
					{
						if(i == 0)					//TK01		温度-
						{
							keyBuf |= KEY_ON_OFF;
						}

						if(i == 1)					//TK02		启动取消
						{
							keyBuf |= KEY_TEMP_SUB;
						}

						if(i == 2)					//TK03		时间+
						{
							keyBuf |= KEY_TIME_ADD;
						}

						if(i == 3)					//TK04		预约
						{
							keyBuf |= KEY_ORDER;
						}

						if(i == 4)					//TK05		时间-
						{
							keyBuf |= KEY_TIME_SUB;
						}

						if(i == 5)					//TK11		功能
						{
							keyBuf |= KEY_CHANGE_FUN;
						}

						if(i == 6)					//TK12		自主烹饪
						{
							keyBuf |= KEY_TEMP_ADD;
						}

						refCntKey[i] = 0;
					}
				}
				else
				{
					refCntKey[i]++;	
					if(refCntKey[i] == 5)
					{
						adRefTk[i]++;				//刷新基准，每隔10ms刷新1个数据
						refCntKey[i] = 0;
					}
				}
			}
			else
			{
				adRefTk[i] = adRefTk[i] - 2;		//刷新基准，每隔2ms刷新2个数据
				refCntKey[i] = 0;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		识别按键值
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TouchKeyDeal(void)
{
	keyCnt++;
	
	if(keyCnt >= ELIM_TREMBLING_CNT)					//大于消抖时间
	{
		if(keyReleaseFlag == 0)							//若之前按键为释放状态
		{
			/*加、减键*/
			if((keyValue == KEY_TEMP_SUB) || (keyValue == KEY_TEMP_ADD) || (keyValue == KEY_TIME_SUB) || (keyValue == KEY_TIME_ADD))
			{
				g_adjPressFlag = 1;						//时间加减键按下（用作长按调节不闪烁）
			
				if((keyCnt == ELIM_TREMBLING_CNT) || (keyCnt >= 500))		//短按（按键按下生效）或长按1S以上
				{
//					keyReleaseFlag = 1;					//按键按下状态
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}
			
			/*预约，功能切换键*/
			else if((keyValue == KEY_CHANGE_FUN))
			{
				if(keyCnt <= (ELIM_TREMBLING_CNT + 4))	//按键按下时操作
				{
					keyReleaseFlag = 1;					//按键按下状态
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}
			
			/*自检、PID键、上电15S内*/
			else if(((keyValue == KEY_PCB_CHECK) || (keyValue == KEY_MACHINE_CHECK) || (keyValue == KEY_DISP_PID)) && 
			(g_selfCheckEnFlag == 1))
			{
				if(keyCnt >= 750)						//长按1.5S
				{
					keyReleaseFlag = 1;					//按键按下状态
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}
			
			/*启动取消键*/
			else if(keyValue == KEY_ON_OFF)
			{
				if(keyCnt >= 750)						//长按1.5S
				{
					keyOnOffLongPressFlag = 1;			//启动取消键长按
					keyReleaseFlag = 1;					//按键按下状态
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}

			/*预约键*/
			else if(keyValue == KEY_ORDER)
			{
				if(keyCnt >= 1000)						//长按2S
				{
					keyOrderLongPressFlag = 1;			//预约键长按
					keyReleaseFlag = 1;					//按键按下状态
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}
			
			/*清洁水垢键*/
			else if(keyValue == KEY_CLEAN_INCRUSTANT)
			{
				if(keyCnt >= 1000)						//长按2S
				{
					keyReleaseFlag = 1;					//按键按下状态
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}
			
			else										//其它按键
			{
				keyBuf		= 65530;					//赋值为无效按键
				keyValue	= 65530;
				keyCnt		= 0;
				
				keyReleaseFlag = 1;						//按键按下状态
				g_adjPressFlag = 0;						//加减键未按下（用作长按调节不闪烁）
			}
		}
		
		else											//按键按下（除加减键）处理过后，赋值为无效键值
		{
			keyBuf		= 65530;						//赋值为无效按键
			keyValue	= 65530;
			keyCnt		= 0;
			
			keyReleaseFlag = 1;							//按键按下状态
			g_adjPressFlag = 0;							//加减键未按下（用作长按调节不闪烁）
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		按键判断
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TouchKeyJudge(void)
{
	static unsigned int xdata s_fakePressCnt = 0;		//假按键计数（长时间检测到有按键则认为是非人为按下）

	unsigned char i = 0;

	if(keyBuf != KEY_NULL)								//有按键按下
	{
		s_fakePressCnt++;
		if(s_fakePressCnt >= 30000)						//2ms时基，若持续1min都检测到有按键，则重新刷新基准值
		{
			for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
			{
				adRefTk[i] = 0;
			}
		}
		else
		{
			if(keyValue == KEY_NULL) 					//按键键值未赋值，进行赋值
			{
				keyValue = keyBuf;
				keyCnt = 0;
			}
			else										//按键键值已经赋值		
			{
				if(keyValue == keyBuf)
				{
					TouchKeyDeal();						//键值消抖计数
				}
				else 									//键值与缓存不一致，重新赋键值
				{
					keyValue = keyBuf;
					keyCnt = 0;
				}	
			}
		}
	}
	else
	{
	
		if(!keyReleaseFlag)
		{
			if(keyCnt >= ELIM_TREMBLING_CNT)			//大于消抖时间
			{
				if(keyValue == KEY_ON_OFF)
				{
					keyOnOffLongPressFlag = 0;			//启动取消键短按，按键抬起有效
					TouchKeyDealSubroutine();			//按键处理子程序
				}
				else if(keyValue == KEY_ORDER)
				{
					keyOrderLongPressFlag = 0;			//预约键键短按，按键抬起有效
					TouchKeyDealSubroutine();			//按键处理子程序
				}
			}
		}
		
		keyOnOffLongPressFlag = 0;						//启动取消键短按
		keyOrderLongPressFlag = 0;						//预约键键短按
	
		keyBuf = KEY_NULL;
		keyValue = KEY_NULL;
		keyCnt = 0;
		
		keyReleaseFlag = 0;								//按键释放状态
		g_adjPressFlag = 0;								//加减键未按下（用作长按调节不闪烁）
		
		s_fakePressCnt = 0;								//假按键计数值清零
	}
}

/*-----------------------------------------------------------------------------
Description:		加载菜单的默认值
Input:				menuNum
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void LoadDefaultValue(unsigned char menuNum)
{
	g_nowStepTechnology	= MenuDefaultValue_Table[menuNum][TECHNOLOGY_STEP_1_NUM];
	g_nowStepworkTemp	= MenuDefaultValue_Table[menuNum][STEP_1_TEMP_NUM];
	g_nowStepworkTime	= MenuDefaultValue_Table[menuNum][STEP_1_TIME_NUM];
	g_workTimeAll		= MenuDefaultValue_Table[menuNum][STEP_1_TIME_NUM] + MenuDefaultValue_Table[menuNum][STEP_2_TIME_NUM] +
						  MenuDefaultValue_Table[menuNum][STEP_3_TIME_NUM] + MenuDefaultValue_Table[menuNum][STEP_4_TIME_NUM];//计算工作总时间
	
	g_tempAdjEnFlag 	= MenuDefaultValue_Table[menuNum][TEMP_ADJ_EN_NUM];	//加载温度可调标志
	g_timeAdjEnFlag 	= MenuDefaultValue_Table[menuNum][TIME_ADJ_EN_NUM];	//加载时间可调标志
	g_orderEnFlag		= MenuDefaultValue_Table[menuNum][ORDER_EN_NUM];	//加载预约有效标志
}
/*-----------------------------------------------------------------------------
Description:		处理各个按键
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void TouchKeyDealSubroutine(void)
{
	switch(g_sysType)
	{
		case SysModePowerUp:								//上电状态
			//所有按键无效
		break;
			
		case SysModeStandby:								//待机状态
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					//无效
				break;

				case KEY_TIME_SUB:
					//无效
				break;

				case KEY_ORDER:
					if(keyOrderLongPressFlag)
					{
						SendCommand(CMD_KX_APP_REMOVE_LINK);
						gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
					}
					else
					{
						SendCommand(CMD_APP_KX_SMART_BAKING);
						g_LedOpenFlag = 1;
						gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
					}
					
				break;

				case KEY_CHANGE_FUN:
					g_sysType = SysModeSelect;				//转到设置待机状态

					g_menuNumber		= DEFAULT_MENU;		//默认菜单

					LoadDefaultValue(g_menuNumber);			//加载菜单1的默认值

					g_tempAdjStateFlag	= 0;				//默认时间调节状态
					g_timeAdjStateFlag	= 1;

					g_sysSelectWaitCnt 	= 0;				//设置待机等待时间清零

					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;

				case KEY_ON_OFF:
					//无效
				break;

				case KEY_PCB_CHECK:							//PCB自检按键
					g_sysType = SysModePCBCheck;			//转入PCB自检状态
					g_sysSelfCheckWaitCnt	= 0;			//自检无操作计时清零

					g_lidSelfCheckOkFlag	= 0;
					g_coreNtcSelfCheckOkFlag= 0;
					g_selfCheckPassFlag 	= 0;			//默认自检不合格
					g_selfCheckOverFlag		= 0;			//标记为自检未完成
					g_selfCheckStep			= 1;			//进入第一步
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//连续发出蜂鸣
				break;
				
				case KEY_MACHINE_CHECK:						//整机自检按键
					g_sysType = SysModeMachineCheck;		//转入整机自检状态
					g_sysSelfCheckWaitCnt	= 0;			//自检无操作计时清零
					
					g_pumpWorkFlag			= 0;			//默认水泵不工作
					g_pumpDelayJudgeFlag	= 0;
					g_selfCheckStep			= 1;			//进入第一步
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//连续发出蜂鸣
				break;
				
				case KEY_DISP_PID:							//PID显示按键
					g_sysType = SysModePID;					//转入PID显示状态
					g_sysSelfCheckWaitCnt	= 0;			//PID无操作计时清零
					
					g_pidDispStep			= 1;			//先显示前3位PID码
					
					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;
				
				case KEY_CLEAN_INCRUSTANT:					//清洁水垢按键
					g_sysType = SysModeClean;				//转入水垢清洁状态
					g_cleanPumpTimeCnt		= 0;			//清洁时间清零

					g_cleanIncrustantStep	= 1;			//水垢清洁第一步
					
					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;
			}
		break;
		
		case SysModeSelect:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					if(g_tempAdjEnFlag)						//若当前时间可调
					{
						if(keyCnt >= 500)					//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//单按蜂鸣，连击不蜂鸣
						}

						if(g_timeAdjStateFlag)				//若当前为时间调节状态
						{
							g_tempAdjStateFlag	= 1;		//设为温度调节状态
							g_timeAdjStateFlag	= 0;
						}
						else
						{
							if(g_nowStepworkTemp >= MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TEMP_NUM])
							{
								g_nowStepworkTemp = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TEMP_NUM];
							}
							else
							{
								g_nowStepworkTemp = g_nowStepworkTemp + 5;
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零
						g_sysSelectWaitCnt 	= 0;			//设置待机等待时间清零
					}
				break;

				case KEY_TEMP_SUB:
					if(g_tempAdjEnFlag)						//若当前时间可调
					{
						if(keyCnt >= 500)					//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//单按蜂鸣，连击不蜂鸣
						}
						
						if(g_timeAdjStateFlag)				//若当前为时间调节状态
						{
							g_tempAdjStateFlag	= 1;		//设为温度调节状态
							g_timeAdjStateFlag	= 0;
						}
						else
						{
							if(g_nowStepworkTemp <= MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TEMP_NUM])
							{
								g_nowStepworkTemp = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TEMP_NUM];
							}
							else
							{
								g_nowStepworkTemp = g_nowStepworkTemp - 5;
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零
						g_sysSelectWaitCnt 	= 0;			//设置待机等待时间清零
					}
				break;

				case KEY_TIME_ADD:
					if(g_timeAdjEnFlag)						//若当前时间可调
					{
						if(keyCnt >= 500)					//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//单按蜂鸣，连击不蜂鸣
						}

						if(g_tempAdjStateFlag)				//若当前为温度调节状态
						{
							g_tempAdjStateFlag	= 0;		//设为时间调节状态
							g_timeAdjStateFlag	= 1;
						}
						else
						{
							if(g_workTimeAll >= MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM])
							{
								g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM];
							}
							else
							{
								if(g_workTimeAll < 30)
								{
									g_workTimeAll++;
								}
								else
								{
									if(g_workTimeAll%2)
									{
										if((g_workTimeAll + 1) > MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll + 1;
										}
									}
									else
									{
										if((g_workTimeAll + 2) > MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll + 2;
										}
									}
								}
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零
						g_sysSelectWaitCnt 	= 0;			//设置待机等待时间清零
					}
				break;

				case KEY_TIME_SUB:
					if(g_timeAdjEnFlag)						//若当前时间可调
					{
						if(keyCnt >= 500)					//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//单按蜂鸣，连击不蜂鸣
						}

						if(g_tempAdjStateFlag)				//若当前为温度调节状态
						{
							g_tempAdjStateFlag	= 0;		//设为时间调节状态
							g_timeAdjStateFlag	= 1;
						}
						else
						{
							if(g_workTimeAll <= MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])	//若当前时间为最小时间，直接跳转到最大值
							{
								g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
							}
							else
							{
								if(g_workTimeAll > 30)						//2分钟可调取2的倍数
								{
									if(g_workTimeAll%2)
									{
										if((g_workTimeAll - 1) < MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll - 1;
										}
									}
									else
									{
										if((g_workTimeAll - 2) < MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll - 2;
										}
									}
								}
								else
								{
									g_workTimeAll--;
								}
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零
						g_sysSelectWaitCnt 	= 0;			//设置待机等待时间清零
					}
				break;

				case KEY_ORDER:
					if(g_orderEnFlag)						//有预约功能的菜单时有效
					{
						g_sysType = SysModeOrderSet;		//转入预约设置状态
						
						g_sysOrderSetWaitCnt = 0;			//预约设置等待时间清零
						
						g_orderTime = 360;					//默认预约时间6H
						
						if(g_workTimeAll%30)				//最小预约时间为工作总时间向上取整
						{
							g_orderMinTime = (g_workTimeAll/30 + 1)*30;
						}
						else
						{
							g_orderMinTime = g_workTimeAll + 30;
						}
						
						g_timeAdjEnFlag		= NO_ADJ;		//温度/工作时间不可调
						g_tempAdjEnFlag		= NO_ADJ;
						
						g_timeAdjStateFlag	= NO_ADJ;		//非温度/时间调节状态
						g_tempAdjStateFlag	= NO_ADJ;

						gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
					}
				break;

				case KEY_CHANGE_FUN:
					if(g_menuNumber >= MENU_3_NUM)			//菜单循环
					{
						g_menuNumber = MENU_1_NUM;
					}
					else
					{
						g_menuNumber++;
					}
					
					LoadDefaultValue(g_menuNumber);			//加载菜单默认值

					if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						g_tempAdjStateFlag	= 0;			//一键通默认时间调节状态
						g_timeAdjStateFlag	= 1;
					}
					else
					{
						g_tempAdjStateFlag	= 1;			//默认温度调节状态
						g_timeAdjStateFlag	= 0;
					}

					g_sysSelectWaitCnt 	= 0;				//设置待机等待时间清零

					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//长按回待机
					{
						g_sysType  = SysModeStandby;		//回到待机状态
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//标记为正常抽水

						g_menuNumber		= DEFAULT_MENU;	//默认菜单
						
						g_sysStandbyWaitCnt = 0;			//待机等待时间清零
					}
					else
					{
						g_sysType			= SysModeWork;	//转入工作状态

						g_enContinueErr		= NoError;		//初始无可恢复报警
						g_waterYieldType 	= NormalType;	//默认正常抽水

						g_workOneMinCnt		= 0;			//1min计时清零

						g_nowStepNum		= 1;			//开始工艺步骤1
						
						g_highSteamPowerCnt	= 0;			//纯蒸工艺下高功率输出时间计时清零
						g_steamLowPowerFlag	= 0;			//纯蒸低功率输出标志清零

						g_tempAdjStateFlag	= 0;			//启动后默认时间调节状态
						g_timeAdjStateFlag	= 1;

						g_coolingFanDealyFlag = 0;			//散热风扇默认不延时停止

						g_pumpDelayJudgeCnt	= 0;

						if(g_pumpCalibrationFlag == 0)		//若未标定成功
						{
							InitPumpCalibrationData();
						}

						g_dispFlashCnt		= 0;			//闪烁计数清零
						g_dispFlashFlag		= 0;			//先亮

						g_recvWorkTime_1 = 0;
						g_recvWorkTime_2 = 0;
						g_recvWorkTime_3 = 0;
					}
					
					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;

				case KEY_PCB_CHECK:							//PCB自检按键
					g_sysType = SysModePCBCheck;			//转入PCB自检状态
					g_sysSelfCheckWaitCnt	= 0;			//自检无操作计时清零

					g_lidSelfCheckOkFlag	= 0;
					g_coreNtcSelfCheckOkFlag= 0;
					g_selfCheckPassFlag 	= 0;			//默认自检不合格
					g_selfCheckOverFlag		= 0;			//标记为自检未完成
					g_selfCheckStep			= 1;			//进入第一步
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//连续发出蜂鸣
				break;
				
				case KEY_MACHINE_CHECK:						//整机自检按键
					g_sysType = SysModeMachineCheck;		//转入整机自检状态
					g_sysSelfCheckWaitCnt	= 0;			//自检无操作计时清零
					
					g_pumpWorkFlag			= 0;			//默认水泵不工作
					g_pumpDelayJudgeFlag	= 0;
					g_selfCheckStep			= 1;			//进入第一步
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//连续发出蜂鸣
				break;
				
				case KEY_DISP_PID:							//PID显示按键
					g_sysType = SysModePID;					//转入PID显示状态
					g_sysSelfCheckWaitCnt	= 0;			//PID无操作计时清零
					
					g_pidDispStep			= 1;			//先显示前3位PID码
					
					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;
				
				case KEY_CLEAN_INCRUSTANT:					//清洁水垢按键
					g_sysType = SysModeClean;				//转入水垢清洁状态
					g_cleanPumpTimeCnt		= 0;			//清洁时间清零

					g_cleanIncrustantStep	= 1;			//水垢清洁第一步
					
					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;
			}
		break;

		case SysModeOrderSet:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					if(keyCnt >= 500)						//若长按，则连击生效
					{
						keyCnt = 400;
					}
					else
					{
						gU8_buzzerType = BUZ_JK_KEY;		//单按蜂鸣，连击不蜂鸣
					}
				
					if(g_orderTime >= ORDER_TIME_MAX)		//若当前预约时间为最长预约时间
					{
						g_orderTime = g_orderMinTime;		//设为最短预约时间
					}
					else
					{
						if(g_orderTime >= 600)				//时间大于10H步长1H
						{
							g_orderTime = g_orderTime + 60;
						}
						else								//10H内步长30
						{
							g_orderTime = g_orderTime + 30;
						}
					}
				
					g_sysOrderSetWaitCnt = 0;				//预约设置等待时间清零
				break;

				case KEY_TIME_SUB:
					if(keyCnt >= 500)						//若长按，则连击生效
					{
						keyCnt = 400;
					}
					else
					{
						gU8_buzzerType = BUZ_JK_KEY;		//连击不蜂鸣
					}
				
					if(g_orderTime <= g_orderMinTime)		//若当前预约时间为最小预约时间
					{
						g_orderTime = ORDER_TIME_MAX;		//设为最长预约时间
					}
					else
					{
						if(g_orderTime > 600)				//时间大于10H步长1H
						{
							g_orderTime = g_orderTime - 60;
						}
						else								//10H内步长30
						{
							g_orderTime = g_orderTime - 30;
						}
					}
				
					g_sysOrderSetWaitCnt = 0;				//预约设置等待时间清零
				break;

				case KEY_ORDER:
					g_sysType = SysModeSelect;				//回到设置待机状态
					
					g_sysSelectWaitCnt	= 0;				//设置待机等待时间清零
					
					LoadDefaultValue(g_menuNumber);			//加载菜单的默认值

					if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						g_tempAdjStateFlag	= 0;			//一键通默认时间调节状态
						g_timeAdjStateFlag	= 1;
					}
					else
					{
						g_tempAdjStateFlag	= 1;			//默认温度调节状态
						g_timeAdjStateFlag	= 0;
					}

					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;

				case KEY_CHANGE_FUN:
					g_sysType = SysModeSelect;				//回到设置待机状态
					
					g_sysSelectWaitCnt	= 0;				//设置待机等待时间清零
					
					LoadDefaultValue(g_menuNumber);			//加载菜单的默认值

					if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						g_tempAdjStateFlag	= 0;			//一键通默认时间调节状态
						g_timeAdjStateFlag	= 1;
					}
					else
					{
						g_tempAdjStateFlag	= 1;			//默认温度调节状态
						g_timeAdjStateFlag	= 0;
					}

					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//长按回待机
					{
						g_sysType  = SysModeStandby;		//回到待机状态
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//标记为正常抽水

						g_menuNumber		= DEFAULT_MENU;	//默认菜单
						
						g_sysStandbyWaitCnt = 0;			//待机等待时间清零
					}
					else
					{
						g_sysType = SysModeOrderWork;		//转入预约工作状态

						g_enContinueErr		= NoError;		//初始无可恢复报警
						g_waterYieldType 	= NormalType;	//默认正常抽水

						g_nowStepNum = 1;					//启动进入第一步工艺
						
						g_workOneMinCnt		= 0;			//1min计时清零
						
						g_highSteamPowerCnt	= 0;			//纯蒸工艺下高功率输出时间计时清零
						g_steamLowPowerFlag	= 0;			//纯蒸低功率输出标志清零

						g_coolingFanDealyFlag = 0;			//散热风扇默认不延时停止
						
						g_pumpDelayJudgeCnt	= 0;

						g_dispFlashCnt		= 0;			//闪烁计数清零
						g_dispFlashFlag		= 0;			//显示先灭
					}
					
					gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
				break;
			}
		break;

		case SysModeOrderWork:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					//无效
				break;

				case KEY_TIME_SUB:
					//无效
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//长按回待机
					{
						g_sysType  = SysModeStandby;		//回到待机状态
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//标记为正常抽水

						g_menuNumber		= DEFAULT_MENU;	//默认菜单
						
						g_sysStandbyWaitCnt = 0;			//待机等待时间清零

						gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
					}
				break;
			}
		break;

		case SysModeWork:
		case SYS_MODE_SMART_WORKING:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//工作状态下，只有3个自主功能可调
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)						//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;		//单按蜂鸣，连击不蜂鸣
						}

						if(g_timeAdjStateFlag)					//若当前为时间调节状态
						{
							g_tempAdjStateFlag	= 1;			//设为温度调节状态
							g_timeAdjStateFlag	= 0;
						}
						else
						{
							if(g_nowStepworkTemp >= MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TEMP_NUM])
							{
								g_nowStepworkTemp = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TEMP_NUM];
							}
							else
							{
								g_nowStepworkTemp = g_nowStepworkTemp + 5;
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零

						g_steamPreHeatFlag 	= 0;			//重新判定锅炉是否预热（重新赋值抽水默认值）
					}
				break;

				case KEY_TEMP_SUB:
					//工作状态下，只有3个自主功能可调
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)					//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//单按蜂鸣，连击不蜂鸣
						}
						
						if(g_timeAdjStateFlag)				//若当前为时间调节状态
						{
							g_tempAdjStateFlag	= 1;		//设为温度调节状态
							g_timeAdjStateFlag	= 0;
						}
						else
						{
							if(g_nowStepworkTemp <= MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TEMP_NUM])
							{
								g_nowStepworkTemp = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TEMP_NUM];
							}
							else
							{
								g_nowStepworkTemp = g_nowStepworkTemp - 5;
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零

						g_steamPreHeatFlag 	= 0;			//重新判定锅炉是否预热（重新赋值抽水默认值）
					}
				break;

				case KEY_TIME_ADD:
					//工作状态下，只有3个自主功能可调
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)						//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;		//单按蜂鸣，连击不蜂鸣
						}

						if(g_tempAdjStateFlag)					//若当前为温度调节状态
						{
							g_tempAdjStateFlag	= 0;			//设为时间调节状态
							g_timeAdjStateFlag	= 1;
						}
						else
						{
							if(g_workTimeAll >= MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM])
							{
								g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM];
							}
							else
							{
								if(g_workTimeAll < 30)
								{
									g_workTimeAll++;
								}
								else
								{
									if(g_workTimeAll%2)
									{
										if((g_workTimeAll + 1) > MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll + 1;
										}
									}
									else
									{
										if((g_workTimeAll + 2) > MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll + 2;
										}
									}
								}
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零

//						g_workOneMinCnt		= 0;			//1min计时清零（显示S后取消清零）
					}
				break;

				case KEY_TIME_SUB:
					//工作状态下，只有3个自主功能可调
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)						//若长按，则连击生效
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;		//单按蜂鸣，连击不蜂鸣
						}

						if(g_tempAdjStateFlag)					//若当前为温度调节状态
						{
							g_tempAdjStateFlag	= 0;			//设为时间调节状态
							g_timeAdjStateFlag	= 1;
						}
						else
						{
							if(g_workTimeAll <= MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])	//若当前时间为最小时间，直接跳转到最大值
							{
								g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
							}
							else
							{
								if(g_workTimeAll > 30)						//2分钟可调取2的倍数
								{
									if(g_workTimeAll%2)
									{
										if((g_workTimeAll - 1) < MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll - 1;
										}
									}
									else
									{
										if((g_workTimeAll - 2) < MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])
										{
											g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM];
										}
										else
										{
											g_workTimeAll = g_workTimeAll - 2;
										}
									}
								}
								else
								{
									g_workTimeAll--;
								}
							}
						}

						g_adjWaitTimeCnt	= 0;			//时间/温度调节等待时间清零

//						g_workOneMinCnt		= 0;			//1min计时清零（显示S后取消清零）
					}
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//长按回待机
					{
						g_sysType  = SysModeStandby;		//回到待机状态
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//标记为正常抽水

						g_menuNumber		= DEFAULT_MENU;	//默认菜单
						
						g_sysStandbyWaitCnt = 0;			//待机等待时间清零

						gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣

						SendCommand(CMD_KX_APP_REPORT_STATUS);	//立即上报状态
					}
					else
					{
						if(g_waterYieldType == LackWater)		//若缺水报警，则继续工作，取消报警重新检测
						{
							g_enContinueErr		= NoError;		//清除报警
							g_waterYieldType	= NormalType;	//标记为正常抽水
							g_pumpDelayJudgeCnt	= 0;
							
							g_highSteamPowerCnt	= 0;			//纯蒸工艺下高功率输出时间计时清零
							g_steamLowPowerFlag	= 0;			//纯蒸低功率输出标志清零

							g_quickPumpingFlag	= 1;			//取消缺水报警后进行快抽
							
							if(g_pumpCalibrationFlag == 0)		//若未标定成功，重新标定
							{
								InitPumpCalibrationData();
							}
							
							gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
						}
					}
				break;
			}
		break;

		case SysModeWarm:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					//无效
				break;

				case KEY_TIME_SUB:
					//无效
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//长按回待机
					{
						g_sysType  = SysModeStandby;		//回到待机状态
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//标记为正常抽水

						g_menuNumber		= DEFAULT_MENU;	//默认菜单
						
						g_sysStandbyWaitCnt = 0;			//待机等待时间清零

						gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
					}
					else
					{
						if(g_waterYieldType == LackWater)		//若缺水报警，则继续工作，取消报警重新检测
						{
							g_enContinueErr		= NoError;		//清除报警
							g_waterYieldType	= NormalType;	//标记为正常抽水
							g_pumpDelayJudgeCnt	= 0;
							
							g_highSteamPowerCnt	= 0;			//纯蒸工艺下高功率输出时间计时清零
							g_steamLowPowerFlag	= 0;			//纯蒸低功率输出标志清零

							g_quickPumpingFlag	= 1;			//取消缺水报警后进行快抽
							
							if(g_pumpCalibrationFlag == 0)		//若未标定成功，重新标定
							{
								InitPumpCalibrationData();
							}
							
							gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
						}
					}
				break;
			}
		break;

		case SysModeEnd:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					//无效
				break;

				case KEY_TIME_SUB:
					//无效
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					g_sysType  = SysModeStandby;		//回到待机状态
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;	//标记为正常抽水

					g_menuNumber		= DEFAULT_MENU;	//默认菜单
					
					g_sysStandbyWaitCnt = 0;			//待机等待时间清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;
			}
		break;

		case SysModeDarkScreen:
			
		break;

		case SysModeError:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					//无效
				break;

				case KEY_TIME_SUB:
					//无效
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					if((measureData.errCode == 0x03) ||
					(measureData.errCode == 0x04) ||
					(measureData.errCode == 0x05) ||
					(measureData.errCode == 0x06) ||
					(measureData.errCode == 0x57) ||
					(measureData.errCode == 0x58) ||
					(measureData.errCode == 0x59))		//锅炉NTC、过零丢失、水泵标定失败、显示板自检报警时，启动取消回待机
					{
						g_sysType	= SysModeStandby;	//回到待机状态
						g_menuNumber = DEFAULT_MENU;	//默认菜单
						
						if((measureData.errCode == 0x57) || (measureData.errCode == 0x58) || (measureData.errCode == 0x59))
						{
							/*掉电检测的AD口做GPIO引脚*/
							ADCH1 &= ~BIT7;
							
							/*掉电检测AD口不带上拉输出高*/
							P3    |= BIT7;									//给掉电记忆的电容充电
							P3CR  |= BIT7;
							P3PCR &= ~BIT7;
							
							SEQCHX = 0x02;									//选择通道2
						}
						
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//标记为正常抽水

						measureData.errCode = 0;		//清除报警
						
						g_sysStandbyWaitCnt = 0;		//待机等待时间清零

						gU8_buzzerType = BUZ_JK_KEY;	//发出按键有效蜂鸣
					}
					else
					{
						gU8_buzzerType = BUZ_JK_MUTE;	//立即停止蜂鸣
					}
				break;
			}
		break;

		case SysModePCBCheck:							//PCB自检状态
			if((keyValue == KEY_ON_OFF) && (g_selfCheckStep != 4))
			{
				g_sysType	= SysModeStandby;			//回到待机状态
				g_menuNumber = DEFAULT_MENU;			//默认菜单
				
				g_enContinueErr		= NoError;
				g_waterYieldType	= NormalType;		//标记为正常抽水
				
				g_sysStandbyWaitCnt = 0;				//待机等待时间清零
				
				g_selfCheckStep	= 0;
				
				/*掉电检测的AD口做GPIO引脚*/
				ADCH1 &= ~BIT7;
				
				/*掉电检测AD口不带上拉输出高*/
				P3    |= BIT7;							//给掉电记忆的电容充电
				P3CR  |= BIT7;
				P3PCR &= ~BIT7;
				
				SEQCHX = 0x02;							//选择通道2

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else
			{
				if((g_selfCheckStep == 1) && (keyValue == KEY_TEMP_ADD))
				{
					g_selfCheckStep = 2;				//进入第二步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 2) && (keyValue == KEY_TEMP_SUB))
				{
					g_selfCheckStep = 3;				//进入第三步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 3) && (keyValue == KEY_ORDER))
				{
					g_selfCheckStep = 4;				//进入第四步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 4) && (keyValue == KEY_ON_OFF))
				{
					g_selfCheckStep = 5;				//进入第五步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 5) && (keyValue == KEY_CHANGE_FUN))
				{
					g_selfCheckStep = 6;				//进入第六步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 6) && (keyValue == KEY_TIME_SUB))
				{
					g_selfCheckStep = 7;				//进入第七步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零
					
//					ADCH1 |= BIT7;						//掉电检测的AD口做模拟输入口
//					SEQCHX = 0x07;						//选择通道7
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 7) && (keyValue == KEY_TIME_ADD))
				{
					g_selfCheckStep = 8;				//进入第八步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
				else if((g_selfCheckStep == 8) && (keyValue == KEY_TEMP_ADD))
				{
					g_selfCheckStep = 1;				//进返回第一步
					g_sysSelfCheckWaitCnt = 0;			//自检无操作计时清零

					SEQCHX = 0x02;						//选择通道2
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				}
			}
		break;

		case SysModeMachineCheck:						//整机自检状态
			if((g_selfCheckStep == 1) && (keyValue == KEY_TEMP_ADD))
			{
				g_selfCheckStep = 2;					//进入第二步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 2) && (keyValue == KEY_TEMP_SUB))
			{
				g_selfCheckStep = 3;					//进入第三步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 3) && (keyValue == KEY_ORDER))
			{
				g_selfCheckStep = 4;					//进入第四步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 4) && (keyValue == KEY_ON_OFF))
			{
				g_selfCheckStep = 5;					//进入第五步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 5) && (keyValue == KEY_CHANGE_FUN))
			{
				g_selfCheckStep = 6;					//进入第六步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 6) && (keyValue == KEY_TIME_SUB))
			{
				g_selfCheckStep = 7;					//进入第七步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 7) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 8;					//进入第八步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 8) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 9;					//进入第九步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 9) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 11;					//跳过第十步（SF1无散热风扇），直接到第十一步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 10) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 11;					//进入第十一步（水泵检测）
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 11) && (keyValue == KEY_TIME_ADD) && (g_pumpWorkFlag == 1))		//未检测到水泵工作则不进行下一步
			{
				g_selfCheckStep = 12;					//进入第十二步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((g_selfCheckStep == 12) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 13;					//进入第十三步
				g_sysSelfCheckWaitCnt = 0;				//自检无操作计时清零
				
				g_darkScreenTime = 3;					//息屏1.5S后回到自检第一步,500ms时基
				
				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
			else if((keyValue == KEY_ON_OFF) && (g_selfCheckStep != 4))
			{
				g_sysType	= SysModeStandby;			//回到待机状态
				g_menuNumber = DEFAULT_MENU;			//默认菜单
				
				g_enContinueErr		= NoError;
				g_waterYieldType	= NormalType;		//标记为正常抽水
				
				g_sysStandbyWaitCnt = 0;				//待机等待时间清零

				g_selfCheckStep	= 0;

				gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
			}
		break;

		case SysModeDebug:								//Debug状态
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					g_debugDispStep = 1;				//Debug显示步骤第一步

					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_TEMP_SUB:
					g_debugDispStep = 2;				//Debug显示步骤第二步

					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_TIME_ADD:
					g_debugDispStep = 3;				//Debug显示步骤第三步

					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_TIME_SUB:
					g_debugDispStep = 4;				//Debug显示步骤第四步

					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					g_sysType	= SysModeStandby;		//回到待机状态
					g_menuNumber = DEFAULT_MENU;		//默认菜单
					
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;	//标记为正常抽水
					
					g_sysStandbyWaitCnt = 0;			//待机等待时间清零

					g_debugDispStep		= 0;

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;
			}
		break;

		case SysModePID:								//PID状态
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					g_sysType = SysModeDebug;			//转入Debug显示状态
					
					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零
					
					g_debugDispStep = 1;				//Debug显示步骤第一步
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_TEMP_SUB:
					g_sysType = SysModeDebug;			//转入Debug显示状态
					
					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零
					
					g_debugDispStep = 2;				//Debug显示步骤第二步
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_TIME_ADD:
					g_sysType = SysModeDebug;			//转入Debug显示状态
					
					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零
					
					g_debugDispStep = 3;				//Debug显示步骤第三步
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_TIME_SUB:
					g_sysType = SysModeDebug;			//转入Debug显示状态
					
					g_sysSelfCheckWaitCnt = 0;			//Debug无操作计时清零
					
					g_debugDispStep = 4;				//Debug显示步骤第四步
					
					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					g_sysType	= SysModeStandby;		//回到待机状态
					g_menuNumber = DEFAULT_MENU;		//默认菜单
					
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;	//标记为正常抽水
					
					g_sysStandbyWaitCnt = 0;			//待机等待时间清零

					g_debugDispStep		= 0;

					gU8_buzzerType = BUZ_JK_KEY;		//发出按键有效蜂鸣
				break;
			}
		break;

		case SysModeClean:								//清洁水垢状态
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//无效
				break;

				case KEY_TEMP_SUB:
					//无效
				break;

				case KEY_TIME_ADD:
					//无效
				break;

				case KEY_TIME_SUB:
					//无效
				break;

				case KEY_ORDER:
					//无效
				break;

				case KEY_CHANGE_FUN:
					//无效
				break;

				case KEY_ON_OFF:
					if((g_cleanIncrustantStep == 2) && (g_cleanPumpTimeCnt == CLEAN_TIME_STEP_2 + 1))		//清洁第二步时间到
					{
						g_cleanPumpTimeCnt		= 0;			//清洁步骤时间清零
						
						g_cleanIncrustantStep	= 3;
						
						gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
					}
					else
					{
						g_sysType	= SysModeStandby;			//回到待机状态
						g_menuNumber = DEFAULT_MENU;			//默认菜单

						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;		//标记为正常抽水

						g_sysStandbyWaitCnt = 0;				//待机等待时间清零
						
						g_cleanPumpTimeCnt	= 0;
						g_cleanIncrustantStep = 0;
						
						g_pidDispStep		= 0;
						
						gU8_buzzerType = BUZ_JK_KEY;			//发出按键有效蜂鸣
					}
				break;
			}
		break;
	}
}
