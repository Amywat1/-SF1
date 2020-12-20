#ifndef UART_H
#define UART_H

#define P_TXD		P4_0
#define P_RXD		P4_2
//#define N_TXD		P0_0
//#define N_RXD		P0_1

#define SEND_MAX				12				//发送数据的最大个数
#define RECV_MAX				26				//接收数据的最大个数

/*控制板接收的信息*/
#define RECV_HARD 				UartData.RecvDataSave_Buff[0]		//头码
#define RECV_SN					UartData.RecvDataSave_Buff[1]		//SN码

#define RECV_STEAM_H_TEM_H8		UartData.RecvDataSave_Buff[2]		//锅炉温度 高温区 高八位
#define RECV_STEAM_H_TEM_L8		UartData.RecvDataSave_Buff[3]		//锅炉温度 高温区 低八位
#define RECV_STEAM_L_TEM_H8		UartData.RecvDataSave_Buff[4]		//锅炉温度 低温区 高八位
#define RECV_STEAM_L_TEM_L8		UartData.RecvDataSave_Buff[5]		//锅炉温度 低温区 低八位

#define RECV_CORE_H_TEM_H8		UartData.RecvDataSave_Buff[6]		//中心温度 高温区 高八位（预留）
#define RECV_CORE_H_TEM_L8		UartData.RecvDataSave_Buff[7]		//中心温度 高温区 低八位（预留）
#define RECV_CORE_L_TEM_H8		UartData.RecvDataSave_Buff[8]		//中心温度 低温区 高八位（预留）
#define RECV_CORE_L_TEM_L8		UartData.RecvDataSave_Buff[9]		//中心温度 低温区 低八位（预留）

#define RECV_PUMP_CURRENT_H8	UartData.RecvDataSave_Buff[10]		//水泵电流AD值高八位
#define RECV_PUMP_CURRENT_L8	UartData.RecvDataSave_Buff[11]		//水泵电流AD值低八位

#define RECV_PUMP_WORK_FLAG		UartData.RecvDataSave_Buff[12]		//是否新抽了一次水

#define RECV_POT_TYPE			UartData.RecvDataSave_Buff[13]		//内锅
#define RECV_ZERO_TYPE			UartData.RecvDataSave_Buff[14]		//过零

#define RECV_POWER_VERSION		UartData.RecvDataSave_Buff[15]		//电源板版本

#define FCHUANG_HUO_OCDE_1		UartData.RecvDataSave_Buff[16]		//防串货码 1
#define FCHUANG_HUO_OCDE_2		UartData.RecvDataSave_Buff[17]		//防串货码 2
#define FCHUANG_HUO_OCDE_3		UartData.RecvDataSave_Buff[18]		//防串货码 3
#define FCHUANG_HUO_OCDE_4		UartData.RecvDataSave_Buff[19]		//防串货码 4
#define FCHUANG_HUO_OCDE_5		UartData.RecvDataSave_Buff[20]		//防串货码 5
#define FCHUANG_HUO_OCDE_6		UartData.RecvDataSave_Buff[21]		//防串货码 6
#define FCHUANG_HUO_OCDE_7		UartData.RecvDataSave_Buff[22]		//防串货码 7
#define FCHUANG_HUO_OCDE_8		UartData.RecvDataSave_Buff[23]		//防串货码 8	

#define RECV_DATA_CHECK_SUM		UartData.RecvDataSave_Buff[24]		//校验和
#define RECV_SUM_RVS_CODE		UartData.RecvDataSave_Buff[25]		//校验和反码

/*控制板发送的信息*/
#define SEND_HARD				UartData.SendDataSave_Buff[0]		//头码
#define SEND_SN					UartData.SendDataSave_Buff[1]		//SN码
#define SEND_OUT_DATA1			UartData.SendDataSave_Buff[2]		//输出信息1（卤素管、风扇、搅拌、散热）
#define SEND_OUT_DATA_STEAM		UartData.SendDataSave_Buff[3]		//输出信息2（锅炉可控硅控制）
#define SEND_OUT_DATA_PUMP		UartData.SendDataSave_Buff[4]		//输出信息3（水泵抽水频率控制）
#define SEND_OUT_DATA_HOT		UartData.SendDataSave_Buff[5]		//输出信息4（预留）
#define SEND_OUT_RVS_CODE1		UartData.SendDataSave_Buff[6]		//输出反码1
#define SEND_OUT_RVS_CODE2		UartData.SendDataSave_Buff[7]		//输出反码2
#define SEND_OUT_RVS_CODE3		UartData.SendDataSave_Buff[8]		//输出反码3
#define SEND_OUT_RVS_CODE4		UartData.SendDataSave_Buff[9]		//输出反码4
#define SEND_DATA_CHECK_SUM		UartData.SendDataSave_Buff[10]		//校验和
#define SEND_SUM_RVS_CODE		UartData.SendDataSave_Buff[11]		//检验和反码

#define BIT_HOT					BIT0								//卤素管控制位
#define BIT_FAN					BIT1								//罩极电机风扇控制位
#define BIT_STIR				BIT2								//搅拌电机控制位
#define BIT_SANRE				BIT3								//散热风扇控制位
#define BIT_NULL_1				BIT4								//预留
#define BIT_NULL_2				BIT5								//预留
#define BIT_NULL_3				BIT6								//预留
#define BIT_NULL_4				BIT7								//预留

typedef struct
{
	unsigned char SendData_Buff[SEND_MAX];				//发送的数据
    unsigned char SendDataTmp_Buff[SEND_MAX];			//发送数据的临时值
	unsigned char SendDataSave_Buff[SEND_MAX];			//发送数据的保存值
	unsigned char RecvData_Buff[RECV_MAX];				//接收的数据
    unsigned char RecvDataTmp_Buff[RECV_MAX];			//接收数据的临时值
	unsigned char RecvDataSave_Buff[RECV_MAX];			//接收数据的保存值
	unsigned char uartRevcCnt;							//接收数据的计数
	unsigned char uartSendCnt;							//发送数据的计数
	unsigned int  uartRecvErrCnt;						//串口接收失败计时
	unsigned char uartSendCrlCnt;						//串口发送处理计数100ms刷新一次
	unsigned int  uartRecvOverCnt;						//串口接收超时，输出停止
}UART_DATA;

enum _UATR_SEND_TYPE_{                           
    sendOver               		=   0,					//发送完成
    sendGoing             		=   1,	      		 	//正在发送
};
enum _UATR_RECV_TYPE_{                           
    recvOver               		=   0,					//接收完成
    recvGoing             		=   1,	       			//正在接收
};

extern UART_DATA xdata			UartData;				//串口数据使用到的结构体
extern enum _UATR_RECV_TYPE_	g_uartRecvType;			//串口接收状态
extern enum _UATR_SEND_TYPE_	g_uartSendType;			//串口发送状态

extern bit 						g_firstRecvFlag;		//首次接收数据标志

void InitUartIo(void);
void InitUartVariable(void);
void InitUart(void);
unsigned char TmpDataSum(unsigned char *buf,unsigned char len);
void TmpDataGive(unsigned char *recv,unsigned char *give,unsigned char len);
void UartSendDeal(void);
void UartRecvDeal(void);

#endif