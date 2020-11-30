#ifndef SYSMODE_H
#define SYSMODE_H

#define POWER_UP_TIME				2					//500msʱ�����ϵ�1��
#define POWER_UP_FIFTEEN_SECONDS	30					//500msʱ�����ϵ�15��
#define SYS_SELFCHECK_WAIT_TIME		30					//500msʱ����15S
#define SYS_PID_WAIT_TIME			24					//500msʱ����12S
#define PID_STEP1_TIME				4					//500msʱ����2S��PID�ĵ�һ��ʱ��
#define PID_STEP2_TIME				8					//500msʱ����4S��PID�ĵڶ���ʱ��
#define PID_STEP3_TIME				12					//500msʱ����6S��PID�ĵ�����ʱ��
#define PID_STEP4_TIME				16					//500msʱ����8S��PID�ĵ��Ĳ�ʱ��
#define PID_STEP5_TIME				20					//500msʱ����10S��PID�ĵ��岽ʱ��
#define SYS_STANDBY_WAIT_TIME		600					//500msʵ�ʣ�5min
#define SYS_SELECT_WAIT_TIME		240					//500msʵ�ʣ�2min
#define SYS_ORDERSET_WAIT_TIME		240					//500msʵ�ʣ�2min
#define SYS_END_WAIT_TIME			600					//500msʵ�ʣ�5min�����������ȴ�ʱ��
#define ONE_MINUTE_CNT				120					//500msʱ����1min
#define ADJ_WAIT_TIME				10					//500msʱ�����¶�/ʱ����ڵȴ�ʱ��
#define COOLING_DELAY_TIME			1200				//500msʱ����ɢ�ȷ�����ʱ�ر�ʱ��
#define HIGH_POWER_MAX_TIME			1800				//500msʱ�����������ո߹������ʱ���ʱ
#define LID_OPEN_WORK_TIME			600					//500msʱ���������ͱ���״̬�£�����ʱ��
	
#define CLEAN_TIME_STEP_1			2400				//500msʱ����ˮ�������򣬲���1ʱ��,20min
#define CLEAN_TIME_STEP_2			240					//500msʱ����ˮ�������򣬲���2ʱ��,2min
#define CLEAN_TIME_STEP_3			360					//500msʱ����ˮ�������򣬲���3ʱ��,3min

#define DISP_FLASH_FRE				50					//10msʱ������ʾ����˸Ƶ��

#define ORDER_TIME_MAX          	1440				//ԤԼ�ʱ��

#define TECHNOLOGY_STEP_1_NUM		0                 	//���ղ���1���ڱ������
#define STEP_1_TEMP_NUM				1                 	//����1Ĭ���¶����ڱ������
#define STEP_1_TIME_NUM				2                 	//����1Ĭ���¶����ڱ������
#define TECHNOLOGY_STEP_2_NUM		3                 	//���ղ���2���ڱ������
#define STEP_2_TEMP_NUM				4                 	//����2Ĭ���¶����ڱ������
#define STEP_2_TIME_NUM				5                 	//����2Ĭ���¶����ڱ������
#define TECHNOLOGY_STEP_3_NUM		6                 	//���ղ���3���ڱ������
#define STEP_3_TEMP_NUM				7                 	//����3Ĭ���¶����ڱ������
#define STEP_3_TIME_NUM				8                 	//����3Ĭ���¶����ڱ������
#define TECHNOLOGY_STEP_4_NUM		9                 	//���ղ���4���ڱ������
#define STEP_4_TEMP_NUM				10                 	//����4Ĭ���¶����ڱ������
#define STEP_4_TIME_NUM				11                 	//����4Ĭ���¶����ڱ������
#define TECHNOLOGY_WARM_NUM			12                 	//���¹������ڱ������
#define KEEP_WARM_TEMP_NUM			13                 	//�����¶����ڱ������
#define KEEP_WARM_TIME_NUM			14                 	//����ʱ�����ڱ������
#define ADJ_MIN_TEMP_NUM			15                 	//�¶ȵ��ڵ���Сֵ���ڱ������
#define ADJ_MAX_TEMP_NUM			16                 	//�¶ȵ��ڵ����ֵ���ڱ������
#define ADJ_MIN_TIME_NUM			17                 	//ʱ����ڵ���Сֵ���ڱ������
#define ADJ_MAX_TIME_NUM			18                 	//ʱ����ڵ����ֵ���ڱ������
#define TEMP_ADJ_EN_NUM				19                 	//�¶ȿɵ���־���ڱ������
#define TIME_ADJ_EN_NUM				20                 	//ʱ��ɵ���־���ڱ������
#define ORDER_EN_NUM				21                 	//ԤԼ��Ч��־���ڱ������

