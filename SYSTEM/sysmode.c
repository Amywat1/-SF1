#include "header.h"

enum _SYS_Type_     	g_sysType;              		//ϵͳ״̬
enum _ERROR_Type_		g_enContinueErr;				//�ɼ���������������
enum _WATER_YIELD_TYPE_	g_waterYieldType;				//ˮ�ó�ˮ״̬

unsigned char xdata		g_menuNumber;					//�˵����

unsigned char xdata		g_zeroType;						//����״̬���й��㣬�޹��㣩
unsigned char xdata		g_pumpType;						//ˮ��״̬�����³�ˮ�����³�ˮ��
		 bit			g_LidFlag;						//�Ƿ�ϸǱ�־

unsigned int  xdata		g_sysStandbyWaitCnt;			//�����ȴ�ʱ�����
unsigned char xdata		g_sysSelectWaitCnt;				//���ô����ȴ�ʱ�����
unsigned char xdata		g_sysOrderSetWaitCnt;			//ԤԼ���õȴ�ʱ�����
unsigned char xdata		g_sysSelfCheckWaitCnt;			//�Լ�ȴ�ʱ�����
unsigned char xdata		g_dispFlashCnt;					//��ʾ��˸����
unsigned char xdata		g_adjWaitTimeCnt;				//ʱ��/�¶ȵ��ڵȴ�ʱ�����
unsigned int  xdata		g_cleanPumpTimeCnt;				//ˮ�������򣬲���ʱ��

unsigned char xdata		g_darkScreenTime;				//�Լ��ʮ��ȫϢʱ��
unsigned char xdata		g_pdWaitTime;					//���������ϵ��ĵȴ�ʱ��

unsigned char xdata		g_nowStepworkTemp;				//��ǰ���蹤���¶�
unsigned char xdata		g_nowStepworkTime;				//��ǰ���蹤��ʱ��
unsigned char xdata		g_workTimeAll;					//������ʱ��
unsigned int  xdata		g_keepWarmTime;					//�ѱ���ʱ��
unsigned int  xdata		g_orderTime;					//ԤԼʱ�䣨��ô��ʱ�������ɣ�
unsigned int  xdata		g_orderMinTime;					//��СԤԼʱ��
unsigned char xdata		g_workOneMinCnt;				//1minʱ���ʱ
unsigned int  xdata		g_highSteamPowerCnt;			//���������¸߹������ʱ���ʱ

unsigned char xdata		g_nowStepTechnology;			//��ǰ����Ĺ���
unsigned char xdata		g_nowStepNum;					//��ǰ���ղ�����

		 bit			g_dispFlashFlag;				//��ʾ��˸��־λ
		 bit			g_selfCheckEnFlag;				//�Լ������־
		 bit			g_timeAdjEnFlag;				//ʱ��ɵ��ڱ�־
		 bit			g_tempAdjEnFlag;				//�¶ȿɵ��ڱ�־
		 bit			g_orderEnFlag;					//ԤԼ��Ч��־
		 bit			g_timeAdjStateFlag;				//ʱ�����״̬��־
		 bit			g_tempAdjStateFlag;				//�¶ȵ���״̬��־
		 bit			g_pumpCalibrationFlag;			//ˮ�õ����궨��־
		 bit			g_coolingFanDealyFlag;			//ɢ�ȷ�����ʱֹͣ��־
		 bit			g_steamLowPowerFlag;			//�����ĵ͹��ʱ�־
		 bit			g_quickPumpingFlag;				//ˮ�ÿ���־����ȱˮ������������ȡ�������������飩
		 
		 bit			g_lidSelfCheckOkFlag;			//���Լ�ϸ��־
		 bit			g_coreNtcSelfCheckOkFlag;		//ǻ��NTC��·�Լ�ϸ��־
		 bit			g_selfCheckPassFlag;			//�Լ�ϸ��־
		 bit			g_selfCheckOverFlag;			//��ʾ���Լ���ϱ�־
		 bit			g_pumpWorkFlag;					//�����Լ�ˮ�ù�����־
		 bit			g_pumpDelayJudgeFlag;			//�����Լ�ˮ����ʱ�жϱ�־

