#include "header.h"

//������ʾ��
unsigned char code		Disp_Table[16]=
{
	DISPDATA_0,	DISPDATA_1,	DISPDATA_2,	DISPDATA_3,
	DISPDATA_4,	DISPDATA_5,	DISPDATA_6,	DISPDATA_7,
	DISPDATA_8,	DISPDATA_9,	DISPDATA_A,	DISPDATA_b,
	DISPDATA_C,	DISPDATA_d,	DISPDATA_E,	DISPDATA_F,
};

LED_MESSAGE Led_Buff[4];

unsigned char			DispComCnt;					//COM��ָ��
bit						g_flashFlag;				//��˸��־λ

unsigned char bdata		DispDataBuf;				//��ʾ���ݻ���
sbit					DispSegA	= DispDataBuf^0;
sbit					DispSegB	= DispDataBuf^1;
sbit					DispSegC	= DispDataBuf^2;
sbit					DispSegD	= DispDataBuf^3;
sbit					DispSegE	= DispDataBuf^4;
sbit					DispSegF	= DispDataBuf^5;
sbit					DispSegG	= DispDataBuf^6;
sbit					DispSegH	= DispDataBuf^7;

/*-----------------------------------------------------------------------------
Description:		��ʼ����ʾ���IO
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitDispIo(void)
{
	BANK0_SET;
	
	/*SEG����������������������*/
	P1CR  |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5);				//���
	P3CR  |= BIT4;
	P4CR  |= BIT5;

	P1PCR &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5));				//������
	P3PCR &= (~(BIT4));
	P4PCR &= (~(BIT5));

	P1	  &= (~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5));				//�����
	P2	  &= (~(BIT4));
	P4	  &= (~(BIT5));

	BANK1_SET;
	P5CR  |= (BIT2);
	P5PCR &= (~(BIT2));
	P5	  &= (~(BIT2));
	BANK0_SET;
	
	/*COM����������������������*/
	P3CR  |= (BIT0 | BIT1 | BIT2 | BIT3);			//���
	P3PCR &= (~(BIT0 | BIT1 | BIT2 | BIT3));		//������
	P3	  &= (~(BIT0 | BIT1 | BIT2 | BIT3));		//�����
}