#define MENU_1_NUM				    0                 	//�˵�1���ڱ������
#define MENU_2_NUM				    1                 	//�˵�2���ڱ������
#define MENU_3_NUM				    2                 	//�˵�3���ڱ������
#define MENU_4_NUM				    3                 	//�˵�4���ڱ������
#define MENU_5_NUM				    4                 	//�˵�5���ڱ������
#define MENU_6_NUM				    5                 	//�˵�6���ڱ������
	
#define NULL_STEP					0					//�˲����޹���
#define ONLY_BAKE					1					//��������
#define BAKE_STEAM					2					//��������
#define ONLY_STEAM					3					//��������

#define STATE_YES 					0X88				//״̬����	���й��㡢ˮ������һ�γ�ˮ��
#define STATE_NO					0X77				//״̬�쳣	���޹��㡢ˮ������һ�γ�ˮ��
#define LID_CLOSE					1					//�ϸ�״̬
#define LID_OPEN					0					//����״̬

#define EN_ADJ                      1                   //�ɵ�
#define NO_ADJ                      0                   //���ɵ�

#define DEFAULT_MENU                MENU_4_NUM          //��ʼĬ�ϲ˵�
	
enum _SYS_Type_{                	           
    SysModePowerUp					= 0,				//�ϵ�״̬
    SysModeStandby					= 1,				//����״̬
    SysModeSelect					= 2,				//���ô���״̬
    SysModeOrderSet					= 3,				//ԤԼ����״̬
    SysModeOrderWork				= 4,				//ԤԼ����״̬
    SysModeWork						= 5,				//����״̬
    SysModeWarm						= 6,				//����״̬
	SysModeEnd						= 7,				//��������״̬
    SysModeDarkScreen				= 8,				//����״̬
	SysModeError					= 9,				//����״̬
    SysModePCBCheck					= 10,				//PCB�Լ�״̬
	SysModeMachineCheck				= 11,				//�����Լ�״̬
	SysModeDebug					= 12,				//Debug״̬
    SysModePID						= 13,				//PID״̬
	SysModeClean					= 14,				//��ˮ��״̬
};
	
enum _FUN_Type_{                	           
    NoFunction						= 0,				//�޹���
    FunOnlyBake						= 1,				//����ը����
    FunBakeSteam					= 2,				//����������
    FunOnlySteam					= 3,				//ԭζ������
};
	
enum _ERROR_Type_{              	             
    NoError							= 0,				//�޿ɼ��������ı���
    NoPotErr						= 1,				//ȱ������
	LidOpenErr						= 2,				//���Ǳ���
    LowWaterErr						= 3,				//ȱˮ����
};	
	
enum _WATER_YIELD_TYPE_{	
    NormalType						= 0,				//������ˮ״̬
	MabyLackWater					= 1,				//����ȱˮ״̬
    LackWater						= 2,				//ȱˮ״̬
    BlockingWater					= 3,				//��ˮ״̬
};

extern enum _SYS_Type_    		g_sysType;            			//ϵͳ״̬
extern enum _ERROR_Type_		g_enContinueErr;				//�ɼ���������������
extern enum _WATER_YIELD_TYPE_	g_waterYieldType;				//ˮ�ó�ˮ״̬

extern unsigned char xdata		g_menuNumber;					//�˵����

extern unsigned char xdata		g_zeroType;						//����״̬���й��㣬�޹��㣩
extern unsigned char xdata		g_pumpType;						//ˮ��״̬�����³�ˮ�����³�ˮ��
extern 			bit				g_LidFlag;						//δ�ϸǱ�־

