#include "header.h" 

unsigned int  				g_scanDataTk[VALID_TOUCHKEY_NUMBER];				//����������ɨ��ֵ
unsigned long xdata			adSumTk[VALID_TOUCHKEY_NUMBER];						//����AD��ֵ�ۼӺ�
unsigned int  xdata			adAvgTk[VALID_TOUCHKEY_NUMBER];						//����AD��ֵ
unsigned int  xdata			adRefTk[VALID_TOUCHKEY_NUMBER];						//����������׼ֵ
unsigned char xdata			refCntKey[VALID_TOUCHKEY_NUMBER];					//����������׼����ˢ��ʱ�����
		 
unsigned char xdata			tkAddCnt;						//����AD��ֵ�ۼӺʹ���

unsigned int				keyBuf;							//����ֵ����
unsigned int				keyValue;						//����ֵ
unsigned int				keyCnt;							//������ʱ

		 bit				g_keyScanEndFlag;				//����ɨ����ɱ�־
		 bit				keyReleaseFlag;					//����̧���־λ
		 bit				g_adjPressFlag;					//�Ӽ������±�־
		 bit				keyOnOffLongPressFlag;			//����ȡ����������־
		 bit				keyOrderLongPressFlag;			//ԤԼ��������־

unsigned int code			TouchKey_Valid_Threshould_Table[VALID_TOUCHKEY_NUMBER] = {
//	�¶�-		����/ȡ��	ʱ��+		ԤԼ	ʱ��-	����	�¶�+
	750,		1100,		900,		750,	750,	750,	950,
}; 

