#ifndef SYSMODE_H
#define SYSMODE_H

#define POWER_UP_TIME				2					//500ms时基，上电1秒
#define POWER_UP_FIFTEEN_SECONDS	30					//500ms时基，上电15秒
#define SYS_SELFCHECK_WAIT_TIME		30					//500ms时基，15S
#define SYS_PID_WAIT_TIME			24					//500ms时基，12S
#define PID_STEP1_TIME				4					//500ms时基，2S，PID的第一步时间
#define PID_STEP2_TIME				8					//500ms时基，4S，PID的第二步时间
#define PID_STEP3_TIME				12					//500ms时基，6S，PID的第三步时间
#define PID_STEP4_TIME				16					//500ms时基，8S，PID的第四步时间
#define PID_STEP5_TIME				20					//500ms时基，10S，PID的第五步时间
#define SYS_STANDBY_WAIT_TIME		600					//500ms实际，5min
#define SYS_SELECT_WAIT_TIME		240					//500ms实际，2min
#define SYS_ORDERSET_WAIT_TIME		240					//500ms实际，2min
#define SYS_END_WAIT_TIME			600					//500ms实际，5min，工作结束等待时间
#define ONE_MINUTE_CNT				120					//500ms时基，1min
#define ADJ_WAIT_TIME				10					//500ms时基，温度/时间调节等待时间
#define COOLING_DELAY_TIME			1200				//500ms时基，散热风扇延时关闭时间
#define HIGH_POWER_MAX_TIME			1800				//500ms时基，纯蒸工艺高功率输出时间计时
#define LID_OPEN_WORK_TIME			600					//500ms时基，工作和保温状态下，开盖时间
	
#define CLEAN_TIME_STEP_1			2400				//500ms时基，水泵清洁程序，步骤1时间,20min
#define CLEAN_TIME_STEP_2			240					//500ms时基，水泵清洁程序，步骤2时间,2min
#define CLEAN_TIME_STEP_3			360					//500ms时基，水泵清洁程序，步骤3时间,3min

#define DISP_FLASH_FRE				50					//10ms时基，显示的闪烁频率

#define ORDER_TIME_MAX          	1440				//预约最长时间

#define TECHNOLOGY_STEP_1_NUM		0                 	//工艺步骤1所在表格列数
#define STEP_1_TEMP_NUM				1                 	//步骤1默认温度所在表格列数
#define STEP_1_TIME_NUM				2                 	//步骤1默认温度所在表格列数
#define TECHNOLOGY_STEP_2_NUM		3                 	//工艺步骤2所在表格列数
#define STEP_2_TEMP_NUM				4                 	//步骤2默认温度所在表格列数
#define STEP_2_TIME_NUM				5                 	//步骤2默认温度所在表格列数
#define TECHNOLOGY_STEP_3_NUM		6                 	//工艺步骤3所在表格列数
#define STEP_3_TEMP_NUM				7                 	//步骤3默认温度所在表格列数
#define STEP_3_TIME_NUM				8                 	//步骤3默认温度所在表格列数
#define TECHNOLOGY_STEP_4_NUM		9                 	//工艺步骤4所在表格列数
#define STEP_4_TEMP_NUM				10                 	//步骤4默认温度所在表格列数
#define STEP_4_TIME_NUM				11                 	//步骤4默认温度所在表格列数
#define TECHNOLOGY_WARM_NUM			12                 	//保温工艺所在表格列数
#define KEEP_WARM_TEMP_NUM			13                 	//保温温度所在表格列数
#define KEEP_WARM_TIME_NUM			14                 	//保温时间所在表格列数
#define ADJ_MIN_TEMP_NUM			15                 	//温度调节的最小值所在表格列数
#define ADJ_MAX_TEMP_NUM			16                 	//温度调节的最大值所在表格列数
#define ADJ_MIN_TIME_NUM			17                 	//时间调节的最小值所在表格列数
#define ADJ_MAX_TIME_NUM			18                 	//时间调节的最大值所在表格列数
#define TEMP_ADJ_EN_NUM				19                 	//温度可调标志所在表格列数
#define TIME_ADJ_EN_NUM				20                 	//时间可调标志所在表格列数
#define ORDER_EN_NUM				21                 	//预约有效标志所在表格列数

#define MENU_1_NUM				    0                 	//菜单1所在表格行数
#define MENU_2_NUM				    1                 	//菜单2所在表格行数
#define MENU_3_NUM				    2                 	//菜单3所在表格行数
#define MENU_4_NUM				    3                 	//菜单4所在表格行数
#define MENU_5_NUM				    4                 	//菜单5所在表格行数
#define MENU_6_NUM				    5                 	//菜单6所在表格行数
	
