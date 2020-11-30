#ifndef MEASURE_H
#define MEASURE_H

#define		P_CORE_NTC_IO		P4_3
#define		P_CORE_NTC_AD		P4_2
#define		P_LID				P5_0

#define		CORE_NTC_IO_OUT		{(P4CR |= BIT3);(P4PCR &= ~BIT3);}
#define		CORE_NTC_IO_IN		{(P4CR &= ~BIT3);(P4PCR &= ~BIT3);}

#define		PD_VALUE						0x02B0		//掉电重新加载的AD阈值（15min左右）

#define		REGION_JUDEG_TIME				7000		//锅炉温度在控温区间的标定判定时间		10ms时基
#define		STOP_HEAT_JUDGE_TIME			3500		//锅炉温度超过强关点后的标定判定时间	10ms时基
#define		HIGH_TEMP_TIME_MAX				5000		//锅炉温度在强关点以上时间				10ms时基
#define		EN_ERROR_VALUE					30			//水泵AD值判断的误差范围(40/1024)*5 = 0.2V
#define		EN_LOW_ERROR_VALUE				20			//水泵标定判定可能少水时所使用的缺水判定阈值

#define 	PD_WAIT_TIME					100			//掉电，上电后的延时时间

#define		NTC_OPEN_DATA					0x03FC		//9.3M（-40℃）	低温区AD  (做开路报警，低温区)
#define		NTC_SHORT_DATA					0x00E3		//276Ω（270℃）高温区AD  (做短路报警，高温区)

#define		SELF_CHECK_AD_HMAX				0x03A2		//自检时高温区允许最大AD值（工装接10K电阻）
#define		SELF_CHECK_AD_HMIN				0x0395		//自检时高温区允许最小AD值（工装接10K电阻）
#define		SELF_CHECK_AD_LMAX				0x0107		//自检时低温区允许最大AD值（工装接10K电阻）
#define		SELF_CHECK_AD_LMIN				0x00F9		//自检时低温区允许最小AD值（工装接10K电阻）

#define		SELF_CHECK_PD_AD				0x03C0		//自检时掉电检测口的最小AD值


/*科敏4537芝浦芯片NTC，200℃时，R=1kΩ±2.5%*/
//-------------------------------------------------------使用低温区进行控制
//零下温度
#define		TEMP_LOW_N40					0x03FC
#define		TEMP_LOW_N35					0x03FB
#define		TEMP_LOW_N30					0x03F9
#define		TEMP_LOW_N25					0x03F7
#define		TEMP_LOW_N20					0x03F4
#define		TEMP_LOW_N15					0x03EF
#define		TEMP_LOW_N10					0x03EA
#define		TEMP_LOW_N05					0x03E3
#define		TEMP_LOW_N00					0x03DB

//001-010
#define		TEMP_LOW_001 					0x03D9
#define		TEMP_LOW_002					0x03D7
#define		TEMP_LOW_003					0x03D5
#define		TEMP_LOW_004					0x03D2
#define		TEMP_LOW_005					0x03D0
#define		TEMP_LOW_006					0x03CE
#define		TEMP_LOW_007					0x03CB
#define		TEMP_LOW_008					0x03C9
#define		TEMP_LOW_009					0x03C6
#define		TEMP_LOW_010					0x03C3

//011-020
#define		TEMP_LOW_011					0x03C0
#define		TEMP_LOW_012					0x03BD
#define		TEMP_LOW_013					0x03BA
#define		TEMP_LOW_014					0x03B7
#define		TEMP_LOW_015					0x03B3
#define		TEMP_LOW_016					0x03B0
#define		TEMP_LOW_017					0x03AC
#define		TEMP_LOW_018					0x03A8
#define		TEMP_LOW_019					0x03A4
#define		TEMP_LOW_020					0x03A0

//021-030
#define		TEMP_LOW_021					0x039C
#define		TEMP_LOW_022					0x0398
#define		TEMP_LOW_023					0x0393
#define		TEMP_LOW_024					0x038F
#define		TEMP_LOW_025					0x038A
#define		TEMP_LOW_026					0x0385
#define		TEMP_LOW_027					0x0380
#define		TEMP_LOW_028					0x037B
#define		TEMP_LOW_029					0x0375
#define		TEMP_LOW_030					0x0370

