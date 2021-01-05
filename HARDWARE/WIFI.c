#include "common_sino_wealth.h"
#include "header.h"

#define N_TXD					P0_0
#define N_RXD					P2_7

#define HEADER_CODE_H			0XFA						//指令头码高8位
#define HEADER_CODE_L			0XFB						//指令头码低8位
#define CONTRACT_VERSION		0X00						//通讯版本
#define CONTRACT_FRAME_VALUE	0X00						//通讯帧总数（在数据包长度比较长是，分包发送时用到，例如一个数据包很长需要分三帧数据才能发送完，那么在发送这三帧数据是该字节为 3）
#define CONTRACT_FRAME_NUM		0X00						//通讯帧序号

#define KX_MODEL_H				0x94						//设备型号
#define KX_MODEL_L				0xFC						//设备型号
#define PROGRAM_VERSION			0x00						//设备程序版本

#define FRAME_LENGTH_MAX		90							//帧的最大长度
#define CMD_NUM_BUFF_MAX		16							//最多的缓存命令个数

#define ACK_TIMER_MAX			50							//10ms时基，应答最长时间
#define RECV_OUT_TIMER			100							//10ms时基，数据接收超时时间
#define SEND_INTERVAL_TIMER 	16							//10ms时基，发送的间隔时间	（1/9600（波特率）* 100（数据个数）* 10（一次发10位）=104ms）

#define SEND_HEADER_H			mcuWifi.sendInf.u8Send_Buff[0]	//发送的数据帧头（高8位）
#define SEND_HEADER_L			mcuWifi.sendInf.u8Send_Buff[1]	//发送的数据帧头（低8位）
#define SEND_CONTRACT_VERSION	mcuWifi.sendInf.u8Send_Buff[2]	//发送的通讯版本
#define SEND_FRAME_VALUE		mcuWifi.sendInf.u8Send_Buff[3]	//发送的通讯帧总数
#define SEND_FRAME_NUM			mcuWifi.sendInf.u8Send_Buff[4]	//发送的通讯帧序号
#define SEND_CONTROL_WORD		mcuWifi.sendInf.u8Send_Buff[5]	//发送的控制字
#define SEND_COMMAND_H			mcuWifi.sendInf.u8Send_Buff[6]	//发送的指令（高8位）
#define SEND_COMMAND_L			mcuWifi.sendInf.u8Send_Buff[7]	//发送的指令（低8位）
#define SEND_DATA_LENGTH_H		mcuWifi.sendInf.u8Send_Buff[8]	//发送的数据长度
#define SEND_DATA_LENGTH_L		mcuWifi.sendInf.u8Send_Buff[9]	//发送的数据长度

#define RECV_HEADER_H			mcuWifi.recvInf.u8Recv_Buff[0]	//接收的数据帧头（高8位）
#define RECV_HEADER_L			mcuWifi.recvInf.u8Recv_Buff[1]	//接收的数据帧头（低8位）
#define RECV_CONTRACT_VERSION	mcuWifi.recvInf.u8Recv_Buff[2]	//接收的通讯版本
#define RECV_FRAME_VALUE		mcuWifi.recvInf.u8Recv_Buff[3]	//接收的通讯帧总数
#define RECV_FRAME_NUM			mcuWifi.recvInf.u8Recv_Buff[4]	//接收的通讯帧序号
#define RECV_CONTROL_WORD		mcuWifi.recvInf.u8Recv_Buff[5]	//接收的控制字
#define RECV_COMMAND_H			mcuWifi.recvInf.u8Recv_Buff[6]	//接收的指令（高8位）
#define RECV_COMMAND_L			mcuWifi.recvInf.u8Recv_Buff[7]	//接收的指令（低8位）
#define RECV_DATA_LENGTH_H		mcuWifi.recvInf.u8Recv_Buff[8]	//接收的数据长度
#define RECV_DATA_LENGTH_L		mcuWifi.recvInf.u8Recv_Buff[9]	//接收的数据长度

/*识别失败原因类型（不允许修改值）*/
typedef enum {
	ERR_UNDEFINED				= 0x00,						//未定义
	WRONG_LAYER,											//层数错误
	RECOGNIZED_EMPTY,										//识别为空
	UNCERTAIN,												//可信度低
	UNRECOGNIZED,											//识别不了
	PROCESS_MISMATCH,										//工艺不匹配
	IDENTIFICATION_OVER_TIME,								//识别超时
	CAMMER_BROKEN,											//摄像头坏

}IDEN_ERR_TypeDef;

/*烤糊类型（不允许修改值）*/
typedef enum {
	BURN_UNDEFINED				= 0x00,						//未定义
	ITS_BURNT,												//烤糊了

}BURN_STATUS_TypeDef;

/*摄像头状态类型（不允许修改值）*/
typedef enum {
	NORMAL						= 0x00,						//摄像头正常
	BROKEN,													//摄像头损坏

}CAMMER_STATUS_TypeDef;

/*图片模糊度类型（不允许修改值）*/
typedef enum {
	/*不报警字段*/
	VERY_CLEAR					= 0x00,						//清晰
	DEGREE_OF_OCCLUSION_1_10	= 0x01,						//10%遮挡
	DEGREE_OF_OCCLUSION_2_10	= 0x02,						//20%遮挡
	DEGREE_OF_OCCLUSION_3_10	= 0x03,						//30%遮挡
	DEGREE_OF_OCCLUSION_4_10	= 0x04,						//40%遮挡
	DEGREE_OF_OCCLUSION_5_10	= 0x05,						//50%遮挡
	DEGREE_OF_OCCLUSION_6_10	= 0x06,						//60%遮挡
	DEGREE_OF_OCCLUSION_7_10	= 0x07,						//70%遮挡
	DEGREE_OF_OCCLUSION_8_10	= 0x08,						//80%遮挡
	DEGREE_OF_OCCLUSION_9_10	= 0x09,						//90%遮挡
	DEGREE_OF_OCCLUSION_10_10	= 0x0A,						//100%遮挡

	/*报警字段*/
	ERR_VERY_CLEAR					= 0x10,					//清晰
	ERR_DEGREE_OF_OCCLUSION_1_10	= 0x11,					//10%遮挡
	ERR_DEGREE_OF_OCCLUSION_2_10	= 0x12,					//20%遮挡
	ERR_DEGREE_OF_OCCLUSION_3_10	= 0x13,					//30%遮挡
	ERR_DEGREE_OF_OCCLUSION_4_10	= 0x14,					//40%遮挡
	ERR_DEGREE_OF_OCCLUSION_5_10	= 0x15,					//50%遮挡
	ERR_DEGREE_OF_OCCLUSION_6_10	= 0x16,					//60%遮挡
	ERR_DEGREE_OF_OCCLUSION_7_10	= 0x17,					//70%遮挡
	ERR_DEGREE_OF_OCCLUSION_8_10	= 0x18,					//80%遮挡
	ERR_DEGREE_OF_OCCLUSION_9_10	= 0x19,					//90%遮挡
	ERR_DEGREE_OF_OCCLUSION_10_10	= 0x1A,					//100%遮挡

}PICTURE_BLUR_TypeDef;

typedef struct {
	unsigned int				u16FrameLen;				//发送帧长度（通讯协议中数据长度为16位）
	unsigned int				u16FrameLenBuff;			//发送帧长度缓存，用于接收信息后缓存需要返回数据的长度
	unsigned int				u16DataCnt;					//发送数据数（通讯协议中数据长度为16位）
	unsigned char				u8IntervalTime;				//发送间隔时间

	unsigned char	 			u8Send_Buff[FRAME_LENGTH_MAX];

	unsigned char				u2WifiAckFlag	: 1;		//wifi的响应标志

}SEND_INF_MemberDef;

typedef struct {
	unsigned int				u16FrameLen;				//接收数据帧长度（通讯协议中数据长度为16位）
	unsigned int				u16DataCnt;					//接收数据计数（通讯协议中数据长度为16位）
	unsigned char				u8OutTime;					//接收超时计数器

	unsigned char	 			u8Recv_Buff[FRAME_LENGTH_MAX];

	unsigned char				u2HeadOkFlag_H	: 1;		//接收数据的头码正确标志（高8位）
	unsigned char				u2HeadOkFlag_L	: 1;		//接收数据的头码正确标志（低8位）

}RECV_INF_MemberDef;