#define NULL_STEP					0					//此步骤无工艺
#define ONLY_BAKE					1					//单烤工艺
#define BAKE_STEAM					2					//蒸烤工艺
#define ONLY_STEAM					3					//单蒸工艺

#define STATE_YES 					0X88				//状态正常	（有过零、水泵有新一次抽水）
#define STATE_NO					0X77				//状态异常	（无过零、水泵无新一次抽水）
#define LID_CLOSE					1					//合盖状态
#define LID_OPEN					0					//开盖状态

#define EN_ADJ                      1                   //可调
#define NO_ADJ                      0                   //不可调

#define DEFAULT_MENU                MENU_4_NUM          //初始默认菜单

/*枚举各菜单的编号（不允许修改值）*/
typedef enum {
	MENU_DIY_BAKING					= 0x1000,			//自主烘焙
	MENU_JI_CHI						= 0x0006,			//鸡翅
	MENU_DAN_TA						= 0x0005,			//蛋挞
	MENU_HONG_SHU					= 0x0004,			//红薯
	MENU_KAO_CHUAN					= 0x0016,			//烤串
	MENU_DAN_GAO					= 0x0007,			//蛋糕
	MENU_YU_MI						= 0x000D,			//玉米
	MENU_KAO_YU						= 0x0015,			//烤鱼
	MENU_QU_QI						= 0x0008,			//曲奇
	MENU_QIE_ZI						= 0x0013,			//茄子
	MENU_KAO_XIA					= 0x0014,			//烤虾
	MENU_MIAN_BAO					= 0x0012,			//面包
	MENU_BAN_LI						= 0x000F,			//板栗
	MENU_NIU_PAI					= 0x000B,			//牛排
	MENU_PI_SA						= 0x0009,			//披萨
	MENU_OTHER_TU_SI				= 0x0001,			//其它——吐司片
	MENU_OTHER_HUA_SHENG			= 0x0002,			//其它——花生
	MENU_OTHER_TU_DOU				= 0x0003,			//其它——土豆
	MENU_OTHER_PAI_GU				= 0x000A,			//其它——排骨
	MENU_OTHER_KAO_JI				= 0x000C,			//其它——烤鸡
	MENU_OTHER_YU_TOU				= 0x000E,			//其它——芋头
	MENU_OTHER_MAN_TOU				= 0x0010,			//其它——小馒头
	MENU_OTHER_JI_TUI				= 0x0011,			//其它——鸡腿
	MENU_OTHER_KAO_CHANG			= 0x0017,			//其它——烤肠
	MENU_OTHER_CANG_JU				= 0x0018,			//其它——餐具
	MENU_OTHER_NULL					= 0x0FFF,			//其它——空
	MENU_OTHER_UNKNOW				= 0x0FFE,			//其它——不认识

}MENU_NUM_TypeDef;


enum _SYS_Type_{                	           
    SysModePowerUp					= 0,				//上电状态
    SysModeStandby,				                        //待机状态
    SysModeDarkScreen,				                    //黑屏状态
	SYS_MODE_SMART_IDENTIFICATION,						//智能识别状态
	SYS_MODE_AUTO_ERR, 									//智能识别失败状态
	SysModeSelect,			                        	//设置待机状态
	SysModeOrderSet,				                    //预约设置状态
	SYS_MODE_PRE_HEAT,									//预热状态
	SYS_MODE_PRE_HEAT_KEEP,								//预热保持状态
	SysModeWork,				                        //工作状态
	SYS_MODE_SMART_WORKING,								//智能烘焙工作状态
	SysModeEnd,				                            //工作结束状态
	SysModeError,				                        //报警状态


    SysModeOrderWork,				                    //预约工作状态
    SysModeWarm	,				                        //保温状态
    SysModePCBCheck,				                    //PCB自检状态
	SysModeMachineCheck,			                    //整机自检状态
	SysModeDebug,				                        //Debug状态
    SysModePID,			        	                    //PID状态
	SysModeClean,				                        //除水垢状态
};
	
enum _FUN_Type_{                	           
    NoFunction						= 0,				//无功能
    FunOnlyBake						= 1,				//空气炸功能
    FunBakeSteam					= 2,				//蒸汽烤功能
    FunOnlySteam					= 3,				//原味蒸功能
};
	