/*-----------------------------------------------------------------------------
Description:		��ʼ�������� IO
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitTouchKeyIo(void)
{
	BANK0_SET;

	P0CR &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6));			//����
	P0PCR &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6));			//������

	P1CR &= (~BIT7);			//����
	P1PCR &= (~BIT7);			//������
}

/*-----------------------------------------------------------------------------
Description:		��ʼ��������������
Input:				void
Return:				void
History:			��
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
Description:		��ʼ��������������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitTouchKey(void)
{
	BANK0_SET;
	
	TKCON1 = 0x81;							//����������������ѡ�����������Ϊ���ݲ�������������3���������
	TKST = 0x0C;							//��������Ƶ��=Fosc/0X06=24/12=2M
	TKRANDOM |= BIT5;        			 	//�Ƚ����ͻ�ʹ�ܣ�������޵�ѹ�����������⣩
	TKF0 = 0;								//�����жϱ�־λ����
	TKDIV01	= 0;                    	 	//���÷Ŵ�ϵ��
	TKDIV02	= 0;
	TKDIV03	= 0xAA;
	TKDIV04	= 0x00;
	
	P0SS |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);			//P0.1~P0.6��Ϊ������
	P1SS |= BIT7;							//P1.7��Ϊ������
	
	TKVREF = 0xA7;                     	 	//Vref=1.0v   ȥ��ʱ��32Tsys  OP�����ѹ3.0V �ŵ�ʱ����ʱ640Tsy
	TKU1 = 0x80;							//����ɨ��˳�����
	TKU2 = 0x7E;
	
	ETK = 1;								//�����������ж�
}

/*-----------------------------------------------------------------------------
Description:		��������ADֵɨ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void TouchKeyAdScan(void)
{
	unsigned long s_tkAdBuf;							//����AD��ֵ�ۼӺͻ���
	unsigned char i = 0;
	
	if(g_keyScanEndFlag == 1)
	{
		for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
		{
			s_tkAdBuf = (unsigned long)g_scanDataTk[i];
			adSumTk[i] += s_tkAdBuf;
		}
		
	   	tkAddCnt++;
		if(tkAddCnt >= 4)								//����ֵ��ƽ��
		{
		   tkAddCnt = 0;
		   
			for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
			{
				adAvgTk[i] = adSumTk[i] >> 2;
				adSumTk[i] = 0;
			}
		}
		
		if(adRefTk[0] == 0)								//��ʼ������׼ֵΪ0����һ��ɨ�����л�׼ֵ��ֵ
		{
			if(adAvgTk[0] != 0)
			{
				for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
				{
					adRefTk[i] = adAvgTk[i];
				}
			}
		}
		g_keyScanEndFlag = 0;							//����ɨ�账����ɣ����¿�ʼɨ��
		
		TKF0 = 0;										//�����жϼĴ�������
		TKCON1 |= BIT5;    								//��������ɨ��
	}
}

/*-----------------------------------------------------------------------------
Description:		����ɨ�裬ͬʱˢ�°�����׼
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void TouchKeyScan(void)
{
	unsigned char i = 0;

	if(adRefTk[0] != 0)								//�״����ݻ�δ���ֲ�ɨ�谴��	
	{
		keyBuf  = 0; 								//��ֵ����Ϊ0���ް���

		for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
		{
			if(adAvgTk[i] >= adRefTk[i])
			{
				if((adAvgTk[i] - adRefTk[i]) >= TouchKey_Valid_Threshould_Table[i])	
				{
					if((adAvgTk[i] - adRefTk[i]) < VALID_THRESHOLD_MAX)		//�޷��˲�����ֵС�����������Ч��
					{
						if(i == 0)					//TK01		�¶�-
						{
							keyBuf |= KEY_ON_OFF;
						}

						if(i == 1)					//TK02		����ȡ��
						{
							keyBuf |= KEY_TEMP_SUB;
						}

						if(i == 2)					//TK03		ʱ��+
						{
							keyBuf |= KEY_TIME_ADD;
						}

						if(i == 3)					//TK04		ԤԼ
						{
							keyBuf |= KEY_ORDER;
						}

						if(i == 4)					//TK05		ʱ��-
						{
							keyBuf |= KEY_TIME_SUB;
						}

						if(i == 5)					//TK11		����
						{
							keyBuf |= KEY_CHANGE_FUN;
						}

						if(i == 6)					//TK12		�������
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
						adRefTk[i]++;				//ˢ�»�׼��ÿ��10msˢ��1������
						refCntKey[i] = 0;
					}
				}
			}
			else
			{
				adRefTk[i] = adRefTk[i] - 2;		//ˢ�»�׼��ÿ��2msˢ��2������
				refCntKey[i] = 0;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		ʶ�𰴼�ֵ
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void TouchKeyDeal(void)
{
	keyCnt++;
	
	if(keyCnt >= ELIM_TREMBLING_CNT)					//��������ʱ��
	{
		if(keyReleaseFlag == 0)							//��֮ǰ����Ϊ�ͷ�״̬
		{
			/*�ӡ�����*/
			if((keyValue == KEY_TEMP_SUB) || (keyValue == KEY_TEMP_ADD) || (keyValue == KEY_TIME_SUB) || (keyValue == KEY_TIME_ADD))
			{
				g_adjPressFlag = 1;						//ʱ��Ӽ������£������������ڲ���˸��
			
				if((keyCnt == ELIM_TREMBLING_CNT) || (keyCnt >= 500))		//�̰�������������Ч���򳤰�1S����
				{
//					keyReleaseFlag = 1;					//��������״̬
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}
			
			/*ԤԼ�������л���*/
			else if((keyValue == KEY_CHANGE_FUN))
			{
				if(keyCnt <= (ELIM_TREMBLING_CNT + 4))	//��������ʱ����
				{
					keyReleaseFlag = 1;					//��������״̬
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}
			
			/*�Լ졢PID�����ϵ�15S��*/
			else if(((keyValue == KEY_PCB_CHECK) || (keyValue == KEY_MACHINE_CHECK) || (keyValue == KEY_DISP_PID)) && 
			(g_selfCheckEnFlag == 1))
			{
				if(keyCnt >= 750)						//����1.5S
				{
					keyReleaseFlag = 1;					//��������״̬
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}
			
			/*����ȡ����*/
			else if(keyValue == KEY_ON_OFF)
			{
				if(keyCnt >= 750)						//����1.5S
				{
					keyOnOffLongPressFlag = 1;			//����ȡ��������
					keyReleaseFlag = 1;					//��������״̬
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}

			/*ԤԼ��*/
			else if(keyValue == KEY_ORDER)
			{
				if(keyCnt >= 1000)						//����2S
				{
					keyOrderLongPressFlag = 1;			//ԤԼ������
					keyReleaseFlag = 1;					//��������״̬
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}
			
			/*���ˮ����*/
			else if(keyValue == KEY_CLEAN_INCRUSTANT)
			{
				if(keyCnt >= 1000)						//����2S
				{
					keyReleaseFlag = 1;					//��������״̬
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}
			
			else										//��������
			{
				keyBuf		= 65530;					//��ֵΪ��Ч����
				keyValue	= 65530;
				keyCnt		= 0;
				
				keyReleaseFlag = 1;						//��������״̬
				g_adjPressFlag = 0;						//�Ӽ���δ���£������������ڲ���˸��
			}
		}
		
		else											//�������£����Ӽ�����������󣬸�ֵΪ��Ч��ֵ
		{
			keyBuf		= 65530;						//��ֵΪ��Ч����
			keyValue	= 65530;
			keyCnt		= 0;
			
			keyReleaseFlag = 1;							//��������״̬
			g_adjPressFlag = 0;							//�Ӽ���δ���£������������ڲ���˸��
		}
	}
}

/*-----------------------------------------------------------------------------
Description:		�����ж�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void TouchKeyJudge(void)
{
	static unsigned int xdata s_fakePressCnt = 0;		//�ٰ�����������ʱ���⵽�а�������Ϊ�Ƿ���Ϊ���£�

	unsigned char i = 0;

	if(keyBuf != KEY_NULL)								//�а�������
	{
		s_fakePressCnt++;
		if(s_fakePressCnt >= 30000)						//2msʱ����������1min����⵽�а�����������ˢ�»�׼ֵ
		{
			for(i=0;i<VALID_TOUCHKEY_NUMBER;i++)
			{
				adRefTk[i] = 0;
			}
		}
		else
		{
			if(keyValue == KEY_NULL) 					//������ֵδ��ֵ�����и�ֵ
			{
				keyValue = keyBuf;
				keyCnt = 0;
			}
			else										//������ֵ�Ѿ���ֵ		
			{
				if(keyValue == keyBuf)
				{
					TouchKeyDeal();						//��ֵ��������
				}
				else 									//��ֵ�뻺�治һ�£����¸���ֵ
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
			if(keyCnt >= ELIM_TREMBLING_CNT)			//��������ʱ��
			{
				if(keyValue == KEY_ON_OFF)
				{
					keyOnOffLongPressFlag = 0;			//����ȡ�����̰�������̧����Ч
					TouchKeyDealSubroutine();			//���������ӳ���
				}
				else if(keyValue == KEY_ORDER)
				{
					keyOrderLongPressFlag = 0;			//ԤԼ�����̰�������̧����Ч
					TouchKeyDealSubroutine();			//���������ӳ���
				}
			}
		}
		
		keyOnOffLongPressFlag = 0;						//����ȡ�����̰�
		keyOrderLongPressFlag = 0;						//ԤԼ�����̰�
	
		keyBuf = KEY_NULL;
		keyValue = KEY_NULL;
		keyCnt = 0;
		
		keyReleaseFlag = 0;								//�����ͷ�״̬
		g_adjPressFlag = 0;								//�Ӽ���δ���£������������ڲ���˸��
		
		s_fakePressCnt = 0;								//�ٰ�������ֵ����
	}
}

/*-----------------------------------------------------------------------------
Description:		���ز˵���Ĭ��ֵ
Input:				menuNum
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void LoadDefaultValue(unsigned char menuNum)
{
	g_nowStepTechnology	= MenuDefaultValue_Table[menuNum][TECHNOLOGY_STEP_1_NUM];
	g_nowStepworkTemp	= MenuDefaultValue_Table[menuNum][STEP_1_TEMP_NUM];
	g_nowStepworkTime	= MenuDefaultValue_Table[menuNum][STEP_1_TIME_NUM];
	g_workTimeAll		= MenuDefaultValue_Table[menuNum][STEP_1_TIME_NUM] + MenuDefaultValue_Table[menuNum][STEP_2_TIME_NUM] +
						  MenuDefaultValue_Table[menuNum][STEP_3_TIME_NUM] + MenuDefaultValue_Table[menuNum][STEP_4_TIME_NUM];//���㹤����ʱ��
	
	g_tempAdjEnFlag 	= MenuDefaultValue_Table[menuNum][TEMP_ADJ_EN_NUM];	//�����¶ȿɵ���־
	g_timeAdjEnFlag 	= MenuDefaultValue_Table[menuNum][TIME_ADJ_EN_NUM];	//����ʱ��ɵ���־
	g_orderEnFlag		= MenuDefaultValue_Table[menuNum][ORDER_EN_NUM];	//����ԤԼ��Ч��־
}
/*-----------------------------------------------------------------------------
Description:		�����������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void TouchKeyDealSubroutine(void)
{
	switch(g_sysType)
	{
		case SysModePowerUp:								//�ϵ�״̬
			//���а�����Ч
		break;
			
		case SysModeStandby:								//����״̬
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					//��Ч
				break;

				case KEY_TIME_SUB:
					//��Ч
				break;

				case KEY_ORDER:
					if(keyOrderLongPressFlag)
					{
						SendCommand(CMD_KX_APP_REMOVE_LINK);
						gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
					}
					else
					{
						SendCommand(CMD_APP_KX_SMART_BAKING);
						g_LedOpenFlag = 1;
						gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
					}
					
				break;

				case KEY_CHANGE_FUN:
					g_sysType = SysModeSelect;				//ת�����ô���״̬

					g_menuNumber		= DEFAULT_MENU;		//Ĭ�ϲ˵�

					LoadDefaultValue(g_menuNumber);			//���ز˵�1��Ĭ��ֵ

					g_tempAdjStateFlag	= 0;				//Ĭ��ʱ�����״̬
					g_timeAdjStateFlag	= 1;

					g_sysSelectWaitCnt 	= 0;				//���ô����ȴ�ʱ������

					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;

				case KEY_ON_OFF:
					//��Ч
				break;

				case KEY_PCB_CHECK:							//PCB�Լ찴��
					g_sysType = SysModePCBCheck;			//ת��PCB�Լ�״̬
					g_sysSelfCheckWaitCnt	= 0;			//�Լ��޲�����ʱ����

					g_lidSelfCheckOkFlag	= 0;
					g_coreNtcSelfCheckOkFlag= 0;
					g_selfCheckPassFlag 	= 0;			//Ĭ���Լ첻�ϸ�
					g_selfCheckOverFlag		= 0;			//���Ϊ�Լ�δ���
					g_selfCheckStep			= 1;			//�����һ��
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//������������
				break;
				
				case KEY_MACHINE_CHECK:						//�����Լ찴��
					g_sysType = SysModeMachineCheck;		//ת�������Լ�״̬
					g_sysSelfCheckWaitCnt	= 0;			//�Լ��޲�����ʱ����
					
					g_pumpWorkFlag			= 0;			//Ĭ��ˮ�ò�����
					g_pumpDelayJudgeFlag	= 0;
					g_selfCheckStep			= 1;			//�����һ��
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//������������
				break;
				
				case KEY_DISP_PID:							//PID��ʾ����
					g_sysType = SysModePID;					//ת��PID��ʾ״̬
					g_sysSelfCheckWaitCnt	= 0;			//PID�޲�����ʱ����
					
					g_pidDispStep			= 1;			//����ʾǰ3λPID��
					
					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;
				
				case KEY_CLEAN_INCRUSTANT:					//���ˮ������
					g_sysType = SysModeClean;				//ת��ˮ�����״̬
					g_cleanPumpTimeCnt		= 0;			//���ʱ������

					g_cleanIncrustantStep	= 1;			//ˮ������һ��
					
					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;
			}
		break;
		
		case SysModeSelect:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					if(g_tempAdjEnFlag)						//����ǰʱ��ɵ�
					{
						if(keyCnt >= 500)					//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//��������������������
						}

						if(g_timeAdjStateFlag)				//����ǰΪʱ�����״̬
						{
							g_tempAdjStateFlag	= 1;		//��Ϊ�¶ȵ���״̬
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������
						g_sysSelectWaitCnt 	= 0;			//���ô����ȴ�ʱ������
					}
				break;

				case KEY_TEMP_SUB:
					if(g_tempAdjEnFlag)						//����ǰʱ��ɵ�
					{
						if(keyCnt >= 500)					//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//��������������������
						}
						
						if(g_timeAdjStateFlag)				//����ǰΪʱ�����״̬
						{
							g_tempAdjStateFlag	= 1;		//��Ϊ�¶ȵ���״̬
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������
						g_sysSelectWaitCnt 	= 0;			//���ô����ȴ�ʱ������
					}
				break;

				case KEY_TIME_ADD:
					if(g_timeAdjEnFlag)						//����ǰʱ��ɵ�
					{
						if(keyCnt >= 500)					//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//��������������������
						}

						if(g_tempAdjStateFlag)				//����ǰΪ�¶ȵ���״̬
						{
							g_tempAdjStateFlag	= 0;		//��Ϊʱ�����״̬
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������
						g_sysSelectWaitCnt 	= 0;			//���ô����ȴ�ʱ������
					}
				break;

				case KEY_TIME_SUB:
					if(g_timeAdjEnFlag)						//����ǰʱ��ɵ�
					{
						if(keyCnt >= 500)					//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//��������������������
						}

						if(g_tempAdjStateFlag)				//����ǰΪ�¶ȵ���״̬
						{
							g_tempAdjStateFlag	= 0;		//��Ϊʱ�����״̬
							g_timeAdjStateFlag	= 1;
						}
						else
						{
							if(g_workTimeAll <= MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])	//����ǰʱ��Ϊ��Сʱ�䣬ֱ����ת�����ֵ
							{
								g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
							}
							else
							{
								if(g_workTimeAll > 30)						//2���ӿɵ�ȡ2�ı���
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������
						g_sysSelectWaitCnt 	= 0;			//���ô����ȴ�ʱ������
					}
				break;

				case KEY_ORDER:
					if(g_orderEnFlag)						//��ԤԼ���ܵĲ˵�ʱ��Ч
					{
						g_sysType = SysModeOrderSet;		//ת��ԤԼ����״̬
						
						g_sysOrderSetWaitCnt = 0;			//ԤԼ���õȴ�ʱ������
						
						g_orderTime = 360;					//Ĭ��ԤԼʱ��6H
						
						if(g_workTimeAll%30)				//��СԤԼʱ��Ϊ������ʱ������ȡ��
						{
							g_orderMinTime = (g_workTimeAll/30 + 1)*30;
						}
						else
						{
							g_orderMinTime = g_workTimeAll + 30;
						}
						
						g_timeAdjEnFlag		= NO_ADJ;		//�¶�/����ʱ�䲻�ɵ�
						g_tempAdjEnFlag		= NO_ADJ;
						
						g_timeAdjStateFlag	= NO_ADJ;		//���¶�/ʱ�����״̬
						g_tempAdjStateFlag	= NO_ADJ;

						gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
					}
				break;

				case KEY_CHANGE_FUN:
					if(g_menuNumber >= MENU_3_NUM)			//�˵�ѭ��
					{
						g_menuNumber = MENU_1_NUM;
					}
					else
					{
						g_menuNumber++;
					}
					
					LoadDefaultValue(g_menuNumber);			//���ز˵�Ĭ��ֵ

					if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						g_tempAdjStateFlag	= 0;			//һ��ͨĬ��ʱ�����״̬
						g_timeAdjStateFlag	= 1;
					}
					else
					{
						g_tempAdjStateFlag	= 1;			//Ĭ���¶ȵ���״̬
						g_timeAdjStateFlag	= 0;
					}

					g_sysSelectWaitCnt 	= 0;				//���ô����ȴ�ʱ������

					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//�����ش���
					{
						g_sysType  = SysModeStandby;		//�ص�����״̬
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//���Ϊ������ˮ

						g_menuNumber		= DEFAULT_MENU;	//Ĭ�ϲ˵�
						
						g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������
					}
					else
					{
						g_sysType			= SysModeWork;	//ת�빤��״̬

						g_enContinueErr		= NoError;		//��ʼ�޿ɻָ�����
						g_waterYieldType 	= NormalType;	//Ĭ��������ˮ

						g_workOneMinCnt		= 0;			//1min��ʱ����

						g_nowStepNum		= 1;			//��ʼ���ղ���1
						
						g_highSteamPowerCnt	= 0;			//���������¸߹������ʱ���ʱ����
						g_steamLowPowerFlag	= 0;			//�����͹��������־����

						g_tempAdjStateFlag	= 0;			//������Ĭ��ʱ�����״̬
						g_timeAdjStateFlag	= 1;

						g_coolingFanDealyFlag = 0;			//ɢ�ȷ���Ĭ�ϲ���ʱֹͣ

						g_pumpDelayJudgeCnt	= 0;

						if(g_pumpCalibrationFlag == 0)		//��δ�궨�ɹ�
						{
							InitPumpCalibrationData();
						}

						g_dispFlashCnt		= 0;			//��˸��������
						g_dispFlashFlag		= 0;			//����

						g_recvWorkTime_1 = 0;
						g_recvWorkTime_2 = 0;
						g_recvWorkTime_3 = 0;
					}
					
					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;

				case KEY_PCB_CHECK:							//PCB�Լ찴��
					g_sysType = SysModePCBCheck;			//ת��PCB�Լ�״̬
					g_sysSelfCheckWaitCnt	= 0;			//�Լ��޲�����ʱ����

					g_lidSelfCheckOkFlag	= 0;
					g_coreNtcSelfCheckOkFlag= 0;
					g_selfCheckPassFlag 	= 0;			//Ĭ���Լ첻�ϸ�
					g_selfCheckOverFlag		= 0;			//���Ϊ�Լ�δ���
					g_selfCheckStep			= 1;			//�����һ��
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//������������
				break;
				
				case KEY_MACHINE_CHECK:						//�����Լ찴��
					g_sysType = SysModeMachineCheck;		//ת�������Լ�״̬
					g_sysSelfCheckWaitCnt	= 0;			//�Լ��޲�����ʱ����
					
					g_pumpWorkFlag			= 0;			//Ĭ��ˮ�ò�����
					g_pumpDelayJudgeFlag	= 0;
					g_selfCheckStep			= 1;			//�����һ��
					
					gU8_buzzerType = BUZ_JK_PREHEATING;		//������������
				break;
				
				case KEY_DISP_PID:							//PID��ʾ����
					g_sysType = SysModePID;					//ת��PID��ʾ״̬
					g_sysSelfCheckWaitCnt	= 0;			//PID�޲�����ʱ����
					
					g_pidDispStep			= 1;			//����ʾǰ3λPID��
					
					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;
				
				case KEY_CLEAN_INCRUSTANT:					//���ˮ������
					g_sysType = SysModeClean;				//ת��ˮ�����״̬
					g_cleanPumpTimeCnt		= 0;			//���ʱ������

					g_cleanIncrustantStep	= 1;			//ˮ������һ��
					
					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;
			}
		break;

		case SysModeOrderSet:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					if(keyCnt >= 500)						//����������������Ч
					{
						keyCnt = 400;
					}
					else
					{
						gU8_buzzerType = BUZ_JK_KEY;		//��������������������
					}
				
					if(g_orderTime >= ORDER_TIME_MAX)		//����ǰԤԼʱ��Ϊ�ԤԼʱ��
					{
						g_orderTime = g_orderMinTime;		//��Ϊ���ԤԼʱ��
					}
					else
					{
						if(g_orderTime >= 600)				//ʱ�����10H����1H
						{
							g_orderTime = g_orderTime + 60;
						}
						else								//10H�ڲ���30
						{
							g_orderTime = g_orderTime + 30;
						}
					}
				
					g_sysOrderSetWaitCnt = 0;				//ԤԼ���õȴ�ʱ������
				break;

				case KEY_TIME_SUB:
					if(keyCnt >= 500)						//����������������Ч
					{
						keyCnt = 400;
					}
					else
					{
						gU8_buzzerType = BUZ_JK_KEY;		//����������
					}
				
					if(g_orderTime <= g_orderMinTime)		//����ǰԤԼʱ��Ϊ��СԤԼʱ��
					{
						g_orderTime = ORDER_TIME_MAX;		//��Ϊ�ԤԼʱ��
					}
					else
					{
						if(g_orderTime > 600)				//ʱ�����10H����1H
						{
							g_orderTime = g_orderTime - 60;
						}
						else								//10H�ڲ���30
						{
							g_orderTime = g_orderTime - 30;
						}
					}
				
					g_sysOrderSetWaitCnt = 0;				//ԤԼ���õȴ�ʱ������
				break;

				case KEY_ORDER:
					g_sysType = SysModeSelect;				//�ص����ô���״̬
					
					g_sysSelectWaitCnt	= 0;				//���ô����ȴ�ʱ������
					
					LoadDefaultValue(g_menuNumber);			//���ز˵���Ĭ��ֵ

					if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						g_tempAdjStateFlag	= 0;			//һ��ͨĬ��ʱ�����״̬
						g_timeAdjStateFlag	= 1;
					}
					else
					{
						g_tempAdjStateFlag	= 1;			//Ĭ���¶ȵ���״̬
						g_timeAdjStateFlag	= 0;
					}

					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;

				case KEY_CHANGE_FUN:
					g_sysType = SysModeSelect;				//�ص����ô���״̬
					
					g_sysSelectWaitCnt	= 0;				//���ô����ȴ�ʱ������
					
					LoadDefaultValue(g_menuNumber);			//���ز˵���Ĭ��ֵ

					if((g_menuNumber == MENU_4_NUM) || (g_menuNumber == MENU_5_NUM) || (g_menuNumber == MENU_6_NUM))
					{
						g_tempAdjStateFlag	= 0;			//һ��ͨĬ��ʱ�����״̬
						g_timeAdjStateFlag	= 1;
					}
					else
					{
						g_tempAdjStateFlag	= 1;			//Ĭ���¶ȵ���״̬
						g_timeAdjStateFlag	= 0;
					}

					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//�����ش���
					{
						g_sysType  = SysModeStandby;		//�ص�����״̬
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//���Ϊ������ˮ

						g_menuNumber		= DEFAULT_MENU;	//Ĭ�ϲ˵�
						
						g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������
					}
					else
					{
						g_sysType = SysModeOrderWork;		//ת��ԤԼ����״̬

						g_enContinueErr		= NoError;		//��ʼ�޿ɻָ�����
						g_waterYieldType 	= NormalType;	//Ĭ��������ˮ

						g_nowStepNum = 1;					//���������һ������
						
						g_workOneMinCnt		= 0;			//1min��ʱ����
						
						g_highSteamPowerCnt	= 0;			//���������¸߹������ʱ���ʱ����
						g_steamLowPowerFlag	= 0;			//�����͹��������־����

						g_coolingFanDealyFlag = 0;			//ɢ�ȷ���Ĭ�ϲ���ʱֹͣ
						
						g_pumpDelayJudgeCnt	= 0;

						g_dispFlashCnt		= 0;			//��˸��������
						g_dispFlashFlag		= 0;			//��ʾ����
					}
					
					gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
				break;
			}
		break;

		case SysModeOrderWork:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					//��Ч
				break;

				case KEY_TIME_SUB:
					//��Ч
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//�����ش���
					{
						g_sysType  = SysModeStandby;		//�ص�����״̬
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//���Ϊ������ˮ

						g_menuNumber		= DEFAULT_MENU;	//Ĭ�ϲ˵�
						
						g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������

						gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
					}
				break;
			}
		break;

		case SysModeWork:
		case SYS_MODE_SMART_WORKING:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//����״̬�£�ֻ��3���������ܿɵ�
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)						//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;		//��������������������
						}

						if(g_timeAdjStateFlag)					//����ǰΪʱ�����״̬
						{
							g_tempAdjStateFlag	= 1;			//��Ϊ�¶ȵ���״̬
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������

						g_steamPreHeatFlag 	= 0;			//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
					}
				break;

				case KEY_TEMP_SUB:
					//����״̬�£�ֻ��3���������ܿɵ�
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)					//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;	//��������������������
						}
						
						if(g_timeAdjStateFlag)				//����ǰΪʱ�����״̬
						{
							g_tempAdjStateFlag	= 1;		//��Ϊ�¶ȵ���״̬
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������

						g_steamPreHeatFlag 	= 0;			//�����ж���¯�Ƿ�Ԥ�ȣ����¸�ֵ��ˮĬ��ֵ��
					}
				break;

				case KEY_TIME_ADD:
					//����״̬�£�ֻ��3���������ܿɵ�
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)						//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;		//��������������������
						}

						if(g_tempAdjStateFlag)					//����ǰΪ�¶ȵ���״̬
						{
							g_tempAdjStateFlag	= 0;			//��Ϊʱ�����״̬
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������

//						g_workOneMinCnt		= 0;			//1min��ʱ���㣨��ʾS��ȡ�����㣩
					}
				break;

				case KEY_TIME_SUB:
					//����״̬�£�ֻ��3���������ܿɵ�
					if((g_menuNumber == MENU_1_NUM) || (g_menuNumber == MENU_2_NUM) || (g_menuNumber == MENU_3_NUM))
					{
						if(keyCnt >= 500)						//����������������Ч
						{
							keyCnt = 400;
						}
						else
						{
							gU8_buzzerType = BUZ_JK_KEY;		//��������������������
						}

						if(g_tempAdjStateFlag)					//����ǰΪ�¶ȵ���״̬
						{
							g_tempAdjStateFlag	= 0;			//��Ϊʱ�����״̬
							g_timeAdjStateFlag	= 1;
						}
						else
						{
							if(g_workTimeAll <= MenuDefaultValue_Table[g_menuNumber][ADJ_MIN_TIME_NUM])	//����ǰʱ��Ϊ��Сʱ�䣬ֱ����ת�����ֵ
							{
								g_workTimeAll = MenuDefaultValue_Table[g_menuNumber][ADJ_MAX_TIME_NUM];
							}
							else
							{
								if(g_workTimeAll > 30)						//2���ӿɵ�ȡ2�ı���
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

						g_adjWaitTimeCnt	= 0;			//ʱ��/�¶ȵ��ڵȴ�ʱ������

//						g_workOneMinCnt		= 0;			//1min��ʱ���㣨��ʾS��ȡ�����㣩
					}
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//�����ش���
					{
						g_sysType  = SysModeStandby;		//�ص�����״̬
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//���Ϊ������ˮ

						g_menuNumber		= DEFAULT_MENU;	//Ĭ�ϲ˵�
						
						g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������

						gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����

						SendCommand(CMD_KX_APP_REPORT_STATUS);	//�����ϱ�״̬
					}
					else
					{
						if(g_waterYieldType == LackWater)		//��ȱˮ�����������������ȡ���������¼��
						{
							g_enContinueErr		= NoError;		//�������
							g_waterYieldType	= NormalType;	//���Ϊ������ˮ
							g_pumpDelayJudgeCnt	= 0;
							
							g_highSteamPowerCnt	= 0;			//���������¸߹������ʱ���ʱ����
							g_steamLowPowerFlag	= 0;			//�����͹��������־����

							g_quickPumpingFlag	= 1;			//ȡ��ȱˮ��������п��
							
							if(g_pumpCalibrationFlag == 0)		//��δ�궨�ɹ������±궨
							{
								InitPumpCalibrationData();
							}
							
							gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
						}
					}
				break;
			}
		break;

		case SysModeWarm:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					//��Ч
				break;

				case KEY_TIME_SUB:
					//��Ч
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					if(keyOnOffLongPressFlag == 1)			//�����ش���
					{
						g_sysType  = SysModeStandby;		//�ص�����״̬
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//���Ϊ������ˮ

						g_menuNumber		= DEFAULT_MENU;	//Ĭ�ϲ˵�
						
						g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������

						gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
					}
					else
					{
						if(g_waterYieldType == LackWater)		//��ȱˮ�����������������ȡ���������¼��
						{
							g_enContinueErr		= NoError;		//�������
							g_waterYieldType	= NormalType;	//���Ϊ������ˮ
							g_pumpDelayJudgeCnt	= 0;
							
							g_highSteamPowerCnt	= 0;			//���������¸߹������ʱ���ʱ����
							g_steamLowPowerFlag	= 0;			//�����͹��������־����

							g_quickPumpingFlag	= 1;			//ȡ��ȱˮ��������п��
							
							if(g_pumpCalibrationFlag == 0)		//��δ�궨�ɹ������±궨
							{
								InitPumpCalibrationData();
							}
							
							gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
						}
					}
				break;
			}
		break;

		case SysModeEnd:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					//��Ч
				break;

				case KEY_TIME_SUB:
					//��Ч
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					g_sysType  = SysModeStandby;		//�ص�����״̬
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;	//���Ϊ������ˮ

					g_menuNumber		= DEFAULT_MENU;	//Ĭ�ϲ˵�
					
					g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;
			}
		break;

		case SysModeDarkScreen:
			
		break;

		case SysModeError:
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					//��Ч
				break;

				case KEY_TIME_SUB:
					//��Ч
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					if((measureData.errCode == 0x03) ||
					(measureData.errCode == 0x04) ||
					(measureData.errCode == 0x05) ||
					(measureData.errCode == 0x06) ||
					(measureData.errCode == 0x57) ||
					(measureData.errCode == 0x58) ||
					(measureData.errCode == 0x59))		//��¯NTC�����㶪ʧ��ˮ�ñ궨ʧ�ܡ���ʾ���Լ챨��ʱ������ȡ���ش���
					{
						g_sysType	= SysModeStandby;	//�ص�����״̬
						g_menuNumber = DEFAULT_MENU;	//Ĭ�ϲ˵�
						
						if((measureData.errCode == 0x57) || (measureData.errCode == 0x58) || (measureData.errCode == 0x59))
						{
							/*�������AD����GPIO����*/
							ADCH1 &= ~BIT7;
							
							/*������AD�ڲ������������*/
							P3    |= BIT7;									//���������ĵ��ݳ��
							P3CR  |= BIT7;
							P3PCR &= ~BIT7;
							
							SEQCHX = 0x02;									//ѡ��ͨ��2
						}
						
						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;	//���Ϊ������ˮ

						measureData.errCode = 0;		//�������
						
						g_sysStandbyWaitCnt = 0;		//�����ȴ�ʱ������

						gU8_buzzerType = BUZ_JK_KEY;	//����������Ч����
					}
					else
					{
						gU8_buzzerType = BUZ_JK_MUTE;	//����ֹͣ����
					}
				break;
			}
		break;

		case SysModePCBCheck:							//PCB�Լ�״̬
			if((keyValue == KEY_ON_OFF) && (g_selfCheckStep != 4))
			{
				g_sysType	= SysModeStandby;			//�ص�����״̬
				g_menuNumber = DEFAULT_MENU;			//Ĭ�ϲ˵�
				
				g_enContinueErr		= NoError;
				g_waterYieldType	= NormalType;		//���Ϊ������ˮ
				
				g_sysStandbyWaitCnt = 0;				//�����ȴ�ʱ������
				
				g_selfCheckStep	= 0;
				
				/*�������AD����GPIO����*/
				ADCH1 &= ~BIT7;
				
				/*������AD�ڲ������������*/
				P3    |= BIT7;							//���������ĵ��ݳ��
				P3CR  |= BIT7;
				P3PCR &= ~BIT7;
				
				SEQCHX = 0x02;							//ѡ��ͨ��2

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else
			{
				if((g_selfCheckStep == 1) && (keyValue == KEY_TEMP_ADD))
				{
					g_selfCheckStep = 2;				//����ڶ���
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 2) && (keyValue == KEY_TEMP_SUB))
				{
					g_selfCheckStep = 3;				//���������
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 3) && (keyValue == KEY_ORDER))
				{
					g_selfCheckStep = 4;				//������Ĳ�
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 4) && (keyValue == KEY_ON_OFF))
				{
					g_selfCheckStep = 5;				//������岽
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 5) && (keyValue == KEY_CHANGE_FUN))
				{
					g_selfCheckStep = 6;				//���������
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 6) && (keyValue == KEY_TIME_SUB))
				{
					g_selfCheckStep = 7;				//������߲�
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����
					
//					ADCH1 |= BIT7;						//�������AD����ģ�������
//					SEQCHX = 0x07;						//ѡ��ͨ��7
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 7) && (keyValue == KEY_TIME_ADD))
				{
					g_selfCheckStep = 8;				//����ڰ˲�
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
				else if((g_selfCheckStep == 8) && (keyValue == KEY_TEMP_ADD))
				{
					g_selfCheckStep = 1;				//�����ص�һ��
					g_sysSelfCheckWaitCnt = 0;			//�Լ��޲�����ʱ����

					SEQCHX = 0x02;						//ѡ��ͨ��2
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				}
			}
		break;

		case SysModeMachineCheck:						//�����Լ�״̬
			if((g_selfCheckStep == 1) && (keyValue == KEY_TEMP_ADD))
			{
				g_selfCheckStep = 2;					//����ڶ���
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 2) && (keyValue == KEY_TEMP_SUB))
			{
				g_selfCheckStep = 3;					//���������
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 3) && (keyValue == KEY_ORDER))
			{
				g_selfCheckStep = 4;					//������Ĳ�
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 4) && (keyValue == KEY_ON_OFF))
			{
				g_selfCheckStep = 5;					//������岽
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 5) && (keyValue == KEY_CHANGE_FUN))
			{
				g_selfCheckStep = 6;					//���������
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 6) && (keyValue == KEY_TIME_SUB))
			{
				g_selfCheckStep = 7;					//������߲�
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 7) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 8;					//����ڰ˲�
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 8) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 9;					//����ھŲ�
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 9) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 11;					//������ʮ����SF1��ɢ�ȷ��ȣ���ֱ�ӵ���ʮһ��
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 10) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 11;					//�����ʮһ����ˮ�ü�⣩
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 11) && (keyValue == KEY_TIME_ADD) && (g_pumpWorkFlag == 1))		//δ��⵽ˮ�ù����򲻽�����һ��
			{
				g_selfCheckStep = 12;					//�����ʮ����
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((g_selfCheckStep == 12) && (keyValue == KEY_TIME_ADD))
			{
				g_selfCheckStep = 13;					//�����ʮ����
				g_sysSelfCheckWaitCnt = 0;				//�Լ��޲�����ʱ����
				
				g_darkScreenTime = 3;					//Ϣ��1.5S��ص��Լ��һ��,500msʱ��
				
				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
			else if((keyValue == KEY_ON_OFF) && (g_selfCheckStep != 4))
			{
				g_sysType	= SysModeStandby;			//�ص�����״̬
				g_menuNumber = DEFAULT_MENU;			//Ĭ�ϲ˵�
				
				g_enContinueErr		= NoError;
				g_waterYieldType	= NormalType;		//���Ϊ������ˮ
				
				g_sysStandbyWaitCnt = 0;				//�����ȴ�ʱ������

				g_selfCheckStep	= 0;

				gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
			}
		break;

		case SysModeDebug:								//Debug״̬
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					g_debugDispStep = 1;				//Debug��ʾ�����һ��

					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_TEMP_SUB:
					g_debugDispStep = 2;				//Debug��ʾ����ڶ���

					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_TIME_ADD:
					g_debugDispStep = 3;				//Debug��ʾ���������

					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_TIME_SUB:
					g_debugDispStep = 4;				//Debug��ʾ������Ĳ�

					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					g_sysType	= SysModeStandby;		//�ص�����״̬
					g_menuNumber = DEFAULT_MENU;		//Ĭ�ϲ˵�
					
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;	//���Ϊ������ˮ
					
					g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������

					g_debugDispStep		= 0;

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;
			}
		break;

		case SysModePID:								//PID״̬
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					g_sysType = SysModeDebug;			//ת��Debug��ʾ״̬
					
					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����
					
					g_debugDispStep = 1;				//Debug��ʾ�����һ��
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_TEMP_SUB:
					g_sysType = SysModeDebug;			//ת��Debug��ʾ״̬
					
					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����
					
					g_debugDispStep = 2;				//Debug��ʾ����ڶ���
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_TIME_ADD:
					g_sysType = SysModeDebug;			//ת��Debug��ʾ״̬
					
					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����
					
					g_debugDispStep = 3;				//Debug��ʾ���������
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_TIME_SUB:
					g_sysType = SysModeDebug;			//ת��Debug��ʾ״̬
					
					g_sysSelfCheckWaitCnt = 0;			//Debug�޲�����ʱ����
					
					g_debugDispStep = 4;				//Debug��ʾ������Ĳ�
					
					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					g_sysType	= SysModeStandby;		//�ص�����״̬
					g_menuNumber = DEFAULT_MENU;		//Ĭ�ϲ˵�
					
					g_enContinueErr		= NoError;
					g_waterYieldType	= NormalType;	//���Ϊ������ˮ
					
					g_sysStandbyWaitCnt = 0;			//�����ȴ�ʱ������

					g_debugDispStep		= 0;

					gU8_buzzerType = BUZ_JK_KEY;		//����������Ч����
				break;
			}
		break;

		case SysModeClean:								//���ˮ��״̬
			switch(keyValue)
			{
				case KEY_TEMP_ADD:
					//��Ч
				break;

				case KEY_TEMP_SUB:
					//��Ч
				break;

				case KEY_TIME_ADD:
					//��Ч
				break;

				case KEY_TIME_SUB:
					//��Ч
				break;

				case KEY_ORDER:
					//��Ч
				break;

				case KEY_CHANGE_FUN:
					//��Ч
				break;

				case KEY_ON_OFF:
					if((g_cleanIncrustantStep == 2) && (g_cleanPumpTimeCnt == CLEAN_TIME_STEP_2 + 1))		//���ڶ���ʱ�䵽
					{
						g_cleanPumpTimeCnt		= 0;			//��ಽ��ʱ������
						
						g_cleanIncrustantStep	= 3;
						
						gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
					}
					else
					{
						g_sysType	= SysModeStandby;			//�ص�����״̬
						g_menuNumber = DEFAULT_MENU;			//Ĭ�ϲ˵�

						g_enContinueErr		= NoError;
						g_waterYieldType	= NormalType;		//���Ϊ������ˮ

						g_sysStandbyWaitCnt = 0;				//�����ȴ�ʱ������
						
						g_cleanPumpTimeCnt	= 0;
						g_cleanIncrustantStep = 0;
						
						g_pidDispStep		= 0;
						
						gU8_buzzerType = BUZ_JK_KEY;			//����������Ч����
					}
				break;
			}
		break;
	}
}
