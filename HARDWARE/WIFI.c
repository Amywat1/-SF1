#include "header.h"

TYPE_BYTE16 RevCmdflag_byte02;//
TYPE_BYTE16 SendCmdflag_byte02;//
TYPE_BYTE16 MCUSendCmdflag_byte02;//

#define  BUF_LENGTH_NUM    80

unsigned char xdata Send_Buf[BUF_LENGTH_NUM];
unsigned char xdata Recv_Buf[BUF_LENGTH_NUM];

MCURevWifi_str xdata MCURevWifiDate;
MCUSendWifi_str xdata MCUSendWifiDate;

MCUSend_hands xdata	MCUSendDate_hands;
MCURev_hands xdata	MCURevDate_hands;

MCUSend_SmartLink xdata MCUSendDate_SmartLink;
MCURev_SmartLink xdata MCURevDate_SmartLink;

MCUSend_SoftGoUp xdata MCUSendDate_SoftGoUp;
MCURev_SoftGoUp xdata MCURevDate_SoftGoUp;

MCUSend_ReBoot xdata MCUSendDate_ReBoot;
MCURev_ReBoot xdata  MCURevDate_ReBoot;

//**********************************************************

unsigned int  xdata recvFrameLen   = 0;				//接收数据长度
unsigned int  xdata recvDataCount  = 0;				//结束计数
unsigned int  xdata recvOutTime    = 0;				//接收超时计数器

unsigned int  xdata sendIntervalTime = 0;				//发送间隔计数器
unsigned int  xdata ackTime        = 0;				//应答间隔时间计数器 

unsigned int  xdata sendFrameLen   = 0;				//发送帧长度
unsigned int  xdata sendDataCount  = 0;				//发送数据数

unsigned int  xdata CmdCode_Buf[16];   				//设置缓冲区
unsigned char xdata cmdNum         = 0;
unsigned int  xdata cmdCode        = 0x0000;			//指令码

unsigned char xdata firmWareHandle_flag = 0x00;
unsigned char xdata recvFirmWareEdit = 0x00;			//固件

//**********************************************************

#define   N_TXD P0_0
#define   N_RXD P2_7

void CmdHandVariableInit(void)
{
//send

	MCUSendDate_hands.Cmdcode_H=0xcc;			//指令高八位
	MCUSendDate_hands.Cmdcode_L=0xc0;			//指令低八位
	MCUSendDate_hands.LengthDate_H=0x00;		//数据长度
	MCUSendDate_hands.LengthDate_L=0x08;		//数据长度
	MCUSendDate_hands.EquNumber_H=0x94;			//设备编号高八位
	MCUSendDate_hands.EquNumber_L=0xfc;			//设备编号低八位
	MCUSendDate_hands.ProtcolVersion=0x00;		//通讯协议版本
	MCUSendDate_hands.EquVersion=0x00;			//设备固件版本	
	MCUSendDate_hands.Datelarge_H=0x00;			//Date区最大长度高八位
	MCUSendDate_hands.Datelarge_L=0xff;			//Date区最大长度低八位
	MCUSendDate_hands.HardWareVersion=0x00;		//设备硬件版本
	MCUSendDate_hands.Remove_sta=0x00;			//解绑后模组的状态，0：默认模式 1：进入AP模式	 2:进入sarmlink  3：进入蓝牙 

//rev

	MCURevDate_hands.Aanser_H=0x00;	 		//应答结果高八位 0x0000成功      ，0001失败
	MCURevDate_hands.Aanser_L=0x00; 			//应答结果低八位
 	MCURevDate_hands.WIFISta=0x00;			//wifi状态 
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
}

void CmdSmartLinkVariableInit(void)
{
	//send

	MCUSendDate_SmartLink.Cmdcode_H=0xcc;		//指令高八位
	MCUSendDate_SmartLink.Cmdcode_L=0xc2;		//指令低八位
	MCUSendDate_SmartLink.CmdDtae_H=0x00;		//00
	MCUSendDate_SmartLink.CmdDtae_L=0x00;		//00

	//rev
	MCURevDate_SmartLink.Aanser_H=0x00;	 		//结果高八位
	MCURevDate_SmartLink.Aanser_L=0x00; 		//结果低八位
}

void CmdSoftGoUpVariableInit(void)
{
	//send

	MCUSendDate_SoftGoUp.Cmdcode_H=0xcc;	 	//指令高八位
	MCUSendDate_SoftGoUp.Cmdcode_L=0xc5; 		//指令低八位
 	MCUSendDate_SoftGoUp.LengthDate_H=0x00;		//数据长度
	MCUSendDate_SoftGoUp.LengthDate_L=0x02;		//数据长度
	MCUSendDate_SoftGoUp.WIFISendDateLength_H=0x00;  //每一帧的数据长度暂定 128个字节  一帧
	MCUSendDate_SoftGoUp.WIFISendDateLength_L=0x80;

	//rev
	MCURevDate_SoftGoUp.Aanser_H=0x00;	 		//结果高八位
	MCURevDate_SoftGoUp.Aanser_L=0x00; 			//结果低八位
}

void CmdReBootVariableInit(void)
{
	//send

	MCUSendDate_ReBoot.Cmdcode_H=0xcc;	 	//指令高八位
	MCUSendDate_ReBoot.Cmdcode_L=0xc9; 		//指令低八位
 
	MCUSendDate_ReBoot.SendDate_H=0x00;  		//数据
	MCUSendDate_ReBoot.SendDate_L=0x00;

	//rev
	MCURevDate_ReBoot.Aanser_H=0x00;	 		//结果高八位
	MCURevDate_ReBoot.Aanser_L=0x00; 			//结果低八位
}

