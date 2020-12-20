#ifndef		_WIFI_H_ 
#define		_WIFI_H_

#define SN_start0							0XFA  
#define SN_start1							0XFB

#define Contract_Verios						0X00 //通讯版本
#define Contract_FrameNum					0X00 //通讯帧总数
#define Contract_FrameNumber				0X00 //通讯帧序号
#define Contract_ReturnCtl00          	 	0x00  //主动发送要求对方回复
#define Contract_ReturnCtl01				0x80  //返回指令
#define Contract_ReturnCtl10				0x40  //主动发送无需对方回复

#define KXModel_H							0x94  //设备型号
#define KXModel_L							0xfc  //设备型号

#define  PROGRAM_VER_GB						0x00 //设备程序版本

#define SEND_INTERVAL_TIMER 				300			//2ms一次
#define RECV_OUT_TIMER						500
#define ACK_TIMER							250	

#define CMD_NUM_MAX             10

typedef struct
{
   
	unsigned char CCC5GoUpAck;
	unsigned char CCC9ReBootAck;
//****************************************************

	//工作状态     0x00B3

	unsigned char McuMumeNumber_0;  //菜案编码 
	unsigned char McuMumeNumber_1;  //菜案编码 
 	unsigned char McuMumeNumber_2;  //菜案编码
	unsigned char McuMumeNumber_3;  //菜案编码 
	unsigned char McuyuyueEn; 		 //预约使能？0x01 使能
	unsigned char McuyuyueFlag; 		 //预约标志位 0x01 默认有预约
	unsigned char McuyuyueTimer_H; 		 //预约时间
	unsigned char McuyuyueTimer_L; 		 //预约时间
	unsigned char McuyureEn; 		 //预热使能？0x01 使能
	unsigned char McuyureFlag; 		 //预热标志位 0x01 默认有预热
	unsigned char McuyureTimer_H; 		 //预热时间
	unsigned char McuyureTimer_L; 		 //预热时间
	unsigned char McuyureKeepFlag; 		 //预热保持
	unsigned char McuyureKeepTimer; 	 //预热保持时间
	unsigned char WorkStep1_Top_Temp; 	 	//工作第一步上管的温度
	unsigned char WorkStep1_Bot_Temp;  		//工作第一步下管的温度
	unsigned char WorkStep1_work_Time;  	//工作第一步工作时间
	unsigned char WorkStep2_Top_Temp;  		//工作第2步上管的温度
	unsigned char WorkStep2_Bot_Temp;	  	//工作第2步下管的温度
	unsigned char WorkStep2_work_Time;  	//工作第2步工作时间
	unsigned char WorkStep3_Top_Temp;		//工作第3步上管的温度
	unsigned char WorkStep3_Bot_Temp;		//工作第3步下管的温度
	unsigned char WorkStep3_work_Time;		//工作第3步工作时间
	unsigned char Temp_Ajust_Line;    		//温度调节 分界点
	unsigned char Temp_Ajust_small;    		//温度调节 小步长
	unsigned char Temp_Ajust_large;    		//温度调节 大步长
	unsigned char Temp_Ajust_samllest;    	//温度调节 最小温度
	unsigned char Temp_Ajust_largeest;    	//温度调节 最大温度
	unsigned char Timer_Ajust_Min;    		//可调最小
	unsigned char Timer_Ajust_Max;    		//可调最大
	unsigned char zhuanchaSta;              //转叉状态 1开 0关
	unsigned char LudengSta;              	//炉灯状态	1开 0关
	unsigned char Resver_1;					//预留
	unsigned char Resver_2;					//预留
	unsigned char Resver_3;					//预留
	unsigned char Resver_4;					//预留
	unsigned char Resver_5;					//预留
	unsigned char Resver_6;					//预留

//********************************************************************

	unsigned char LudengONOFF;	
	unsigned char ZhuanchaONOFF;	
	unsigned char WorkRevise_Top_temp;
	unsigned char WorkRevise_Bot_temp;
	unsigned char WorkRevise_time;
	unsigned char Yurejump;
	unsigned char YureKeep;
	unsigned char YureKeeptime;
	unsigned char CamerSta;	
	unsigned char CamerErr;
	unsigned char Camerlast;
	unsigned char FoodMsg;	
	unsigned char FoodMsgLast;	
	unsigned char AutoLoseCause;
	unsigned char AutoLoseCause_0;	
	unsigned char AutoLoseCause_1;	
	unsigned char AutoLoseCause_3;	
	
}MCURevWifi_str;

extern MCURevWifi_str xdata MCURevWifiDate;