enum _ERROR_Type_{              	             
    NoError							= 0,				//无可继续工作的报警
    NoPotErr						= 1,				//缺锅报警
	LidOpenErr						= 2,				//开盖报警
    LowWaterErr						= 3,				//缺水报警
};	
	
enum _WATER_YIELD_TYPE_{	
    NormalType						= 0,				//正常抽水状态
	MabyLackWater					= 1,				//可能缺水状态
    LackWater						= 2,				//缺水状态
    BlockingWater					= 3,				//堵水状态
};

extern enum _SYS_Type_    		g_sysType;            			//系统状态
extern enum _ERROR_Type_		g_enContinueErr;				//可继续工作报警类型
extern enum _WATER_YIELD_TYPE_	g_waterYieldType;				//水泵出水状态

extern unsigned char xdata		g_menuNumber;					//菜单编号

extern unsigned char xdata		g_zeroType;						//过零状态（有过零，无过零）
extern unsigned char xdata		g_pumpType;						//水泵状态（有新抽水，无新抽水）
extern 			bit				g_LidFlag;						//未合盖标志

extern unsigned int  xdata		g_sysStandbyWaitCnt;			//待机等待时间计数
extern unsigned char xdata		g_sysSelectWaitCnt;				//设置待机等待时间计数
extern unsigned char xdata		g_sysOrderSetWaitCnt;			//预约设置等待时间计数
extern unsigned char xdata		g_sysSelfCheckWaitCnt;			//自检等待时间计数
extern unsigned char xdata		g_adjWaitTimeCnt;				//时间/温度调节等待时间计数
extern unsigned char xdata		g_dispFlashCnt;					//显示闪烁计数
extern unsigned int  xdata		g_cleanPumpTimeCnt;				//水泵清洁程序，步骤时间

extern unsigned char xdata		g_darkScreenTime;				//自检第十步全息时间
extern unsigned char xdata		g_pdWaitTime;					//掉电重新上电后的等待时间

extern unsigned char xdata		g_nowStepworkTemp;				//当前步骤工作温度
extern unsigned char xdata		g_nowStepworkTime;				//当前步骤工作时间
extern unsigned char xdata		g_workTimeAll;					//工作总时间
extern unsigned int  xdata		g_keepWarmTime;					//保温时间
extern unsigned int  xdata		g_orderTime;					//预约时间
extern unsigned int  xdata		g_orderMinTime;					//最小预约时间
extern unsigned char xdata		g_workOneMinCnt;				//1min时间计时
extern unsigned int  xdata		g_highSteamPowerCnt;			//纯蒸工艺下高功率输出时间计时

extern unsigned char xdata		g_nowStepTechnology;			//当前步骤的工艺
extern unsigned char xdata		g_nowStepNum;					//当前工艺步骤数

extern 			bit				g_dispFlashFlag;				//显示闪烁标志位
extern			bit				g_selfCheckEnFlag;				//自检允许标志
extern			bit			    g_timeAdjEnFlag;				//时间可调节标志
extern			bit			    g_tempAdjEnFlag;				//温度可调节标志
extern			bit			    g_orderEnFlag;					//预约有效标志
extern 			bit				g_timeAdjStateFlag;				//时间调节状态标志
extern 			bit				g_tempAdjStateFlag;				//温度调节状态标志
extern			bit				g_pumpCalibrationFlag;			//水泵电流标定标志
extern			bit				g_coolingFanDealyFlag;			//散热风扇延时停止标志
extern			bit				g_steamLowPowerFlag;			//纯蒸的低功率标志
extern			bit			    g_quickPumpingFlag;				//水泵快抽标志（在缺水报警按“启动取消”键继续后快抽）

extern			bit				g_lidSelfCheckOkFlag;			//盖自检合格标志
extern			bit				g_coreNtcSelfCheckOkFlag;		//腔体NTC电路自检合格标志
extern			bit				g_selfCheckPassFlag;			//自检合格标志
extern			bit				g_selfCheckOverFlag;			//显示板自检完毕标志
extern			bit				g_pumpWorkFlag;					//整机自检水泵工作标志
extern			bit				g_pumpDelayJudgeFlag;			//整机自检水泵延时判断标志

extern unsigned char xdata		g_selfCheckStep;				//自检步骤
extern unsigned char xdata		g_pidDispStep;					//PID步骤
extern unsigned char xdata		g_debugDispStep;				//Debug步骤
extern unsigned char xdata		g_cleanIncrustantStep;			//水垢清洁步骤

extern unsigned char code MenuDefaultValue_Table[6][22];

void InitSysModeVariable(void);
void SysModelCrl(void);

#endif