void Cmd0x5000VariableIint(void)
{



	MCURevWifiDate.LudengONOFF = 0;





	MCUSendWifiDate.McuWorkSta=0; //工作状态     0x5000

	MCUSendWifiDate.McuMumeNumber_0=0;  //菜案编码 

	MCUSendWifiDate.McuMumeNumber_1=0;  //菜案编码 

	MCUSendWifiDate.McuMumeNumber_2=0;  //菜案编码 

	MCUSendWifiDate.McuMumeNumber_3=0;  //菜案编码 

	MCUSendWifiDate.McuyuyueEn=0; 		 //预约使能？0x01 使能

	MCUSendWifiDate.McuyuyueFlag=0; 	 //预约标志位 0x01 默认有预约

	MCUSendWifiDate.McuyuyueTimer_H=0; 	  //预约时间

	MCUSendWifiDate.McuyuyueTimer_L=0; 	  //预约时间

	MCUSendWifiDate.McuyureEn=0; 		 //预热使能？0x01 使能

	MCUSendWifiDate.McuyureFlag=0; 		 //预热标志位 0x01 默认有预热

	MCUSendWifiDate.McuyureTimer_H=0; 		 //预热时间

	MCUSendWifiDate.McuyureTimer_L=0; 		 //预热时间

	MCUSendWifiDate.McuyureKeepFlag=0; 		 //预热保持

	MCUSendWifiDate.McuyureKeepTimer=0; 	 //预热保持时间

	MCUSendWifiDate.WorkStep1_Top_Temp=0; 	 	//工作第一步上管的温度

	MCUSendWifiDate.WorkStep1_Bot_Temp=0;  		//工作第一步下管的温度

	MCUSendWifiDate.WorkStep1_work_Time=0;  	//工作第一步工作时间

	MCUSendWifiDate.WorkStep2_Top_Temp=0;  		//工作第2步上管的温度

	MCUSendWifiDate.WorkStep2_Bot_Temp=0;	  	//工作第2步下管的温度

	MCUSendWifiDate.WorkStep2_work_Time=0;  	//工作第2步工作时间


	MCUSendWifiDate.WorkStep3_Top_Temp=0;		//工作第3步上管的温度

	MCUSendWifiDate.WorkStep3_Bot_Temp=0;		//工作第3步下管的温度
		
	MCUSendWifiDate.WorkStep3_work_Time=0;		//工作第3步工作时间

	MCUSendWifiDate.Temp_Ajust_Line=0;    		//温度调节 分界点

	MCUSendWifiDate.Temp_Ajust_small=0;    		//温度调节 小步长

	MCUSendWifiDate.Temp_Ajust_large=0;    		//温度调节 大步长

	MCUSendWifiDate.Temp_Ajust_samllest=0;    	//温度调节 最小温度

	MCUSendWifiDate.Temp_Ajust_largeest=0;    	//温度调节 最大温度

	MCUSendWifiDate.Timer_work_leftMin=0;    	//剩余工作时间

	MCUSendWifiDate.Timer_work_leftsecond=0;    //剩余工作时间秒钟

	MCUSendWifiDate.Timer_Ajust_Min=0;    		//可调最小

	MCUSendWifiDate.Timer_Ajust_Max=0;    		//可调最大

	MCUSendWifiDate.zhuanchaSta=0;              //转叉状态 1开 0关

	MCUSendWifiDate.LudengSta=0;              	//炉灯状态	1开 0关

	MCUSendWifiDate.RealTemp_Top=0;				//上管实时温度

	MCUSendWifiDate.RealTemp_Bot=0;				//下管实时温度

	MCUSendWifiDate.Resver_1=0;					//预留
	MCUSendWifiDate.Resver_2=0;					//预留
	MCUSendWifiDate.Resver_3=0;					//预留
	MCUSendWifiDate.Resver_4=0;					//预留
	MCUSendWifiDate.Resver_5=0;					//预留
	MCUSendWifiDate.Resver_6=0;					//预留

	MCUSendWifiDate.ErroDate_H=0;              //故障代码

	MCUSendWifiDate.ErroDate_L=0;              //故障代码		

	MCUSendWifiDate.KeyNumber=0;

//	TemCharDataInt(&MCUSendWifiDate.KeyCountN[0],10);

	MCUSendCmdflag_Date=0;

	SendCmdflag_Date=0;

	RevCmdflag_Date=0;
}

/*-----------------------------------------------------------------------------
Description:		初始化串口IO
					电源板通讯：TXD：P0.0；RXD：P2.7
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void Uart0IOSet(void)
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
Description:		初始化串口配置
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void Uart0Init(void)
{	

	//UartDataInit();       //UART 数据复位  
	Uart0IOSet();	

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


//串口中断函数入口
void INT_EUART0(void) interrupt 4
{
	//unsigned char data UartRevBak;
	//unsigned char data i; 
	unsigned char recvSbuf = 0;
	
	_push_(INSCON);
	BANK0_SET;	
	if(RI)	
	{
		RI = 0;

		if(!RevWifiEnd_flag)
		{
			recvSbuf = SBUF;
			RevWifiworking_flag= 1;
			recvOutTime = RECV_OUT_TIMER;

			if((frameHead_H_Flag==1) && (frameHead_L_Flag== 1))
			{
				recvDataCount++;
				Recv_Buf[recvDataCount]=recvSbuf;

				if(recvDataCount == 9)
				{
					recvFrameLen = Recv_Buf[8];
					recvFrameLen <<= 8;
					recvFrameLen += Recv_Buf[9];
					recvFrameLen += 10;				//加上前十位的帧头
				}
				if(recvDataCount > 9)
				{
					if(recvDataCount >= recvFrameLen)
					{
						recvDataCount = 0;
						frameHead_H_Flag = 0;
						frameHead_L_Flag = 0;
						RevWifiworking_flag= 0;
						RevWifiEnd_flag= 1;	
					}
				}
			}
			
			if((frameHead_H_Flag== 1) && (frameHead_L_Flag== 0))
			{
				if(recvSbuf == 0xFB)
				{
					frameHead_L_Flag=1;
					Recv_Buf[0] = 0xFA;
					Recv_Buf[1] = 0xFB;
					recvDataCount	= 1;
				}
				else
				{
					frameHead_H_Flag= 0;
				}
			}
			
			if((frameHead_H_Flag== 0) && (frameHead_L_Flag== 0))
			{
				if(recvSbuf == 0xFA)
				{
					frameHead_H_Flag= 1;	
				}
			}
		}
	}

	if(TI)
	{
		TI=0;


		sendDataCount++;
		if(sendDataCount == sendFrameLen)
		{
			sendDataCount = 0;
			MCUSendWorking_Flag=0;	//
			sendIntervalTime = SEND_INTERVAL_TIMER;
		}
		else
		{
			SBUF = Send_Buf[sendDataCount];	//
			MCUSendWorking_Flag= 1;
		}


	}
	BANK0_SET;			
	_pop_(INSCON);
}

/****************************
函数名称：
函数功能：计算八位校验位
函数参数：
函数返回值：
作者时间：Zhengmingwei-20150728
备    注：  
修改纪要:
*******************************/
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

