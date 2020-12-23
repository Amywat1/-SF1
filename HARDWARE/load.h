#ifndef LOAD_H
#define LOAD_H

#define	P_CAMERA_FAN			P2_4
#define	P_CAMERA_LED			P2_5

#define HEAT_JUDGE_TIME			8						//250msʱ���������жϼ��ʱ��
#define HEAT_JUDGE_TIME_LONG	16						//250msʱ���������жϼ��ʱ��
#define PRE_DEALY_TIME			40						//250msʱ����Ԥ�ȵȴ�ʱ��

#define PLATE_GEAR_2_20			2						//��¯��λ2/20
#define PLATE_GEAR_4_20			4						//��¯��λ4/20
#define PLATE_GEAR_6_20			6						//��¯��λ6/20
#define PLATE_GEAR_8_20			8						//��¯��λ8/20
#define PLATE_GEAR_10_20		10						//��¯��λ10/20
#define PLATE_GEAR_12_20		12						//��¯��λ12/20
#define PLATE_GEAR_14_20		14						//��¯��λ14/20
#define PLATE_GEAR_16_20		16						//��¯��λ16/20
#define PLATE_GEAR_18_20		18						//��¯��λ18/20
#define PLATE_GEAR_MAX			20						//��¯���λ

#define PLATE_HIGH_GEAR			15						//��¯�߹��ʵ�λ
#define PLATE_MID_GEAR			11						//��¯�й��ʵ�λ
#define PLATE_LOW_GEAR			7						//��¯�͹��ʵ�λ

#define STEAM_PRE_HEAT_AD		0x03A2					//��¯Ԥ���¶ȵ㣨110�棩
#define STEAM_LOW_TEMP_AD		0x0385					//��¯���µ�	��120����ٽ���ˮ�㣩
#define STEAM_MAX_TEMP_AD		0x0259					//��¯����¶ȵ㣨185��ǿ���¶ȵ㣩
#define CORE_MAX_TEMP_AD		0x014A					//ǻ������¶ȵ㣨240��ǿ���¶ȵ㣩

#define PUMP_JUDGE_TIME			500						//10msʱ����ˮ�ü�ˮƵ���жϼ��ʱ��
#define PUMP_GEAR_MAX			250						//��ˮƵ�ʵ����λ
#define PUMP_GEAR_MIN			35						//��ˮƵ�ʵ���С��λ
#define QUICK_PUMPING			15						//����ȱˮʱˮ�ÿ�鵵λ
#define PUMP_CHECK_GEAR			25						//�����Լ첽��ĳ�ˮƵ�ʣ�10����Ϊһֱ��ˮ��
#define PUMP_CONTINUE_GEAR		9						//ˮ�ó�����ˮƵ��	��10����Ϊһֱ��ˮ��

#define OPEN_LOAD				1						//��������
#define CLOSE_LOAD				0						//�رո���

typedef struct
{
	unsigned int				tubeHeatTempOn;			//±�عܿ�����
	unsigned int				tubeHeatTempOff;		//±�عܹرյ�
	unsigned int				steamHeatTempOn;		//�����̿�����
	unsigned int				steamHeatTempOff;		//�����̹رյ�
	unsigned int				corepreHeatTemp;		//�����¶�Ԥ�ȵ�
	unsigned int				coreNowAd;				//��ǰ�����¶�ADֵ
	unsigned int				coreLastAd;				//�ϴ������¶�ADֵ
	unsigned int				SteamLastAd;			//�ϴι�¯�¶�ADֵ
	unsigned char				preHeatDelayCnt;		//Ԥ����ʱ����
	unsigned char				heatJudgeCycCnt;		//�����ж����ڼ���
	unsigned int				pumpJudgeCycCnt;		//ˮ�ó�ˮƵ���ж����ڼ���
	unsigned int				forceSubWaterCnt;		//ǿ�Ƽ��ٳ�ˮ��ʱ�����
	unsigned char				plateHeatGear;			//��¯�ļ��ȵ�λ
	unsigned char				pumpFreGear;			//ˮ�õ�Ƶ�ʵ�λ
}LOAD_CRL_MESSAGE;

extern bit						g_steamPreHeatFlag;		//��¯�¶�Ԥ�ȱ�־

extern bit						g_corePreHeatFlag;		//�����¶�Ԥ�ȱ�־
extern bit						g_corePreHeatDelayFlag;	//Ԥ����ʱ��־
extern bit						g_corePreHeatFirstFlag;	//�״�Ԥ�ȱ�־

extern LOAD_CRL_MESSAGE xdata	loadCrlData;			//���ؿ��Ƶ�����

extern bit						g_LedOpenFlag;				//�����ƿ�����־

void InitLoadIo(void);
void InitLoadVariable(void);
void LoadHeatCrlData(void);
void TubeInterimHeat(unsigned char funCnt, unsigned char val, unsigned char T);
void CloseAllOutPut(void);
void HeatCrl(void);
void OnlyBakeHeatCrl(void);
void OnlySteamHeatCrl(void);
void SteamBakeHeatCrl(void);
void OnlyBakePreHeatCrl(void);
void OnlyBakeRegionHeatCrl(void);
void OnlySteamPreHeatCrl(void);
void OnlySteamRegionHeatCrl(void);
void PumpCrl(void);
void LoadMessageDeliver(void);
void LoadCrl(void);
#endif