extern unsigned int  xdata		g_sysStandbyWaitCnt;			//�����ȴ�ʱ�����
extern unsigned char xdata		g_sysSelectWaitCnt;				//���ô����ȴ�ʱ�����
extern unsigned char xdata		g_sysOrderSetWaitCnt;			//ԤԼ���õȴ�ʱ�����
extern unsigned char xdata		g_sysSelfCheckWaitCnt;			//�Լ�ȴ�ʱ�����
extern unsigned char xdata		g_adjWaitTimeCnt;				//ʱ��/�¶ȵ��ڵȴ�ʱ�����
extern unsigned char xdata		g_dispFlashCnt;					//��ʾ��˸����
extern unsigned int  xdata		g_cleanPumpTimeCnt;				//ˮ�������򣬲���ʱ��

extern unsigned char xdata		g_darkScreenTime;				//�Լ��ʮ��ȫϢʱ��
extern unsigned char xdata		g_pdWaitTime;					//���������ϵ��ĵȴ�ʱ��

extern unsigned char xdata		g_nowStepworkTemp;				//��ǰ���蹤���¶�
extern unsigned char xdata		g_nowStepworkTime;				//��ǰ���蹤��ʱ��
extern unsigned char xdata		g_workTimeAll;					//������ʱ��
extern unsigned int  xdata		g_keepWarmTime;					//����ʱ��
extern unsigned int  xdata		g_orderTime;					//ԤԼʱ��
extern unsigned int  xdata		g_orderMinTime;					//��СԤԼʱ��
extern unsigned char xdata		g_workOneMinCnt;				//1minʱ���ʱ
extern unsigned int  xdata		g_highSteamPowerCnt;			//���������¸߹������ʱ���ʱ

extern unsigned char xdata		g_nowStepTechnology;			//��ǰ����Ĺ���
extern unsigned char xdata		g_nowStepNum;					//��ǰ���ղ�����

extern 			bit				g_dispFlashFlag;				//��ʾ��˸��־λ
extern			bit				g_selfCheckEnFlag;				//�Լ������־
extern			bit			    g_timeAdjEnFlag;				//ʱ��ɵ��ڱ�־
extern			bit			    g_tempAdjEnFlag;				//�¶ȿɵ��ڱ�־
extern			bit			    g_orderEnFlag;					//ԤԼ��Ч��־
extern 			bit				g_timeAdjStateFlag;				//ʱ�����״̬��־
extern 			bit				g_tempAdjStateFlag;				//�¶ȵ���״̬��־
extern			bit				g_pumpCalibrationFlag;			//ˮ�õ����궨��־
extern			bit				g_coolingFanDealyFlag;			//ɢ�ȷ�����ʱֹͣ��־
extern			bit				g_steamLowPowerFlag;			//�����ĵ͹��ʱ�־
extern			bit			    g_quickPumpingFlag;				//ˮ�ÿ���־����ȱˮ������������ȡ�������������飩

extern			bit				g_lidSelfCheckOkFlag;			//���Լ�ϸ��־
extern			bit				g_coreNtcSelfCheckOkFlag;		//ǻ��NTC��·�Լ�ϸ��־
extern			bit				g_selfCheckPassFlag;			//�Լ�ϸ��־
extern			bit				g_selfCheckOverFlag;			//��ʾ���Լ���ϱ�־
extern			bit				g_pumpWorkFlag;					//�����Լ�ˮ�ù�����־
extern			bit				g_pumpDelayJudgeFlag;			//�����Լ�ˮ����ʱ�жϱ�־

extern unsigned char xdata		g_selfCheckStep;				//�Լ첽��
extern unsigned char xdata		g_pidDispStep;					//PID����
extern unsigned char xdata		g_debugDispStep;				//Debug����
extern unsigned char xdata		g_cleanIncrustantStep;			//ˮ����ಽ��

extern unsigned char code MenuDefaultValue_Table[6][22];

void InitSysModeVariable(void);
void SysModelCrl(void);

#endif