typedef struct {
	SEND_INF_MemberDef			sendInf;					//发送信息
	RECV_INF_MemberDef			recvInf;					//接收信息
	UATR_RECV_TYPE				recvType;					//MCU接收状态
	UATR_SEND_TYPE				sendType;					//MCU发送状态

	WIFI_STATUS_TypeDef			u8WifiStatus;				//WIFI当前的状态
	CAMMER_STATUS_TypeDef		u8CammerStatus;				//摄像头状态
	IDEN_ERR_TypeDef			u8IdentificationErrData;	//识别失败信息
	BURN_STATUS_TypeDef			u8BurntStatus;				//烤糊状态
	PICTURE_BLUR_TypeDef		u8PictureBlur;				//图片模糊度

	unsigned char				u8FoodLocation;				//放置位置
	unsigned char				u8FoodSize;					//食材大小信息
	unsigned char				u8FoodQuantity;				//食材数量
	unsigned char				u8FoodMiddleFlag;			//居中与否

	unsigned int  		 		u16CmdCode_Buff[CMD_NUM_BUFF_MAX];	//命令缓冲区

	unsigned char				u8AckTimeCnt;					//应答时间计数
	unsigned char				u8CmdBuffNum;					//缓存的命令数
	unsigned char				u8SnapshotCnt;					//抓图倒计时
	unsigned int				u16RecommandMenu_1;				//推荐食谱1
	unsigned int				u16RecommandMenu_2;				//推荐食谱2
	unsigned int				u16RecommandMenu_3;				//推荐食谱3
	unsigned int				u16RecommandMenu_4;				//推荐食谱4
	unsigned int				u16RecommandMenu_5;				//推荐食谱5

	unsigned char				u2HandsOkflag			: 1;	//握手成功标志
	unsigned char				u2CmdSmartlinkflag		: 1;	//启动SmartLink标志
	unsigned char				u2SmartlinkWorkFlag		: 1;	//处于SmartLink配置状态
	unsigned char				u2FirmWareHandleflag	: 1;	//固件升级握手标志	
	unsigned char				u2LampOpenConfirmflag	: 1;	//抓图时炉灯打开确认标志

	/*指令启动标志*/
	unsigned char				u2CmdDeviceReprotflag	: 1;	//设备上报状态指令
	unsigned char				u2CmdSnapshotflag		: 1;	//抓图指令
	unsigned char				u2CmdSmartBakingFlag	: 1;	//启动智能烘焙指令
	unsigned char				u2CmdFirmwareUpgradeFlag: 1;	//设备请求固件升级指令
	unsigned char				u2CmdRebootWifiFlag		: 1;	//重启wifi指令
	unsigned char				u2CmdQueryDeviceStaFlag	: 1;	//查询设备状态指令
	unsigned char				u2CmdKeyReportFlag		: 1;	//按键行为上报指令
	unsigned char				u2CmdRemoveLinkFlag		: 1;	//解绑指令
	unsigned char				u2CmdGetWiFiStatusFlag	: 1;	//获取wifi状态指令
	unsigned char				u2ReturnSuccesAckFlag	: 1;	//返回接收成功响应
	
}MCU_WIFI_INF_MemberDef;

MCU_WIFI_INF_MemberDef xdata	mcuWifi;						//MCU与wifi通讯的串口信息

bit g_820ASureSendFlag = 0;

/*-----------------------------------------------------------------------------
Description:		发送命令
					cmd：		需要发送的命令
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SendCommand(COMMAND_TypeDef cmd)
{
	switch (cmd)
	{
	case CMD_KX_APP_HANDS:
		/* code */
		break;

	case CMD_KX_APP_SMART_LINK:
		mcuWifi.u2CmdSmartlinkflag			= 1;
		break;

	case CMD_KX_APP_FIRMWARE_UPGRADE:
		mcuWifi.u2CmdFirmwareUpgradeFlag	= 1;
		break;

	case CMD_KX_APP_GET_STATUS:
		mcuWifi.u2CmdGetWiFiStatusFlag		= 1;
		break;

	case CMD_KX_APP_RESTART_WIFI:
		mcuWifi.u2CmdRebootWifiFlag			= 1;
		break;

	case CMD_KX_APP_REMOVE_LINK:
		mcuWifi.u2CmdRemoveLinkFlag			= 1;
		break;

	case CMD_KX_APP_REPORT_STATUS:
		mcuWifi.u2CmdDeviceReprotflag		= 1;
		break;

	case CMD_KX_APP_KEY_DATA:
		mcuWifi.u2CmdKeyReportFlag			= 1;
		break;

	case CMD_APP_KX_SMART_BAKING:
		mcuWifi.u2CmdSmartBakingFlag		= 1;
		break;
	
	default:
		break;
	}
}

/*-----------------------------------------------------------------------------
Description:		读取wifi的相关信息
					
Input:				inf：		需要读取的信息

Return:				读取的信息值
History:			无
-----------------------------------------------------------------------------*/
unsigned char ReadWifiInf(WIFI_INF_TypeDef inf)
{
	switch (inf)
	{
	case WIFI_STATUS:
		return mcuWifi.u8WifiStatus;
		break;
	
	default:
		return 0xFF;
		break;
	}
}

/*-----------------------------------------------------------------------------
Description:		初始化wifi串口IO
					电源板通讯：TXD：P0.0；RXD：P2.7
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitWifiUartIo(void)
{
	BANK0_SET;										//指向Bank0

	/*电源板*/
	/*TXD输出，不带上拉，输出高电平*/
	P0CR |= BIT0;
	P0PCR &= (~BIT0);
	N_TXD = 1;
	
	/*RXD输入，不带上拉*/
	P2CR &= (~BIT7);
	P2PCR &= (~BIT7);
}

/*-----------------------------------------------------------------------------
Description:		初始化wifi模块变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitWifiVariable(void)
{
	mcuWifi.sendInf.u16FrameLen		= 0;
	mcuWifi.sendInf.u16FrameLenBuff	= 0;
	mcuWifi.sendInf.u16DataCnt		= 0;
	mcuWifi.sendInf.u8IntervalTime	= 0;
	mcuWifi.sendInf.u2WifiAckFlag	= 0;
	InitCharArrayData(mcuWifi.sendInf.u8Send_Buff, FRAME_LENGTH_MAX);

	mcuWifi.recvInf.u16FrameLen		= 0;
	mcuWifi.recvInf.u16DataCnt		= 0;
	mcuWifi.recvInf.u8OutTime		= 0;
	mcuWifi.recvInf.u2HeadOkFlag_H	= 0;
	mcuWifi.recvInf.u2HeadOkFlag_L	= 0;
	InitCharArrayData(mcuWifi.recvInf.u8Recv_Buff, FRAME_LENGTH_MAX);

	mcuWifi.recvType = RECV_OVER;
	mcuWifi.sendType = SEND_OVER;

	mcuWifi.u8WifiStatus			= TRY_CONNET_ROUTER;
	mcuWifi.u8CammerStatus			= NORMAL;
	mcuWifi.u8IdentificationErrData	= ERR_UNDEFINED;
	mcuWifi.u8BurntStatus			= BURN_UNDEFINED;
	mcuWifi.u8PictureBlur			= VERY_CLEAR;

	mcuWifi.u8FoodLocation			= 0;
	mcuWifi.u8FoodSize				= 0;
	mcuWifi.u8FoodQuantity			= 0;
	mcuWifi.u8FoodMiddleFlag		= 0;

	InitIntArrayData(mcuWifi.u16CmdCode_Buff, CMD_NUM_BUFF_MAX);
	
	mcuWifi.u8AckTimeCnt			= 0;
	mcuWifi.u8CmdBuffNum			= 0;
	mcuWifi.u8SnapshotCnt			= 0;
	mcuWifi.u16RecommandMenu_1		= 0x0000;
	mcuWifi.u16RecommandMenu_2		= 0x0000;
	mcuWifi.u16RecommandMenu_3		= 0x0000;
	mcuWifi.u16RecommandMenu_4		= 0x0000;
	mcuWifi.u16RecommandMenu_5		= 0x0000;

	mcuWifi.u2HandsOkflag			= 0;
	mcuWifi.u2CmdSmartlinkflag		= 0;
	mcuWifi.u2SmartlinkWorkFlag		= 0;
	mcuWifi.u2FirmWareHandleflag	= 0;
	mcuWifi.u2LampOpenConfirmflag	= 0;

	mcuWifi.u2CmdDeviceReprotflag	= 0;
	mcuWifi.u2CmdSnapshotflag		= 0;
	mcuWifi.u2CmdSmartBakingFlag	= 0;
	mcuWifi.u2CmdFirmwareUpgradeFlag= 0;
	mcuWifi.u2CmdRebootWifiFlag		= 0;
	mcuWifi.u2CmdQueryDeviceStaFlag	= 0;
	mcuWifi.u2CmdKeyReportFlag		= 0;
	mcuWifi.u2CmdRemoveLinkFlag		= 0;
	mcuWifi.u2CmdGetWiFiStatusFlag	= 0;
	mcuWifi.u2ReturnSuccesAckFlag	= 0;
}

/*-----------------------------------------------------------------------------
Description:		初始化wifi串口配置
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitWifiUart(void)
{
	/*BANK1*/
	BANK1_SET;	
	UART0CR = 0x05;							//TXD0映射到P0.0；RXD0映射到P2.7
	BANK0_SET;	
	SCON = 0x50;							//0101 0000，方式1，8位异步，可变波特率，接收允许
	
											//24000000/16/9600 = 156.25
											//SBRT = 32768 - 156 = 32612 = 0x7F64
											//9600 = 24000000/(16*156+BFINE)
											//BFINE = 4，实际波特率为24000000/(16*156+4)=9600
	SBRTH = 0x7F;							//EUART波特率发生器计数器
	SBRTL = 0x64;
	SFINE = 0x04;
	
	SBRTH |= BIT7;							//开EUART0波特率
	
	BANK0_SET;	
	
	UTOS |= BIT0;							//RXD0电平为TTL逻辑
	
	IEN0 |= BIT4;							//开EUART0中断
}