typedef struct
{
	unsigned char McuWorkSta; //工作状态     0x5000
	unsigned char McuMumeNumber_0;  //菜案编码 
	unsigned char McuMumeNumber_1;  //菜案编码 
 	unsigned char McuMumeNumber_2;  //菜案编码 
	unsigned char McuMumeNumber_3;  //菜案编码 
	unsigned char McuyuyueEn; 		 //预约使能？0x01 使能
	unsigned char McuyuyueFlag; 		 //预约标志位 0x01 默认有预约
	unsigned char McuyuyueTimer_H; 		 //预约时间
	unsigned char McuyuyueTimer_L; 		 //预约时间
	unsigned char McuyureEn; 		 //预热使能？0x01 使能
	unsigned char McuyureFlag; 		 //预热标志位 0x01 默认有预热
	unsigned char McuyureTimer_H; 		 //预热时间
	unsigned char McuyureTimer_L; 		 //预热时间
	unsigned char McuyureKeepFlag; 		 //预热保持
	unsigned char McuyureKeepTimer; 	 //预热保持时间
	unsigned char WorkStep1_Top_Temp; 	 	//工作第一步上管的温度
	unsigned char WorkStep1_Bot_Temp;  		//工作第一步下管的温度
	unsigned char WorkStep1_work_Time;  	//工作第一步工作时间
	unsigned char WorkStep2_Top_Temp;  		//工作第2步上管的温度
	unsigned char WorkStep2_Bot_Temp;	  	//工作第2步下管的温度
	unsigned char WorkStep2_work_Time;  	//工作第2步工作时间
	unsigned char WorkStep3_Top_Temp;		//工作第3步上管的温度
	unsigned char WorkStep3_Bot_Temp;		//工作第3步下管的温度
	unsigned char WorkStep3_work_Time;		//工作第3步工作时间
	unsigned char Temp_Ajust_Line;    		//温度调节 分界点
	unsigned char Temp_Ajust_small;    		//温度调节 小步长
	unsigned char Temp_Ajust_large;    		//温度调节 大步长
	unsigned char Temp_Ajust_samllest;    	//温度调节 最小温度
	unsigned char Temp_Ajust_largeest;    	//温度调节 最大温度
	unsigned char Timer_work_leftMin;    	//剩余工作时间
	unsigned char Timer_work_leftsecond;    //剩余工作时间秒钟
	unsigned char Timer_Ajust_Min;    		//可调最小
	unsigned char Timer_Ajust_Max;    		//可调最大
	unsigned char zhuanchaSta;              //转叉状态 1开 0关
	unsigned char LudengSta;              	//炉灯状态	1开 0关
	unsigned char RealTemp_Top;				//上管实时温度
	unsigned char RealTemp_Bot;				//下管实时温度
	unsigned char Resver_1;					//预留
	unsigned char Resver_2;					//预留
	unsigned char Resver_3;					//预留
	unsigned char Resver_4;					//预留
	unsigned char Resver_5;					//预留
	unsigned char Resver_6;					//预留
	unsigned char ErroDate_H;              //故障代码
	unsigned char ErroDate_L;              //故障代码
//****************************************************************************0x6003

	unsigned char KeyNumber; //按键个数
	unsigned char KeyCountN[10];
}MCUSendWifi_str;

extern MCUSendWifi_str xdata MCUSendWifiDate;

#define	MCUSendCmdflag_Date				MCUSendCmdflag_byte02.byte16     //
extern TYPE_BYTE16 MCUSendCmdflag_byte02;//

#define	CmdSmartlinkStart_flag					MCUSendCmdflag_byte02.b1.b0//smartlink
#define	CmdRemovelinkStart_flag					MCUSendCmdflag_byte02.b1.b1//解绑
#define	CmdSbGoUpStart_flag						MCUSendCmdflag_byte02.b1.b2//设备要求固件升级
#define	CmdSbReBootStart_flag					MCUSendCmdflag_byte02.b1.b3//设备要求重启wifi
#define	CmdSbReprotStart_flag					MCUSendCmdflag_byte02.b1.b4//设备上报状态
#define	CmdSbAutoBootStart_flag					MCUSendCmdflag_byte02.b1.b5//设备启动智能烘焙
#define	CmdSbKeyActionStart_flag				MCUSendCmdflag_byte02.b1.b6//按键

#define	SendCmdflag_Date				SendCmdflag_byte02.byte16     //
extern TYPE_BYTE16 SendCmdflag_byte02;//

#define	HandsCmd_OK_flag				SendCmdflag_byte02.b1.b0//握手
#define	McuSendWifi_ACK_flag			SendCmdflag_byte02.b1.b1//需不需要应答  1需要，  0不需要

#define	RevCmdflag_Date				RevCmdflag_byte02.byte16     //
extern TYPE_BYTE16 RevCmdflag_byte02;//

