#ifndef LOAD_H
#define LOAD_H

#define	P_CAMERA_FAN			P2_4
#define	P_CAMERA_LED			P2_5

#define HEAT_JUDGE_TIME			8						//250ms时基，加热判断间隔时间
#define HEAT_JUDGE_TIME_LONG	16						//250ms时基，加热判断间隔时间
#define PRE_DEALY_TIME			40						//250ms时基，预热等待时间

#define PLATE_GEAR_2_20			2						//锅炉档位2/20
#define PLATE_GEAR_4_20			4						//锅炉档位4/20
#define PLATE_GEAR_6_20			6						//锅炉档位6/20
#define PLATE_GEAR_8_20			8						//锅炉档位8/20
#define PLATE_GEAR_10_20		10						//锅炉档位10/20
#define PLATE_GEAR_12_20		12						//锅炉档位12/20
#define PLATE_GEAR_14_20		14						//锅炉档位14/20
#define PLATE_GEAR_16_20		16						//锅炉档位16/20
#define PLATE_GEAR_18_20		18						//锅炉档位18/20
#define PLATE_GEAR_MAX			20						//锅炉最大档位

#define PLATE_HIGH_GEAR			15						//锅炉高功率档位
#define PLATE_MID_GEAR			11						//锅炉中功率档位
#define PLATE_LOW_GEAR			7						//锅炉低功率档位

#define STEAM_PRE_HEAT_AD		0x03A2					//锅炉预热温度点（110℃）
#define STEAM_LOW_TEMP_AD		0x0385					//锅炉低温点	（120℃加速降抽水点）
#define STEAM_MAX_TEMP_AD		0x0259					//锅炉最高温度点（185℃强关温度点）
#define CORE_MAX_TEMP_AD		0x014A					//腔体最高温度点（240℃强关温度点）

#define PUMP_JUDGE_TIME			500						//10ms时基，水泵加水频率判断间隔时间
#define PUMP_GEAR_MAX			250						//抽水频率的最大档位
#define PUMP_GEAR_MIN			35						//抽水频率的最小档位
#define QUICK_PUMPING			15						//可能缺水时水泵快抽档位
#define PUMP_CHECK_GEAR			25						//整机自检步骤的抽水频率（10以下为一直抽水）
#define PUMP_CONTINUE_GEAR		9						//水泵持续抽水频率	（10以下为一直抽水）

#define OPEN_LOAD				1						//开启负载
#define CLOSE_LOAD				0						//关闭负载

typedef struct
{
	unsigned int				tubeHeatTempOn;			//卤素管开启点
	unsigned int				tubeHeatTempOff;		//卤素管关闭点
	unsigned int				steamHeatTempOn;		//蒸发盘开启点
	unsigned int				steamHeatTempOff;		//蒸发盘关闭点
	unsigned int				corepreHeatTemp;		//中心温度预热点
	unsigned int				coreNowAd;				//当前中心温度AD值
	unsigned int				coreLastAd;				//上次中心温度AD值
	unsigned int				SteamLastAd;			//上次锅炉温度AD值
	unsigned char				preHeatDelayCnt;		//预热延时计数
	unsigned char				heatJudgeCycCnt;		//加热判断周期计数
	unsigned int				pumpJudgeCycCnt;		//水泵抽水频率判断周期计数
	unsigned int				forceSubWaterCnt;		//强制减少抽水量时间计数
	unsigned char				plateHeatGear;			//锅炉的加热档位
	unsigned char				pumpFreGear;			//水泵的频率档位
}LOAD_CRL_MESSAGE;

extern bit						g_steamPreHeatFlag;		//锅炉温度预热标志

extern bit						g_corePreHeatFlag;		//中心温度预热标志
extern bit						g_corePreHeatDelayFlag;	//预热延时标志
extern bit						g_corePreHeatFirstFlag;	//首次预热标志

extern LOAD_CRL_MESSAGE xdata	loadCrlData;			//负载控制的数据

extern bit						g_LedOpenFlag;				//照明灯开启标志

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