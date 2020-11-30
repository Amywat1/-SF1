//**********************Copyright (c)***************************
//
//		杭州九阳股份有限公司
//
//		杭州九阳生活电器有限公司
//
//		杭州九阳欧南多小家电有限公司
//
//		西式生活事业部.电控部
//
//*********************项目信息*********************************
//
//项目名称:	
//
//编程语言:	C语言		
//
//产品品类: 
//
//芯片信息:	中颖SH79F328(44PIN 32K_ROM、256BYTE 内RAM、1280BYTE 外RAM、4096BYTE E2PROM)
//
//振荡方式:	内部RC 24M
//
//显示方式:	数码管，LED
//
//按键方式:	触摸按键
//
//配电源板:	SF1-P
//
//配显示板:	SF1-K
//
//编制人员: 胡望伟
//
//编制日期:	
//
//备注信息：
//***********************版本信息********************************

//*************历史版本01*****************************
//程序版本: A01
//
//校 验 和:
//
//修 改 人: 胡望伟
//
//修改日期: 2020.11.30
//
//更改记录：1、继承SF1 A10程序

#include "header.h"

/*-----------------------------------------------------------------------------
Description:		主函数
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void main(void)
{
	unsigned char tmp = (PROCESS_NUM - 1);	//使从case 0开始执行程序
	unsigned char cycleCnt = (PROCESS_NUM - 1);
	
	InitAll();								//所有配置初始化

	while(1)
	{
		if(g_flg2ms)
		{
			g_flg2ms = 0;
			tmp++;
			if(tmp >= PROCESS_NUM)
			{
				tmp = 0;
			}
			
			/*时效性较强程序*/
			InitWdt();						//清狗
			TouchKeyAdScan();				//按键AD扫描				//3个按键程序最多392us
			TouchKeyScan();					//按键扫描
			TouchKeyJudge();				//按键判断
			DisplayScan();					//显示扫描					//7.08us
		}
		
		else
		{

		}
		
		if(cycleCnt != tmp)
		{
			cycleCnt = tmp;
			
			/*先接收数据，再控制输出负载，再发送数据*/
			switch(tmp)						//每个时间片程序循环周期为   PROCESS_NUM*2ms
			{
				case 0:
					UartRecvDeal();			//串口接收处理				//210.2us
					break;
					
				case 1:
					MeasureDataScan();		//检测数据扫描				//107.5us
					break;
					
				case 2:
					LoadCrl();				//所有输出负载控制			//43.6us
					BuzzerCtrl();										//26us
					break;
					
				case 3:
					SysModelCrl();			//系统状态控制				//23.6us
					DispCrl();				//显示控制					//61.2us
					break;
			
				case 4:
					UartSendDeal();			//串口发送处理				//202us
					
					#ifdef DEBUG
					DebugUartDeal();		//调试串口处理
					#endif
					break;
					
				default:
					break;
			}
		}
	}
}