/****************************
函数名称：
函数功能：解密八位校验位
函数参数：
函数返回值：
作者时间：Zhengmingwei-20150728
备    注：  
修改纪要:
*******************************/
unsigned char SumVerify(unsigned char *PBuf, unsigned int Len)
{
	unsigned char Verify = 0;
	while(Len--)
	{
	  Verify = Verify + (*PBuf);
	  PBuf ++;
	}
	return (Verify == 0); //如果verify为0 返回1 否则返回0
}

void KeyAction_buf(uchar lenNumber)
{
	uchar i=0;

	for(i=0;i<lenNumber;i++)
	{

		Send_Buf[11+i]=MCUSendWifiDate.KeyCountN[i];

	}	
	
}

/****************************
函数名称：
函数功能：
函数参数：
函数返回值：
作者时间：
备    注：  
修改纪要:
*******************************/
unsigned int SendUartData(unsigned int len)
{
	Send_Buf[0] = SN_start0;        						//帧头高位
	Send_Buf[1] = SN_start1;				 					//帧头低位
	Send_Buf[2] = Contract_Verios;						//通信版本
	Send_Buf[3] = 0x00;					 				//帧总数
	Send_Buf[4] = 0x00;					 				//帧序号
	Send_Buf[5]	= 0x00;					 				//增加控制字; 0x00:主动发送指令要求对方回复
	Send_Buf[len] = SumMakeVerify(Send_Buf,len);
	sendDataCount = 0;
	McuSendWifi_ACK_flag=0;		 				//1表示需要应答标识位，0表示不需要
	ackTime = ACK_TIMER;
	MCUSendWorking_Flag=1;
	SBUF = Send_Buf[0];
	return (len + 1);
}