//031-040
#define		TEMP_LOW_031					0x036A
#define		TEMP_LOW_032					0x0365
#define		TEMP_LOW_033					0x035F
#define		TEMP_LOW_034					0x0359
#define		TEMP_LOW_035					0x0352
#define		TEMP_LOW_036					0x034C
#define		TEMP_LOW_037					0x0345
#define		TEMP_LOW_038					0x033F
#define		TEMP_LOW_039					0x0338
#define		TEMP_LOW_040					0x0331

//041-050
#define		TEMP_LOW_041					0x032A
#define		TEMP_LOW_042					0x0322
#define		TEMP_LOW_043					0x031B
#define		TEMP_LOW_044					0x0314
#define		TEMP_LOW_045					0x030C
#define		TEMP_LOW_046					0x0304
#define		TEMP_LOW_047					0x02FC
#define		TEMP_LOW_048					0x02F4
#define		TEMP_LOW_049					0x02EC
#define		TEMP_LOW_050					0x02E4

//051-060
#define		TEMP_LOW_051					0x02DB
#define		TEMP_LOW_052					0x02D3
#define		TEMP_LOW_053					0x02CA
#define		TEMP_LOW_054					0x02C2
#define		TEMP_LOW_055					0x02B9
#define		TEMP_LOW_056					0x02B0
#define		TEMP_LOW_057					0x02A7
#define		TEMP_LOW_058					0x029E
#define		TEMP_LOW_059					0x0295
#define		TEMP_LOW_060					0x028C

//061-070
#define		TEMP_LOW_061					0x0283
#define		TEMP_LOW_062					0x027A
#define		TEMP_LOW_063					0x0271
#define		TEMP_LOW_064					0x0268
#define		TEMP_LOW_065					0x025E
#define		TEMP_LOW_066					0x0255
#define		TEMP_LOW_067					0x024C
#define		TEMP_LOW_068					0x0242
#define		TEMP_LOW_069					0x0239
#define		TEMP_LOW_070					0x0230

//071-080
#define		TEMP_LOW_071					0x0227
#define		TEMP_LOW_072					0x021D
#define		TEMP_LOW_073					0x0214
#define		TEMP_LOW_074					0x020B
#define		TEMP_LOW_075					0x0202
#define		TEMP_LOW_076					0x01F9
#define		TEMP_LOW_077					0x01F0
#define		TEMP_LOW_078					0x01E7
#define		TEMP_LOW_079					0x01DE
#define		TEMP_LOW_080					0x01D5

//081-090
#define		TEMP_LOW_081					0x01CC
#define		TEMP_LOW_082					0x01C3
#define		TEMP_LOW_083					0x01BB
#define		TEMP_LOW_084					0x01B2
#define		TEMP_LOW_085					0x01A9
#define		TEMP_LOW_086					0x01A1
#define		TEMP_LOW_087					0x0199
#define		TEMP_LOW_088					0x0191
#define		TEMP_LOW_089					0x0188
#define		TEMP_LOW_090					0x0180

//091-100
#define		TEMP_LOW_091					0x0178
#define		TEMP_LOW_092					0x0171
#define		TEMP_LOW_093					0x0169
#define		TEMP_LOW_094					0x0161
#define		TEMP_LOW_095					0x015A
#define		TEMP_LOW_096					0x0152
#define		TEMP_LOW_097					0x014B
#define		TEMP_LOW_098					0x0144
#define		TEMP_LOW_099					0x013D
#define		TEMP_LOW_100					0x0136

//101-110
#define		TEMP_LOW_101					0x012F
#define		TEMP_LOW_102					0x0129
#define		TEMP_LOW_103					0x0122
#define		TEMP_LOW_104					0x011C
#define		TEMP_LOW_105					0x0115
#define		TEMP_LOW_106					0x010F
#define		TEMP_LOW_107					0x0109
#define		TEMP_LOW_108					0x0103
#define		TEMP_LOW_109					0x00FD
#define		TEMP_LOW_110					0x00F7

