#include "header.h"

LOAD_CRL_MESSAGE xdata			loadCrlData;				//���ؿ��Ƶ�����

		 bit					tempZoneFlag;				//������־
		 bit					tempTrendFlag;				//�¶����Ʊ�־
		 bit					g_corePreHeatFlag;			//�����¶�Ԥ�ȱ�־
		 bit					g_corePreHeatDelayFlag;		//Ԥ����ʱ��־
		 bit					g_corePreHeatFirstFlag;		//�״�Ԥ�ȱ�־
		 bit					highSteamFirstHeatFlag;		//110�洿���״μ��ȱ�־
		 bit					g_steamPreHeatFlag;			//��¯�¶�Ԥ�ȱ�־
		 
		 bit					tubeHeatFlag;				//±�عܼ��ȱ�־
		 bit					fanOpenFlag;				//�ּ����ȿ�����־
		 bit					steamOpenFlag;				//��¯������־
		 bit					sanReOpenFlag;				//��Դ��ɢ�ȿ�����־

/*����ը����Ԥ�Ȳ�����²�����ѯ��*/
unsigned int code OnlyBakePreHeat_Table[35]=
{
	TEMP_LOW_028,	TEMP_LOW_030,	TEMP_LOW_032,	TEMP_LOW_035,	TEMP_LOW_039,	//30��~50��

	TEMP_LOW_043,	TEMP_LOW_045,	TEMP_LOW_049,	TEMP_LOW_053,	TEMP_LOW_057,	//55��~75��

	TEMP_LOW_062,	TEMP_LOW_067,	TEMP_LOW_072,	TEMP_LOW_077,	TEMP_LOW_082,	//80��~100��
									
	TEMP_LOW_087,	TEMP_LOW_092,	TEMP_LOW_097,	TEMP_LOW_102,	TEMP_LOW_107,	//105��~125��
	
	TEMP_LOW_112,	TEMP_LOW_117,	TEMP_LOW_122,	TEMP_LOW_127,	TEMP_LOW_132,	//130��~150��
	
	TEMP_HIG_137,	TEMP_HIG_142,	TEMP_HIG_147,	TEMP_HIG_152,	TEMP_HIG_158,	//155��~175��
	
	TEMP_HIG_163,	TEMP_HIG_167,	TEMP_HIG_171,	TEMP_HIG_174,	TEMP_HIG_178,	//180��~200��
};

/*����ը����������²�����ѯ��*/
unsigned int code OnlyBakeRegionHeat_Table[35][2]=
{
//		������			�رյ�			������			�رյ�			������			�رյ�
	{TEMP_LOW_032,	TEMP_LOW_031},	{TEMP_LOW_034,	TEMP_LOW_033},	{TEMP_LOW_036,	TEMP_LOW_035},		//30��~40��
	
	{TEMP_LOW_039,	TEMP_LOW_037},	{TEMP_LOW_043,	TEMP_LOW_041},	{TEMP_LOW_047,	TEMP_LOW_045},		//45��~55��

	{TEMP_LOW_051,	TEMP_LOW_049},	{TEMP_LOW_056,	TEMP_LOW_054},	{TEMP_LOW_061,	TEMP_LOW_059},		//60��~70��
	
	{TEMP_LOW_065,	TEMP_LOW_063},	{TEMP_LOW_070,	TEMP_LOW_068},	{TEMP_LOW_075,	TEMP_LOW_073},		//75��~85��
	
	{TEMP_LOW_080,	TEMP_LOW_078},	{TEMP_LOW_085,	TEMP_LOW_083},	{TEMP_LOW_090,	TEMP_LOW_088},		//90��~100��
	
	{TEMP_LOW_094,	TEMP_LOW_092},	{TEMP_LOW_099,	TEMP_LOW_097},	{TEMP_LOW_104,	TEMP_LOW_102},		//105��~115��
	
	{TEMP_LOW_109,	TEMP_LOW_107},	{TEMP_LOW_114,	TEMP_LOW_112},	{TEMP_LOW_119,	TEMP_LOW_117},		//120��~130��
	
	{TEMP_LOW_123,	TEMP_LOW_121},	{TEMP_LOW_128,	TEMP_LOW_126},	{TEMP_HIG_133,	TEMP_HIG_131},		//135��~145��
	
	{TEMP_HIG_138,	TEMP_HIG_136},	{TEMP_HIG_143,	TEMP_HIG_141},	{TEMP_HIG_148,	TEMP_HIG_146},		//150��~160��
	
	{TEMP_HIG_153,	TEMP_HIG_151},	{TEMP_HIG_158,	TEMP_HIG_156},	{TEMP_HIG_163,	TEMP_HIG_161},		//165��~175��
	
	{TEMP_HIG_168,	TEMP_HIG_166},	{TEMP_HIG_172,	TEMP_HIG_170},	{TEMP_HIG_175,	TEMP_HIG_173},		//180��~190��
	
	{TEMP_HIG_178,	TEMP_HIG_176},	{TEMP_HIG_182,	TEMP_HIG_180},										//195��~200��
};

/*��������Ԥ�Ȳ�����²�����ѯ��*/
unsigned int code OnlySteamPreHeat_Table[7]=
{
	TEMP_LOW_071,	TEMP_LOW_074,	TEMP_LOW_077,	TEMP_LOW_080,	TEMP_LOW_080,	TEMP_LOW_060,	TEMP_LOW_060,	//80��~110��
};

/*��������������²�����ѯ��*/
unsigned int code OnlySteamRegionHeat_Table[7][3]=
{
//		������			�رյ�			���ʵ�λ
	TEMP_LOW_080,	TEMP_LOW_081,	PLATE_GEAR_6_20,	//80��
	
	TEMP_LOW_083,	TEMP_LOW_084,	PLATE_GEAR_8_20,	//85��
	
	TEMP_LOW_086,	TEMP_LOW_087,	PLATE_GEAR_10_20,	//90��
	
	TEMP_LOW_091,	TEMP_LOW_092,	PLATE_GEAR_12_20,	//95��
	
	TEMP_HIG_199,	TEMP_HIG_200,	PLATE_GEAR_14_20,	//100��		һֱ������
	
	/*±�عܸ������ȵ�*/
	TEMP_LOW_102,	TEMP_LOW_103,	PLATE_GEAR_14_20,	//105��
	
	TEMP_LOW_102,	TEMP_LOW_103,	PLATE_GEAR_14_20,	//110��
};