/*-----------------------------------------------------------------------------
Description:		生成和校验值
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
unsigned char SumMakeVerify(unsigned char *PBuf, unsigned int Len)
{
	unsigned char Verify = 0;
	while(Len--)
	{
		Verify = Verify + (*PBuf);
		++PBuf;
	}
	return (~Verify + 1);
}

/*-----------------------------------------------------------------------------
Description:		解密校验
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
unsigned char SumVerify(unsigned char *PBuf, unsigned int Len)
{
	unsigned char Verify = 0;

	while(Len--)
	{
		Verify = Verify + (*PBuf);
		PBuf ++;
	}
	return (Verify == 0);									//如果verify为0 返回1 否则返回0
}

/*-----------------------------------------------------------------------------
Description:		发送串口数据处理
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
unsigned int SendUartData(unsigned int len)
{
	SEND_HEADER_H			= HEADER_CODE_H;				//帧头高位
	SEND_HEADER_L			= HEADER_CODE_L;				//帧头低位
	SEND_CONTRACT_VERSION	= CONTRACT_VERSION;				//通信版本
	SEND_FRAME_VALUE		= CONTRACT_FRAME_VALUE;			//帧总数
	SEND_FRAME_NUM			= CONTRACT_FRAME_NUM;			//帧序号

	if(mcuWifi.sendInf.u2WifiAckFlag)						//MCU发送要求对方回复时
	{
		SEND_CONTROL_WORD = 0x00;							//控制字：	0x00：主动发送指令要求对方回复
															//			0x40：主动发送无需对方回复
															//			0x80：返回指令
	}
	else
	{
		SEND_CONTROL_WORD = 0x80;
	}

	if((SEND_COMMAND_H == 0x50) && (SEND_COMMAND_L == 0x00) || \
	  ((SEND_COMMAND_H == 0x60) && (SEND_COMMAND_L == 0x03)))		//5000（设备主动上报）和6003（按键行为上报）指令时，主动发送无需对方回复
	{
		SEND_CONTROL_WORD = 0x40;
	}
	
	mcuWifi.sendInf.u8Send_Buff[len] = SumMakeVerify(mcuWifi.sendInf.u8Send_Buff, len);		//最后一个字节赋值校验和

	mcuWifi.sendInf.u16DataCnt = 0;
	mcuWifi.u8AckTimeCnt = ACK_TIMER_MAX;					//更新应答间隔时间
	mcuWifi.sendType = SEND_GOING;							//置MCU状态为发送中
	SBUF = mcuWifi.sendInf.u8Send_Buff[0];

	return (len + 1);
}

/*-----------------------------------------------------------------------------
Description:		赋值机器所有需要传递的信息（共76个数据）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DeviceDataAssignment(void)
{
	mcuWifi.sendInf.u8Send_Buff[10] = g_sysType;		//系统状态
	mcuWifi.sendInf.u8Send_Buff[11] = 0;	//菜单编码（24~31位）
	mcuWifi.sendInf.u8Send_Buff[12] = 0;	//菜单编码（16~23位）
	mcuWifi.sendInf.u8Send_Buff[13] = 0;	//菜单编码（8~15位）
	mcuWifi.sendInf.u8Send_Buff[14] = 0;			//菜单编码（0~7位）
	mcuWifi.sendInf.u8Send_Buff[15] = 0;	//预约使能
	mcuWifi.sendInf.u8Send_Buff[16] = 0;	//预约标识位
	mcuWifi.sendInf.u8Send_Buff[17] = 0;	//预约时间（高8位）
	mcuWifi.sendInf.u8Send_Buff[18] = 0;	//预约时间（低8位）
	mcuWifi.sendInf.u8Send_Buff[19] = 0;	//预热使能
	mcuWifi.sendInf.u8Send_Buff[20] = 0;	//预热标识位
	mcuWifi.sendInf.u8Send_Buff[21] = 0;	//预热时间（高8位）
	mcuWifi.sendInf.u8Send_Buff[22] = 0;	//预热时间（低8位）
	mcuWifi.sendInf.u8Send_Buff[23] = 0;	//预热保持
	mcuWifi.sendInf.u8Send_Buff[24] = 0;	//预热保持时间
	mcuWifi.sendInf.u8Send_Buff[25] = g_nowStepworkTemp;	//第一步上管温度
	mcuWifi.sendInf.u8Send_Buff[26] = 0;	//第一步下管温度
	mcuWifi.sendInf.u8Send_Buff[27] = g_workTimeAll;		//第一步工作时间
	mcuWifi.sendInf.u8Send_Buff[28] = 0;	//第二步上管温度
	mcuWifi.sendInf.u8Send_Buff[29] = 0;	//第二步下管温度
	mcuWifi.sendInf.u8Send_Buff[30] = 0;		//第二步工作时间
	mcuWifi.sendInf.u8Send_Buff[31] = 0;	//第三步上管温度
	mcuWifi.sendInf.u8Send_Buff[32] = 0;	//第三步下管温度
	mcuWifi.sendInf.u8Send_Buff[33] = 0;		//第三步工作时间
	mcuWifi.sendInf.u8Send_Buff[34] = 45;	//温度调节步长分界点温度
	mcuWifi.sendInf.u8Send_Buff[35] = 5;	//温度小于分界点，温度调整步长
	mcuWifi.sendInf.u8Send_Buff[36] = 5;	//温度大于等于分界点，温度调整步长
	mcuWifi.sendInf.u8Send_Buff[37] = 35;	//可调最小温度
	mcuWifi.sendInf.u8Send_Buff[38] = 200;	//可调最大温度
	mcuWifi.sendInf.u8Send_Buff[39] = g_workTimeAll;	//当前剩余工作时间（分钟）
	mcuWifi.sendInf.u8Send_Buff[40] = 0;	//当前剩余工作时间（秒钟）
	// if(mcuWifi.sendInf.u8Send_Buff[40] != 0)
	// {
	// 	mcuWifi.sendInf.u8Send_Buff[39] -= 1;			//读取的分钟值为进位值，若S不为零，则上传的分钟值减1
	// }
	mcuWifi.sendInf.u8Send_Buff[41] = 1;	//可调最小时间
	mcuWifi.sendInf.u8Send_Buff[42] = 60;	//可调最大时间
	mcuWifi.sendInf.u8Send_Buff[43] = 0;	//转叉状态
	mcuWifi.sendInf.u8Send_Buff[44] = g_LedOpenFlag;	//炉灯状态
	mcuWifi.sendInf.u8Send_Buff[45] = 0;	//上管实时温度符号
	mcuWifi.sendInf.u8Send_Buff[46] = 0;	//上管实时温度（高8位）
	mcuWifi.sendInf.u8Send_Buff[47] = 0;	//上管实时温度（低8位）
	mcuWifi.sendInf.u8Send_Buff[48] = 0;	//下管实时温度符号
	mcuWifi.sendInf.u8Send_Buff[49] = 0;	//下管实时温度（高8位）
	mcuWifi.sendInf.u8Send_Buff[50] = 0;	//下管实时温度（低8位）
	mcuWifi.sendInf.u8Send_Buff[51] = 1;	//时间调节步长
	mcuWifi.sendInf.u8Send_Buff[52] = mcuWifi.u8IdentificationErrData;	//识别失败原因
	mcuWifi.sendInf.u8Send_Buff[53] = mcuWifi.u8BurntStatus;	//烤糊
	mcuWifi.sendInf.u8Send_Buff[54] = mcuWifi.u8PictureBlur;	//图片模糊度
	mcuWifi.sendInf.u8Send_Buff[55] = mcuWifi.u8FoodLocation;	//烤盘位置
	mcuWifi.sendInf.u8Send_Buff[56] = mcuWifi.u8FoodSize;		//食材形态
	mcuWifi.sendInf.u8Send_Buff[57] = mcuWifi.u8FoodQuantity;	//食材数量
	mcuWifi.sendInf.u8Send_Buff[58] = mcuWifi.u8FoodMiddleFlag;	//放置位置
	mcuWifi.sendInf.u8Send_Buff[59] = 0;	//推荐菜单编码1（24~31位）
	mcuWifi.sendInf.u8Send_Buff[60] = 0;	//推荐菜单编码1（16~23位）
	mcuWifi.sendInf.u8Send_Buff[61] = (unsigned char)((mcuWifi.u16RecommandMenu_1 & 0xFF00) >> 8);	//推荐菜单编码1（8~15位）
	mcuWifi.sendInf.u8Send_Buff[62] = (unsigned char)(mcuWifi.u16RecommandMenu_1 & 0x00FF);	//推荐菜单编码1（0~7位）
	mcuWifi.sendInf.u8Send_Buff[63] = 0;	//推荐菜单编码2（24~31位）
	mcuWifi.sendInf.u8Send_Buff[64] = 0;	//推荐菜单编码2（16~23位）
	mcuWifi.sendInf.u8Send_Buff[65] = (unsigned char)((mcuWifi.u16RecommandMenu_2 & 0xFF00) >> 8);	//推荐菜单编码2（8~15位）
	mcuWifi.sendInf.u8Send_Buff[66] = (unsigned char)(mcuWifi.u16RecommandMenu_2 & 0x00FF);	//推荐菜单编码2（0~7位）
	mcuWifi.sendInf.u8Send_Buff[67] = 0;	//推荐菜单编码3（24~31位）
	mcuWifi.sendInf.u8Send_Buff[68] = 0;	//推荐菜单编码3（16~23位）
	mcuWifi.sendInf.u8Send_Buff[69] = (unsigned char)((mcuWifi.u16RecommandMenu_3 & 0xFF00) >> 8);	//推荐菜单编码3（8~15位）
	mcuWifi.sendInf.u8Send_Buff[70] = (unsigned char)(mcuWifi.u16RecommandMenu_3 & 0x00FF);	//推荐菜单编码3（0~7位）
	mcuWifi.sendInf.u8Send_Buff[71] = 0;	//推荐菜单编码4（24~31位）
	mcuWifi.sendInf.u8Send_Buff[72] = 0;	//推荐菜单编码4（16~23位）
	mcuWifi.sendInf.u8Send_Buff[73] = (unsigned char)((mcuWifi.u16RecommandMenu_4 & 0xFF00) >> 8);	//推荐菜单编码4（8~15位）
	mcuWifi.sendInf.u8Send_Buff[74] = (unsigned char)(mcuWifi.u16RecommandMenu_4 & 0x00FF);	//推荐菜单编码4（0~7位）
	mcuWifi.sendInf.u8Send_Buff[75] = 0;	//推荐菜单编码5（24~31位）
	mcuWifi.sendInf.u8Send_Buff[76] = 0;	//推荐菜单编码5（16~23位）
	mcuWifi.sendInf.u8Send_Buff[77] = (unsigned char)((mcuWifi.u16RecommandMenu_5 & 0xFF00) >> 8);	//推荐菜单编码5（8~15位）
	mcuWifi.sendInf.u8Send_Buff[78] = (unsigned char)(mcuWifi.u16RecommandMenu_5 & 0x00FF);	//推荐菜单编码5（0~7位）
	mcuWifi.sendInf.u8Send_Buff[79] = mcuWifi.u8CammerStatus;	//摄像头状态
	mcuWifi.sendInf.u8Send_Buff[80] = 0;	//预留1（高8位）
	mcuWifi.sendInf.u8Send_Buff[81] = 0;	//预留1（低8位）
	mcuWifi.sendInf.u8Send_Buff[82] = 0;	//预留2（高8位）
	mcuWifi.sendInf.u8Send_Buff[83] = 0;	//预留2（低8位）
//	mcuWifi.sendInf.u8Send_Buff[84] = (unsigned char)((ERR_NUM & 0xFF00) >> 8);	//故障代码（高8位）
//	mcuWifi.sendInf.u8Send_Buff[85] = (unsigned char)(ERR_NUM & 0x00FF);		//故障代码（低8位）
}

/*-----------------------------------------------------------------------------
Description:		Wifi的启动信息读取并更新
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void DeviceWorkDataUpdate(void)
{
	// unsigned int intDataBuff = 0;

	// intDataBuff = mcuWifi.recvInf.u8Recv_Buff[12];
	// intDataBuff = (intDataBuff << 8) + mcuWifi.recvInf.u8Recv_Buff[13];
	// ChangeMachineStatus(MENU_NUM, intDataBuff);					//菜单编码			目前只用到低16位（2020/12/9）

	// ChangeMachineStatus(ORDER_ENABLE,		mcuWifi.recvInf.u8Recv_Buff[14]);	//预约使能
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[15]);	//预约标识位

	// intDataBuff = mcuWifi.recvInf.u8Recv_Buff[16];
	// intDataBuff = (intDataBuff << 8) + mcuWifi.recvInf.u8Recv_Buff[17];
	// ChangeMachineStatus(ORDER_TIME, intDataBuff);				//预约时间

	// ChangeMachineStatus(PRE_HEAT_ENABLE,	mcuWifi.recvInf.u8Recv_Buff[18]);	//预热使能
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[19]);	//预热标识位

	// intDataBuff = mcuWifi.recvInf.u8Recv_Buff[20];
	// intDataBuff = (intDataBuff << 8) + mcuWifi.recvInf.u8Recv_Buff[21];
	// ChangeMachineStatus(PRE_HEAT_TIME, intDataBuff);			//预热时间

	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[22]);	//预热保持
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[23]);	//预热保持时间

	g_nowStepworkTemp 	= mcuWifi.recvInf.u8Recv_Buff[24];
	if((g_nowStepworkTemp < 30) || (g_nowStepworkTemp > 200))
	{
		g_nowStepworkTemp = 200;		//温度超过范围，默认200℃
	}
	// loadCrlData.plateHeatGear = (mcuWifi.recvInf.u8Recv_Buff[25])*2;
	g_nowStepTechnology = ONLY_BAKE;
	g_workTimeAll		= mcuWifi.recvInf.u8Recv_Buff[26];

	// ChangeMachineStatus(WORK_TEMP_TOP_1,	mcuWifi.recvInf.u8Recv_Buff[24]);	//第一步上管温度
	// ChangeMachineStatus(WORK_TEMP_BOT_1,	mcuWifi.recvInf.u8Recv_Buff[25]);	//第一步下管温度
	// ChangeMachineStatus(WORK_TIME_1,		mcuWifi.recvInf.u8Recv_Buff[26]);	//第一步工作时间
	// ChangeMachineStatus(WORK_TEMP_TOP_2,	mcuWifi.recvInf.u8Recv_Buff[27]);	//第二步上管温度
	// ChangeMachineStatus(WORK_TEMP_BOT_2,	mcuWifi.recvInf.u8Recv_Buff[28]);	//第二步下管温度
	// ChangeMachineStatus(WORK_TIME_2,		mcuWifi.recvInf.u8Recv_Buff[29]);	//第二步工作时间
	// ChangeMachineStatus(WORK_TEMP_TOP_3,	mcuWifi.recvInf.u8Recv_Buff[30]);	//第三步上管温度
	// ChangeMachineStatus(WORK_TEMP_BOT_3,	mcuWifi.recvInf.u8Recv_Buff[31]);	//第三步下管温度
	// ChangeMachineStatus(WORK_TIME_3,		mcuWifi.recvInf.u8Recv_Buff[32]);	//第三步工作时间
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[33]);	//温度调节步长分界点温度
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[34]);	//温度小于分界点，温度调整步长
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[35]);	//温度大于等于分界点，温度调整步长
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[36]);	//可调最小温度
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[37]);	//可调最大温度
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[38]);	//可调最小时间
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[39]);	//可调最大时间
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[40]);	//转叉状态
	// ChangeMachineStatus(LAMP_ENABLE,		mcuWifi.recvInf.u8Recv_Buff[41]);	//炉灯状态
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[42]);	//时间调节步长
	// mcuWifi.u8FoodLocation		= mcuWifi.recvInf.u8Recv_Buff[43];	//放置位置
	// mcuWifi.u8FoodSize			= mcuWifi.recvInf.u8Recv_Buff[44];	//食材形态信息
	// mcuWifi.u8FoodQuantity		= mcuWifi.recvInf.u8Recv_Buff[45];	//食材数量
	// mcuWifi.u8FoodMiddleFlag	= mcuWifi.recvInf.u8Recv_Buff[46];	//是否居中

	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[47]);	//预留1（高8位）
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[48]);	//预留1（低8位）
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[49]);	//预留2（高8位）
	// // ChangeMachineStatus(0, mcuWifi.recvInf.u8Recv_Buff[50]);	//预留2（低8位）
}

/*-----------------------------------------------------------------------------
Description:		MCU向wifi发送ACK
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
unsigned int SendWifiAck(unsigned int len)
{
	unsigned int i;

	for(i=0;i<len;i++)
	{
		mcuWifi.sendInf.u8Send_Buff[i] = mcuWifi.recvInf.u8Recv_Buff[i];
	}

	mcuWifi.recvType = RECV_GOING;						//继续执行接收

	SEND_CONTROL_WORD = 0x80;							//mcu接收成功回复，不用wifi答复

	mcuWifi.sendInf.u8Send_Buff[len] = SumMakeVerify(mcuWifi.sendInf.u8Send_Buff, len);

	mcuWifi.sendInf.u16DataCnt = 0;
	mcuWifi.sendType = SEND_GOING;
	SBUF = mcuWifi.sendInf.u8Send_Buff[0];

	return(len + 1);
}

/*-----------------------------------------------------------------------------
Description:		MCU向wifi发送数据处理
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SendUart(void)
{
	unsigned char i = 0;
	unsigned int cmdCode = 0;
	// unsigned char lampStatus = 0;

	//wifi已响应 且 数据已发送完毕 且 发送间隔时间到
	if((mcuWifi.sendInf.u2WifiAckFlag == 0) && (mcuWifi.sendType == SEND_OVER) && (mcuWifi.sendInf.u8IntervalTime == 0))
	{
		if(mcuWifi.u2ReturnSuccesAckFlag)					//需要返回接收成功指令时（上一帧数据发送完后，马上发送ACK）
		{
			mcuWifi.sendInf.u16FrameLen = mcuWifi.sendInf.u16FrameLenBuff;		//缓存的发送长度赋值给发送长度变量
			mcuWifi.sendInf.u16FrameLen = SendWifiAck(mcuWifi.sendInf.u16FrameLen);
			mcuWifi.u2ReturnSuccesAckFlag = 0;				//ACK发送完毕

			return;
		}

		if(mcuWifi.u2CmdSmartBakingFlag)					//若有启动智能烘焙指令
		{
			// lampStatus = g_LedOpenFlag;			//读炉灯状态

			if(g_820ASureSendFlag == 1)
			{
				mcuWifi.u2CmdSmartBakingFlag	= 0;		//指令已执行
				mcuWifi.sendInf.u2WifiAckFlag	= 0;
				SEND_COMMAND_H		= 0x82;					//发送820A指令
				SEND_COMMAND_L		= 0x0A;
				SEND_DATA_LENGTH_H	= 0x00;					//数据长度
				SEND_DATA_LENGTH_L	= 0x00;
				mcuWifi.sendInf.u16FrameLen		= 10;
				mcuWifi.sendInf.u16FrameLen		= SendUartData(mcuWifi.sendInf.u16FrameLen);

				mcuWifi.u2CmdDeviceReprotflag = 1;			//确认炉灯开启后，设备上报状态
				return;										//直接返回，不执行以下代码
			}
		}

		if(mcuWifi.u2CmdSnapshotflag)						//若有抓图指令
		{
			// lampStatus = g_LedOpenFlag;			//读炉灯状态

			// if(lampStatus == STATUS_YES)
			// {
			// 	mcuWifi.u2CmdSnapshotflag		= 0;		//指令已执行
			// 	mcuWifi.sendInf.u2WifiAckFlag	= 0;
			// 	SEND_COMMAND_H		= 0x82;					//发送820B指令
			// 	SEND_COMMAND_L		= 0x0B;
			// 	SEND_DATA_LENGTH_H	= 0x00;					//数据长度
			// 	SEND_DATA_LENGTH_L	= 0x00;
			// 	mcuWifi.sendInf.u16FrameLen		= 10;
			// 	mcuWifi.sendInf.u16FrameLen		= SendUartData(mcuWifi.sendInf.u16FrameLen);

			// 	mcuWifi.u2CmdDeviceReprotflag = 1;			//确认炉灯开启后，设备上报状态
			// 	return;										//直接返回，不执行以下代码
			// }
		}

		if(mcuWifi.u8CmdBuffNum)							//存在缓存命令时，加载命令
		{
			cmdCode = mcuWifi.u16CmdCode_Buff[0];
			mcuWifi.u8CmdBuffNum--;
			for(i=0;i<mcuWifi.u8CmdBuffNum;i++)				//将剩余指令向前移位
			{
				mcuWifi.u16CmdCode_Buff[i] = mcuWifi.u16CmdCode_Buff[i + 1];
			}

			if(mcuWifi.u16CmdCode_Buff[CMD_NUM_BUFF_MAX - 1] != 0)	//若缓存命令Buff最后一个命令移位赋值后，不为零，则清零。防止一直移位，把所有缓存值都赋值为最后一个的命令（虽然u8CmdBuffNum会计数，命令值会更新）
			{
				mcuWifi.u16CmdCode_Buff[CMD_NUM_BUFF_MAX - 1] = 0;
			}

			SEND_COMMAND_H	= (unsigned char)((cmdCode & 0xff00) >> 8 );	//取指令高8位 
			SEND_COMMAND_L	= (unsigned char)(cmdCode & 0x00ff);			//取指令低8位

			switch(cmdCode)
			{
				case 0xCCC0:								//握手指令
					mcuWifi.sendInf.u2WifiAckFlag = 1;		//要求回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x06;				//数据长度
					mcuWifi.sendInf.u8Send_Buff[10] = KX_MODEL_H; 		//设备型号高位
					mcuWifi.sendInf.u8Send_Buff[11] = KX_MODEL_L ; 		//设备型号低位
					mcuWifi.sendInf.u8Send_Buff[12] = 0x00; 			//设备通信版本
					mcuWifi.sendInf.u8Send_Buff[13] = PROGRAM_VERSION;	//设备固件版本
					mcuWifi.sendInf.u8Send_Buff[14] = 0x00;	//通信数据长度高位
					mcuWifi.sendInf.u8Send_Buff[15] = 0xFF;	//通信数据长度低位

					//通讯协议中还有其它数据位，是否增加？

					mcuWifi.sendInf.u16FrameLen = 16;		//赋值数据长度
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);	//加校验位后的长度
					break;

				case 0xCCC2:								//启动Smartlink
					mcuWifi.u2SmartlinkWorkFlag		= 1;	//标记为Smartlink配置中
					mcuWifi.sendInf.u2WifiAckFlag	= 0;	//不需要回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x00;				//数据长度
					mcuWifi.sendInf.u16FrameLen = 10;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0x6001:								//清除设备绑定
					mcuWifi.sendInf.u2WifiAckFlag	= 1;	//要求回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x00;				//数据长度
					mcuWifi.sendInf.u16FrameLen = 10;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0xCCC5:								//设备要求固件升级
					mcuWifi.sendInf.u2WifiAckFlag = 1;		//要求回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x02;				//数据长度
					mcuWifi.sendInf.u8Send_Buff[10] = 0x00;	//每帧长度高位
					mcuWifi.sendInf.u8Send_Buff[11] = 0x01;	//每帧长度低位 暂定1
					mcuWifi.sendInf.u16FrameLen = 12;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0xCCC7:								//获取WiFi模块的连接状态、以及信号强度
					mcuWifi.sendInf.u2WifiAckFlag	= 0;	//不需要回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x00;				//数据长度
					mcuWifi.sendInf.u16FrameLen = 10;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0xCCC9:								//重启模块
					mcuWifi.sendInf.u2WifiAckFlag	= 1;	//要求回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x00;				//数据长度
					mcuWifi.sendInf.u16FrameLen = 10;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0x00B2:								//APP查询设备状态
					mcuWifi.sendInf.u2WifiAckFlag	= 0;	//不需要回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 76;				//数据长度

					DeviceDataAssignment();					//赋值机器状态信息（共76个数据）
					
					mcuWifi.sendInf.u16FrameLen = 86;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0x5000:								//设备主动上报状态
					mcuWifi.sendInf.u2WifiAckFlag	= 0;	//不需要回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 76;				//数据长度

					DeviceDataAssignment();					//赋值机器状态信息（共76个数据）

					mcuWifi.sendInf.u16FrameLen = 86;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0x820A:								//启动智能烘焙
					mcuWifi.sendInf.u2WifiAckFlag	= 0;	//不需要回复响应

					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
					SEND_DATA_LENGTH_L	= 0x00;				//数据长度
					mcuWifi.sendInf.u16FrameLen = 10;
					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;

				case 0x6003:								//按键行为上报
					mcuWifi.sendInf.u2WifiAckFlag	= 0;	//不需要回复响应

//					SEND_DATA_LENGTH_H	= 0x00;				//数据长度
//					SEND_DATA_LENGTH_L	= keyReportData.u8KeyNumber + 1;			//数据长度
//					mcuWifi.sendInf.u8Send_Buff[10] = keyReportData.u8KeyNumber;	//按键个数

//					for(i=0;i<keyReportData.u8KeyNumber;i++)	//赋值所有需要上传的按键
//					{
//						mcuWifi.sendInf.u8Send_Buff[11+i] = keyReportData.u8KeyValue[i];
//					}

//					keyReportData.u8KeyNumber = 0;			//值传递给发送的缓存变量后清零
//					InitCharArrayData(keyReportData.u8KeyValue, VALID_TOUCHKEY_NUMBER);

//					mcuWifi.sendInf.u16FrameLen = keyReportData.u8KeyNumber + 11;
//					mcuWifi.sendInf.u16FrameLen = SendUartData(mcuWifi.sendInf.u16FrameLen);
					break;
			}
		}
		else
		{
			//无缓存命令，跳出
		}
		
	}
	else
	{
		//等待发送允许条件准备好
	}
}

/*-----------------------------------------------------------------------------
Description:		MCU向wifi接收数据处理
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void RecvUart(void)
{
	unsigned char verify;
	unsigned int cmdRecv;
//	unsigned char nowSysMode;
//	unsigned char workTimeAll;

	unsigned char i = 0;
	unsigned char j = 0;

	if((mcuWifi.recvType == RECV_OVER) && (mcuWifi.u2ReturnSuccesAckFlag == 0))		//接收已完成且ACK已发送
	{
		// mcuWifi.recvType = RECV_GOING;						//继续执行接收

		verify = SumVerify(mcuWifi.recvInf.u8Recv_Buff, (mcuWifi.recvInf.u16FrameLen + 1));

		if(verify == 1)
		{
			cmdRecv = RECV_COMMAND_H;
			cmdRecv = cmdRecv << 8;
			cmdRecv = cmdRecv + RECV_COMMAND_L;				//赋值指令代码

			switch(cmdRecv)
			{
				case 0xCCC0:								//握手指令
					if(mcuWifi.u2HandsOkflag == 0)			//握手未成功时，蜂鸣
					{
						gU8_buzzerType = BUZ_HP_KEY;
					}

					if((mcuWifi.recvInf.u8Recv_Buff[10] == 0x00) && \
					   (mcuWifi.recvInf.u8Recv_Buff[11] == 0x00))		//结果标识：U16： 0x0000：成功； 0x0001： 失败
					{
						mcuWifi.u2HandsOkflag = 1;			//握手成功

						mcuWifi.u8WifiStatus = mcuWifi.recvInf.u8Recv_Buff[12];	//赋值wifi当前状态

						mcuWifi.sendInf.u2WifiAckFlag = 0;	//标记已回复MCU
						mcuWifi.u8CmdBuffNum = 0;			//缓存指令数清零

						InitIntArrayData(mcuWifi.u16CmdCode_Buff,CMD_NUM_BUFF_MAX);		//缓存命令数组清零
					}
					else
					{
						//握手失败
					}

					break;

				case 0xCCC2:								//SmartLink配置
					if((mcuWifi.recvInf.u8Recv_Buff[10] == 0x00) && \
					   (mcuWifi.recvInf.u8Recv_Buff[11] == 0x00))		//结果标识：U16： 0x0000：成功； 0x0001： 失败
					{
						mcuWifi.sendInf.u2WifiAckFlag	= 0;	//标记已回复MCU
						mcuWifi.u2SmartlinkWorkFlag		= 0;	//标记smartLink已配置成功
						mcuWifi.u8CmdBuffNum = 0;				//缓存指令数清零

						InitIntArrayData(mcuWifi.u16CmdCode_Buff,CMD_NUM_BUFF_MAX);		//缓存命令数组清零
					}

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0xCCD0:								//WiFi 模块上报状态信息
					if(RECV_CONTROL_WORD == 0x00)			//正常工作过程中需要回复信息
					{
						mcuWifi.u8WifiStatus = mcuWifi.recvInf.u8Recv_Buff[10];

						RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
						RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
						mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
						mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
						mcuWifi.sendInf.u16FrameLenBuff	= 12;
						mcuWifi.u2ReturnSuccesAckFlag	= 1;		//需要返回接收成功信息
					}
					else									//smartlink 配置时，不需要回复信息
					{
						mcuWifi.u8WifiStatus = mcuWifi.recvInf.u8Recv_Buff[10];
					}
					
					if(mcuWifi.u2SmartlinkWorkFlag)
					{
						if((mcuWifi.u8WifiStatus != TRY_CONNET_ROUTER) && (mcuWifi.u8WifiStatus != CAN_NOT_CONNET_SERVER))	//已连路由器（指示灯常亮）
						{
							mcuWifi.u2SmartlinkWorkFlag = 0;	//标记Smartlink已配置成功
							mcuWifi.u2CmdSmartlinkflag	= 0;	//标记Smartlink已处理
						}
					}

					break;

				case 0xCCC5:								//设备要求固件升级
					if((mcuWifi.recvInf.u8Recv_Buff[10] == 0x00) && (mcuWifi.recvInf.u8Recv_Buff[11] == 0x00))	//成功
					{
						mcuWifi.sendInf.u2WifiAckFlag = 0;	//标记已回复MCU
						mcuWifi.u2CmdFirmwareUpgradeFlag = 1;
					}
					else
					{
						mcuWifi.u2CmdFirmwareUpgradeFlag = 0;
					}
					
					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0xCCC7:								//WiFi 模块的连接状态以及信号强度
					if((mcuWifi.recvInf.u8Recv_Buff[10] == 0x00) && (mcuWifi.recvInf.u8Recv_Buff[11] == 0x00))	//成功
					{
						mcuWifi.u8WifiStatus = mcuWifi.recvInf.u8Recv_Buff[12];
					}
					else 
					{
						
					}
					break;

				case 0xCCC9:								//重启模块
					if((mcuWifi.recvInf.u8Recv_Buff[10] == 0x00) && (mcuWifi.recvInf.u8Recv_Buff[11] == 0x00))	//成功
					{
						mcuWifi.sendInf.u2WifiAckFlag = 0;	//标记已回复MCU
						mcuWifi.u2CmdRebootWifiFlag = 1;	
					}
					else
					{
						mcuWifi.u2CmdRebootWifiFlag = 0;
					}

					gU8_buzzerType = BUZ_HP_KEY;
					break;		
			
				case 0xCCD1:								//固件升级握手指令

//					if(ReadMachineStatus(SYS_MODE) == SYS_MODE_STANDBY)		//待机状态时
//					{
//						// recvFirmWareEdit = Recv_Buf[10];
//						/*Sector_Erase(0x0300,1); 			//固件升级擦除密钥
//						Byte_Write1(0x0300,0x55,1); 		//写入密钥0x55
//						Byte_Write1(0x0301,0xaa,1); 		//写入密钥0xaa */	
//						RECV_DATA_LENGTH_H = 0;
//						RECV_DATA_LENGTH_L = 4;
//						mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;
//						mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
//						mcuWifi.recvInf.u8Recv_Buff[12] = 0x01;
//						mcuWifi.recvInf.u8Recv_Buff[13] = 0x00;
//						mcuWifi.sendInf.u16FrameLenBuff	= 14;