void SendUart(void)
{
	unsigned char i = 0;
	if((McuSendWifi_ACK_flag== 0) && (MCUSendWorking_Flag== 0) && (sendIntervalTime == 0))
	{
		if(cmdNum > 0)
		{
			cmdCode = CmdCode_Buf[0];
			cmdNum--;
			for(i = 0;i < cmdNum;i++)
			{
				CmdCode_Buf[i] = CmdCode_Buf[i + 1];
			}
			MCUSendStart_Flag = 1;		
		}
	}

	if(MCUSendStart_Flag)
	{
		MCUSendStart_Flag= 0;
		switch(cmdCode)
		{
			case 0xCCC0:
				Send_Buf[6] = 0xcc;					//指令高位 
				Send_Buf[7] = 0xc0;					//指令低位
				Send_Buf[8] = 0x00;					//数据长度
				Send_Buf[9] = 0x06;					//数据长度
				Send_Buf[10] = KXModel_H; 		 	//设备型号高位
				Send_Buf[11] = KXModel_L ; 		 	//设备型号低位 电烤箱KX-38I95 0x3c02
				Send_Buf[12] = 0x00; 				//设备通信版本
				Send_Buf[13] = PROGRAM_VER_GB;		//设备固件版本
				Send_Buf[14] = 0x00; 				//通信数据长度高位
				Send_Buf[15] = 0xFF; 		 		//通信数据长度低位
				sendFrameLen = 16;
				sendFrameLen = SendUartData(sendFrameLen);	
				McuSendWifi_ACK_flag=1;
				
				break;
			case 0xCCC2:
				MCUSmartlinkWorking_Flag = 1;
				//smarkLinkStatus = 0x00;  			//去掉解决WiFi状态灯闪烁的问题
				Send_Buf[6] = 0xcc;					//指令高位 
				Send_Buf[7] = 0xc2;					//指令低位
				Send_Buf[8] = 0x00;					//数据长度
				Send_Buf[9] = 0x00;					//数据长度
				sendFrameLen = 10;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=1;
				break;
			case 0x6001:								//解绑定指令
				Send_Buf[6] = 0x60;
				Send_Buf[7] = 0x01;
				Send_Buf[8] = 0;						//数据长度高位
				Send_Buf[9] = 2;						//数据长度低位
				Send_Buf[10] = 0x00; 					
				Send_Buf[11] = 0x00;
				sendFrameLen = 12;
				sendFrameLen = SendUartData(sendFrameLen);
				Send_Buf[5] = 0x40;
				McuSendWifi_ACK_flag= 0; 				
				break;
			case 0xCCC5:                             //设备要求固件升级   
				Send_Buf[6] = 0xcc;					//指令高位 
				Send_Buf[7] = 0xc5;					//指令低位
				Send_Buf[8] = 0x00;					//数据长度
				Send_Buf[9] = 0x02;					//数据长度
				Send_Buf[10] = 0x00;				//每帧长度高位
				Send_Buf[11] = 0x01;				//每帧长度低位 暂定1
				sendFrameLen = 12;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=1;
				break;

			
			case 0xCCC9:                             //重启模块   
				Send_Buf[6] = 0xcc;					//指令高位 
				Send_Buf[7] = 0xc9;					//指令低位
				Send_Buf[8] = 0x00;					//数据长度
				Send_Buf[9] = 0x00;					//数据长度
				sendFrameLen = 10;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=1;
				break;

			case 0x5000:                            //主动上报状态   
				Send_Buf[6] = 0x50;					//指令高位 
				Send_Buf[7] = 0x00;					//指令低位
				Send_Buf[8] = 0x00;					//数据长度
				Send_Buf[9] = 45;					//数据长度
				Send_Buf[10] =MCUSendWifiDate.McuWorkSta; //工作状态     0x5000

				Send_Buf[11] =MCUSendWifiDate.McuMumeNumber_0;  //菜案编码 

				Send_Buf[12] =MCUSendWifiDate.McuMumeNumber_1;  //菜案编码 
				
			 	Send_Buf[13] =MCUSendWifiDate.McuMumeNumber_2;  //菜案编码 

				Send_Buf[14] =MCUSendWifiDate.McuMumeNumber_3;  //菜案编码 

				Send_Buf[15] =MCUSendWifiDate.McuyuyueEn; 		 //预约使能？0x01 使能

				Send_Buf[16] =MCUSendWifiDate.McuyuyueFlag; 	 //预约标志位 0x01 默认有预约

				Send_Buf[17] =MCUSendWifiDate.McuyuyueTimer_H; 	  //预约时间

				Send_Buf[18] =MCUSendWifiDate.McuyuyueTimer_L; 	  //预约时间

				Send_Buf[19] =MCUSendWifiDate.McuyureEn; 		 //预热使能？0x01 使能

				Send_Buf[20] =MCUSendWifiDate.McuyureFlag; 		 //预热标志位 0x01 默认有预热

				Send_Buf[21] =MCUSendWifiDate.McuyureTimer_H; 		 //预热时间

				Send_Buf[22] =MCUSendWifiDate.McuyureTimer_L; 		 //预热时间

				Send_Buf[23] =MCUSendWifiDate.McuyureKeepFlag; 		 //预热保持

				Send_Buf[24] =MCUSendWifiDate.McuyureKeepTimer; 	 //预热保持时间

				Send_Buf[25] =MCUSendWifiDate.WorkStep1_Top_Temp; 	 	//工作第一步上管的温度

				Send_Buf[26] =MCUSendWifiDate.WorkStep1_Bot_Temp;  		//工作第一步下管的温度
				
				Send_Buf[27] =MCUSendWifiDate.WorkStep1_work_Time;  	//工作第一步工作时间

				Send_Buf[28] =MCUSendWifiDate.WorkStep2_Top_Temp;  		//工作第2步上管的温度

				Send_Buf[29] =MCUSendWifiDate.WorkStep2_Bot_Temp;	  	//工作第2步下管的温度
				
				Send_Buf[30] =MCUSendWifiDate.WorkStep2_work_Time;  	//工作第2步工作时间

				
				Send_Buf[31] =MCUSendWifiDate.WorkStep3_Top_Temp;		//工作第3步上管的温度
				
				Send_Buf[32] =MCUSendWifiDate.WorkStep3_Bot_Temp;		//工作第3步下管的温度
					
				Send_Buf[33] =MCUSendWifiDate.WorkStep3_work_Time;		//工作第3步工作时间

				Send_Buf[34] =MCUSendWifiDate.Temp_Ajust_Line;    		//温度调节 分界点
				
				Send_Buf[35] =MCUSendWifiDate.Temp_Ajust_small;    		//温度调节 小步长

				Send_Buf[36] =MCUSendWifiDate.Temp_Ajust_large;    		//温度调节 大步长

				Send_Buf[37] =MCUSendWifiDate.Temp_Ajust_samllest;    	//温度调节 最小温度

				Send_Buf[38] =MCUSendWifiDate.Temp_Ajust_largeest;    	//温度调节 最大温度

				Send_Buf[39] =MCUSendWifiDate.Timer_work_leftMin;    	//剩余工作时间

				Send_Buf[40] =MCUSendWifiDate.Timer_work_leftsecond;    //剩余工作时间秒钟

				Send_Buf[41] =MCUSendWifiDate.Timer_Ajust_Min;    		//可调最小

				Send_Buf[42] =MCUSendWifiDate.Timer_Ajust_Max;    		//可调最大

				Send_Buf[43] =MCUSendWifiDate.zhuanchaSta;              //转叉状态 1开 0关

				Send_Buf[44] =MCUSendWifiDate.LudengSta;              	//炉灯状态	1开 0关

				Send_Buf[45] =MCUSendWifiDate.RealTemp_Top;				//上管实时温度

				Send_Buf[46] =MCUSendWifiDate.RealTemp_Bot;				//下管实时温度

				Send_Buf[47] =MCUSendWifiDate.Resver_1;					//预留
				Send_Buf[48] =MCUSendWifiDate.Resver_2;					//预留
				Send_Buf[49] =MCUSendWifiDate.Resver_3;					//预留
				Send_Buf[50] =MCUSendWifiDate.Resver_4;					//预留
				Send_Buf[51] =MCUSendWifiDate.Resver_5;					//预留
				Send_Buf[52] =MCUSendWifiDate.Resver_6;					//预留
			

				Send_Buf[53] =MCUSendWifiDate.ErroDate_H;              //故障代码

				Send_Buf[54] =MCUSendWifiDate.ErroDate_L;              //故障代码	
							
				sendFrameLen = 55;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=0;
				break;
			
			case 0x820A:
				
				Send_Buf[6] = 0x82;					//指令高位 
				Send_Buf[7] = 0x0A;					//指令低位
				Send_Buf[8] = 0x00;					//数据长度
				Send_Buf[9] = 0x00;					//数据长度
				sendFrameLen = 10;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=0;
				break;


			case 0x6003:
				
				Send_Buf[6] = 0x60;									//指令高位 
				Send_Buf[7] = 0x03;									//指令低位
				Send_Buf[8] = 0x00;									//数据长度
				Send_Buf[9] = (MCUSendWifiDate.KeyNumber+1);		//数据长度
				Send_Buf[10] = (MCUSendWifiDate.KeyNumber);	     	//按键个数
				KeyAction_buf(MCUSendWifiDate.KeyNumber);
				sendFrameLen=(MCUSendWifiDate.KeyNumber+11);
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=0;
				break;
				

				
				default:
				break;
		}	
	}
}                  