/*����������������²�����ѯ��*/
unsigned int code SteamBakeRegionHeat_Table[21]=
{
	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	//100��~120��
	
	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_2_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	//125��~145��
	
	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	PLATE_GEAR_4_20,	//150��~170��

	PLATE_GEAR_6_20,	PLATE_GEAR_6_20,	PLATE_GEAR_6_20,	PLATE_GEAR_6_20,	PLATE_GEAR_8_20,	//175��~195��
	
	PLATE_GEAR_8_20,																					//200��
};


/*-----------------------------------------------------------------------------
Description:		��ʼ������IO�����ư�ĸ���ֻ��ɢ�ȷ��ȣ�Ԥ��
					LED�ƣ�			P2.4
					����ͷ���ȣ�	P2.5
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitLoadIo(void)
{
	BANK0_SET;
	
	P2CR |= (BIT4 | BIT5);											//��������������������
	P2PCR &= (~(BIT4 | BIT5));
	P2 &= (~(BIT4 | BIT5));
}

/*-----------------------------------------------------------------------------
Description:		��ʼ�����ر���
Input:				void
Return:				void
History:			��
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
	loadCrlData.heatJudgeCycCnt = HEAT_JUDGE_TIME - 1;		//�������������¶�
	loadCrlData.pumpJudgeCycCnt = 0;
	loadCrlData.forceSubWaterCnt= 0;
	
	loadCrlData.plateHeatGear 	= 0;
	loadCrlData.pumpFreGear		= 0;
	
	tempZoneFlag	= 0;
	tempTrendFlag	= 1;									//����ʱϣ���¶�����
	
	g_corePreHeatFlag		= 0;
	g_corePreHeatDelayFlag	= 0;
	g_corePreHeatFirstFlag	= 1;
	highSteamFirstHeatFlag	= 1;							//����ʱĬ�ϸ��´����״μ�����ɣ���������⵽�¶ȵͻ����������״μ��ȣ�
	g_steamPreHeatFlag		= 0;
	
	tubeHeatFlag			= 0;
	fanOpenFlag				= 0;
	steamOpenFlag			= 0;
	sanReOpenFlag			= 0;
}

/*-----------------------------------------------------------------------------
Description:		���ؿ��²���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void LoadHeatCrlData(void)
{
	if(g_nowStepTechnology == ONLY_BAKE)					//����ը����
	{
		loadCrlData.corepreHeatTemp		= (OnlyBakePreHeat_Table[(g_nowStepworkTemp - 30)/5]);
		loadCrlData.tubeHeatTempOn  	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 30)/5][0]);
		loadCrlData.tubeHeatTempOff 	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 30)/5][1]);
		loadCrlData.plateHeatGear		= 0;
	}
	else if(g_nowStepTechnology == ONLY_STEAM)				//ԭζ������
	{
		loadCrlData.corepreHeatTemp 	= (OnlySteamPreHeat_Table[(g_nowStepworkTemp - 80)/5]);
		loadCrlData.steamHeatTempOn 	= (OnlySteamRegionHeat_Table[(g_nowStepworkTemp - 80)/5][0]);
		loadCrlData.steamHeatTempOff	= (OnlySteamRegionHeat_Table[(g_nowStepworkTemp - 80)/5][1]);
		
		if(measureData.coreAdLValue >= loadCrlData.corepreHeatTemp)				//�����¶ȵ���Ԥ�ȵ㣬��¯ȫ���ʼ���
		{
			
			/*С���Բ�E06������ĳ���20191027*/
			if(g_pumpCalibrationFlag == 0)				//ˮ�õ���δ�궨ǰ
			{
				if(g_reachStopHeatFlag == 0)			//δ����ǿ�ص�ʱ����¯�߹��ʼ���
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
		else												//�����¶ȸ���Ԥ�ȵ�
		{
			if((g_adDataHSteam > STEAM_PRE_HEAT_AD) || (g_pumpCalibrationFlag == 0))	//��¯�¶ȵ���Ԥ�ȵ��ˮ�õ���δ�궨ʱ����¯ȫ���ʼ���
			{
				
				/*С���Բ�E06������ĳ���20191027*/
				if(g_pumpCalibrationFlag == 0)				//ˮ�õ���δ�궨ǰ
				{
					if(g_reachStopHeatFlag == 0)			//δ����ǿ�ص�ʱ����¯�߹��ʼ���
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
			else											//�¶ȸ��ڹ�¯Ԥ�ȵ���ˮ���ѱ궨ʱ����¯���趨���ʼ���
			{
				if((g_nowStepworkTemp >= 100) && (g_steamLowPowerFlag == 1))	//����ǰ�˵������¶ȡ�100�棬�ҹ�¯�͹��������־Ϊ1
				{
					loadCrlData.plateHeatGear = PLATE_GEAR_10_20;				//��¯��50%�������
				}
				else										//����100�����»�߹������ʱ��δ�������ֵ�����趨�������
				{
					loadCrlData.plateHeatGear = (OnlySteamRegionHeat_Table[(g_nowStepworkTemp - 80)/5][2]);
				}
			}
		}
	}
	else if(g_nowStepTechnology == BAKE_STEAM)				//����������
	{
		loadCrlData.corepreHeatTemp 	= (OnlyBakePreHeat_Table[(g_nowStepworkTemp - 100)/5 + 14]);
		loadCrlData.tubeHeatTempOn  	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 100)/5 + 14][0]);
		loadCrlData.tubeHeatTempOff 	= (OnlyBakeRegionHeat_Table[(g_nowStepworkTemp - 100)/5 + 14][1]);
		
		if((g_adDataHSteam > STEAM_PRE_HEAT_AD) || (g_pumpCalibrationFlag == 0))	//��¯�¶ȵ���Ԥ�ȵ��ˮ�õ���δ�궨ʱ����¯ȫ���ʼ���
		{
			
			/*С���Բ�E06������ĳ���20191027*/
			if(g_pumpCalibrationFlag == 0)				//ˮ�õ���δ�궨ǰ
			{
				if(g_reachStopHeatFlag == 0)			//δ����ǿ�ص�ʱ����¯�߹��ʼ���
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
		else												//�¶ȸ��ڹ�¯Ԥ�ȵ��ˮ�õ����ѱ궨ʱ����¯���趨���ʼ���
		{
			loadCrlData.plateHeatGear	= (SteamBakeRegionHeat_Table[(g_nowStepworkTemp - 100)/5]);
		}
	}
	else													//�޹���״̬
	{
		loadCrlData.corepreHeatTemp	= 0x3ff;
		loadCrlData.tubeHeatTempOn	= 0x3ff;
		loadCrlData.tubeHeatTempOff	= 0x3ff;
		loadCrlData.plateHeatGear	= 0;
	}
}

