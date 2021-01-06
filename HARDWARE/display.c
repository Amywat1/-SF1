#include "header.h"

//数据显示表
unsigned char code		Disp_Table[16]=
{
	DISPDATA_0,	DISPDATA_1,	DISPDATA_2,	DISPDATA_3,
	DISPDATA_4,	DISPDATA_5,	DISPDATA_6,	DISPDATA_7,
	DISPDATA_8,	DISPDATA_9,	DISPDATA_A,	DISPDATA_b,
	DISPDATA_C,	DISPDATA_d,	DISPDATA_E,	DISPDATA_F,
};

LED_MESSAGE Led_Buff[4];

unsigned char			DispComCnt;					//COM口指针
bit						g_flashFlag;				//闪烁标志位

unsigned char bdata		DispDataBuf;				//显示数据缓存
sbit					DispSegA	= DispDataBuf^0;
sbit					DispSegB	= DispDataBuf^1;
sbit					DispSegC	= DispDataBuf^2;
sbit					DispSegD	= DispDataBuf^3;
sbit					DispSegE	= DispDataBuf^4;
sbit					DispSegF	= DispDataBuf^5;
sbit					DispSegG	= DispDataBuf^6;
sbit					DispSegH	= DispDataBuf^7;

/*-----------------------------------------------------------------------------
Description:		初始化显示相关IO
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitDispIo(void)
{
	BANK0_SET;
	
	/*SEG口做输出不带上拉，输出低*/
	P1CR  |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5);				//输出
	P3CR  |= BIT4;
	P4CR  |= BIT5;

	P1PCR &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5));				//无上拉
	P3PCR &= (~(BIT4));
	P4PCR &= (~(BIT5));

	P1	  &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5));				//输出低
	P2	  &= (~(BIT4));
	P4	  &= (~(BIT5));

	BANK1_SET;
	P5CR  |= (BIT2);
	P5PCR &= (~(BIT2));
	P5	  &= (~(BIT2));
	BANK0_SET;
	
	/*COM口做输出不带上拉，输出低*/
	P3CR  |= (BIT0 | BIT1 | BIT2 | BIT3);			//输出
	P3PCR &= (~(BIT0 | BIT1 | BIT2 | BIT3));		//无上拉
	P3	  &= (~(BIT0 | BIT1 | BIT2 | BIT3));		//输出低
}