//						mcuWifi.u2ReturnSuccesAckFlag 	= 1;	//需要返回接收成功信息

//						mcuWifi.u2FirmWareHandleflag	= 1;
//					}

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x00B5:								//APP启动菜单
					DeviceWorkDataUpdate();					//更新工作参数信息

					if((g_sysType == SysModeStandby) || (g_sysType == SysModeSelect))
					{
						// if(mcuWifi.recvInf.u8Recv_Buff[12] == 0x30)	//仅取了最高8位判断食谱编码，不能显示详细菜单信息
						// {
						// 	if(mcuWifi.recvInf.u8Recv_Buff[18])	//若预热使能
						// 	{
						// 		ChangeMachineStatus(SYS_MODE, SYS_MODE_PRE_HEAT);	//转到预热状态
						// 	}
						// 	else
						// 	{
						// 		ChangeMachineStatus(SYS_MODE, SYS_MODE_DIY_WORKING);//转到自主烘焙工作状态
						// 	}
						// }
						// else if(mcuWifi.recvInf.u8Recv_Buff[12] == 0x00)
						// {
						// 	ChangeMachineStatus(SYS_MODE, SYS_MODE_SMART_WORKING);	//转到智能烘焙工作状态
						// }

						g_sysType = SysModeWork;

						// ChangeMachineStatus(NOW_WORK_STEP, 1);						//从第一步开始执行
						// workTimeAll = (unsigned char)(ReadMachineStatus(WORK_TIME_1));
						// workTimeAll += (unsigned char)(ReadMachineStatus(WORK_TIME_2));
						// workTimeAll += (unsigned char)(ReadMachineStatus(WORK_TIME_3));	//计算总时间
						// ChangeMachineStatus(NOW_WORK_TIME_ALL_MINUTE, workTimeAll);		//更新总时间(函数内会清零S计时器)
					}

					RECV_DATA_LENGTH_H = 0x00;  				//数据长度（为返回信息的数据长度）
					RECV_DATA_LENGTH_L = 6;

					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;		//返回成功信息

					// mcuWifi.recvInf.u8Recv_Buff[12] = 0x00;	//返回菜单编码
					// mcuWifi.recvInf.u8Recv_Buff[13] = 0x00;
					// mcuWifi.recvInf.u8Recv_Buff[14] = 0x00;
					// mcuWifi.recvInf.u8Recv_Buff[15] = 0x00;

					mcuWifi.sendInf.u16FrameLenBuff = 12;

					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态
					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x00B1:									//APP取消工作
					RECV_DATA_LENGTH_H = 0x00;  				//数据长度（为返回信息的数据长度）
					RECV_DATA_LENGTH_L = 6;

					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;		//返回成功信息

					mcuWifi.recvInf.u8Recv_Buff[12] = 0x00;		//返回菜单编码
					mcuWifi.recvInf.u8Recv_Buff[13] = 0x00;
					mcuWifi.recvInf.u8Recv_Buff[14] = 0x00;
					mcuWifi.recvInf.u8Recv_Buff[15] = 0x00;