/*-----------------------------------------------------------------------------
Description:		±�عܼ�Ъ���ȿ���
Input:				funCnt��������ǰ����ʱ��
					val��	���ؿ���ʱ��
					T:		���ؿ����ж�����
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void TubeInterimHeat(unsigned char funCnt, unsigned char val, unsigned char T)//������/±�عܿ���ʱ�䣬�����ڣ�
{
	static unsigned char s_cnt = 0;

	if((val == 0) || (T == 0))
	{
		s_cnt = 0;
		
		tubeHeatFlag = CLOSE_LOAD;							//��±�ع�
		fanOpenFlag  = CLOSE_LOAD;							//���ּ�����
	}
	else
	{
		s_cnt++;
		if(s_cnt <= funCnt)									//����ǰ�ȿ����ȣ�2sʱ��
		{
			fanOpenFlag  = OPEN_LOAD;						//���ּ�����
		}
		else if(s_cnt <= (val + funCnt))
		{
			tubeHeatFlag = OPEN_LOAD;						//��±�ع�
			fanOpenFlag  = OPEN_LOAD;						//���ּ�����
		}
		else
		{
			tubeHeatFlag = CLOSE_LOAD;						//��±�ع�
			fanOpenFlag  = CLOSE_LOAD;						//���ּ�����
			
			if(s_cnt >= T)									//�������ڽ���ѭ��
			{
				s_cnt = 0;
				
				if(highSteamFirstHeatFlag == 0)				//�����״����ڽ������״α�־λ��һ
				{
					highSteamFirstHeatFlag = 1;
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		��������ر�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void CloseAllOutPut(void)
{
	/*��־λ�͵�λ����*/
	tubeHeatFlag	= 0;
	fanOpenFlag		= 0;
	steamOpenFlag	= 0;
	sanReOpenFlag	= 0;
	loadCrlData.plateHeatGear 	= 0;
	loadCrlData.pumpFreGear		= 0;

	/*������������*/
	SEND_OUT_DATA1		= 0;								//�ر��������
	SEND_OUT_DATA_STEAM = 0;
	SEND_OUT_DATA_PUMP	= 0;
}