unsigned char xdata		g_selfCheckStep;				//�Լ첽��
unsigned char xdata		g_pidDispStep;					//PID����
unsigned char xdata		g_debugDispStep;				//Debug����
unsigned char xdata		g_cleanIncrustantStep;			//ˮ����ಽ��

unsigned char code MenuDefaultValue_Table[6][22]={			//6���˵���Ĭ�ϲ���
//	(		����1	)	(		����2	)	(		����3	)	(		����4	)	�����¹��գ�	 ���¶ȵ��ڷ�Χ����ʱ����ڷ�Χ��	(		�����Ƿ���Ч		)
//	����	�¶�/ʱ��	����	�¶�/ʱ��	����	�¶�/ʱ��	����	�¶�/ʱ��	����	�¶�/ʱ��		MIN	MAX		MIN	MAX			�¶�	ʱ��	ԤԼ
	ONLY_BAKE,180,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		30,	200,	1,	60,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//��������ը
	
	BAKE_STEAM,180,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		100,200,	1,	60,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//����������

	ONLY_STEAM,100,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		80,	110,	1,	60,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//����ԭζ��
																											            			
	ONLY_BAKE,180,	20,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		170,190,	15,	25,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//�˵�01
																											            			
	BAKE_STEAM,200,	5,	ONLY_BAKE,200,	11,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	NULL_STEP,0,	0,		180,200,	15,	25,		EN_ADJ,	EN_ADJ,	EN_ADJ,	//�˵�02
																											            			
	ONLY_STEAM,100,	28,	ONLY_STEAM,80,	8,	NULL_STEP,0,	0,	NULL_STEP,0,	0,	ONLY_STEAM,80,	60,		100,100,	30,	50,		NO_ADJ,	EN_ADJ,	EN_ADJ,	//�˵�03
};