//					ChangeMachineStatus(SYS_MODE, SYS_MODE_STANDBY);	//转到待机状态
//					ChangeMachineStatus(LAMP_ENABLE, DISABLE);	//关闭炉灯

					g_sysType = SysModeStandby;
					g_LedOpenFlag = 0;

					mcuWifi.sendInf.u16FrameLenBuff	= 16;
					mcuWifi.u2ReturnSuccesAckFlag = 1;					//需要返回接收成功信息

					mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态
					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x00B2:									//APP查询设备状态
					mcuWifi.u2CmdQueryDeviceStaFlag = 1;
					break;

				case 0x1000:									//开关炉灯
//					ChangeMachineStatus(LAMP_ENABLE, mcuWifi.recvInf.u8Recv_Buff[10]);

					mcuWifi.u2CmdSnapshotflag	= 0;			//取消抓图
					mcuWifi.u8SnapshotCnt		= 0;			//抓图倒计时清零

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;

					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态
					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x1001:									//开关转叉
//					ChangeMachineStatus(ROT_ENABLE, mcuWifi.recvInf.u8Recv_Buff[10]);

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;

					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					// mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态
					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x1002:										//APP参数修改
//					ChangeMachineStatus(NOW_WORK_TEMP_TOP,			mcuWifi.recvInf.u8Recv_Buff[10]);
//					ChangeMachineStatus(NOW_WORK_TEMP_BOT,			mcuWifi.recvInf.u8Recv_Buff[11]);
//					ChangeMachineStatus(NOW_WORK_TIME_ALL_MINUTE,	mcuWifi.recvInf.u8Recv_Buff[12]);

					g_nowStepworkTemp	= mcuWifi.recvInf.u8Recv_Buff[10];
					g_workTimeAll		= mcuWifi.recvInf.u8Recv_Buff[12];

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;
					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x1004:									//预热跳转
					// nowSysMode = (unsigned char)(ReadMachineStatus(SYS_MODE));
					// if(nowSysMode == SYS_MODE_PRE_HEAT)			//若单前是预热阶段，跳转到自主烘焙工作状态
					// {
					// 	ChangeMachineStatus(SYS_MODE, SYS_MODE_DIY_WORKING);
					// 	ChangeMachineStatus(NOW_WORK_STEP, 1);	//从第一步开始执行
					// }

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;
					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x1005:									//预热保持
					//赋值预热保持使能和预热保持时间

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;
					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x2000:									//WIFI模组下发摄像头状态信息
					mcuWifi.u8CammerStatus = mcuWifi.recvInf.u8Recv_Buff[10];
					if(mcuWifi.u8CammerStatus)					//若接收数据非0，表示摄像头损坏
					{
						mcuWifi.u8IdentificationErrData = CAMMER_BROKEN;
					}

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;
					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息
					mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态
					break;

				case 0x820B:									//摄像头抓图
					/* nowSysMode = (unsigned char)(ReadMachineStatus(SYS_MODE));
					// if((nowSysMode == SYS_MODE_STANDBY) || (nowSysMode == SYS_MODE_BLACK) || \
					//    (nowSysMode == SYS_MODE_DIY_SETTING) || (nowSysMode == SYS_MODE_SELF_CHECK) || \
					//    (nowSysMode == SYS_MODE_PID))			//待机，黑屏，自主烘焙设置，自检，PID状态允许抓图
					// {
					// 	if(nowSysMode != SYS_MODE_DIY_SETTING)	//？？？？？？非自主设置状态时，若启动摄像头抓图，返回待机状态？？？？？？？？
					// 	{
					// 		ChangeMachineStatus(SYS_MODE, SYS_MODE_STANDBY);
					// 	}

					// 	mcuWifi.u2CmdSnapshotflag = 1;
					// 	ChangeMachineStatus(LAMP_ENABLE, ENABLE);	//开启炉灯
					// }*/

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x820A:										//设备启动智能烘焙
					// nowSysMode = (unsigned char)(ReadMachineStatus(SYS_MODE));

						mcuWifi.u2CmdSmartBakingFlag = 1;
						// g_sysType = SYS_MODE_SMART_IDENTIFICATION;	//转到智能识别状态

						g_LedOpenFlag = 1;
						//炉灯一直亮，在驱动中开启

					gU8_buzzerType = BUZ_HP_KEY;
					break;

				case 0x2002:									//云服务下发异常信息
					mcuWifi.u8IdentificationErrData = mcuWifi.recvInf.u8Recv_Buff[10];
					mcuWifi.u8BurntStatus			= mcuWifi.recvInf.u8Recv_Buff[11];
					mcuWifi.u8PictureBlur			= mcuWifi.recvInf.u8Recv_Buff[12];
					mcuWifi.u8FoodLocation			= mcuWifi.recvInf.u8Recv_Buff[13];
					mcuWifi.u8FoodSize				= mcuWifi.recvInf.u8Recv_Buff[14];

					mcuWifi.u16RecommandMenu_1 = (((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[17])) << 8) + \
												 ((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[18]));		//只取后面的16位（协议菜单编码为32位）

					mcuWifi.u16RecommandMenu_2 = (((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[21])) << 8) + \
												 ((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[22]));		//只取后面的16位（协议菜单编码为32位）

					mcuWifi.u16RecommandMenu_3 = (((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[25])) << 8) + \
												 ((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[26]));		//只取后面的16位（协议菜单编码为32位）

					mcuWifi.u16RecommandMenu_4 = (((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[29])) << 8) + \
												 ((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[30]));		//只取后面的16位（协议菜单编码为32位）

					mcuWifi.u16RecommandMenu_5 = (((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[33])) << 8) + \
												 ((unsigned int)(mcuWifi.recvInf.u8Recv_Buff[34]));		//只取后面的16位（协议菜单编码为32位）