/*-----------------------------------------------------------------------------
Description:		��ʼ����ʾ����
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitDispVariable(void)
{
	unsigned char i;

	for(i=0;i<4;i++)
	{
		Led_Buff[i].ramDat = DISPDATA_OFF;		//��ʾȫ��
	}
	
	DispComCnt  = 0;							//COM��ָ��ָ��COM1��
	DispDataBuf = 0;							//���ݻ�����Ϊ0
	
	g_flashFlag = 0;
}

/*-----------------------------------------------------------------------------
Description:		������ʾ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void DispAllOn(void)
{
	unsigned char i;
	
	for(i=0;i<4;i++)
	{
		Led_Buff[i].ramDat = DISPDATA_ON;				//��ʾȫ��
	}
}

/*-----------------------------------------------------------------------------
Description:		������ʾ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void DispAllOff(void)
{
	unsigned char i;

	for(i=0;i<4;i++)
	{
		Led_Buff[i].ramDat = DISPDATA_OFF;				//��ʾȫ��
	}
}

/*-----------------------------------------------------------------------------
Description:		������ʾ���ݣ�seg��ƽ���ƣ�
Input:				void
Return:				void
History:			��
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
	
	BANK1_SET;									//P5������BANK1
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
Description:		��ʾɨ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void DisplayScan(void)
{
	P_COM1 = 1;									//�ر���ʾ��������
	P_COM2 = 1;
	P_COM3 = 1;
	P_COM4 = 1;
	
	_nop_();									//�����ƽ�ȶ�
	_nop_();									//�����ƽ�ȶ�
	_nop_();									//�����ƽ�ȶ�
	_nop_();									//�����ƽ�ȶ�
	_nop_();									//�����ƽ�ȶ�
	_nop_();									//�����ƽ�ȶ�
	
	if(DispComCnt >= 4)							//4��COM��ѭ��
	{
		DispComCnt = 0;
	}

	switch(DispComCnt)
	{
		case 0:									//COM1��
			DispDataBuf = Led_Buff[0].ramDat;	//���ݸ�ֵ��������
			
			DisplayDataLoad();					//���ݼ��أ���ѡͨseg����ѡͨCOM��
			P_COM1 = 0;							//P_COM1���õ͵�ƽ-ѡͨ
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;
		case 1:									//COM2��
			DispDataBuf = Led_Buff[1].ramDat;	//���ݸ�ֵ��������
			
			DisplayDataLoad();					//���ݼ��أ���ѡͨseg����ѡͨCOM��
			P_COM2 = 0;							//P_COM2���õ͵�ƽ-ѡͨ
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;
		case 2:									//COM3��
			DispDataBuf = Led_Buff[2].ramDat;	//���ݸ�ֵ��������
			
			DisplayDataLoad();					//���ݼ��أ���ѡͨseg����ѡͨCOM��
			P_COM3 = 0;							//P_COM3���õ͵�ƽ-ѡͨ
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;
		case 3:									//COM4��
			DispDataBuf = Led_Buff[3].ramDat;	//���ݸ�ֵ��������
			
			DisplayDataLoad();					//���ݼ��أ���ѡͨseg����ѡͨCOM��		
			P_COM4 = 0;							//P_COM4���õ͵�ƽ-ѡͨ
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			break;

		default:
			break;
	}
	
	DispComCnt++;								//ָ����һ��COM��
}

/*-----------------------------------------------------------------------------
Description:		��ʾ����
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void DispCrl(void)
{
	unsigned char i = 0;

	unsigned char wifiStatus;

	DispAllOff();
	
	switch(g_sysType)
	{
		case SysModePowerUp:											//�ϵ�ȫ��
			DispAllOn();
			break;
			
		case SysModeStandby:											//����״̬��ʾ��---��
			DIGITAL_FIRST_NUM(DISPDATA_SG);
			DIGITAL_SECOND_NUM(DISPDATA_SG);
			DIGITAL_THIRD_NUM(DISPDATA_SG);
			break;
			
		case SysModeSelect:												//���ô���״̬
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

			if(g_tempAdjStateFlag)										//�����¶ȵ���״̬����ʾ�¶ȣ���ͼ��
			{
				if(g_nowStepworkTemp/100)								//�¶��а�λʱ��ʾ
				{
					DIGITAL_FIRST_NUM(Disp_Table[g_nowStepworkTemp/100]);
				}
				
				DIGITAL_SECOND_NUM(Disp_Table[g_nowStepworkTemp%100/10]);
				DIGITAL_THIRD_NUM(Disp_Table[g_nowStepworkTemp%100%10]);

				SYMBOL_TEMP = 1;
			}
			else														//������ʾʱ�䣬��ͼ��
			{
				DIGITAL_SECOND_NUM(Disp_Table[g_workTimeAll/10]);
				DIGITAL_THIRD_NUM(Disp_Table[g_workTimeAll%10]);

				SYMBOL_MINITE = 1;
			}

			break;
		
		case SysModeOrderSet:											//ԤԼ����״̬
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

			if(g_orderTime >= 600)										//10H����
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
			
		case SysModeOrderWork:											//ԤԼ����״̬
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

			if(g_orderTime > 570)										//9.5H����
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
				if(g_orderTime%60 > 30)										//�������룬��Сʱ��һСʱ��9.5H�����£�
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

			if(g_LidFlag == LID_OPEN)									//����ʱ����ʾE11
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			break;
		
		case SysModeWork:												//����״̬
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

			if(g_tempAdjStateFlag)										//�����¶ȵ���״̬����ʾ�¶ȣ���ͼ��
			{	
				if(g_nowStepworkTemp/100)								//�¶��а�λʱ��ʾ
				{
					DIGITAL_FIRST_NUM(Disp_Table[g_nowStepworkTemp/100]);
				}
				
				DIGITAL_SECOND_NUM(Disp_Table[g_nowStepworkTemp%100/10]);
				DIGITAL_THIRD_NUM(Disp_Table[g_nowStepworkTemp%100%10]);

				SYMBOL_TEMP = 1;
			}
			else														//������ʾʱ�䣬��ͼ��
			{
				if(g_workOneMinCnt < 12)								//g_workOneMinCnt������500ms����һ�Σ�12Ϊ6S
				{
					if((g_workTimeAll/10) > 0)
					{
						DIGITAL_FIRST_NUM(Disp_Table[g_workTimeAll/10]);
					}
					else
					{
						//ʮλ������ʱ����ʾ
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
						//ʮλ������ʱ����ʾ
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

			if(g_LidFlag == LID_OPEN)									//����ʱ����ʾE11
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			
			if(g_waterYieldType == LackWater)							//ȱˮʱ����ʾE12
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_2);
			}
			break;
		
		case SysModeWarm:												//����״̬
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

			DIGITAL_FIRST_NUM(DISPDATA_b);								//��ʾ����ʱ��
			DIGITAL_SECOND_NUM(Disp_Table[g_keepWarmTime/10]);
			DIGITAL_THIRD_NUM(Disp_Table[g_keepWarmTime%10]);

			SYMBOL_MINITE = 1;

			if(g_LidFlag == LID_OPEN)									//����ʱ����ʾE11
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			
			if(g_waterYieldType == LackWater)							//ȱˮʱ����ʾE12
			{
				DIGITAL_FIRST_NUM(DISPDATA_E);
				DIGITAL_SECOND_NUM(DISPDATA_1);
				DIGITAL_THIRD_NUM(DISPDATA_2);
			}
			break;

		case SysModeEnd:												//��������״̬
			DIGITAL_FIRST_NUM(DISPDATA_E);
			DIGITAL_SECOND_NUM(DISPDATA_n);
			DIGITAL_THIRD_NUM(DISPDATA_d);
			break;
			
		case SysModeError:												//����״̬
			DIGITAL_FIRST_NUM(DISPDATA_E);								//��ʾ��������
			DIGITAL_SECOND_NUM(Disp_Table[(measureData.errCode & 0xf0) >> 4]);
			DIGITAL_THIRD_NUM(Disp_Table[(measureData.errCode & 0x0f)]);
			break;
			
		case SysModePCBCheck:											//PCB�Լ�״̬
			switch(g_selfCheckStep)
			{
				case 1:
					DispAllOn();
				break;

				case 2:													//��ʾ��ʾ�巢�а汾��
					DIGITAL_FIRST_NUM(DISPDATA_A);
					DIGITAL_SECOND_NUM(Disp_Table[RELEASE_VERSION/10]);
					DIGITAL_THIRD_NUM(Disp_Table[RELEASE_VERSION%10]);
				break;

				case 3:													//��ʾ����123
					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 4:													//��ʾ����123���˵�1��3
					SYMBOL_FUN_WING		= 1;
					SYMBOL_FUN_CHIPS	= 1;

					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 5:													//��ʾ����123���˵�1��3,��������ը��ԭζ��
					SYMBOL_FUN_WING		= 1;
					SYMBOL_FUN_CHIPS	= 1;
					SYMBOL_FUN_BAKE		= 1;
					SYMBOL_FUN_STEAM	= 1;

					DIGITAL_FIRST_NUM(DISPDATA_1);
					DIGITAL_SECOND_NUM(DISPDATA_2);
					DIGITAL_THIRD_NUM(DISPDATA_3);
				break;

				case 6:													//��ʾ����123���˵�1��2��3,��������ը����������ԭζ��
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

				case 7:													//��ʾ����123���˵�1��2��3,��������ը����������ԭζ��,��
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

				case 8:													//��ʾ����123���˵�1��2��3,��������ը����������ԭζ��,��,��
					if(g_selfCheckOverFlag)								//���Լ������
					{
						if(g_selfCheckPassFlag)							//�����ϸ�
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
							//ת�뱨��״̬
						}
					}
				break;
			}
			break;
			
		case SysModeMachineCheck:										//�����Լ�״̬
			switch(g_selfCheckStep)
			{
				case 1:													//ȫ��
					DispAllOn();
				break;

				case 2:													//��ʾSF1
					DIGITAL_FIRST_NUM(DISPDATA_S);
					DIGITAL_SECOND_NUM(DISPDATA_F);
					DIGITAL_THIRD_NUM(DISPDATA_1);
				break;

				case 3:													//��ʾ��Դ�巢�а汾��
					DIGITAL_FIRST_NUM(DISPDATA_P);
					DIGITAL_SECOND_NUM(Disp_Table[((RECV_POWER_VERSION & 0xf0) >> 4)/10]);
					DIGITAL_THIRD_NUM(Disp_Table[((RECV_POWER_VERSION & 0xf0) >> 4)%10]);
				break;

				case 4:													//��ʾ��ʾ�巢�а汾��
					DIGITAL_FIRST_NUM(DISPDATA_A);
					DIGITAL_SECOND_NUM(Disp_Table[RELEASE_VERSION/10]);
					DIGITAL_THIRD_NUM(Disp_Table[RELEASE_VERSION%10]);
				break;

				case 5:													//��ʾǻ�������AD
					DIGITAL_FIRST_NUM(Disp_Table[(measureData.coreAdHValue & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(measureData.coreAdHValue & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(measureData.coreAdHValue & 0x000f)]);
				break;

				case 6:													//��ʾǻ�������AD
					DIGITAL_FIRST_NUM(Disp_Table[(measureData.coreAdLValue & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(measureData.coreAdLValue & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(measureData.coreAdLValue & 0x000f)]);
				break;

				case 7:													//��ʾ��¯������AD
					DIGITAL_FIRST_NUM(Disp_Table[(g_adDataHSteam & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(g_adDataHSteam & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(g_adDataHSteam & 0x000f)]);
				break;

				case 8:													//��ʾ��¯������AD
					DIGITAL_FIRST_NUM(Disp_Table[(g_adDataLSteam & 0x0f00) >> 8]);
					DIGITAL_SECOND_NUM(Disp_Table[(g_adDataLSteam & 0x00f0) >> 4]);
					DIGITAL_THIRD_NUM(Disp_Table[(g_adDataLSteam & 0x000f)]);
				break;

				case 9:
					if(g_LidFlag == LID_CLOSE)							//�ϸ�ʱ��ʾGGy������ʱ��ʾGGn
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

				case 10:												//��ʾSr
					DIGITAL_SECOND_NUM(DISPDATA_S);
					DIGITAL_THIRD_NUM(DISPDATA_r);
				break;

				case 11:
					if(g_pumpWorkFlag)									//��⵽ˮ�ù�������ʾOn
					{
						DIGITAL_FIRST_NUM(DISPDATA_S);
						DIGITAL_SECOND_NUM(DISPDATA_b);
						DIGITAL_THIRD_NUM(DISPDATA_SG);
					}
					else												//δ��⵽ˮ�ù�������ʾErr
					{
						if(g_pumpDelayJudgeFlag)						//��⵽��ˮ�ù���������ʱ�ж�ʱ�䵽
						{
							DIGITAL_FIRST_NUM(DISPDATA_E);
							DIGITAL_SECOND_NUM(DISPDATA_r);
							DIGITAL_THIRD_NUM(DISPDATA_r);
						}
					}
				break;

				case 12:												//��ʾSN��
					DIGITAL_FIRST_NUM(Disp_Table[RECV_SN/100]);
					DIGITAL_SECOND_NUM(Disp_Table[RECV_SN%100/10]);
					DIGITAL_THIRD_NUM(Disp_Table[RECV_SN%100%10]);
				break;

				case 13:												//ȫϢ
					
				break;
			}
			break;
			
		case SysModeDebug:												//Debug״̬
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
			
		case SysModePID:												//PID״̬
			if(g_pidDispStep == 1)										//�������ʾPID��
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
			
		case SysModeClean:												//���ˮ��״̬
			if((g_cleanIncrustantStep == 1) || (g_cleanIncrustantStep == 2))		//�������ʾSP1��
			{
				DIGITAL_FIRST_NUM(DISPDATA_S);
				DIGITAL_SECOND_NUM(DISPDATA_P);
				DIGITAL_THIRD_NUM(DISPDATA_1);
			}
			else if(g_cleanIncrustantStep == 3)							//�������ʾSP2��
			{
				DIGITAL_FIRST_NUM(DISPDATA_S);
				DIGITAL_SECOND_NUM(DISPDATA_P);
				DIGITAL_THIRD_NUM(DISPDATA_2);
			}
			break;
	}

	/*Wifiͼ�����*/
	wifiStatus = ReadWifiInf(WIFI_STATUS);

	if(wifiStatus == TRY_CONNET_ROUTER)				//WIFIģ�鴦��δ��������������״̬
	{
		if(g_dispFlashFlag == 0)
		{
			DP = 1;			//500ms��˸
		}
	}
	else if((wifiStatus == SMART_LINK_OK_WAIT) || (wifiStatus == SOFT_AP_STATUS))
	{
		if(g_dispQuickFlashFlag)
		{
			DP = 1;			//100ms��˸
		}
	}
	else if((wifiStatus == CAN_NOT_CONNET_SERVER) || (wifiStatus == CONNET_ROUTER_OK))
	{
		if(g_dispFlashFlag == 0)
		{
			DP = 1;			//500ms��˸
		}
	}
	else if(wifiStatus == CONNET_SERVER_OK)			//WIFIģ�������ӷ�����
	{
		DP = 1;				//ȫ��
	}
}