/*-----------------------------------------------------------------------------
Description:		��ʼ��ϵͳ״̬����
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitSysModeVariable(void)
{
	g_sysType				= SysModePowerUp;
	g_enContinueErr			= NoError;
	g_waterYieldType		= NormalType;

	g_menuNumber			= DEFAULT_MENU;
	
	g_zeroType				= STATE_YES;
	g_pumpType				= STATE_NO;								//Ĭ��ˮ���޳�ˮ����
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
	
	g_nowStepTechnology		= NULL_STEP;							//Ĭ�ϵ�ǰ�޹��ղ���
	g_nowStepNum			= 1;									//Ĭ�ϵ�һ������

	g_dispFlashFlag			= 0;
	g_selfCheckEnFlag		= 1;									//Ĭ����������Լ�
	g_timeAdjEnFlag 		= 0;
	g_tempAdjEnFlag 		= 0;
	g_orderEnFlag 			= 0;
	g_timeAdjStateFlag 		= 0;
	g_tempAdjStateFlag 		= 1;									//Ĭ���¶ȵ���״̬
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
Description:		ϵͳ״̬����
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void SysModelCrl(void)
{
	unsigned char i = 0;
	
	static unsigned int  xdata	s_pdAdValue					= 0;
	static unsigned int  xdata	s_coolingFanDealyCnt		= 0;
	static unsigned int  xdata  s_lidOpenWorkCnt			= 0;		//�����ͱ���״̬�¿���ʱ���ʱ
	static unsigned int  xdata  s_sysEndWaitCnt				= 0;		//���������ȴ�ʱ��
	
	static 			bit 		s_selfCheckLidFlag			= 0;
	
	static			bit			s_pumpNoWorkWoringFlag		= 0;		//������ˮ���Լ챨���ѷ�����־
	static unsigned char xdata  s_pumpTestWaitCnt			= 0;		//������ˮ���Լ�ȴ�ʱ��

	static unsigned char xdata  s_powerUpCnt				= 0;		//�ϵ�ʱ���ʱ
	static unsigned char xdata  s_powerUpFifteenSecondsCnt	= 0;		//�ϵ�15S��ʱ

	static unsigned char xdata  s_keepWarmMaxTime			= 0;

	if(g_sysType == SysModePCBCheck)									//PCB�Լ�״̬ʱ
	{
		if(g_selfCheckStep == 1)										//�Լ��һ����¼��״̬
		{
			s_selfCheckLidFlag		= g_LidFlag;
			s_pdAdValue				= 0;
			
			g_lidSelfCheckOkFlag	= 0;								//��һ����ʼĬ���Լ첻�ϸ�
			g_coreNtcSelfCheckOkFlag= 0;
			g_selfCheckPassFlag		= 0;
			g_selfCheckOverFlag		= 0;								//���Ϊ�Լ�δ���
		}
		else if(g_selfCheckStep == 8)
		{
			if(g_lidSelfCheckOkFlag)									//���Ǽ��ϸ񣬼��ǻ��NTCֵ
			{
				if((measureData.coreAdHValue > SELF_CHECK_AD_HMAX) || (measureData.coreAdHValue < SELF_CHECK_AD_HMIN) || (measureData.coreAdLValue > SELF_CHECK_AD_LMAX) || (measureData.coreAdLValue < SELF_CHECK_AD_LMIN))
				{
					g_coreNtcSelfCheckOkFlag = 0;						//���ǻ��NTC��·�Լ첻�ϸ�
					g_selfCheckPassFlag = 0;							//����Լ첻�ϸ�
					g_selfCheckOverFlag		= 1;						//���Ϊ�Լ����
					
					gU8_buzzerType = BUZ_JK_ALARM;						//������������
						
					g_sysType = SysModeError;
					measureData.errCode = 0x58;
				}
				else
				{
					g_coreNtcSelfCheckOkFlag = 1;						//���ǻ��NTC��·�Լ�ϸ�

//					s_pdAdValue = AdScanValue();						//�������˿ڵ�ѹ

					g_selfCheckPassFlag = 1;							//����Լ�ϸ�
					
					g_selfCheckOverFlag	= 1;							//���Ϊ�Լ����
				}
			}
			else
			{
				g_selfCheckPassFlag	= 0;								//����Լ첻�ϸ�
				g_selfCheckOverFlag		= 1;							//���Ϊ�Լ����
				
				gU8_buzzerType = BUZ_JK_ALARM;							//������������
						
				g_sysType = SysModeError;
				measureData.errCode = 0x57;
			}
		}
		else															//�ڶ������Ժ�
		{
			if(g_lidSelfCheckOkFlag == 0)								//���Ǽ��δ�ϸ���һֱ�������ޱ仯
			{
				if(g_LidFlag != s_selfCheckLidFlag)						//�����б仯�����ǸǼ��ϸ�
				{
					g_lidSelfCheckOkFlag = 1;
				}
			}
		}
	}
	
	/*����������ʱ��ʱ*/
	if(g_pdWaitTime)													//���е����ӳ�ʱ��������ʱ����ʱ
	{
		g_pdWaitTime--;
		
		if(g_pdWaitTime == 0)											//�����������ɨ��
		{
			TKCON1 |= BIT5;    											//��������ɨ��
		}
	}
	
	/*��ʾ��˸����*/
	g_dispFlashCnt++;
	if(g_dispFlashCnt >= DISP_FLASH_FRE)
	{
		g_dispFlashCnt  = 0;
		g_dispFlashFlag = ~g_dispFlashFlag;								//һ��Ƶ����˸
	}
	