#define	RevWifiEnd_flag					RevCmdflag_byte02.b1.b0//接收数据完成
#define	RevWifiworking_flag				RevCmdflag_byte02.b1.b1//接收数据过程中
#define frameHead_H_Flag				RevCmdflag_byte02.b1.b3//
#define frameHead_L_Flag				RevCmdflag_byte02.b1.b4//
#define MCUSendWorking_Flag				RevCmdflag_byte02.b1.b5//发送中
#define MCUSendStart_Flag				RevCmdflag_byte02.b1.b6//开始发送
#define MCUSmartlinkWorking_Flag		RevCmdflag_byte02.b1.b6//smartlink 中

//***************************************************************************************************

//设备——>模块


typedef struct
{
   
	unsigned char Cmdcode_H;	 	//指令高八位
	unsigned char Cmdcode_L; 		//指令低八位
 	unsigned char LengthDate_H;		//数据长度
 	unsigned char LengthDate_L;
	unsigned char EquNumber_H;		//设备编号高八位
	unsigned char EquNumber_L;		//设备编号低八位
	unsigned char ProtcolVersion;	//通讯协议版本
	unsigned char EquVersion;		//设备固件版本	
	unsigned char Datelarge_H;		//Date区最大长度高八位
	unsigned char Datelarge_L;		//Date区最大长度低八位
	unsigned char HardWareVersion;//设备硬件版本
	unsigned char Remove_sta;		//解绑后模组的状态，0：默认模式 1：进入AP模式     2:进入sarmlink  3：进入蓝牙    

	
}MCUSend_hands;

extern MCUSend_hands xdata MCUSendDate_hands;

typedef struct
{
   
	unsigned char Aanser_H;	 		//应答结果高八位
	unsigned char Aanser_L; 		//应答结果低八位
 	unsigned char WIFISta;			//wifi状态 
 	/*
		00：未联网
		01：无法连接服务器
		02：已经连接路由器
		03：已经连接服务器
		04：已经进入smartlink
		05：预留
		06：预留
		07：sartlink配置成功
		08：ap状态
		

 	*/
   
}MCURev_hands;

extern MCURev_hands xdata MCURevDate_hands;


//***************************************************************************************************



//***************************************************************************************************
//设备——>模块


typedef struct
{
   
	unsigned char Cmdcode_H;	 	//指令高八位
	unsigned char Cmdcode_L; 		//指令低八位
 	unsigned char CmdDtae_H;     	//00
	unsigned char CmdDtae_L;		//00
	
}MCUSend_SmartLink;

extern MCUSend_SmartLink xdata MCUSendDate_SmartLink;

typedef struct
{
   
	unsigned char Aanser_H;	 		//结果高八位
	unsigned char Aanser_L; 		//结果低八位
 
	
}MCURev_SmartLink;

extern MCURev_SmartLink xdata MCURevDate_SmartLink;

//***************************************************************************************************



//***************************************************************************************************
//设备要求固件升级            设备——>模块

typedef struct
{
   
	unsigned char Cmdcode_H;	 	//指令高八位
	unsigned char Cmdcode_L; 		//指令低八位
 	unsigned char LengthDate_H;		//数据长度
	unsigned char LengthDate_L;		//数据长度
	unsigned char WIFISendDateLength_H;  //每一帧的数据长度
	unsigned char WIFISendDateLength_L;
	
}MCUSend_SoftGoUp;

extern MCUSend_SoftGoUp xdata MCUSendDate_SoftGoUp;

typedef struct     //FAFB 00 00 00 80 CCC5 0002 0000 xx
{
   
	unsigned char Aanser_H;	 		//结果高八位
	unsigned char Aanser_L; 		//结果低八位
 
	
}MCURev_SoftGoUp;

extern MCURev_SoftGoUp xdata MCURevDate_SoftGoUp;

//***************************************************************************************************


//***************************************************************************************************
//设备重启WIFI模块         设备——>模块

typedef struct
{
   
	unsigned char Cmdcode_H;	 	//指令高八位
	unsigned char Cmdcode_L; 		//指令低八位
 
	unsigned char SendDate_H;  		//数据
	unsigned char SendDate_L;
	
}MCUSend_ReBoot;

extern MCUSend_ReBoot xdata MCUSendDate_ReBoot;

typedef struct     //FAFB 00 00 00 80 CCC9 0002 0000 xx
{
   
	unsigned char Aanser_H;	 		//结果高八位
	unsigned char Aanser_L; 		//结果低八位
 
	
}MCURev_ReBoot;

extern MCURev_ReBoot xdata MCURevDate_ReBoot;

//***************************************************************************************************


#define	HandsCmd_val	0x01  //握手

extern void Uart0Init(void);
extern void WifiDealFunction(void);
void CmdHandVariableInit(void);
void CmdSmartLinkVariableInit(void);
void CmdSoftGoUpVariableInit(void);
void CmdReBootVariableInit(void);
extern void Cmd0x5000VariableIint(void);

#endif