/****************************

*******************************/
unsigned int SendUartAck(unsigned int len)
{
   	unsigned int i;
	McuSendWifi_ACK_flag= 0; //不需要应答
	//cmdNum = 0;
	for(i = 0; i < len;i++)
	{
		Send_Buf[i] = Recv_Buf[i];
	}
	Send_Buf[5] = 0x80; //mcu接收成功回复，不用wifi答复
	Send_Buf[len] = SumMakeVerify(Send_Buf,len);
	sendDataCount = 0;
	MCUSendWorking_Flag= 1;
	SBUF = Send_Buf[0];
	return(len + 1);
}



void CommRecvData(void)//接收wifi数据成功
{
	Recv_Buf[8]  = 0x00;
	Recv_Buf[9]  = 0x02;
	Recv_Buf[10] = 0x00;
	Recv_Buf[11] = 0x00; 	 
	sendFrameLen = 12;
	sendFrameLen = SendUartAck(sendFrameLen);
}

void returnData(void)//接收wifi数据成功
{
	// Recv_Buf[8]  = 0x00;
	// Recv_Buf[9]  = 0x02;
	// Recv_Buf[10] = 0x00;
	// Recv_Buf[11] = 0x00; 	 
	sendFrameLen = 11;
	sendFrameLen = SendUartAck(sendFrameLen);
}