//111-120
#define		TEMP_LOW_111					0x00F2
#define		TEMP_LOW_112					0x00EC
#define		TEMP_LOW_113					0x00E7
#define		TEMP_LOW_114					0x00E2
#define		TEMP_LOW_115					0x00DC
#define		TEMP_LOW_116					0x00D7
#define		TEMP_LOW_117					0x00D2
#define		TEMP_LOW_118					0x00CE
#define		TEMP_LOW_119					0x00C9
#define		TEMP_LOW_120					0x00C4

//121-130
#define		TEMP_LOW_121					0x00C0
#define		TEMP_LOW_122					0x00BB
#define		TEMP_LOW_123					0x00B7
#define		TEMP_LOW_124					0x00B3
#define		TEMP_LOW_125					0x00AE
#define		TEMP_LOW_126					0x00AA
#define		TEMP_LOW_127					0x00A6
#define		TEMP_LOW_128					0x00A3
#define		TEMP_LOW_129					0x009F
#define		TEMP_LOW_130					0x009B

//131-140
#define		TEMP_LOW_131					0x0097
#define		TEMP_LOW_132					0x0094
#define		TEMP_LOW_133					0x0090
#define		TEMP_LOW_134					0x008D
#define		TEMP_LOW_135					0x008A
#define		TEMP_LOW_136					0x0087
#define		TEMP_LOW_137					0x0084
#define		TEMP_LOW_138					0x0080
#define		TEMP_LOW_139					0x007D
#define		TEMP_LOW_140					0x007B

//-------------------------------------------------------使用高温区进行控制

//131-140
#define		TEMP_HIG_131					0x8360
#define		TEMP_HIG_132					0x835C
#define		TEMP_HIG_133					0x8358
#define		TEMP_HIG_134					0x8354
#define		TEMP_HIG_135					0x8350
#define		TEMP_HIG_136					0x834C
#define		TEMP_HIG_137					0x8348
#define		TEMP_HIG_138					0x8344
#define		TEMP_HIG_139					0x8340
#define		TEMP_HIG_140					0x833C

//141-150
#define		TEMP_HIG_141					0x8338
#define		TEMP_HIG_142					0x8333
#define		TEMP_HIG_143					0x832F
#define		TEMP_HIG_144					0x832B
#define		TEMP_HIG_145					0x8326
#define		TEMP_HIG_146					0x8322
#define		TEMP_HIG_147					0x831D
#define		TEMP_HIG_148					0x8319
#define		TEMP_HIG_149					0x8314
#define		TEMP_HIG_150					0x830F

//151-160
#define		TEMP_HIG_151					0x830B
#define		TEMP_HIG_152					0x8306
#define		TEMP_HIG_153					0x8301
#define		TEMP_HIG_154					0x82FC
#define		TEMP_HIG_155					0x82F8
#define		TEMP_HIG_156					0x82F3
#define		TEMP_HIG_157					0x82EE
#define		TEMP_HIG_158					0x82E9
#define		TEMP_HIG_159					0x82E4
#define		TEMP_HIG_160					0x82DF

//161-170
#define		TEMP_HIG_161					0x82DA
#define		TEMP_HIG_162					0x82D5
#define		TEMP_HIG_163					0x82CF
#define		TEMP_HIG_164					0x82CA
#define		TEMP_HIG_165					0x82C5
#define		TEMP_HIG_166					0x82C0
#define		TEMP_HIG_167					0x82BB
#define		TEMP_HIG_168					0x82B5
#define		TEMP_HIG_169					0x82B0
#define		TEMP_HIG_170					0x82AB

//171-180
#define		TEMP_HIG_171					0x82A5
#define		TEMP_HIG_172					0x82A0
#define		TEMP_HIG_173					0x829B
#define		TEMP_HIG_174					0x8295
#define		TEMP_HIG_175					0x8290
#define		TEMP_HIG_176					0x828B
#define		TEMP_HIG_177					0x8285
#define		TEMP_HIG_178					0x8280
#define		TEMP_HIG_179					0x827A
#define		TEMP_HIG_180					0x8275

//181-190
#define		TEMP_HIG_181					0x826F
#define		TEMP_HIG_182					0x826A
#define		TEMP_HIG_183					0x8264
#define		TEMP_HIG_184					0x825F
#define		TEMP_HIG_185					0x8259
#define		TEMP_HIG_186					0x8254
#define		TEMP_HIG_187					0x824E
#define		TEMP_HIG_188					0x8249
#define		TEMP_HIG_189					0x8243
#define		TEMP_HIG_190					0x823E