//	ErrorCheck();														//�������
	
	if(g_flg500ms)														//500msʱ���һ��
	{
		g_flg500ms = 0;

		/*�ϵ�15S��ʱ*/
		if(g_selfCheckEnFlag == 1)										//���Լ�����
		{
			s_powerUpFifteenSecondsCnt++;
			if(s_powerUpFifteenSecondsCnt >= POWER_UP_FIFTEEN_SECONDS)
			{
				g_selfCheckEnFlag = 0;									//��ֹ�����Լ�
				s_powerUpFifteenSecondsCnt = POWER_UP_FIFTEEN_SECONDS;
			}
		}
		
		/*�¶�/ʱ��ȴ�ʱ���ʱ*/
		if(g_sysType == SysModeSelect)									//���ô���״̬�³�̬��ʾ�¶�
		{
			if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
			{
				if(g_tempAdjStateFlag == EN_ADJ)						//һ��ͨ�˵�Ĭ����ʾʱ��
				{
					g_adjWaitTimeCnt++;
					if(g_adjWaitTimeCnt >= ADJ_WAIT_TIME)				//���ڵȴ�ʱ�䵽���л��س�̬��ʾ
					{
						g_adjWaitTimeCnt = 0;
						
						g_timeAdjStateFlag = EN_ADJ;
						g_tempAdjStateFlag = NO_ADJ;
					}
				}
			}
			else														//�����˵�Ĭ����ʾ�¶�
			{
				if(g_timeAdjStateFlag == EN_ADJ)
				{
					g_adjWaitTimeCnt++;
					if(g_adjWaitTimeCnt >= ADJ_WAIT_TIME)				//���ڵȴ�ʱ�䵽���л��س�̬��ʾ
					{
						g_adjWaitTimeCnt = 0;
						
						g_timeAdjStateFlag = NO_ADJ;
						g_tempAdjStateFlag = EN_ADJ;
					}
				}
			}
		}
		else if(g_sysType == SysModeWork)								//����״̬�³�̬��ʾʱ��
		{
			if(g_tempAdjStateFlag == EN_ADJ)
			{
				g_adjWaitTimeCnt++;
				if(g_adjWaitTimeCnt >= ADJ_WAIT_TIME)					//���ڵȴ�ʱ�䵽���л��س�̬��ʾ
				{
					g_adjWaitTimeCnt = 0;
					
					g_timeAdjStateFlag = EN_ADJ;
					g_tempAdjStateFlag = NO_ADJ;
				}
			}
		}
		else
		{
			//����״̬�����任
		}
		
		
		/*����������ɢ�ȷ�����ʱ�رռ���*/
		if(g_coolingFanDealyFlag)										//��ɢ����ʱ�ر���
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
		
		/*����ģʽ�³�ʼ�׶θ߹��������ʱ*/
		if((g_sysType == SysModeWork) && (g_nowStepTechnology == ONLY_STEAM) && (g_nowStepworkTemp >= 100))	//����ǰ����״̬Ϊ100�漰���ϴ���
		{
			/*�޿ɻָ��������ϸǡ�������ȱˮ��ʱ��ʱ*/
			if((g_LidFlag == LID_CLOSE) && (g_waterYieldType == NormalType))
			{
				if(g_steamLowPowerFlag == 0)							//���͹��������־λΪ0
				{
					g_highSteamPowerCnt++;
					if(g_highSteamPowerCnt >= HIGH_POWER_MAX_TIME)		//�����߹������ʱ�䳬���������ʱ��
					{
						g_steamLowPowerFlag = 1;
						g_highSteamPowerCnt = HIGH_POWER_MAX_TIME;
					}
				}
			}
		}
		
		/*��������״̬�������ǳ���5min���˻ص�����״̬*/
		if((g_sysType == SysModeWork) || (g_sysType == SysModeWarm))
		{
			if(g_LidFlag == LID_OPEN)
			{
				s_lidOpenWorkCnt++;
				if(s_lidOpenWorkCnt >= LID_OPEN_WORK_TIME)
				{
					g_sysType  = SysModeStandby;						//�ص�����״̬
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;					//���Ϊ������ˮ
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������

					gU8_buzzerType = BUZ_JK_FINISH;						//����������������
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
		
		/*��ͬϵͳ״̬�¿���*/
		switch(g_sysType)
		{
			case SysModePowerUp:										//�ϵ�״̬
				if(s_powerUpCnt < POWER_UP_TIME)
				{
					s_powerUpCnt++;
				}
				else
				{
					g_sysType	= SysModeStandby;						//�ϵ����ת������ת̬
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
					
					TKCON1 |= BIT5;    									//��������ɨ��
				}
				break;
				
			case SysModeStandby:										//����״̬
				/* g_sysStandbyWaitCnt++;
				if(g_sysStandbyWaitCnt >= SYS_STANDBY_WAIT_TIME)		//�����ȴ�����5���ӣ�ת������״̬
				{
					g_sysType = SysModeDarkScreen;
				} */
				break;
				
			case SysModeSelect:											//���ô���״̬
				g_sysSelectWaitCnt++;
				if(g_sysSelectWaitCnt >= SYS_SELECT_WAIT_TIME)			//���ô����ȴ�����2���ӣ�ת������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
				}
				break;
			
			case SysModeOrderSet:										//ԤԼ����״̬
				g_sysOrderSetWaitCnt++;
				if(g_sysOrderSetWaitCnt >= SYS_ORDERSET_WAIT_TIME)		//ԤԼ�ȴ�ʱ�䳬��2min���������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
				}
				break;

			case SysModeOrderWork:										//ԤԼ����״̬
				if(g_LidFlag == LID_OPEN)
				{
					if(g_enContinueErr == NoError)
					{
						g_enContinueErr = LidOpenErr;					//��ֵΪ���Ǳ���
				
						gU8_buzzerType = BUZ_JK_POINT;					//�����ɻָ���������
					}
				}
			
				if(g_orderTime > g_workTimeAll)							//ԤԼʱ����ڹ���ʱ��
				{
					g_workOneMinCnt++;
					if(g_workOneMinCnt >= ONE_MINUTE_CNT)				//1min��ʱ
					{
						g_workOneMinCnt = 0;
						
						g_orderTime--;
						
						/*�˴�ֱ��ת״̬����ֹ500ms���ת*/
						if(g_orderTime <= g_workTimeAll)				//���￪ʼ����ʱ��
						{
							g_sysType 			= SysModeWork;			//ת������״̬
							
							g_enContinueErr		= NoError;				//��ɻָ�����
							g_waterYieldType 	= NormalType;			//Ĭ��������ˮ

							g_nowStepNum		= 1;					//��ʼ��һ������

							g_timeAdjStateFlag	= 1;					//����Ĭ����ʾʱ��
							g_tempAdjStateFlag	= 0;

							g_highSteamPowerCnt	= 0;					//���������¸߹������ʱ���ʱ����
							g_steamLowPowerFlag	= 0;					//�����͹��������־����
							
							g_coolingFanDealyFlag = 0;					//ɢ�ȷ���Ĭ�ϲ���ʱֹͣ
							
							g_pumpDelayJudgeCnt	= 0;
							
							if(g_pumpCalibrationFlag == 0)				//��δ�궨�ɹ�
							{
								InitPumpCalibrationData();				//��ʼ������ˮ�ñ궨����
							}
						}
					}
				}
				else													//ԤԼʱ����ڿ�ʼ����ʱ��ʱ
				{
					g_sysType 			= SysModeWork;					//ת������״̬

					g_enContinueErr		= NoError;						//��ɻָ�����
					g_waterYieldType 	= NormalType;					//Ĭ��������ˮ
					
					g_nowStepNum		= 1;							//��ʼ��һ������

					g_timeAdjStateFlag	= 1;							//����Ĭ����ʾʱ��
					g_tempAdjStateFlag	= 0;

					g_highSteamPowerCnt	= 0;							//���������¸߹������ʱ���ʱ����
					g_steamLowPowerFlag	= 0;							//�����͹��������־����
					
					g_coolingFanDealyFlag = 0;							//ɢ�ȷ���Ĭ�ϲ���ʱֹͣ

					g_pumpDelayJudgeCnt	= 0;
					
					if(g_pumpCalibrationFlag == 0)						//��δ�궨�ɹ�
					{
						InitPumpCalibrationData();						//��ʼ������ˮ�ñ궨����
					}
				}
				break;
				
			case SysModeWork:											//����״̬
				if(g_LidFlag == LID_CLOSE)								//�ϸ�״̬�£����빤������ʱ
				{
					if(g_waterYieldType == LackWater)					//ȱˮʱ�������е���ʱ
					{
						if(g_enContinueErr == NoError)					//��֮ǰ�ޱ���
						{
							g_enContinueErr = LowWaterErr;				//��ֵΪȱˮ����
						
							gU8_buzzerType = BUZ_JK_POINT;				//�����ɻָ���������
						}
					}
					else
					{
						if(g_workTimeAll > 0)							//ʣ�๤��ʱ�������
						{
							g_workOneMinCnt++;
							if(g_workOneMinCnt >= ONE_MINUTE_CNT)		//1min��ʱ
							{
								g_workOneMinCnt = 0;
								
								g_workTimeAll--;						//��ʱ���1
										
								/*�˴�ֱ��ת״̬����ֹ500ms���ת*/
								if(g_workTimeAll == 0)					//����ʱ�����
								{
									if(g_menuNumber == MENU_6_NUM)
									{
										g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][KEEP_WARM_TEMP_NUM];	//���ر����¶�
										g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_WARM_NUM];//���ر��¹���
										
										g_sysType 		= SysModeWarm;	//ת������״̬
										g_keepWarmTime	= 0;			//����ʱ������ʱ��ʼ
										g_workOneMinCnt = 0;

										//�л�����ʱ�����л����������գ����¸���ˮ��λ��ʼֵ
										if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))
										{
											g_steamPreHeatFlag 	= 0;	//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
										}
									}
									else
									{
										g_sysType = SysModeEnd;			//ת����������״̬
										s_sysEndWaitCnt = 0;			//���������ȴ�ʱ������
									}

									g_coolingFanDealyFlag = 1;				//ɢ�ȷ�����ʱ�رձ�־��һ
									s_coolingFanDealyCnt  = 0;				//ɢ�ȷ�����ʱ�رռ�������
									
									gU8_buzzerType = BUZ_JK_FINISH;			//������������
								}
								else
								{
									if(g_nowStepworkTime)					//����ǰ���ղ���ʣ��ʱ�������
									{
										g_nowStepworkTime--;
										if(g_nowStepworkTime == 0)			//����ʱ���������ת����һ������
										{
											if(g_nowStepNum == 1)			//���ǵ�һ�����ս���
											{
												g_nowStepNum = 2;			//ת��ڶ�������

												g_nowStepworkTime		= MenuDefaultValue_Table[g_menuNumber][STEP_2_TIME_NUM];//���صڶ������յ�Ĭ��ʱ��

												if(g_nowStepworkTime)		//������ʱ�䲻Ϊ�㣬�������պ͹����¶�
												{
													if(g_menuNumber == MENU_5_NUM)		//�����ĵڶ��������¶�ͬ��һ�������¶�
													{
														//�����¶Ȳ����
													}
													else
													{
														g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][STEP_2_TEMP_NUM];
													}

													g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_STEP_2_NUM];	//���صڶ�������
												}
											}
											else if(g_nowStepNum == 2)		//���ǵڶ������ս���
											{
												g_nowStepNum = 3;			//ת�����������
												
												g_nowStepworkTime		= MenuDefaultValue_Table[g_menuNumber][STEP_3_TIME_NUM];//���ص��������յ�Ĭ��ʱ��

												if(g_nowStepworkTime)		//������ʱ�䲻Ϊ�㣬�������պ͹����¶�
												{
													g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][STEP_3_TEMP_NUM];
													g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_STEP_3_NUM];	//���ص���������
												}
											}
											else if(g_nowStepNum == 3)			//���ǵ��������ս���
											{
												g_nowStepNum = 4;				//ת����Ĳ�����

												g_nowStepworkTime		= MenuDefaultValue_Table[g_menuNumber][STEP_4_TIME_NUM];//���ص��Ĳ����յ�Ĭ��ʱ��

												if(g_nowStepworkTime)		//������ʱ�䲻Ϊ�㣬�������պ͹����¶�
												{
													g_nowStepworkTemp	= MenuDefaultValue_Table[g_menuNumber][STEP_4_TEMP_NUM];
													g_nowStepTechnology	= MenuDefaultValue_Table[g_menuNumber][TECHNOLOGY_STEP_4_NUM];	//���ص��Ĳ�����
												}
											}
											
											g_corePreHeatFlag		= 0;		//�л����պ������ж�Ԥ��
											g_corePreHeatDelayFlag	= 0;
											g_corePreHeatFirstFlag	= 1;

											//�л�����ʱ�����л����������գ����¸���ˮ��λ��ʼֵ
											if((g_nowStepTechnology == ONLY_STEAM) || (g_nowStepTechnology == BAKE_STEAM))
											{
												g_steamPreHeatFlag 	= 0;		//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
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
						g_enContinueErr = LidOpenErr;					//��ֵΪ���Ǳ���
				
						gU8_buzzerType = BUZ_JK_POINT;					//�����ɻָ���������
					}
				}
				break;
			
			case SysModeWarm:											//����״̬
				if(g_LidFlag == LID_CLOSE)								//�ϸ�״̬�£����뱣������ʱ
				{
					if(g_waterYieldType == LackWater)					//ȱˮʱ�������е���ʱ
					{
						if(g_enContinueErr == NoError)					//��֮ǰ�ޱ���
						{
							g_enContinueErr = LowWaterErr;				//��ֵΪȱˮ����
						
							gU8_buzzerType = BUZ_JK_POINT;				//�����ɻָ���������
						}
					}
					else
					{
						g_workOneMinCnt++;
						if(g_workOneMinCnt >= ONE_MINUTE_CNT)			//1min��ʱ
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
							if(g_keepWarmTime >= s_keepWarmMaxTime)		//����ʱ�䵽
							{
								g_sysType = SysModeEnd;					//ת����������״̬
								s_sysEndWaitCnt = 0;					//���������ȴ�ʱ������
								
								gU8_buzzerType = BUZ_JK_FINISH;			//������������
							}
						}
					}
				}
				else
				{
					if(g_enContinueErr == NoError)
					{
						g_enContinueErr = LidOpenErr;					//��ֵΪ���Ǳ���
				
						gU8_buzzerType = BUZ_JK_POINT;					//�����ɻָ���������
					}
				}
				break;
			
			case SysModeEnd:
				s_sysEndWaitCnt++;
				if(s_sysEndWaitCnt >= SYS_END_WAIT_TIME)				//���������ȴ�����5���ӣ�ת������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
				}
				break;
				
			case SysModeDarkScreen:
				//���������
				break;
				
			case SysModeError:
				//���������
				break;

			case SysModePCBCheck:										//PCB�Լ�״̬
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt >= SYS_SELFCHECK_WAIT_TIME)	//�Լ�ȴ�����15S��ת������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
					
					g_selfCheckStep	= 0;
					
					/*�������AD����GPIO����*/
					ADCH1 &= ~BIT7;
					
					/*������AD�ڲ������������*/
					P3    |= BIT7;										//���������ĵ��ݳ��
					P3CR  |= BIT7;
					P3PCR &= ~BIT7;
					
					SEQCHX = 0x02;										//ѡ��ͨ��2
				}
				break;
				
			case SysModeMachineCheck:									//�����Լ�״̬
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt >= SYS_SELFCHECK_WAIT_TIME)	//�Լ�ȴ�����15S��ת������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�

					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
					
					g_selfCheckStep	= 0;
				}
				
				if(g_selfCheckStep == 11)
				{
					s_pumpTestWaitCnt++;								//�����ˮ���쳣������ʱ1S��������������ʹ�ø���ʱ��־
					if(s_pumpTestWaitCnt >= 2)
					{
						g_pumpDelayJudgeFlag = 1;
						
						s_pumpTestWaitCnt = 2;							//��ֹ���
					}
					else
					{
						g_pumpDelayJudgeFlag = 0;
					}
				
					if((g_pumpCurrentAd < 0x3F0) && (g_pumpCurrentAd > 0x100))		//��ˮ�õ���ADֵС��0x3F0������0x100����Ϊˮ�ù�����ˮ�ù�����־λ��1
					{
						g_pumpWorkFlag = 1;

						s_pumpNoWorkWoringFlag = 0;
					}
					else
					{
						g_pumpWorkFlag = 0;
						
						if(g_pumpDelayJudgeFlag)						//����ʱʱ���ѵ�
						{
							if(s_pumpNoWorkWoringFlag == 0)				//��δ�������򷢳�����
							{
								gU8_buzzerType = BUZ_JK_POINT;			//�����ɻָ���������
								
								s_pumpNoWorkWoringFlag = 1;				//���Ϊ�ѷ���
							}
						}
					}
				}
				else if(g_selfCheckStep == 13)
				{
					if(g_darkScreenTime)								//�Լ��10����������ʱ
					{
						g_darkScreenTime--;
						if(g_darkScreenTime == 0)
						{
							g_selfCheckStep = 1;						//���ص�һ��
							
							gU8_buzzerType = BUZ_JK_PREHEATING;			//������������
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
				if(g_sysSelfCheckWaitCnt >= SYS_SELFCHECK_WAIT_TIME)	//�Լ�ȴ�����15S��ת������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
					
					g_debugDispStep = 0;
				}
				break;
			
			case SysModePID:
				g_sysSelfCheckWaitCnt++;
				if(g_sysSelfCheckWaitCnt <= PID_STEP1_TIME)
				{
					g_pidDispStep = 1;									//2S����ʾ��һ��
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP2_TIME)
				{
					g_pidDispStep = 2;									//4S����ʾ�ڶ���
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP3_TIME)
				{
					g_pidDispStep = 3;									//6S����ʾ�ڶ���
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP4_TIME)
				{
					g_pidDispStep = 4;									//8S����ʾ�ڶ���
				}
				else if(g_sysSelfCheckWaitCnt <= PID_STEP5_TIME)
				{
					g_pidDispStep = 5;									//10S����ʾ�ڶ���
				}
				else
				{
					g_pidDispStep = 6;									//12S����ʾ������
				}

				if(g_sysSelfCheckWaitCnt >= SYS_PID_WAIT_TIME)			//PID�ȴ�����12S��ת������״̬
				{
					g_sysType	= SysModeStandby;
					g_menuNumber		= DEFAULT_MENU;					//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;							//�����ȴ�ʱ������
					
					g_pidDispStep = 0;
				}
				break;
			
			case SysModeClean:											//ˮ��������
				if(g_cleanIncrustantStep == 1)							//����һ��
				{
					g_cleanPumpTimeCnt++;
					if(g_cleanPumpTimeCnt >= CLEAN_TIME_STEP_1)			//����һʱ�䵽
					{
						g_cleanIncrustantStep	= 2;					//���벽���
						
						g_cleanPumpTimeCnt		= 0;					//��ʱ����
					}
				}
				else if(g_cleanIncrustantStep == 2)						//���ڶ���
				{
					g_cleanPumpTimeCnt++;
					if(g_cleanPumpTimeCnt >= CLEAN_TIME_STEP_2)			//�����ʱ�䵽
					{
						if(g_cleanPumpTimeCnt == CLEAN_TIME_STEP_2)
						{
							gU8_buzzerType = BUZ_JK_FINISH;				//������������
						}
						
						g_cleanPumpTimeCnt	= CLEAN_TIME_STEP_2 + 1;	//ѭ���ȴ���������
					}
				}
				
				else
				{
					g_cleanPumpTimeCnt++;
					if(g_cleanPumpTimeCnt >= CLEAN_TIME_STEP_3)			//������ʱ�䵽
					{
						g_sysType = SysModeEnd;							//ת����������״̬
						s_sysEndWaitCnt = 0;							//���������ȴ�ʱ������
						
						gU8_buzzerType = BUZ_JK_FINISH;					//������������
					}
				}
				break;
		}
	}
}