//					if(nowSysMode == SYS_MODE_SMART_WORKING)
//					{
//						ChangeMachineStatus(SYS_MODE, SYS_MODE_AUTO_ERR);	//跳转到智能识别失败状态
//						//炉灯一直亮，在驱动中开启
//					}
//					else if((nowSysMode == SYS_MODE_PRE_HEAT) || (nowSysMode == SYS_MODE_DIY_WORKING) || (nowSysMode == SYS_MODE_SMART_WORKING))		//预热，自主烘焙，智能烘焙状态时
//					{
//						if(mcuWifi.u8BurntStatus)
//						{
//							mcuWifi.u8BurntStatus = 0;

//							if(((unsigned char)ReadMachineStatus(SYS_MODE)) != SYS_MODE_ERROR)
//							{
//								ChangeMachineStatus(SYS_MODE, SYS_MODE_ERROR);		//转到报警状态
//								ChangeMachineStatus(ERR_NUM, FOOD_BURNT_E09);
//							}
//						}
//					}

					RECV_DATA_LENGTH_H = 0x00;					//数据长度（高8位）
					RECV_DATA_LENGTH_L = 0x02;					//数据长度（低8位）
					mcuWifi.recvInf.u8Recv_Buff[10] = 0x00;		//0000表示成功
					mcuWifi.recvInf.u8Recv_Buff[11] = 0x00;
					mcuWifi.sendInf.u16FrameLenBuff = 12;
					mcuWifi.u2ReturnSuccesAckFlag = 1;			//需要返回接收成功信息

					mcuWifi.u2CmdDeviceReprotflag = 1;			//设备上报状态
					break;
			}
		}

		if(mcuWifi.u2ReturnSuccesAckFlag == 0)					//若不需要返回ACK
		{
			mcuWifi.recvType = RECV_GOING;						//继续执行接收
		}
		else
		{
			//在将ACK信息赋值完后再执行接收
		}
		
	}
}