/*-----------------------------------------------------------------------------
Description:		���ȿ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void HeatCrl(void)
{
	static unsigned char s_time250msCnt = 0;				//250ms��ʱ

	LoadHeatCrlData();										//���ؿ��²�����ˮ�õ����궨��ɺ�ʱ������ˮ��λ��

	s_time250msCnt++;
	if(s_time250msCnt >= 25)								//10msʱ����250ms�ж�һ��
	{
		s_time250msCnt = 0;

		if(g_nowStepTechnology == ONLY_BAKE)				//����ը����
		{
			OnlyBakeHeatCrl();								//����ը�¶ȿ���
		}
		else if(g_nowStepTechnology == ONLY_STEAM)			//��������
		{
			OnlySteamHeatCrl();								//�����¶ȿ���
		
			/*С���Բ�E06������ĳ���20191027*/
			if(g_pumpCalibrationFlag == 0)					//ˮ�õ���δ�궨ǰ
			{
				if(g_reachStopHeatFlag == 0)				//δ����ǿ�ص�ʱ����¯����
				{
					steamOpenFlag = OPEN_LOAD;				//������¯
				}
				else										//����ǿ�ص�󣬹�¯�ر�
				{
					steamOpenFlag = CLOSE_LOAD;
				}
			}
			
		}
		else if(g_nowStepTechnology == BAKE_STEAM)			//����������
		{
			SteamBakeHeatCrl();								//�����¶ȿ���
			
			/*С���Բ�E06������ĳ���20191027*/
			if(g_pumpCalibrationFlag == 0)					//ˮ�õ���δ�궨ǰ
			{
				if(g_reachStopHeatFlag == 0)				//δ����ǿ�ص�ʱ����¯����
				{
					steamOpenFlag = OPEN_LOAD;				//������¯
				}
				else										//����ǿ�ص�󣬹�¯�ر�
				{
					steamOpenFlag = CLOSE_LOAD;
				}
			}
			
		}
		else												//�޹���״̬
		{
			CloseAllOutPut();								//�ر��������
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		����ը���ȿ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void OnlyBakeHeatCrl(void)
{
	if(g_corePreHeatFlag == 0)								//��δԤ�ȣ�����Ԥ��
	{
		OnlyBakePreHeatCrl();								//ִ�п���ը��Ԥ���ӳ���
	}
	else													//Ԥ����ɺ������½׶�
	{
		OnlyBakeRegionHeatCrl();							//ִ�п���ը��������ӳ���
	}
	
	if(g_firstAdGetGlag)									//���õ����״�ǻ��AD����ֵ
	{
		fanOpenFlag = OPEN_LOAD;							//����ը����ȫ�̿��ּ�����
	}
	else
	{
		fanOpenFlag = CLOSE_LOAD;							//�����ڼ�ر��ּ�����
	}
	steamOpenFlag	= CLOSE_LOAD;							//ȫ�̹�¯�ر�
}

/*-----------------------------------------------------------------------------
Description:		�������ȿ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void OnlySteamHeatCrl(void)
{
	if(g_corePreHeatFlag == 0)								//��δԤ�ȣ�����Ԥ��
	{
		OnlySteamPreHeatCrl();								//ִ�д�����Ԥ���ӳ���
		
		tubeHeatFlag = CLOSE_LOAD;							//Ԥ�Ƚ׶η��ȹܹر�
		fanOpenFlag	 = CLOSE_LOAD;							//Ԥ�Ƚ׶��ּ�������ȹر�
	}
	else													//Ԥ����ɺ������½׶�
	{
		OnlySteamRegionHeatCrl();							//ִ�д�����������ӳ���
	}
}

/*-----------------------------------------------------------------------------
Description:		���������ȿ��ƣ����·�ʽͬ���������ܣ��������Ӳ�ͬ���ʵĴ������ܣ�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void SteamBakeHeatCrl(void)
{
	if(g_corePreHeatFlag == 0)								//��δԤ�ȣ�����Ԥ��
	{
		OnlyBakePreHeatCrl();								//ִ�п���ը��Ԥ���ӳ���
	}
	else													//Ԥ����ɺ������½׶�
	{
		OnlyBakeRegionHeatCrl();							//ִ�п���ը��������ӳ���
	}

	if(g_firstAdGetGlag)									//���õ����״�ǻ��AD����ֵ
	{
		fanOpenFlag = OPEN_LOAD;							//ȫ�̿��ּ�����
	}
	else
	{
		fanOpenFlag = CLOSE_LOAD;							//�����ڼ�ر��ּ�����
	}

	steamOpenFlag	= OPEN_LOAD;							//ȫ�̿�����¯
}

/*-----------------------------------------------------------------------------
Description:		����ըԤ���ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void OnlyBakePreHeatCrl(void)
{
	unsigned int tubePreHeatTempBuf;
	
	static unsigned char s_onTimeCnt = 0;					//Ԥ�ȼ�Ъ���ȵ�����ʱ�����
	static bit 			 s_tubeHeatState = 0;				//±�عܼ���״̬

	if(g_corePreHeatDelayFlag == 1)							//Ԥ����ʱ�У������ȣ�
	{
		tubeHeatFlag = CLOSE_LOAD;							//��±�ع�
		
		loadCrlData.preHeatDelayCnt++;
		if(loadCrlData.preHeatDelayCnt >= PRE_DEALY_TIME)	//Ԥ���ӳ�40*250ms=10s
		{
			g_corePreHeatFlag				= 1;			//Ԥ�ȼ�Ԥ����ʱ���
			g_corePreHeatDelayFlag			= 0;			//����Ԥ����ʱ״̬��Ԥ����ʱ������
			g_corePreHeatFirstFlag			= 1;			//����״�Ԥ��
			loadCrlData.preHeatDelayCnt		= 0;
			
			tempTrendFlag = 1;								//Ԥ�����¶���������
		}
	}
	else													//����Ԥ����ʱ״̬
	{
		if((loadCrlData.corepreHeatTemp & 0x8000) == 0x8000)	//���ڸ�����
		{
			tubePreHeatTempBuf = loadCrlData.corepreHeatTemp & 0x7FFF;
			
			loadCrlData.coreNowAd = measureData.coreAdHValue;	//��ֵ������AD
			
			if(tempZoneFlag == 0)							//�鿴�Ƿ���������л�״̬
			{
				loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//���ϴδ��ڵ������������ڵ�ADֵ��ֵ���ϴε�ֵ
			}
			tempZoneFlag = 1;
		}
		else												//���ڵ�����
		{
			tubePreHeatTempBuf = loadCrlData.corepreHeatTemp;
			
			loadCrlData.coreNowAd = measureData.coreAdLValue;	//��ֵ������AD
			
			if(tempZoneFlag == 1)							//�鿴�Ƿ���������л�״̬
			{
				loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//���ϴδ��ڵ������������ڵ�ADֵ��ֵ���ϴε�ֵ
			}
			tempZoneFlag = 0;
		}

		if(g_corePreHeatFirstFlag == 1)						//���״�Ԥ��
		{
			if(loadCrlData.coreNowAd > tubePreHeatTempBuf)	//��ǰ�¶ȵ���Ԥ�ȵ��¶�
			{
				tubeHeatFlag = OPEN_LOAD;					//����±�ع�
				
				g_corePreHeatFlag			= 0;
				g_corePreHeatDelayFlag		= 0;
				g_corePreHeatFirstFlag		= 0;			//���½����״�Ԥ��
				loadCrlData.preHeatDelayCnt	= 0;
			}
			else											//��ǰ�¶ȴﵽ������Ԥ���ӳ�
			{
				tubeHeatFlag = CLOSE_LOAD;					//��±�ع�
			
				g_corePreHeatFlag			= 1;			//ֱ�ӽ���Ԥ�Ƚ׶�
				g_corePreHeatDelayFlag		= 0;
				g_corePreHeatFirstFlag		= 1;
				loadCrlData.preHeatDelayCnt	= 0;
				
				tempTrendFlag = 1;							//Ԥ�����¶���������
			}
		}
		else												//�״�Ԥ��
		{
			if(loadCrlData.coreNowAd > tubePreHeatTempBuf)	//��ǰ�¶ȵ���Ԥ���¶�
			{
				tubeHeatFlag = OPEN_LOAD;					//��±�ع�
				
				g_corePreHeatFlag			= 0;
				g_corePreHeatDelayFlag		= 0;
				g_corePreHeatFirstFlag		= 0;
				loadCrlData.preHeatDelayCnt	= 0;
			}
			else
			{
				tubeHeatFlag = CLOSE_LOAD;				//��±�ع�
				
				g_corePreHeatFlag			= 0;
				g_corePreHeatDelayFlag		= 1;
				g_corePreHeatFirstFlag		= 0;
				loadCrlData.preHeatDelayCnt	= 0;
			}
		}
		
		/*��Գ��¹������⣬Ԥ�Ƚ׶�ʹ�ü�Ъ����*/
		if(measureData.coreAdHValue < (TEMP_HIG_140 & 0x7FFF))	//Ԥ�Ƚ׶��¶ȸ���150��󣬲��ü�Ъ���ȣ��˴��迼���Ⱥ�������ܵ�ȫ����ʱ�䣩
		{
			if(tubeHeatFlag == OPEN_LOAD)						//��±�عܿ���
			{
				s_onTimeCnt++;
				if(s_onTimeCnt <= 12)							//250msʱ���������ڼ���һ��ʱ��ͣһ��ʱ��
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
			else												//����ֹ±�عܼ��ȣ������ڼ�ʱ����
			{
				s_onTimeCnt		= 0;
				s_tubeHeatState = CLOSE_LOAD;
			}
			
			if(s_tubeHeatState == OPEN_LOAD)					//��±�ع�״̬�ж�Ϊ��������±�ع�
			{
				tubeHeatFlag = OPEN_LOAD;
			}
			else												//��±�ع�״̬�ж�Ϊ�رգ����±�ع�
			{
				tubeHeatFlag = CLOSE_LOAD;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		����ը���½׶��ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void OnlyBakeRegionHeatCrl(void)
{
	unsigned int tubeHeatTempOnBuf;
	unsigned int tubeHeatTempOffBuf;
	
	unsigned char judgeFre = 0;

	if((loadCrlData.tubeHeatTempOn & 0x8000) == 0x8000)		//���ڸ�����
	{
		tubeHeatTempOnBuf	= loadCrlData.tubeHeatTempOn & 0x7FFF;
		tubeHeatTempOffBuf	= loadCrlData.tubeHeatTempOff & 0x7FFF;
		
		loadCrlData.coreNowAd = measureData.coreAdHValue;	//��ֵ������AD
		
		if(tempZoneFlag == 0)								//�鿴�Ƿ���������л�״̬
		{
			loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//���ϴδ��ڵ������������ڵ�ADֵ��ֵ���ϴε�ֵ
		}
		tempZoneFlag = 1;
	}
	else													//���ڵ�����
	{
		tubeHeatTempOnBuf	= loadCrlData.tubeHeatTempOn;
		tubeHeatTempOffBuf	= loadCrlData.tubeHeatTempOff;
		
		loadCrlData.coreNowAd = measureData.coreAdLValue;	//��ֵ������AD
		
		if(tempZoneFlag == 1)								//�鿴�Ƿ���������л�״̬
		{
			loadCrlData.coreLastAd = loadCrlData.coreNowAd;	//���ϴδ��ڵ������������ڵ�ADֵ��ֵ���ϴε�ֵ
		}
		tempZoneFlag = 0;
	}

	/*�������*/
	if(g_nowStepworkTemp < 100)
	{
		judgeFre = HEAT_JUDGE_TIME;
	}
	else
	{
		judgeFre = HEAT_JUDGE_TIME_LONG;
	}
	
	loadCrlData.heatJudgeCycCnt++;
	if(loadCrlData.heatJudgeCycCnt >= judgeFre)				//����ÿ�����ж�һ��
	{
		loadCrlData.heatJudgeCycCnt = 0;

		/*	�˴����³���������һ�ο��³����Ч
		if(loadCrlData.coreNowAd >= tubeHeatTempOffBuf )	//��ǰ�¶ȱȹرյ㻹��
		{
			tubeHeatFlag = OPEN_LOAD;						//��±�ع�
			tempTrendFlag = 1;								//�¶�����״̬
		}
		else if(loadCrlData.coreNowAd <= tubeHeatTempOnBuf)	//��ǰ�¶ȱȿ����㻹��
		{
			tubeHeatFlag = CLOSE_LOAD;						//��±�ع�
			tempTrendFlag = 0;								//�¶��½�״̬
		}
		else												//�м�����
		{
			if(tempTrendFlag)								//�¶�Ϊ����״̬
			{
				if(loadCrlData.coreNowAd < tubeHeatTempOffBuf)	//�¶ȵ���رյ�
				{
					tubeHeatFlag = CLOSE_LOAD;				//��±�ع�
					tempTrendFlag = 0;						//�¶��½�״̬
				}
			}
			else											//�¶�Ϊ�½�״̬
			{
				if(loadCrlData.coreNowAd > tubeHeatTempOnBuf)	//�¶ȵ��ڿ�����
				{
					tubeHeatFlag = OPEN_LOAD;	//��±�ع�
					tempTrendFlag = 1;						//�¶�����״̬
				}
			}
		}
		
		loadData.coreLastAd = loadData.coreNowAd;			//�����ϴ�ADֵ
		*/
		
		if(loadCrlData.coreNowAd > (tubeHeatTempOffBuf + 3))//��ǰ�¶ȱȹرյ㻹��
		{
			tubeHeatFlag = OPEN_LOAD;						//��±�ع�
			tempTrendFlag = 1;								//�¶�����״̬
		}
		else if(loadCrlData.coreNowAd < (tubeHeatTempOnBuf - 3))	//��ǰ�¶ȱȿ����㻹��
		{
			tubeHeatFlag = CLOSE_LOAD;						//��±�ع�
			tempTrendFlag = 0;								//�¶��½�״̬
		}
		else												//�м�����
		{
			if(tempTrendFlag)								//�¶�Ϊ����״̬
			{
				if(loadCrlData.coreNowAd >= loadCrlData.coreLastAd)	//�¶��½��򲻱�
				{
					tubeHeatFlag = OPEN_LOAD;				//��±�ع�
					tempTrendFlag = 1;						//�¶�����״̬
				}
				else										//�¶�����
				{
					if(loadCrlData.coreNowAd <= tubeHeatTempOffBuf)	//�¶ȵ���رյ�
					{
						tubeHeatFlag = CLOSE_LOAD;			//��±�ع�
						tempTrendFlag = 0;					//�¶��½�״̬
					}
					else									//�¶�δ����رյ�
					{
						tubeHeatFlag = OPEN_LOAD;			//��±�ع�
						tempTrendFlag = 1;					//�¶�����״̬
					}
				}
			}
			else											//�¶�Ϊ�½�״̬
			{
				if(loadCrlData.coreNowAd >= loadCrlData.coreLastAd)	//�¶��½��򲻱�
				{
					if(loadCrlData.coreNowAd >= tubeHeatTempOnBuf)	//�¶ȵ��￪����
					{
						tubeHeatFlag = OPEN_LOAD;			//��±�ع�
						tempTrendFlag = 1;					//�¶�����״̬
					}
					else									//�¶ȴ��ڹرյ�
					{
						tubeHeatFlag = CLOSE_LOAD;			//��±�ع�
						tempTrendFlag = 0;					//�¶��½�״̬
					}
				}
				else										//�¶�����
				{
					tubeHeatFlag = CLOSE_LOAD;				//��±�ع�
					tempTrendFlag = 0;						//�¶��½�״̬
				}
			}
		}
		
		loadCrlData.coreLastAd = loadCrlData.coreNowAd;		//�����ϴ�ADֵ
	}
}

/*-----------------------------------------------------------------------------
Description:		����Ԥ���ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void OnlySteamPreHeatCrl(void)
{
	if(measureData.coreAdLValue < loadCrlData.corepreHeatTemp)	//����Ԥ���¶ȵ�
	{
		g_corePreHeatFlag = 1;								//���Ԥ�����

		g_steamPreHeatFlag = 0;								//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
	}
	else
	{
		highSteamFirstHeatFlag = 0;							//������´����״μ��ȱ�־�����½��и��´����״μ��ȣ�
		
		steamOpenFlag = OPEN_LOAD;							//������¯
	}
}

/*-----------------------------------------------------------------------------
Description:		�������½׶��ӳ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void OnlySteamRegionHeatCrl(void)
{
	unsigned int steamHeatTempOnBuf;
	unsigned int steamHeatTempOffBuf;
	static	 bit tubeInterimHeatFlag = 0;

	if((loadCrlData.steamHeatTempOn & 0x8000) == 0x8000)	//���ڸ�����
	{
		steamHeatTempOnBuf	= loadCrlData.steamHeatTempOn & 0x7FFF;
		steamHeatTempOffBuf	= loadCrlData.steamHeatTempOff & 0x7FFF;
		
		loadCrlData.coreNowAd = measureData.coreAdHValue;	//��ֵ������AD
	}
	else													//���ڵ�����
	{
		steamHeatTempOnBuf	= loadCrlData.steamHeatTempOn;
		steamHeatTempOffBuf	= loadCrlData.steamHeatTempOff;
		
		loadCrlData.coreNowAd = measureData.coreAdLValue;	//��ֵ������AD
	}
	
	/*�������*/
	loadCrlData.heatJudgeCycCnt++;
	if(loadCrlData.heatJudgeCycCnt >= HEAT_JUDGE_TIME)		//����2S�ж�һ��
	{
		loadCrlData.heatJudgeCycCnt = 0;
		
		if(g_nowStepworkTemp > 100)							//���������趨�¶ȸ���100��ʱ��±�عܸ������ȣ�һֱ������
		{
			if(loadCrlData.coreNowAd > steamHeatTempOnBuf)	//��ǰ�¶ȱȿ����㻹��
			{
				tubeInterimHeatFlag = 1;					//��±�عܼ�Ъ����
				tempTrendFlag = 1;							//�¶�����״̬
			}
			else if(loadCrlData.coreNowAd < steamHeatTempOffBuf)//��ǰ�¶ȱȹرյ㻹��
			{
				tubeInterimHeatFlag = 0;					//��±�عܼ�Ъ����
				tempTrendFlag = 0;							//�¶��½�״̬
			}
			else
			{
				if(tempTrendFlag)
				{
					tubeInterimHeatFlag = 1;				//��±�عܼ�Ъ����
					tempTrendFlag = 1;						//�¶�����״̬
				}
				else
				{
					tubeInterimHeatFlag = 0;				//��±�عܼ�Ъ����
					tempTrendFlag = 0;						//�¶��½�״̬
				}
			}
			
			if(tubeInterimHeatFlag)							//±�ع���Ҫ������Ъ����
			{
				if(highSteamFirstHeatFlag == 0)				//δ�����״μ���
				{
					TubeInterimHeat(5,3,15);				//��ֵ������ǰʱ�䣬����ʱ������ڣ�2sʱ��
				}
				else
				{
					if(g_nowStepworkTemp == 105)			//�����¶�Ϊ105��ʱ
					{
						TubeInterimHeat(5,1,50);			//��ֵ������ǰʱ�䣬����ʱ������ڣ�2sʱ��
					}
					else if(g_nowStepworkTemp == 110)
					{
						TubeInterimHeat(5,1,20);			//��ֵ������ǰʱ�䣬����ʱ������ڣ�2sʱ��
					}
					else
					{
						TubeInterimHeat(0,0,0);				//��ֵ������ǰʱ�䣬����ʱ������ڶ�Ϊ0��±�عܲ����ȣ�
					}
				}
			}
			else
			{
				TubeInterimHeat(0,0,0);						//��ֵ������ǰʱ�䣬����ʱ������ڶ�Ϊ0��±�عܲ����ȣ�
			}
			
			steamOpenFlag = OPEN_LOAD;						//һֱ������¯
		}
		else												//��������80��~100�棬ֻ����������
		{
			if(loadCrlData.coreNowAd > steamHeatTempOnBuf)	//��ǰ�¶ȱȿ����㻹��
			{
				steamOpenFlag = OPEN_LOAD;					//������¯
				tempTrendFlag = 1;							//�¶�����״̬
			}
			else if(loadCrlData.coreNowAd < steamHeatTempOffBuf)//��ǰ�¶ȱȹرյ㻹��
			{
				steamOpenFlag = CLOSE_LOAD;					//�رչ�¯
				tempTrendFlag = 0;							//�¶��½�״̬
			}
			else
			{
				if(tempTrendFlag)
				{
					steamOpenFlag = OPEN_LOAD;				//������¯
					tempTrendFlag = 1;						//�¶�����״̬
				}
				else
				{
					steamOpenFlag = CLOSE_LOAD;				//�رչ�¯
					tempTrendFlag = 0;						//�¶��½�״̬
				}
			}
			
			tubeHeatFlag	= CLOSE_LOAD;					//100�漰���´������ȹܹر�
			fanOpenFlag		= CLOSE_LOAD;					//100�漰���´����ּ�������ȹر�
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		ˮ�ü�ˮ����
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void PumpCrl(void)
{
	unsigned char xdata defrentAd = 0;						//AD��ֵ
	unsigned char xdata changeGear = 0; 					//��ˮ�����ĵ�λ

	/*Ԥ�Ƚ׶�*/
	if(g_steamPreHeatFlag == 0)								//δԤ��ʱ
	{
		if(g_adDataHSteam > STEAM_PRE_HEAT_AD)				//�¶ȵ���Ԥ�ȵ㣬����ˮ
		{
			loadCrlData.pumpFreGear = 0;
		}
		else												//�¶ȸ���Ԥ�ȵ�
		{	
			g_steamPreHeatFlag = 1;							//����Ԥ�Ƚ׶�
			
			if(g_pumpCalibrationFlag == 0)					//ˮ�õ���δ�궨ǰ��ˮ�ù̶���ˮƵ��
			{
				if(g_reachStopHeatFlag == 0)				//δ����ǿ�ص�ʱ����ˮƵ����
				{
					loadCrlData.pumpFreGear = 60;
				}
				else										//ˮ�ñ궨����ǿ�ص���ˮƵ�ʿ�
				{
					loadCrlData.pumpFreGear = 25;
				}
			}
			else
			{
				if(loadCrlData.plateHeatGear > PLATE_HIGH_GEAR)	//���ݹ�¯���ʵ�λѡ���ˮ������ֵԽ�󣬳�ˮƵ��ԽС��
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
	
	/*��ˮƵ���Զ����ڽ׶�*/
	else
	{
		/*С���Բ�E06������ĳ���20191027*/
		if(g_pumpCalibrationFlag == 0)							//ˮ�õ���δ�궨ǰ��ˮ�ù̶���ˮƵ��
		{
			if(g_reachStopHeatFlag == 0)						//δ����ǿ�ص�ʱ����ˮƵ����
			{
				loadCrlData.pumpFreGear = 60;
			}
			else												//ˮ�ñ궨����ǿ�ص���ˮƵ�ʿ�
			{
				loadCrlData.pumpFreGear = 25;
			}
		}
		else
		{
			loadCrlData.pumpJudgeCycCnt++;

			if(loadCrlData.pumpJudgeCycCnt >= PUMP_JUDGE_TIME)	//��һ�������ж�����/����ֵ
			{
				loadCrlData.pumpJudgeCycCnt = 0;

				loadCrlData.forceSubWaterCnt++;
				if(loadCrlData.forceSubWaterCnt >= 2)			//�����ж����ڣ�ǿ�Ƽ��ٳ�ˮ��
				{
					loadCrlData.forceSubWaterCnt = 0;
					if(loadCrlData.pumpFreGear < PUMP_GEAR_MAX)
					{
						loadCrlData.pumpFreGear++;				//ǿ�Ƽ��ٳ�ˮ��
					}
					else
					{
						loadCrlData.pumpFreGear = PUMP_GEAR_MAX;
					}
				}

				if(g_adDataHSteam < STEAM_MAX_TEMP_AD)			//��¯�¶ȸ���ǿ�ƹر��¶�
				{
					if(loadCrlData.pumpFreGear > (PUMP_GEAR_MIN + 5))		//��֤��С����С��ˮ��λ����λԽС����ˮԽ�ࣩ
					{
						loadCrlData.pumpFreGear = loadCrlData.pumpFreGear - 5;		//����ӿ��ˮƵ��
					}
					else
					{
						loadCrlData.pumpFreGear = PUMP_GEAR_MIN;
					}
				}

				else if(g_adDataHSteam > STEAM_LOW_TEMP_AD)		//��¯�¶Ƚϵ�ʱ
				{
					if(loadCrlData.pumpFreGear < (PUMP_GEAR_MAX - 8))	//��֤����������ˮ��λ
					{
						loadCrlData.pumpFreGear = loadCrlData.pumpFreGear + 8;		//������ٳ�ˮƵ��
					}
					else
					{
						loadCrlData.pumpFreGear = PUMP_GEAR_MAX;
					}
				}

				else											//����ǰ���´������������¶����䣬�򰴱仯��΢��
				{
					if(loadCrlData.SteamLastAd != 0)			//�״β������ж�
					{
						/*������΢��������ˮƵ��*/
						if(g_adDataHSteam > loadCrlData.SteamLastAd)		//��ǰ����
						{
							defrentAd = g_adDataHSteam - loadCrlData.SteamLastAd;
							changeGear = defrentAd/4;
							if(changeGear > 2)
							{
								changeGear = 2;					//���Ƴ�ˮ�����ٶ�
							}
							
							if(loadCrlData.pumpFreGear < (PUMP_GEAR_MAX - changeGear))//��֤��������ˮ��λ
							{
								loadCrlData.pumpFreGear = loadCrlData.pumpFreGear + changeGear;	//��ˮƵ�ʼ���
							}
							else
							{
								loadCrlData.pumpFreGear = PUMP_GEAR_MAX;
							}
						}
			
						/*������΢���ӿ��ˮƵ��*/
						else if(g_adDataHSteam < loadCrlData.SteamLastAd)	//��ǰ����
						{
							defrentAd = loadCrlData.SteamLastAd - g_adDataHSteam;
							changeGear = defrentAd/4;
							if(changeGear >= 3)
							{
								changeGear = 3;					//���Ƴ�ˮ�ӿ��ٶȣ��ӿ��ٶȽϿ죬�ù�¯�¶Ȳ����ڹ��ߣ�
							}
							
							if(loadCrlData.pumpFreGear > (PUMP_GEAR_MIN + changeGear))//��֤��С����С��ˮ��λ
							{
								loadCrlData.pumpFreGear = loadCrlData.pumpFreGear - changeGear;	//��ˮƵ�ʼӿ�
							}
							else
							{
								loadCrlData.pumpFreGear = PUMP_GEAR_MIN;
							}
						}
					}
				}

				loadCrlData.SteamLastAd = g_adDataHSteam;		//ˢ�¹�¯�ϴ�ADֵ
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		���ص�������Ϣ���ݸ���������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void LoadMessageDeliver(void)
{
	if(tubeHeatFlag)										//±�ع���������ֵ������Ϣ������ֵ�ر���Ϣ
	{
		SEND_OUT_DATA1 |= BIT_HOT;
	}
	else
	{
		SEND_OUT_DATA1 &= ~BIT_HOT;
	}
	
	if(fanOpenFlag)											//�ּ�������������ֵ������Ϣ������ֵ�ر���Ϣ
	{
		SEND_OUT_DATA1 |= BIT_FAN;
	}
	else
	{
		SEND_OUT_DATA1 &= ~BIT_FAN;
	}
	
	if(sanReOpenFlag)										//��Դ��ɢ�ȷ�����������ֵ������Ϣ������ֵ�ر���Ϣ
	{
		SEND_OUT_DATA1 |= BIT_SANRE;
	}
	else
	{
		SEND_OUT_DATA1 &= ~BIT_SANRE;
	}
	
	if(steamOpenFlag)										//��¯��������ֵ��¯�ļ��ȵ�λ������ֵ�ر���Ϣ
	{
		SEND_OUT_DATA_STEAM = loadCrlData.plateHeatGear;
	}
	else
	{
		SEND_OUT_DATA_STEAM = 0;
	}

	SEND_OUT_DATA_PUMP = loadCrlData.pumpFreGear;			//��ֵˮ�õĳ�ˮƵ����Ϣ
}

/*-----------------------------------------------------------------------------
Description:		����������ؿ���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void LoadCrl(void)
{
	static unsigned char s_pumpFreGeartemp		= 0;				//ǻ���¶ȹ��߹ر�ˮ��ʱ��ˮ�õ�λ����ֵ
	static unsigned char s_maybeLackPumpTemp	= 0;				//����ȱˮʱ��ˮ�õ�λ����ֵ
	static bit			 s_maybeLackWaterFlag 	= 0;				//����ȱˮ��־

	/*��������״̬��*/
	if((g_sysType == SysModeWork) || (g_sysType == SysModeWarm))
	{
		/*�޿ɻָ��������ϸǡ���ȱˮ��ʱ������������*/
		if((g_LidFlag == LID_CLOSE) && (g_waterYieldType == NormalType))
		{
			/*���ȿ���*/
			HeatCrl();
			
			if(s_maybeLackWaterFlag)							//��֮ǰ�п���ȱˮ����
			{
				loadCrlData.pumpFreGear = s_maybeLackPumpTemp;	//��ֵԭ�ȳ�ˮ��λ

				g_quickPumpingFlag		= 0;					//����־����
			}
			s_maybeLackWaterFlag = 0;							//����ȱˮ��־����
			
			/*ˮ�õ�λ����*/
			if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))	//��������ʱ
			{
				/*С���Բ�E06������ĳ���20191027*/
				if(g_pumpCalibrationFlag == 0)					//ˮ�õ���δ�궨ǰ��ˮ�ù̶���ˮƵ��
				{
					PumpCrl();
				}
				else
				{
					if(steamOpenFlag)							//����¯Ϊ����״̬����¯�¶ȹ���ǿ��ֹͣ����Ч��
					{
						//��¯����ʱ����Ԥ�������ˮ��Ϊ�ر�״̬������رյ�رպ����»ص��������״̬������ֵˮ�õ�λ����ֵ
						if((g_steamPreHeatFlag == 1) && (loadCrlData.pumpFreGear == 0))
						{
							loadCrlData.pumpFreGear = s_pumpFreGeartemp;
						}
						
						PumpCrl();								//ˮ�ü�ˮ������
					}
					else										//��¯����������״̬���������������¶ȹ��ߣ�
					{
						if(loadCrlData.pumpFreGear != 0)		//��¯Ϊ�ر�״̬����ˮ�õ�λ��Ϊ��
						{
							if(g_adDataHSteam > STEAM_MAX_TEMP_AD)	//��¯�¶ȵ���ǿ�ƹر��¶�ʱ�����ݴ棬������
							{
								s_pumpFreGeartemp = loadCrlData.pumpFreGear;
								loadCrlData.pumpFreGear = 0;
							}
							else
							{
								//����ǰ��¯������������ˮ�õ�λ��Ϊ�㣬��¯�¶ȸ���ǿ�ص��¶ȣ����ˮֱ���¶ȵ���ǿ�ص�
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
		else													//���ֿɻָ�����ʱ���ȼ�¼ˮ�õ�ǰ��λ������
		{
			if(g_waterYieldType == MabyLackWater)				//���ǿ���ȱˮ����
			{
				if(s_maybeLackWaterFlag == 0)					//��֮ǰ�޿���ȱˮ����
				{
					s_maybeLackPumpTemp = loadCrlData.pumpFreGear;	//�ݴ浱ǰ��ˮ��λ
				}
				s_maybeLackWaterFlag = 1;						//����ȱˮ��־��һ
			
				if(g_quickPumpingFlag)
				{
					loadCrlData.pumpFreGear = QUICK_PUMPING;	//ȡ��ȱˮ�����󣬿��ٳ�ˮ
				}
				else
				{
					loadCrlData.pumpFreGear = s_maybeLackPumpTemp;	//����������ȱˮ����ԭ���ٶȳ�ˮ
				}
				
				/*����ȱˮ״̬�����ϸ����������*/
				if(g_LidFlag == LID_CLOSE)
				{
					HeatCrl();									//�������ȿ���
				}
				else
				{
					InitLoadVariable();							//��ʼ�����ص����б����������裩
				
					s_pumpFreGeartemp 		= 0;
					s_maybeLackPumpTemp 	= 0;
					s_maybeLackWaterFlag 	= 0;				//����ȱˮ��־����
				}
			}
			else
			{
				InitLoadVariable();								//��ʼ�����ص����б���
				
				s_pumpFreGeartemp 		= 0;
				s_maybeLackPumpTemp 	= 0;
				s_maybeLackWaterFlag 	= 0;					//����ȱˮ��־����
			}
		}
		
		sanReOpenFlag = OPEN_LOAD;								//������Դ��ɢ�ȷ���
	}
	else if(g_sysType == SysModeMachineCheck)					//�����Լ�״̬ʱ
	{
		switch(g_selfCheckStep)
		{
			case 1:
				tubeHeatFlag	= OPEN_LOAD;					//����ȫ���ʿ�������ˮ�ã�
				fanOpenFlag		= OPEN_LOAD;
				steamOpenFlag	= OPEN_LOAD;
				sanReOpenFlag	= OPEN_LOAD;
				loadCrlData.plateHeatGear = PLATE_GEAR_MAX;
			break;
			
			case 2:
				CloseAllOutPut();								//�ر��������
			break;
			
			case 3:
				CloseAllOutPut();								//�ر��������
			break;
			
			case 4:
				CloseAllOutPut();								//�ر��������
			break;

			case 5:
				tubeHeatFlag = OPEN_LOAD;						//��±�ع�
			break;
			
			case 6:
				tubeHeatFlag = OPEN_LOAD;						//��±�ع�
			break;

			case 7:
				CloseAllOutPut();								//�ر��������
				steamOpenFlag = OPEN_LOAD;						//����¯
				loadCrlData.plateHeatGear = PLATE_GEAR_MAX;		//ȫ���ʼ���
			break;

			case 8:
				CloseAllOutPut();								//�ر��������
				steamOpenFlag = OPEN_LOAD;						//����¯
				loadCrlData.plateHeatGear = PLATE_GEAR_MAX;		//ȫ���ʼ���
			break;
			
			case 9:
				CloseAllOutPut();								//�ر��������
				fanOpenFlag 	= OPEN_LOAD;					//���ּ��������
			break;
			
			case 10:
				CloseAllOutPut();								//�ر��������
				sanReOpenFlag = OPEN_LOAD;						//����Դ��ɢ�ȷ���
			break;
			
			case 11:
				CloseAllOutPut();								//�ر��������
				loadCrlData.pumpFreGear = PUMP_CHECK_GEAR;		//ˮ�ð���0.1Sͣ1S��Ƶ�ʳ�����ˮ
			break;
			
			case 12:
				CloseAllOutPut();								//�ر��������
			break;
			
			case 13:
				//�ر��������
			break;
		}
	}
	
	else if(g_sysType == SysModeClean)							//ˮ�����״̬ʱ
	{
		CloseAllOutPut();										//�ر��������
		
		if((g_cleanIncrustantStep == 2) && (g_cleanPumpTimeCnt == CLEAN_TIME_STEP_2 + 1))		//�ڶ�������״̬ʱ
		{
			CloseAllOutPut();									//�ر��������
		}
		else
		{
			if(g_adDataHSteam > 0x03E0)							//��¯�¶ȿ�����75��				
			{
				steamOpenFlag = OPEN_LOAD;						//����¯
				loadCrlData.plateHeatGear = PLATE_GEAR_10_20;	//�빦�ʼ���
			}
			
			if(g_cleanIncrustantStep == 1)						//����һ��
			{
				loadCrlData.pumpFreGear = 40;					//��һ������
			}

			else if(g_cleanIncrustantStep == 2)					//���ڶ���
			{
				loadCrlData.pumpFreGear = 9;					//�ڶ���������
			}
			
			else if(g_cleanIncrustantStep == 3)					//���ڶ���
			{
				loadCrlData.pumpFreGear = 9;					//������������
			}
		}
	}
	
	else
	{
		InitLoadVariable();										//��ʼ�����ص����б���

		s_pumpFreGeartemp 		= 0;
		s_maybeLackPumpTemp 	= 0;
		s_maybeLackWaterFlag 	= 0;
	}

	if(g_coolingFanDealyFlag)									//��ɢ�ȷ�����ʱ�ر���
	{
		sanReOpenFlag = OPEN_LOAD;								//������Դ��ɢ�ȷ���
	}

	if(g_sysType == SysModeWork)								//����״̬�£���LED�ƺ�����ͷɢ�ȷ���
	{
		P_CAMERA_FAN = 1;
		P_CAMERA_LED = 1;
	}
	else
	{
		P_CAMERA_FAN = 0;
		P_CAMERA_LED = 0;
	}

	/*��ֵ������صķ�����Ϣ*/
	LoadMessageDeliver();
}