/****************************
函数名称：
函数功能：
函数参数：
函数返回值：
作者时间：
备    注：  
修改纪要:
*******************************/
void RecvUart(void)
{
	unsigned char verify;
	unsigned int cmdRecv;

   unsigned char i = 0;
   unsigned char j = 0;
   
	if(RevWifiworking_flag== 0)
	{
		if(RevWifiEnd_flag)
		{
			RevWifiEnd_flag=0;
			verify = SumVerify(Recv_Buf,(recvFrameLen + 1));
			if(verify == 1)
			{
				cmdRecv = Recv_Buf[6];
				cmdRecv = cmdRecv << 8;
				cmdRecv = cmdRecv + Recv_Buf[7];
				switch(cmdRecv)
				{
					case 	0xCCC0:
						if((Recv_Buf[10] == 0x00) && (Recv_Buf[11] == 0x00))	    
						{
							HandsCmd_OK_flag=1;		//握手成功
							MCURevDate_hands.WIFISta = Recv_Buf[12];
							McuSendWifi_ACK_flag=0;
							cmdNum = 0;
						}
						break;
					case 	0xCCC2:
						if((Recv_Buf[10] == 0x00) && (Recv_Buf[11] == 0x00))
						{
							McuSendWifi_ACK_flag=0;
							CmdSmartlinkStart_flag= 0;	//smartLink指令
							cmdNum = 0;
						}
						break;	
					case 0xCCD0:
						if(Recv_Buf[5] == 0x00)
						{
							MCURevDate_hands.WIFISta=Recv_Buf[10];
							CommRecvData();
						}
						else //smartlink 配置中
						{
							MCURevDate_hands.WIFISta=Recv_Buf[10];
						}
						
						if(MCUSmartlinkWorking_Flag)
						{
							if((MCURevDate_hands.WIFISta != 0) && (MCURevDate_hands.WIFISta != 1))	//已连路由器（指示灯常亮）
							{
								MCUSmartlinkWorking_Flag = 0; 
								CmdSmartlinkStart_flag= 0;	//程序更改防止一直发送smartLink指令
							}
						}
						break;

					case 0xCCC5:   //CCC5设备要求固件升级
						if((Recv_Buf[10] == 0x00)&&(Recv_Buf[10] == 0x00))//成功
						{
							MCURevWifiDate.CCC5GoUpAck=1;	
						}
						else //
						{
							MCURevWifiDate.CCC5GoUpAck=0;
						}
						
						
						break;
					case 0xCCC9:   //重启模块
						if((Recv_Buf[10] == 0x00)&&(Recv_Buf[10] == 0x00))//成功
						{
							MCURevWifiDate.CCC9ReBootAck=1;	
						}
						else //
						{
							MCURevWifiDate.CCC9ReBootAck=0;	
						}
						
						break;		
				
					case 0xCCD1:									//固件升级握手指令
						if(g_sysType == 1) //待机
						{
							recvFirmWareEdit = Recv_Buf[10];
							/*Sector_Erase(0x0300,1); 			//固件升级擦除密钥
							Byte_Write1(0x0300,0x55,1); 		//写入密钥0x55
							Byte_Write1(0x0301,0xaa,1); 		//写入密钥0xaa */	
							Recv_Buf[8] = 0;
							Recv_Buf[9] = 4;
							Recv_Buf[10] = 0x00;
							Recv_Buf[11] = 0x00;
							Recv_Buf[12] = 0x01;
							Recv_Buf[13] = 0x00; 
							sendFrameLen = 14;
							sendFrameLen = SendUartAck(sendFrameLen); 
							firmWareHandle_flag = 0x01;				
						}
						break;
					case	0x00B1:										//取消指令			
						//g_systemState = SYS_DEFAULT_STAND_BY;
						//g_buzzerType  = BUZ_TYP_KEY_AUTO;
						Recv_Buf[8] = 0;
						Recv_Buf[9] = 6;
						Recv_Buf[10] = 0x00;
						Recv_Buf[11] = 0x00;
						Recv_Buf[12] = MCUSendWifiDate.McuMumeNumber_0;
						Recv_Buf[13] = MCUSendWifiDate.McuMumeNumber_0;
						Recv_Buf[14] = MCUSendWifiDate.McuMumeNumber_0;
						Recv_Buf[15] = MCUSendWifiDate.McuMumeNumber_0;	

						sendFrameLen = 16;
						sendFrameLen = SendUartAck(sendFrameLen);						
						break;
					case 0x00B2:
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						Recv_Buf[8]  = 0x00;  //数据长度
						Recv_Buf[9]  = 43;
						
						Recv_Buf[10] =MCUSendWifiDate.McuWorkSta; //工作状态     0x5000

						Recv_Buf[11] =MCUSendWifiDate.McuMumeNumber_0;  //菜案编码 

						Recv_Buf[12] =MCUSendWifiDate.McuMumeNumber_1;  //菜案编码 
				
			 			Recv_Buf[13] =MCUSendWifiDate.McuMumeNumber_2;  //菜案编码 

						Recv_Buf[14] =MCUSendWifiDate.McuMumeNumber_3;  //菜案编码 

						Recv_Buf[15] =MCUSendWifiDate.McuyuyueEn; 		 //预约使能？0x01 使能

						Recv_Buf[16] =MCUSendWifiDate.McuyuyueFlag; 	 //预约标志位 0x01 默认有预约

						Recv_Buf[17] =MCUSendWifiDate.McuyuyueTimer_H; 	  //预约时间

						Recv_Buf[18] =MCUSendWifiDate.McuyuyueTimer_L; 	  //预约时间

						Recv_Buf[19] =MCUSendWifiDate.McuyureEn; 		 //预热使能？0x01 使能

						Recv_Buf[20] =MCUSendWifiDate.McuyureFlag; 		 //预热标志位 0x01 默认有预热

						Recv_Buf[21] =MCUSendWifiDate.McuyureTimer_H; 		 //预热时间

						Recv_Buf[22] =MCUSendWifiDate.McuyureTimer_L; 		 //预热时间

						Recv_Buf[23] =MCUSendWifiDate.McuyureKeepFlag; 		 //预热保持

						Recv_Buf[24] =MCUSendWifiDate.McuyureKeepTimer; 	 //预热保持时间

						Recv_Buf[25] =MCUSendWifiDate.WorkStep1_Top_Temp; 	 	//工作第一步上管的温度

						Recv_Buf[26] =MCUSendWifiDate.WorkStep1_Bot_Temp;  		//工作第一步下管的温度
						
						Recv_Buf[27] =MCUSendWifiDate.WorkStep1_work_Time;  	//工作第一步工作时间

						Recv_Buf[28] =MCUSendWifiDate.WorkStep2_Top_Temp;  		//工作第2步上管的温度

						Recv_Buf[29] =MCUSendWifiDate.WorkStep2_Bot_Temp;	  	//工作第2步下管的温度
						
						Recv_Buf[30] =MCUSendWifiDate.WorkStep2_work_Time;  	//工作第2步工作时间

						
						Recv_Buf[31] =MCUSendWifiDate.WorkStep3_Top_Temp;		//工作第3步上管的温度
						
						Recv_Buf[32] =MCUSendWifiDate.WorkStep3_Bot_Temp;		//工作第3步下管的温度
							
						Recv_Buf[33] =MCUSendWifiDate.WorkStep3_work_Time;		//工作第3步工作时间

						Recv_Buf[34] =MCUSendWifiDate.Temp_Ajust_Line;    		//温度调节 分界点
						
						Recv_Buf[35] =MCUSendWifiDate.Temp_Ajust_small;    		//温度调节 小步长

						Recv_Buf[36] =MCUSendWifiDate.Temp_Ajust_large;    		//温度调节 大步长

						Recv_Buf[37] =MCUSendWifiDate.Temp_Ajust_samllest;    	//温度调节 最小温度

						Recv_Buf[38] =MCUSendWifiDate.Temp_Ajust_largeest;    	//温度调节 最大温度

						Recv_Buf[39] =MCUSendWifiDate.Timer_work_leftMin;    	//剩余工作时间

						Recv_Buf[40] =MCUSendWifiDate.Timer_work_leftsecond;    //剩余工作时间秒钟

						Recv_Buf[41] =MCUSendWifiDate.Timer_Ajust_Min;    		//可调最小

						Recv_Buf[42] =MCUSendWifiDate.Timer_Ajust_Max;    		//可调最大

						Recv_Buf[43] =MCUSendWifiDate.zhuanchaSta;              //转叉状态 1开 0关

						Recv_Buf[44] =MCUSendWifiDate.LudengSta;              	//炉灯状态	1开 0关

						Recv_Buf[45] =MCUSendWifiDate.Resver_1;					//预留
						Recv_Buf[46] =MCUSendWifiDate.Resver_2;					//预留
						Recv_Buf[47] =MCUSendWifiDate.Resver_3;					//预留
						Recv_Buf[48] =MCUSendWifiDate.Resver_4;					//预留
						Recv_Buf[49] =MCUSendWifiDate.Resver_5;					//预留
						Recv_Buf[50] =MCUSendWifiDate.Resver_6;					//预留
					

						Recv_Buf[51] =MCUSendWifiDate.ErroDate_H;              //故障代码

						Recv_Buf[52] =MCUSendWifiDate.ErroDate_L;              //故障代码	
									
	                  	sendFrameLen = 53;
						sendFrameLen = SendUartAck(sendFrameLen);								
						break;

					case 0x00B3:
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						MCURevWifiDate.McuMumeNumber_0=Recv_Buf[9];  //菜案编码 

						MCURevWifiDate.McuMumeNumber_1=Recv_Buf[10];  //菜案编码 
						
					 	MCURevWifiDate.McuMumeNumber_2=Recv_Buf[11];  //菜案编码 

						MCURevWifiDate.McuMumeNumber_3=Recv_Buf[12];  //菜案编码 

						MCURevWifiDate.McuyuyueEn=Recv_Buf[13]; 		 //预约使能？0x01 使能

						MCURevWifiDate.McuyuyueFlag=Recv_Buf[14]; 		 //预约标志位 0x01 默认有预约

						MCURevWifiDate.McuyuyueTimer_H=Recv_Buf[15]; 		 //预约时间

						MCURevWifiDate.McuyuyueTimer_L=Recv_Buf[16]; 		 //预约时间

						MCURevWifiDate.McuyureEn=Recv_Buf[17]; 		 //预热使能？0x01 使能

						MCURevWifiDate.McuyureFlag=Recv_Buf[18]; 		 //预热标志位 0x01 默认有预热

						MCURevWifiDate.McuyureTimer_H=Recv_Buf[19]; 		 //预热时间

						MCURevWifiDate.McuyureTimer_L=Recv_Buf[20]; 		 //预热时间

						MCURevWifiDate.McuyureKeepFlag=Recv_Buf[21]; 		 //预热保持

						MCURevWifiDate.McuyureKeepTimer=Recv_Buf[22]; 	 //预热保持时间

						MCURevWifiDate.WorkStep1_Top_Temp=Recv_Buf[23]; 	 	//工作第一步上管的温度

						MCURevWifiDate.WorkStep1_Bot_Temp=Recv_Buf[24];  		//工作第一步下管的温度
						
						MCURevWifiDate.WorkStep1_work_Time=Recv_Buf[25];  	//工作第一步工作时间

						MCURevWifiDate.WorkStep2_Top_Temp=Recv_Buf[26];  		//工作第2步上管的温度

						MCURevWifiDate.WorkStep2_Bot_Temp=Recv_Buf[27];	  	//工作第2步下管的温度
						
						MCURevWifiDate.WorkStep2_work_Time=Recv_Buf[28];  	//工作第2步工作时间

						
						MCURevWifiDate.WorkStep3_Top_Temp=Recv_Buf[29];		//工作第3步上管的温度
						
						MCURevWifiDate.WorkStep3_Bot_Temp=Recv_Buf[30];		//工作第3步下管的温度
							
						MCURevWifiDate.WorkStep3_work_Time=Recv_Buf[31];		//工作第3步工作时间

						MCURevWifiDate.Temp_Ajust_Line=Recv_Buf[32];    		//温度调节 分界点
						
						MCURevWifiDate.Temp_Ajust_small=Recv_Buf[33];    		//温度调节 小步长

						MCURevWifiDate.Temp_Ajust_large=Recv_Buf[34];    		//温度调节 大步长

						MCURevWifiDate.Temp_Ajust_samllest=Recv_Buf[35];    	//温度调节 最小温度

						MCURevWifiDate.Temp_Ajust_largeest=Recv_Buf[36];    	//温度调节 最大温度

						MCURevWifiDate.Timer_Ajust_Min=Recv_Buf[37];    		//可调最小

						MCURevWifiDate.Timer_Ajust_Max=Recv_Buf[38];    		//可调最大

						MCURevWifiDate.zhuanchaSta=Recv_Buf[39];              //转叉状态 1开 0关

						MCURevWifiDate.LudengSta=Recv_Buf[40];              	//炉灯状态	1开 0关

						MCURevWifiDate.Resver_1=Recv_Buf[41];					//预留
						MCURevWifiDate.Resver_2=Recv_Buf[42];					//预留
						MCURevWifiDate.Resver_3=Recv_Buf[43];					//预留
						MCURevWifiDate.Resver_4=Recv_Buf[44];					//预留
						MCURevWifiDate.Resver_5=Recv_Buf[45];					//预留
						MCURevWifiDate.Resver_6=Recv_Buf[46];					//预留

						Recv_Buf[8]  = 0x00;  //数据长度
						Recv_Buf[9]  = 6;

						Recv_Buf[10]  = 0x00;  
					    Recv_Buf[11]  = 0x00;

						Recv_Buf[10]  = MCURevWifiDate.McuMumeNumber_0;  
					    Recv_Buf[11]  = MCURevWifiDate.McuMumeNumber_1;

						Recv_Buf[10]  = MCURevWifiDate.McuMumeNumber_2;  
					    Recv_Buf[11]  = MCURevWifiDate.McuMumeNumber_3;
						
	                  	sendFrameLen =12;
						sendFrameLen = SendUartAck(sendFrameLen);								
						break;
					
					// case 0x5001:					//炉灯启动
					// 	//g_buzzerType = BUZ_TYP_KEY_AUTO;
						
					// 	MCURevWifiDate.LudengONOFF=Recv_Buf[10];
					// 	CommRecvData(); 	
					// 	break;

					case 0x820C:					//启动抓图
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						
						MCURevWifiDate.LudengONOFF = 1;
//						g_pictureCnt = 300;				//10ms时基，抓图倒计时
						returnData(); 	
						break;

					case 0x5002:						//转叉启动
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						
						MCURevWifiDate.ZhuanchaONOFF=Recv_Buf[10];
						CommRecvData(); 
						break;

					case 0x5003:						//参数修改
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.WorkRevise_Top_temp=Recv_Buf[10];
						
						MCURevWifiDate.WorkRevise_Bot_temp=Recv_Buf[11];
						
						MCURevWifiDate.WorkRevise_time=Recv_Buf[12];
						CommRecvData(); 
						break;

					case 0x5004:						//预热跳转
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.Yurejump=1;
						
						CommRecvData(); 
						break;
						
					case 0x5005:						//预热保持
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.YureKeep=Recv_Buf[10];
						MCURevWifiDate.YureKeeptime=Recv_Buf[11];
						
						CommRecvData(); 
						break;
						
					case 0x5008:						//摄像头信息下发
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						
						MCURevWifiDate.CamerSta=Recv_Buf[10];	

						MCURevWifiDate.CamerErr=Recv_Buf[11];

						MCURevWifiDate.Camerlast=Recv_Buf[12];
						
						CommRecvData(); 
						break;

						
					case 0x5009:						//下发食材信息
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.FoodMsg=Recv_Buf[10];	

						MCURevWifiDate.FoodMsgLast=Recv_Buf[11];

						CommRecvData(); 
						break;

					case 0x500A:						//下发识别失败原因
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.AutoLoseCause=Recv_Buf[10];	

						MCURevWifiDate.AutoLoseCause_0=Recv_Buf[11];	

						MCURevWifiDate.AutoLoseCause_1=Recv_Buf[12];	

						MCURevWifiDate.AutoLoseCause_3=Recv_Buf[13];	

						CommRecvData(); 
						break;

				
					
               
					default:
						break;
				}
			}
		}
	}
}