/*-----------------------------------------------------------------------------
Description:		发送和接收处理
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SendAndRecvDeal(void)
{
	SendUart();

	if(mcuWifi.sendInf.u8IntervalTime)					//发送间隔时间倒计时
	{
		mcuWifi.sendInf.u8IntervalTime--;
	}

	if(mcuWifi.sendInf.u2WifiAckFlag)					//等待响应
	{
		if(mcuWifi.u8AckTimeCnt)
		{
			mcuWifi.u8AckTimeCnt--;
		}

		if(mcuWifi.u8AckTimeCnt == 0)					//响应等待时间超时后，清零需求响应标志，使可以重新发送指令
		{
			mcuWifi.sendInf.u2WifiAckFlag = 0;
		}
	}
	
	RecvUart();
	if(mcuWifi.recvInf.u8OutTime)						//接收数据超时倒计时
	{
		mcuWifi.recvInf.u8OutTime--;
	}

	if(mcuWifi.recvInf.u8OutTime == 0)					//MCU数据接收时间超时，重新开始接收
	{
		mcuWifi.recvInf.u16DataCnt		= 0;
		mcuWifi.recvInf.u2HeadOkFlag_H 	= 0;
		mcuWifi.recvInf.u2HeadOkFlag_L 	= 0;
		// mcuWifi.recvType	 			= RECV_OVER;
	}
}

/*-----------------------------------------------------------------------------
Description:		Wifi处理
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void WifiDealFunction(void)
{
	if(mcuWifi.u2HandsOkflag == 0)							//未握手时
	{
		mcuWifi.u8CmdBuffNum = 0;							//缓存指令数清零
		mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0xCCC0;			//赋值握手指令
		mcuWifi.u8CmdBuffNum++;								//指令缓存值加1

		// if(mcuWifi.u8AckTimeCnt == 0)						//未握手时，响应等待时间超时后，清零需求响应标志，使握手指令一直发送
		// {
		// 	mcuWifi.sendInf.u2WifiAckFlag = 0;
		// }
	}
	else													//已握手成功
	{
		if(mcuWifi.u2CmdSmartlinkflag)						//启动smartLink_start
		{
			mcuWifi.u2CmdSmartlinkflag = 0;
			mcuWifi.u8WifiStatus = SMART_LINK_OK_WAIT;

			mcuWifi.u8CmdBuffNum = 0;
			mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0xCCC2;
			mcuWifi.u8CmdBuffNum++;
		}
		else
		{
			if(mcuWifi.u2CmdFirmwareUpgradeFlag == 1)		//设备要求固件升级指令
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)	//当前命令数未超过缓存值时，加载命令
				{
					mcuWifi.u2CmdFirmwareUpgradeFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0xCCC5;
					mcuWifi.u8CmdBuffNum++;
				}	
			}
			else if(mcuWifi.u2CmdSmartBakingFlag == 1)		//智能烘焙指令
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdSmartBakingFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0x820A;
					mcuWifi.u8CmdBuffNum++;
				}	
			}
			else if(mcuWifi.u2CmdRebootWifiFlag == 1)		//wifi重启指令
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdRebootWifiFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0xCCC9;
					mcuWifi.u8CmdBuffNum++;
				}	
			}
			else if(mcuWifi.u2CmdQueryDeviceStaFlag == 1)	//查询设备状态指令
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdQueryDeviceStaFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0x00B2;
					mcuWifi.u8CmdBuffNum++;
				}
			}	
			else if(mcuWifi.u2CmdDeviceReprotflag == 1)		//设备主动上报状态
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdDeviceReprotflag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0x5000;
					mcuWifi.u8CmdBuffNum++;
				}
			}
			else if(mcuWifi.u2CmdKeyReportFlag == 1)		//按键行为上报
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdKeyReportFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0x6003;
					mcuWifi.u8CmdBuffNum++;
				}
			}
			else if(mcuWifi.u2CmdRemoveLinkFlag == 1)		//解绑指令0x6001
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdRemoveLinkFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0x6001;
					mcuWifi.u8CmdBuffNum++;
				}
			}
			else if(mcuWifi.u2CmdGetWiFiStatusFlag ==1)		//获取Wifi状态指令
			{
				if(mcuWifi.u8CmdBuffNum < CMD_NUM_BUFF_MAX)
				{
					mcuWifi.u2CmdGetWiFiStatusFlag = 0;
					mcuWifi.u16CmdCode_Buff[mcuWifi.u8CmdBuffNum] = 0xCCC7;
					mcuWifi.u8CmdBuffNum++;
				}
			}
			else
			{

			}
		}
	}

	SendAndRecvDeal();										//数据发送和接收处理
}

/*-----------------------------------------------------------------------------
Description:		Wifi串口处理的中断服务函数
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void IntUart0(void) interrupt 4
{
	unsigned char recvBuff = 0;
	
	_push_(INSCON);
	BANK0_SET;

	if(RI)
	{
		RI = 0;

		if(mcuWifi.recvType == RECV_GOING)				//处于接收状态时，处理接收信息
		{
			recvBuff = SBUF;
			
			mcuWifi.recvInf.u8OutTime = RECV_OUT_TIMER;	//重新计接收超时时间

			if((mcuWifi.recvInf.u2HeadOkFlag_H == 1) && (mcuWifi.recvInf.u2HeadOkFlag_L == 1))	//头码的正确
			{
				mcuWifi.recvInf.u16DataCnt++;
				mcuWifi.recvInf.u8Recv_Buff[mcuWifi.recvInf.u16DataCnt] = recvBuff;

				if(mcuWifi.recvInf.u16DataCnt == 9)		//接收到第10个数据，赋值帧的总长度
				{
					mcuWifi.recvInf.u16FrameLen = mcuWifi.recvInf.u8Recv_Buff[8];
					mcuWifi.recvInf.u16FrameLen <<= 8;
					mcuWifi.recvInf.u16FrameLen += mcuWifi.recvInf.u8Recv_Buff[9];
					mcuWifi.recvInf.u16FrameLen += 10;	//加上前十位的帧头
				}
				else
				{
					if(mcuWifi.recvInf.u16DataCnt > 9)
					{
						if(mcuWifi.recvInf.u16DataCnt >= mcuWifi.recvInf.u16FrameLen)	//所有数据接收完成
						{
							mcuWifi.recvInf.u16DataCnt		= 0;
							mcuWifi.recvInf.u2HeadOkFlag_H	= 0;
							mcuWifi.recvInf.u2HeadOkFlag_L	= 0;
							mcuWifi.recvType				= RECV_OVER;
						}
					}
				}
			}
			else if(mcuWifi.recvInf.u2HeadOkFlag_H == 1)	//头码高8位正确
			{
				if(recvBuff == HEADER_CODE_L)				//头码低8位正确
				{
					mcuWifi.recvInf.u2HeadOkFlag_L = 1;

					RECV_HEADER_H = HEADER_CODE_H;
					RECV_HEADER_L = HEADER_CODE_L;

					mcuWifi.recvInf.u16DataCnt = 1;			// 1表示已有2个数据
				}
				else										//头码高位正确后紧接着的数据，头码低8位错误，重新识别高8位
				{
					mcuWifi.recvInf.u2HeadOkFlag_H = 0;
				}
			}
			else											//未识别到头码的高位值
			{
				if(recvBuff == HEADER_CODE_H)				//识别到头码的高位值时，置高位头码标志
				{
					mcuWifi.recvInf.u2HeadOkFlag_H = 1;
				}
			}
		}
	}

	if(TI)
	{
		TI = 0;

		mcuWifi.sendInf.u16DataCnt++;
		if(mcuWifi.sendInf.u16DataCnt >= mcuWifi.sendInf.u16FrameLen)	//帧数据发送完毕
		{
			mcuWifi.sendInf.u16DataCnt = 0;
			mcuWifi.sendType = SEND_OVER;
			mcuWifi.sendInf.u8IntervalTime = SEND_INTERVAL_TIMER;		//重置发送间隔倒计时
		}
		else
		{
			SBUF = mcuWifi.sendInf.u8Send_Buff[mcuWifi.sendInf.u16DataCnt];

			mcuWifi.sendType = SEND_GOING;
		}
	}

	_pop_(INSCON);
}