//191-200
#define		TEMP_HIG_191					0x8238
#define		TEMP_HIG_192					0x8233
#define		TEMP_HIG_193					0x822E
#define		TEMP_HIG_194					0x8228
#define		TEMP_HIG_195					0x8223
#define		TEMP_HIG_196					0x821D
#define		TEMP_HIG_197					0x8218
#define		TEMP_HIG_198					0x8213
#define		TEMP_HIG_199					0x820D
#define		TEMP_HIG_200					0x8208

//201-210
#define		TEMP_HIG_201					0x8203
#define		TEMP_HIG_202					0x81FD
#define		TEMP_HIG_203					0x81F8
#define		TEMP_HIG_204					0x81F3
#define		TEMP_HIG_205					0x81EE
#define		TEMP_HIG_206					0x81E8
#define		TEMP_HIG_207					0x81E3
#define		TEMP_HIG_208					0x81DE
#define		TEMP_HIG_209					0x81D9
#define		TEMP_HIG_210					0x81D4

//211-220
#define		TEMP_HIG_211					0x81CF
#define		TEMP_HIG_212					0x81CA
#define		TEMP_HIG_213					0x81C5
#define		TEMP_HIG_214					0x81C0
#define		TEMP_HIG_215					0x81BB
#define		TEMP_HIG_216					0x81B6
#define		TEMP_HIG_217					0x81B1
#define		TEMP_HIG_218					0x81AC
#define		TEMP_HIG_219					0x81A7
#define		TEMP_HIG_220					0x81A2

typedef struct
{
	unsigned int  ntcAd_buff[10];						//AD采样的缓存值
	unsigned int  coreAdHValue;							//中心温度高温AD均值
	unsigned int  coreAdLValue;							//中心温度低温AD均值
	unsigned char adcnt;								//AD采样次数
	unsigned char errCode;								//NTC的错误代码
}MEASURE_DATA;

typedef struct
{
	unsigned char smoothingCnt;							//锅炉降温AD值滤波确认
	unsigned int  regionHeatCnt;						//锅炉在正常控温区间计时
	unsigned int  stopHeatCnt;							//锅炉到达强制关闭点后计时
	unsigned int  highTempTimeCnt;						//锅炉温度在强关点以上时间计数
	unsigned int  steamMinAd;							//锅炉最小AD值
	unsigned int  pumpWorkAdTemp;						//水泵工作电流AD的缓存值
	unsigned int  pumpWorkAdBuff[5];					//水泵工作电流AD的值
	unsigned int  pumpNormalWorkAd;						//水泵抽到水时的标定AD值
}CALIBRATION_DATA;

extern MEASURE_DATA  xdata 			measureData;
extern CALIBRATION_DATA	xdata		calibrationData;

extern unsigned int  xdata			g_adDataHSteam;		//锅炉高温区AD值
extern unsigned int  xdata			g_adDataLSteam;		//锅炉低温区AD值

extern unsigned int  xdata 			g_pumpCurrentAd;	//水泵电流采样AD值
extern unsigned int  xdata 			g_pumpNormalWorkAd;	//水泵正常工作电流AD值
extern unsigned char xdata 			g_pumpDelayJudgeCnt;//考虑进水管长度，延迟判缺水

extern unsigned int  xdata			g_lastPumpNormalWorkAd;		//上次记录的水泵正常抽水电流值

extern 			bit					g_reachStopHeatFlag;//是否到达过强关点标志
extern 			bit					g_reachRegionTempFlag;		//是否到达过区间温度标志

extern 			bit					g_firstAdGetGlag;			//采集到首次腔体AD值标志

void InitMeasureIo(void);
void InitMeasureVariable(void);
void InitAd(void);
unsigned int AdScanValue(void);
void NtcAdScan(void);
void LidScan(void);
void InitPumpCalibrationData(void);
void PumpCurrentCalibration(void);
void LackWaterScan(void);
void ErrorCheck(void);
void MeasureDataScan(void);
#endif