/****************************
函数名称：
函数功能：
函数参数：
函数返回值：
作者时间：
备    注：  
修改纪要:
*******************************/
void WifiUartExceptionHandl(void)
{
	if(McuSendWifi_ACK_flag)
	{
		if(ackTime > 0)
		{
			ackTime--;
		}
		if(ackTime == 0)
		{
			MCUSendStart_Flag = 1;
		}
	}

	if((recvOutTime == 0) && (RevWifiworking_flag))
	{
		recvDataCount = 0;
		frameHead_H_Flag= 0;
		frameHead_L_Flag = 0;	
		RevWifiworking_flag = 0;
	}
}


/****************************
函数名称：
函数功能：
函数参数：
函数返回值：
作者时间：
备    注：  
修改纪要:
*******************************/
void WifiDealFunction(void)
{
	//如果设备MCU没有与WIFI模块进行握手成功
	if(HandsCmd_OK_flag==0)
	{
		cmdNum = 0;
		if(cmdNum < CMD_NUM_MAX)
		{
			CmdCode_Buf[cmdNum] = 0xCCC0;
			cmdNum++;
		}
		
		SendUart();
		if(sendIntervalTime > 0)
		{
			sendIntervalTime--;
		}
		
		RecvUart();

		if(recvOutTime > 0)
		{
			recvOutTime--;
		}
		
		WifiUartExceptionHandl();		
	}
	else
	{
		if(CmdSmartlinkStart_flag==1)				//smartLink_start
		{
			CmdSmartlinkStart_flag = 0;
			cmdNum = 0;
			if(cmdNum < CMD_NUM_MAX)
			{
				CmdCode_Buf[cmdNum] = 0xCCC2;
				cmdNum++;
			}	
			SendUart();
			
			if(sendIntervalTime > 0)
			{
				sendIntervalTime--;
			}
			
			RecvUart();
			
			if(recvOutTime > 0)
			{
				recvOutTime--;
			}
			WifiUartExceptionHandl();
		}
		else
		{
			if(CmdSbGoUpStart_flag== 1)
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbGoUpStart_flag=0;
					CmdCode_Buf[cmdNum] = 0xCCC5;
					cmdNum++;
				}	
			}
			else if(CmdSbReBootStart_flag== 1)
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbReBootStart_flag= 0;
					CmdCode_Buf[cmdNum] = 0xCCC9;
					cmdNum++;
				}
			}
			else if(CmdSbReprotStart_flag== 1)				//主动上报
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbReprotStart_flag= 0;
					CmdCode_Buf[cmdNum] = 0x5000;
					cmdNum++;
				}	
			}
			else if(CmdSbAutoBootStart_flag== 1)			//智能烘焙
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbAutoBootStart_flag = 0;
					CmdCode_Buf[cmdNum] = 0x820A;
					cmdNum++;
				}	
			}
			else if(CmdSbKeyActionStart_flag== 1)			//按键行为上报
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbKeyActionStart_flag = 0;
					CmdCode_Buf[cmdNum] = 0x1000;
					cmdNum++;
				}	
			}
			
			else if(CmdRemovelinkStart_flag== 1) 	//解绑指令0x6001
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdRemovelinkStart_flag = 0;
					CmdCode_Buf[cmdNum] = 0x6001;
					cmdNum++;
				}	
			}
			else
			{


			}
			
			if((CmdCode_Buf[0] != 0xccc0) && (CmdCode_Buf[0] != 0xccc2))
			{
				SendUart();
			}			
			if(sendIntervalTime > 0)
			{
				sendIntervalTime--;
			}
			RecvUart();
			if(recvOutTime > 0)
			{
				recvOutTime--;
			}
			WifiUartExceptionHandl();
		}
	}
}