/*-----------------------------------------------------------------------------
Description:		初始化显示变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitDispVariable(void)
{
	unsigned char i;

	for(i=0;i<4;i++)
	{
		Led_Buff[i].ramDat = DISPDATA_OFF;		//显示全灭
	}
	
	DispComCnt  = 0;							//COM口指针指向COM1口
	DispDataBuf = 0;							//数据缓存区为0
	
	g_flashFlag = 0;
}

/*-----------------------------------------------------------------------------
Description:		所有显示开
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DispAllOn(void)
{
	unsigned char i;
	
	for(i=0;i<4;i++)
	{
		Led_Buff[i].ramDat = DISPDATA_ON;				//显示全亮
	}
}

/*-----------------------------------------------------------------------------
Description:		所有显示关
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DispAllOff(void)
{
	unsigned char i;

	for(i=0;i<4;i++)
	{
		Led_Buff[i].ramDat = DISPDATA_OFF;				//显示全灭
	}
}

/*-----------------------------------------------------------------------------
Description:		加载显示数据（seg电平控制）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DisplayDataLoad(void)
{
	if(DispSegA == 1)							//segA
	{
		P_SEG_A = 1;
	}
	else
	{
		P_SEG_A = 0;
	}
	
	BANK1_SET;									//P5口需切BANK1
	if(DispSegB == 1)							//segB
	{
		P_SEG_B = 1;
	}
	else
	{
		P_SEG_B = 0;
	}
	BANK0_SET;
	
	if(DispSegC == 1)							//segC
	{
		P_SEG_C = 1;
	}
	else
	{
		P_SEG_C = 0;
	}
	
	if(DispSegD == 1)							//segD
	{
		P_SEG_D = 1;
	}
	else
	{
		P_SEG_D = 0;
	}
	
	if(DispSegE == 1)							//segE
	{
		P_SEG_E = 1;
	}
	else
	{
		P_SEG_E = 0;
	}
	
	if(DispSegF == 1)							//segF
	{
		P_SEG_F = 1;
	}
	else
	{
		P_SEG_F = 0;
	}
	
	if(DispSegG == 1)							//segG
	{
		P_SEG_G = 1;
	}
	else
	{
		P_SEG_G = 0;
	}
	
	if(DispSegH == 1)							//segH
	{
		P_SEG_H = 1;
	}
	else
	{
		P_SEG_H = 0;
	}
}

/*-----------------------------------------------------------------------------
Description:		显示扫描
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DisplayScan(void)
{
	P_COM1 = 1;									//关闭显示（消隐）
	P_COM2 = 1;
	P_COM3 = 1;
	P_COM4 = 1;
	
	_nop_();									//冗余电平稳定
	_nop_();									//冗余电平稳定
	_nop_();									//冗余电平稳定
	_nop_();									//冗余电平稳定
	_nop_();									//冗余电平稳定
	_nop_();									//冗余电平稳定
	
	if(DispComCnt >= 4)							//4个COM口循环
	{
		DispComCnt = 0;
	}

	switch(DispComCnt)
	{
		case 0:									//COM1口
			DispDataBuf = Led_Buff[0].ramDat;	//数据赋值给缓存区
			
			DisplayDataLoad();					//数据加载（先选通seg口再选通COM）
			P_COM1 = 0;							//P_COM1口置低电平-选通
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;
		case 1:									//COM2口
			DispDataBuf = Led_Buff[1].ramDat;	//数据赋值给缓存区
			
			DisplayDataLoad();					//数据加载（先选通seg口再选通COM）
			P_COM2 = 0;							//P_COM2口置低电平-选通
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;
		case 2:									//COM3口
			DispDataBuf = Led_Buff[2].ramDat;	//数据赋值给缓存区
			
			DisplayDataLoad();					//数据加载（先选通seg口再选通COM）
			P_COM3 = 0;							//P_COM3口置低电平-选通
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;
		case 3:									//COM4口
			DispDataBuf = Led_Buff[3].ramDat;	//数据赋值给缓存区
			
			DisplayDataLoad();					//数据加载（先选通seg口再选通COM）		
			P_COM4 = 0;							//P_COM4口置低电平-选通
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;

		default:
			break;
	}
	
	DispComCnt++;								//指向下一个COM口
}

/*-----------------------------------------------------------------------------
Description:		显示控制
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DispCrl(void)
{
	unsigned char i = 0;

	unsigned char wifiStatus;

	DispAllOff();
	
	switch(g_sysType)
	{
		case SysModePowerUp:											//上电全亮
			DispAllOn();
			break;
			
		case SysModeStandby:											//待机状态显示“---”
			DIGITAL_FIRST_NUM(DISPDATA_SG);
			DIGITAL_SECOND_NUM(DISPDATA_SG);
			DIGITAL_THIRD_NUM(DISPDATA_SG);
			break;
			
		case SysModeSelect:												//设置待机状态
			if(g_menuNumber == MENU_1_NUM)
			{
				SYMBOL_FUN_BAKE = 1;
			}
			else if(g_menuNumber == MENU_2_NUM)
			{
				SYMBOL_FUN_BA_ST = 1;
			}
			else if(g_menuNumber == MENU_3_NUM)
			{
				SYMBOL_FUN_STEAM = 1;
			}
			else if(g_menuNumber == MENU_4_NUM)
			{
				SYMBOL_FUN_WING = 1;
			}
			else if(g_menuNumber == MENU_5_NUM)
			{
				SYMBOL_FUN_RICE = 1;
			}
			else if(g_menuNumber == MENU_6_NUM)
			{
				SYMBOL_FUN_CHIPS = 1;
			}

			if(g_tempAdjStateFlag)										//若是温度调节状态，显示温度，℃图标
			{
				if(g_nowStepworkTemp/100)								//温度有百位时显示
				{
					DIGITAL_FIRST_NUM(Disp_Table[g_nowStepworkTemp/100]);
				}
				
				DIGITAL_SECOND_NUM(Disp_Table[g_nowStepworkTemp%100/10]);
				DIGITAL_THIRD_NUM(Disp_Table[g_nowStepworkTemp%100%10]);

				SYMBOL_TEMP = 1;
			}
			else														//否则显示时间，分图标
			{
				DIGITAL_SECOND_NUM(Disp_Table[g_workTimeAll/10]);
				DIGITAL_THIRD_NUM(Disp_Table[g_workTimeAll%10]);

				SYMBOL_MINITE = 1;
			}

			break;
		
		case SysModeOrderSet:											//预约设置状态
			if(g_menuNumber == MENU_1_NUM)
			{
				SYMBOL_FUN_BAKE = 1;
			}
			else if(g_menuNumber == MENU_2_NUM)
			{
				SYMBOL_FUN_BA_ST = 1;
			}
			else if(g_menuNumber == MENU_3_NUM)
			{
				SYMBOL_FUN_STEAM = 1;
			}
			else if(g_menuNumber == MENU_4_NUM)
			{
				SYMBOL_FUN_WING = 1;
			}
			else if(g_menuNumber == MENU_5_NUM)
			{
				SYMBOL_FUN_RICE = 1;
			}
			else if(g_menuNumber == MENU_6_NUM)
			{
				SYMBOL_FUN_CHIPS = 1;
			}

			if(g_orderTime >= 600)										//10H以上
			{
				DIGITAL_FIRST_NUM(Disp_Table[g_orderTime/60/10]);
				DIGITAL_SECOND_NUM(Disp_Table[g_orderTime/60%10]);
			}
			else
			{
				DIGITAL_FIRST_NUM((Disp_Table[g_orderTime/60%10]) | DISPDATA_SDP);

				if(g_orderTime%60 >= 30)
				{
					DIGITAL_SECOND_NUM(DISPDATA_5);
				}
				else
				{
					DIGITAL_SECOND_NUM(DISPDATA_0);
				}
			}

			DIGITAL_THIRD_NUM(DISPDATA_H);

			break;
			
		case SysModeOrderWork:											//预约工作状态
			if(g_menuNumber == MENU_1_NUM)
			{
				SYMBOL_FUN_BAKE = 1;
			}
			else if(g_menuNumber == MENU_2_NUM)
			{
				SYMBOL_FUN_BA_ST = 1;
			}
			else if(g_menuNumber == MENU_3_NUM)
			{
				SYMBOL_FUN_STEAM = 1;
			}
			else if(g_menuNumber == MENU_4_NUM)
			{
				SYMBOL_FUN_WING = 1;
			}
			else if(g_menuNumber == MENU_5_NUM)
			{
				SYMBOL_FUN_RICE = 1;
			}
			else if(g_menuNumber == MENU_6_NUM)
			{
				SYMBOL_FUN_CHIPS = 1;
			}

			if(g_orderTime > 570)										//9.5H以上
			{
				if(g_orderTime%60)
				{
					DIGITAL_FIRST_NUM(Disp_Table[(g_orderTime/60 + 1)/10]);
					DIGITAL_SECOND_NUM(Disp_Table[(g_orderTime/60 + 1)%10]);
				}
				else
				{
					DIGITAL_FIRST_NUM(Disp_Table[g_orderTime/60/10]);
					DIGITAL_SECOND_NUM(Disp_Table[g_orderTime/60%10]);
				}
			}
			else
			{
				if(g_orderTime%60 > 30)										//四舍五入，半小时进一小时（9.5H及以下）
				{
					DIGITAL_FIRST_NUM((Disp_Table[g_orderTime/60 + 1]) | DISPDATA_SDP);
					DIGITAL_SECOND_NUM(DISPDATA_0);
				}
				else
				{
					DIGITAL_FIRST_NUM((Disp_Table[g_orderTime/60]) | DISPDATA_SDP);

					if(g_orderTime%60 == 0)
					{
						DIGITAL_SECOND_NUM(DISPDATA_0);
					}
					else
					{
						DIGITAL_SECOND_NUM(DISPDATA_5);
					}
				}
			}

			if(g_dispFlashFlag)
			{
				DIGITAL_THIRD_NUM(DISPDATA_H);
			}

			if(g_LidFlag == LID_OPEN)									//开盖时，显示E11
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			break;
		
		case SysModeWork:												//工作状态
			if(g_menuNumber == MENU_1_NUM)
			{
				SYMBOL_FUN_BAKE = 1;
			}
			else if(g_menuNumber == MENU_2_NUM)
			{
				SYMBOL_FUN_BA_ST = 1;
			}
			else if(g_menuNumber == MENU_3_NUM)
			{
				SYMBOL_FUN_STEAM = 1;
			}
			else if(g_menuNumber == MENU_4_NUM)
			{
				SYMBOL_FUN_WING = 1;
			}
			else if(g_menuNumber == MENU_5_NUM)
			{
				SYMBOL_FUN_RICE = 1;
			}
			else if(g_menuNumber == MENU_6_NUM)
			{
				SYMBOL_FUN_CHIPS = 1;
			}

			if(g_tempAdjStateFlag)										//若是温度调节状态，显示温度，℃图标
			{	
				if(g_nowStepworkTemp/100)								//温度有百位时显示
				{
					DIGITAL_FIRST_NUM(Disp_Table[g_nowStepworkTemp/100]);
				}
				
				DIGITAL_SECOND_NUM(Disp_Table[g_nowStepworkTemp%100/10]);
				DIGITAL_THIRD_NUM(Disp_Table[g_nowStepworkTemp%100%10]);

				SYMBOL_TEMP = 1;
			}
			else														//否则显示时间，分图标
			{
				if(g_workOneMinCnt < 12)								//g_workOneMinCnt变量，500ms计数一次，12为6S
				{
					if((g_workTimeAll/10) > 0)
					{
						DIGITAL_FIRST_NUM(Disp_Table[g_workTimeAll/10]);
					}
					else
					{
						//十位等于零时不显示
					}
					
					if(g_dispFlashFlag)
					{
						DIGITAL_SECOND_NUM(Disp_Table[g_workTimeAll%10]);
					}
					else
					{
						DIGITAL_SECOND_NUM((Disp_Table[g_workTimeAll%10]) | DISPDATA_SDP);
					}

					DIGITAL_THIRD_NUM(DISPDATA_0);
				}
				else
				{
					if(((g_workTimeAll - 1)/10) > 0)
					{
						DIGITAL_FIRST_NUM(Disp_Table[(g_workTimeAll - 1)/10]);
					}
					else
					{
						//十位等于零时不显示
					}
					
					if(g_dispFlashFlag)
					{
						DIGITAL_SECOND_NUM(Disp_Table[(g_workTimeAll - 1)%10]);
					}
					else
					{
						DIGITAL_SECOND_NUM((Disp_Table[(g_workTimeAll - 1)%10]) | DISPDATA_SDP);
					}

					if(g_workOneMinCnt < 24)
					{
						DIGITAL_THIRD_NUM(DISPDATA_9);
					}
					else if (g_workOneMinCnt < 36)
					{
						DIGITAL_THIRD_NUM(DISPDATA_8);
					}
					else if (g_workOneMinCnt < 48)
					{
						DIGITAL_THIRD_NUM(DISPDATA_7);
					}
					else if (g_workOneMinCnt < 60)
					{
						DIGITAL_THIRD_NUM(DISPDATA_6);
					}
					else if (g_workOneMinCnt < 72)
					{
						DIGITAL_THIRD_NUM(DISPDATA_5);
					}
					else if (g_workOneMinCnt < 84)
					{
						DIGITAL_THIRD_NUM(DISPDATA_4);
					}
					else if (g_workOneMinCnt < 96)
					{
						DIGITAL_THIRD_NUM(DISPDATA_3);
					}
					else if (g_workOneMinCnt < 108)
					{
						DIGITAL_THIRD_NUM(DISPDATA_2);
					}
					else
					{
						DIGITAL_THIRD_NUM(DISPDATA_1);
					}
				}

				SYMBOL_MINITE = 1;
			}

			if(g_LidFlag == LID_OPEN)									//开盖时，显示E11
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			
			if(g_waterYieldType == LackWater)							//缺水时，显示E12
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_2);
			}
			break;
		
		case SysModeWarm:												//保温状态
			if(g_menuNumber == MENU_1_NUM)
			{
				SYMBOL_FUN_BAKE = 1;
			}
			else if(g_menuNumber == MENU_2_NUM)
			{
				SYMBOL_FUN_BA_ST = 1;
			}
			else if(g_menuNumber == MENU_3_NUM)
			{
				SYMBOL_FUN_STEAM = 1;
			}
			else if(g_menuNumber == MENU_4_NUM)
			{
				SYMBOL_FUN_WING = 1;
			}
			else if(g_menuNumber == MENU_5_NUM)
			{
				SYMBOL_FUN_RICE = 1;
			}
			else if(g_menuNumber == MENU_6_NUM)
			{
				SYMBOL_FUN_CHIPS = 1;
			}

			DIGITAL_FIRST_NUM(DISPDATA_b);								//显示保温时间
			DIGITAL_SECOND_NUM(Disp_Table[g_keepWarmTime/10]);
			DIGITAL_THIRD_NUM(Disp_Table[g_keepWarmTime%10]);

			SYMBOL_MINITE = 1;

			if(g_LidFlag == LID_OPEN)									//开盖时，显示E11
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			
			if(g_waterYieldType == LackWater)							//缺水时，显示E12
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_2);
			}
			break;

		case SysModeEnd:												//工作结束状态
			DIGITAL_FIRST_NUM(DISPDATA_E);
			DIGITAL_SECOND_NUM(DISPDATA_n);
			DIGITAL_THIRD_NUM(DISPDATA_d);
			break;
			
		case SysModeError:												//报警状态
			DIGITAL_FIRST_NUM(DISPDATA_E);								//显示报警代码
			DIGITAL_SECOND_NUM(Disp_Table[(measureData.errCode & 0xf0) >> 4]);
			DIGITAL_THIRD_NUM(Disp_Table[(measureData.errCode & 0x0f)]);
			break;
			
		case SysModePCBCheck:											//PCB自检状态
			switch(g_selfCheckStep)
			{
				case 1:
					DispAllOn();
				break;

				case 2:													//显示显示板发行版本号
					DIGITAL_FIRST_NUM(DISPDATA_A);
					DIGITAL_SECOND_NUM(Disp_Table[RELEASE_VERSION/10]);
					DIGITAL_THIRD_NUM(Disp_Table[RELEASE_VERSION%10]);
				break;

				case 3:													//显示数字123
					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 4:													//显示数字123；菜单1、3
					SYMBOL_FUN_WING		= 1;
					SYMBOL_FUN_CHIPS	= 1;

					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 5:													//显示数字123；菜单1、3,自主空气炸、原味蒸
					SYMBOL_FUN_WING		= 1;
					SYMBOL_FUN_CHIPS	= 1;
					SYMBOL_FUN_BAKE		= 1;
					SYMBOL_FUN_STEAM	= 1;

					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 6:													//显示数字123；菜单1、2、3,自主空气炸、蒸汽烤、原味蒸
					SYMBOL_FUN_WING		= 1;
					SYMBOL_FUN_RICE		= 1;
					SYMBOL_FUN_CHIPS	= 1;
					SYMBOL_FUN_BAKE		= 1;
					SYMBOL_FUN_BA_ST	= 1;
					SYMBOL_FUN_STEAM	= 1;

					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 7:													//显示数字123；菜单1、2、3,自主空气炸、蒸汽烤、原味蒸,℃
					SYMBOL_FUN_WING		= 1;
					SYMBOL_FUN_RICE		= 1;
					SYMBOL_FUN_CHIPS	= 1;
					SYMBOL_FUN_BAKE		= 1;
					SYMBOL_FUN_BA_ST	= 1;
					SYMBOL_FUN_STEAM	= 1;
					SYMBOL_TEMP			= 1;

					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 8:													//显示数字123；菜单1、2、3,自主空气炸、蒸汽烤、原味蒸,℃,分
					if(g_selfCheckOverFlag)								//若自检已完成
					{
						if(g_selfCheckPassFlag)							//若检测合格
						{
							SYMBOL_FUN_WING		= 1;
							SYMBOL_FUN_RICE		= 1;
							SYMBOL_FUN_CHIPS	= 1;
							SYMBOL_FUN_BAKE		= 1;
							SYMBOL_FUN_BA_ST	= 1;
							SYMBOL_FUN_STEAM	= 1;
							SYMBOL_TEMP			= 1;
							SYMBOL_MINITE		= 1;

							DIGITAL_FIRST_NUM(DISPDATA_P);
							DIGITAL_SECOND_NUM(DISPDATA_A);
							DIGITAL_THIRD_NUM(DISPDATA_S);
						}
						else
						{
							//转入报警状态
						}
					}
				break;
			}
			break;
			
		case SysModeMachineCheck:										//整机自检状态
			switch(g_selfCheckStep)
			{
				case 1:													//全亮
					DispAllOn();
				break;

				case 2:													//显示SF1
					DIGITAL_FIRST_NUM(DISPDATA_S);
					DIGITAL_SECOND_NUM(DISPDATA_F);
					DIGITAL_THIRD_NUM(DISPDATA_1);
				break;

				case 3:													//显示电源板发行版本号
					DIGITAL_FIRST_NUM(DISPDATA_P);
					DIGITAL_SECOND_NUM(Disp_Table[((RECV_POWER_VERSION & 0xf0) >> 4)/10]);
					DIGITAL_THIRD_NUM(Disp_Table[((RECV_POWER_VERSION & 0xf0) >> 4)%10]);
				break;

				case 4:													//显示显示板发行版本号
					DIGITAL_FIRST_NUM(DISPDATA_A);
					DIGITAL_SECOND_NUM(Disp_Table[RELEASE_VERSION/10]);
					DIGITAL_THIRD_NUM(Disp_Table[RELEASE_VERSION%10]);
				break;

				case 5:													//显示腔体高温区AD
					DIGITAL_FIRST_NUM(Disp_Table[(measureData.coreAdHValue & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(measureData.coreAdHValue & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(measureData.coreAdHValue & 0x000f)]);
				break;

				case 6:													//显示腔体低温区AD
					DIGITAL_FIRST_NUM(Disp_Table[(measureData.coreAdLValue & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(measureData.coreAdLValue & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(measureData.coreAdLValue & 0x000f)]);
				break;

				case 7:													//显示锅炉高温区AD
					DIGITAL_FIRST_NUM(Disp_Table[(g_adDataHSteam & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(g_adDataHSteam & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(g_adDataHSteam & 0x000f)]);
				break;

				case 8:													//显示锅炉低温区AD
					DIGITAL_FIRST_NUM(Disp_Table[(g_adDataLSteam & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(g_adDataLSteam & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(g_adDataLSteam & 0x000f)]);
				break;

				case 9:
					if(g_LidFlag == LID_CLOSE)							//合盖时显示GGy，开盖时显示GGn
					{
						DIGITAL_FIRST_NUM(DISPDATA_G);
						DIGITAL_SECOND_NUM(DISPDATA_G);
						DIGITAL_THIRD_NUM(DISPDATA_y);
					}
					else
					{
						DIGITAL_FIRST_NUM(DISPDATA_G);
						DIGITAL_SECOND_NUM(DISPDATA_G);
						DIGITAL_THIRD_NUM(DISPDATA_n);
					}
				break;

				case 10:												//显示Sr
					DIGITAL_SECOND_NUM(DISPDATA_S);
					DIGITAL_THIRD_NUM(DISPDATA_r);
				break;

				case 11:
					if(g_pumpWorkFlag)									//检测到水泵工作，显示On
					{
						DIGITAL_FIRST_NUM(DISPDATA_S);
						DIGITAL_SECOND_NUM(DISPDATA_b);
						DIGITAL_THIRD_NUM(DISPDATA_SG);
					}
					else												//未检测到水泵工作，显示Err
					{
						if(g_pumpDelayJudgeFlag)						//检测到无水泵工作，且延时判断时间到
						{
							DIGITAL_FIRST_NUM(DISPDATA_E);
							DIGITAL_SECOND_NUM(DISPDATA_r);
							DIGITAL_THIRD_NUM(DISPDATA_r);
						}
					}
				break;

				case 12:												//显示SN码
					DIGITAL_FIRST_NUM(Disp_Table[RECV_SN/100]);
					DIGITAL_SECOND_NUM(Disp_Table[RECV_SN%100/10]);
					DIGITAL_THIRD_NUM(Disp_Table[RECV_SN%100%10]);
				break;

				case 13:												//全息
					
				break;
			}
			break;
			
		case SysModeDebug:												//Debug状态
			if(g_debugDispStep == 1)
			{
				DIGITAL_FIRST_NUM(DISPDATA_P);
				DIGITAL_SECOND_NUM(Disp_Table[((RECV_POWER_VERSION & 0xf0) >> 4)/10]);
				DIGITAL_THIRD_NUM(Disp_Table[((RECV_POWER_VERSION & 0xf0) >> 4)%10]);
			}
			else if(g_debugDispStep == 2)
			{
				DIGITAL_FIRST_NUM(DISPDATA_d);
				DIGITAL_SECOND_NUM(Disp_Table[(RECV_POWER_VERSION & 0x0f)/10]);
				DIGITAL_THIRD_NUM(Disp_Table[(RECV_POWER_VERSION & 0x0f)%10]);
			}
			else if(g_debugDispStep == 3)
			{
				DIGITAL_FIRST_NUM(DISPDATA_A);
				DIGITAL_SECOND_NUM(Disp_Table[RELEASE_VERSION/10]);
				DIGITAL_THIRD_NUM(Disp_Table[RELEASE_VERSION%10]);
			}
			else if(g_debugDispStep == 4)
			{
				DIGITAL_FIRST_NUM(DISPDATA_d);
				DIGITAL_SECOND_NUM(Disp_Table[DEBUG_VERSION/10]);
				DIGITAL_THIRD_NUM(Disp_Table[DEBUG_VERSION%10]);
			}
			break;
			
		case SysModePID:												//PID状态
			if(g_pidDispStep == 1)										//数码管显示PID码
			{
				DIGITAL_FIRST_NUM(DISPDATA_P);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_d);
			}
			else if(g_pidDispStep == 2)
			{
				DIGITAL_FIRST_NUM(Disp_Table[(FCHUANG_HUO_OCDE_2 & 0xf0) >> 4]);
				DIGITAL_SECOND_NUM(Disp_Table[(FCHUANG_HUO_OCDE_2 & 0x0f)]);
				DIGITAL_THIRD_NUM(Disp_Table[(FCHUANG_HUO_OCDE_1 & 0xf0) >> 4]);
			}
			else if(g_pidDispStep == 3)
			{
				DIGITAL_FIRST_NUM(Disp_Table[(FCHUANG_HUO_OCDE_1 & 0x0f)]);
				DIGITAL_SECOND_NUM(Disp_Table[(FCHUANG_HUO_OCDE_4 & 0xf0) >> 4]);
				DIGITAL_THIRD_NUM(Disp_Table[(FCHUANG_HUO_OCDE_4 & 0x0f)]);
			}
			else if(g_pidDispStep == 4)
			{
				DIGITAL_FIRST_NUM(Disp_Table[(FCHUANG_HUO_OCDE_3 & 0xf0) >> 4]);
				DIGITAL_SECOND_NUM(Disp_Table[(FCHUANG_HUO_OCDE_3 & 0x0f)]);
				DIGITAL_THIRD_NUM(Disp_Table[(FCHUANG_HUO_OCDE_6 & 0xf0) >> 4]);
			}
			else if(g_pidDispStep == 5)
			{
				DIGITAL_FIRST_NUM(Disp_Table[(FCHUANG_HUO_OCDE_6 & 0x0f)]);
				DIGITAL_SECOND_NUM(Disp_Table[(FCHUANG_HUO_OCDE_5 & 0xf0) >> 4]);
				DIGITAL_THIRD_NUM(Disp_Table[(FCHUANG_HUO_OCDE_5 & 0x0f)]);
			}
			else if(g_pidDispStep == 6)
			{
				DIGITAL_FIRST_NUM(Disp_Table[(FCHUANG_HUO_OCDE_8 & 0xf0) >> 4]);
				DIGITAL_SECOND_NUM(Disp_Table[(FCHUANG_HUO_OCDE_8 & 0x0f)]);
				DIGITAL_THIRD_NUM(Disp_Table[(FCHUANG_HUO_OCDE_7 & 0xf0) >> 4]);
			}
			break;
			
		case SysModeClean:												//清洁水垢状态
			if((g_cleanIncrustantStep == 1) || (g_cleanIncrustantStep == 2))		//数码管显示SP1码
			{
				DIGITAL_FIRST_NUM(DISPDATA_S);
				DIGITAL_SECOND_NUM(DISPDATA_P);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			else if(g_cleanIncrustantStep == 3)							//数码管显示SP2码
			{
				DIGITAL_FIRST_NUM(DISPDATA_S);
				DIGITAL_SECOND_NUM(DISPDATA_P);
				DIGITAL_THIRD_NUM(DISPDATA_2);
			}
			break;
	}

	/*Wifi图标控制*/
	wifiStatus = ReadWifiInf(WIFI_STATUS);

	if(wifiStatus == TRY_CONNET_ROUTER)				//WIFI模组处于未联网，尝试连接状态
	{
		if(g_dispFlashFlag == 0)
		{
			DP = 1;			//500ms闪烁
		}
	}
	else if((wifiStatus == SMART_LINK_OK_WAIT) || (wifiStatus == SOFT_AP_STATUS))
	{
		if(g_dispQuickFlashFlag)
		{
			DP = 1;			//100ms闪烁
		}
	}
	else if((wifiStatus == CAN_NOT_CONNET_SERVER) || (wifiStatus == CONNET_ROUTER_OK))
	{
		if(g_dispFlashFlag == 0)
		{
			DP = 1;			//500ms闪烁
		}
	}
	else if(wifiStatus == CONNET_SERVER_OK)			//WIFI模组已连接服务器
	{
		DP = 1;				//全亮
	}
}