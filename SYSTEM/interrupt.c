#include "header.h"

/********对应按键在芯片的存储区映射****************/
unsigned char xdata		TK01L 	_at_	0x500;
unsigned char xdata		TK01H 	_at_	0x501; 
unsigned char xdata		TK02L 	_at_	0x502; 
unsigned char xdata		TK02H 	_at_	0x503; 
unsigned char xdata		TK03L 	_at_	0x504;
unsigned char xdata		TK03H 	_at_	0x505; 
unsigned char xdata		TK04L 	_at_	0x506; 
unsigned char xdata		TK04H 	_at_	0x507; 
unsigned char xdata		TK05L 	_at_	0x508; 
unsigned char xdata		TK05H 	_at_	0x509; 
unsigned char xdata		TK06L 	_at_	0x50A;
unsigned char xdata		TK06H 	_at_	0x50B; 
unsigned char xdata		TK07L 	_at_	0x50C;
unsigned char xdata		TK07H 	_at_	0x50D; 
unsigned char xdata		TK08L 	_at_	0x50E;
unsigned char xdata		TK08H 	_at_	0x50F; 
unsigned char xdata		TK09L 	_at_	0x510;
unsigned char xdata		TK09H 	_at_	0x511; 
unsigned char xdata		TK10L 	_at_	0x512;
unsigned char xdata		TK10H 	_at_	0x513; 
unsigned char xdata		TK11L 	_at_	0x514;
unsigned char xdata		TK11H 	_at_	0x515; 
unsigned char xdata		TK12L 	_at_	0x516;
unsigned char xdata		TK12H 	_at_	0x517; 
unsigned char xdata		TK13L 	_at_	0x518; 
unsigned char xdata		TK13H 	_at_	0x519; 
unsigned char xdata		TK14L 	_at_	0x51A; 
unsigned char xdata		TK14H 	_at_	0x51B; 
unsigned char xdata		TK15L 	_at_	0x51C;
unsigned char xdata		TK15H 	_at_	0x51D; 
unsigned char xdata		TK16L 	_at_	0x51E; 
unsigned char xdata		TK16H 	_at_	0x51F; 
unsigned char xdata		TK17L 	_at_	0x520;
unsigned char xdata		TK17H 	_at_	0x521; 
unsigned char xdata		TK18L 	_at_	0x522;
unsigned char xdata		TK18H 	_at_	0x523; 
unsigned char xdata		TK19L 	_at_	0x524; 
unsigned char xdata		TK19H 	_at_	0x525; 
unsigned char xdata		TK20L 	_at_	0x526;
unsigned char xdata		TK20H 	_at_	0x527; 
unsigned char xdata		TK21L 	_at_	0x528; 
unsigned char xdata		TK21H 	_at_	0x529; 
unsigned char xdata		TK22L 	_at_	0x52A; 
unsigned char xdata		TK22H 	_at_	0x52B; 
unsigned char xdata		TK23L 	_at_	0x52C;
unsigned char xdata		TK23H 	_at_	0x52D; 
unsigned char xdata		TK24L 	_at_	0x52E;
unsigned char xdata		TK24H 	_at_	0x52F;

unsigned char xdata		t2msCnt = 0;			//2ms计数值
unsigned int  xdata		t500msCnt = 0;			//500ms计数值

		 bit			g_flg2ms = 0;			//2ms计时标志位
		 bit			g_flg500ms = 0;			//500ms计时标志位
		 
		 bit			flgPwoeroff = 0;		//掉电标志位
unsigned char xdata		testCnt = 0;			//单次测试周期计数
unsigned char xdata		highCnt = 0;			//高电平计数

/*-----------------------------------------------------------------------------
Description:		定时器5中断子程序（125us进入一次中断）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void IntTimer5(void) interrupt 3
{
	unsigned char checkSum = 0;
	unsigned char i =0,j =0;

	_push_(INSCON);								//压栈
    BANK1_SET;	
	TF5 = 0;   									//清除Timer5中断溢出标志
	BANK0_SET;
	
	if(flgPwoeroff == 0)						//无检测到掉电时，正常执行程序
	{
		t2msCnt++;								//2ms计时
		if(t2msCnt >= T2MS_OVER_CNT)
		{
			t2msCnt = 0;
			g_flg2ms = 1;						//2ms时间标志
		}
		
		t500msCnt++;
		if(t500msCnt >= T500MS_OVER_CNT)
		{
			t500msCnt = 0;
			g_flg500ms = 1;						//500ms时间标志
		}
		
		BuzzerDrive(); 							//125ms执行一次	 4K蜂鸣器 驱动必须用125μs 声音
	}
	else										//有检测到掉电
	{
		testCnt++;
		if((LPDCON & BIT6) == BIT6)				//LPD标志位为高，高电平加1
		{
			highCnt++;
		}
		
		if(testCnt == 10)						//测试周期10*0.25 = 2.5ms
		{
			if(highCnt > 8)						//测到多个高电平，判断为真掉电
			{
				EA = 0;							//关总中断
				InitWdt();						//清狗
				
				P4CR &= ~BIT2;					//掉电检测口做输入不带上拉
				P4PCR &= ~BIT2;
				
				/*关闭所有输出*/
				P_COM1 = 1;
				P_COM2 = 1;
				P_COM3 = 1;
				P_COM4 = 1;
				P_SEG_A = 0;
				P_SEG_B = 0;
				P_SEG_C = 0;
				P_SEG_D = 0;
				P_SEG_E = 0;
				P_SEG_F = 0;
				P_SEG_G = 0;
				P_SEG_H = 0;
				
				P_TXD = 0;
				P_RXD = 0;

				checkSum = 0;
				
				if(g_pumpCalibrationFlag)
				{
					i = (unsigned char)((calibrationData.pumpNormalWorkAd & 0xff00) >> 8);
					j = (unsigned char)(calibrationData.pumpNormalWorkAd & 0x00ff);
					SectorWrite(0x0010,i,EEPROM_CRL);					//记录当前的水泵电流标定值
					SectorWrite(0x0011,j,EEPROM_CRL);
				}
				
				/*预约工作、工作、保温状态下保存数据*/
				if((g_sysType == SysModeOrderWork) || (g_sysType == SysModeWork) || (g_sysType == SysModeWarm))
				{
					SectorWrite(0x0000,0x5A,EEPROM_CRL);				//储存头码
					checkSum += 0x5A;
					
					SectorWrite(0x0001,g_sysType,EEPROM_CRL);			//储存系统当前状态
					checkSum += g_sysType;
					
					SectorWrite(0x0002,g_menuNumber,EEPROM_CRL);		//储存当前菜单编号
					checkSum += g_menuNumber;

					SectorWrite(0x0003,g_nowStepNum,EEPROM_CRL);		//储存当前工艺步骤数
					checkSum += g_nowStepNum;
					
					SectorWrite(0x0004,g_nowStepTechnology,EEPROM_CRL);	//储存当前步骤的工艺
					checkSum += g_nowStepTechnology;
					
					SectorWrite(0x0005,g_nowStepworkTemp,EEPROM_CRL);	//储存当前步骤温度
					checkSum += g_nowStepworkTemp;
				
					SectorWrite(0x0006,g_nowStepworkTime,EEPROM_CRL);	//储存当前步骤时间
					checkSum += g_nowStepworkTime;
					
					SectorWrite(0x0007,g_workTimeAll,EEPROM_CRL);		//储存工作总时间
					checkSum += g_workTimeAll;

					i = (unsigned char)((g_highSteamPowerCnt & 0xff00) >> 8);
					j = (unsigned char)(g_highSteamPowerCnt & 0x00ff);
					SectorWrite(0x0008,i,EEPROM_CRL);					//储存当前纯蒸高功率时间的高八位
					checkSum += i;
					SectorWrite(0x0009,j,EEPROM_CRL);					//储存当前纯蒸高功率时间的低八位
					checkSum += j;

					i = (unsigned char)((g_keepWarmTime & 0xff00) >> 8);
					j = (unsigned char)(g_keepWarmTime & 0x00ff);
					SectorWrite(0x000A,i,EEPROM_CRL);					//储存当前保温时间的高八位
					checkSum += i;
					SectorWrite(0x000B,j,EEPROM_CRL);					//储存当前保温时间的低八位
					checkSum += j;
					
					i = (unsigned char)((g_orderTime & 0xff00) >> 8);
					j = (unsigned char)(g_orderTime & 0x00ff);
					SectorWrite(0x000C,i,EEPROM_CRL);					//储存当前预约时间的高八位
					checkSum += i;
					SectorWrite(0x000D,j,EEPROM_CRL);					//储存当前预约时间的低八位
					checkSum += j;
					
					SectorWrite(0x000E,g_workOneMinCnt,EEPROM_CRL);		//储存当前分钟的计数值
					checkSum += g_workOneMinCnt;
					
					SectorWrite(0x000F,checkSum,EEPROM_CRL);			//储存校验和
					
					while(1)											//死循环，快速耗电
					{
						CORE_NTC_IO_OUT;								//IO口做输出，耗电
					}
				}
			}
			
			testCnt = 0;
			highCnt = 0;
			flgPwoeroff = 0;					//非真掉电，清零掉电标志
			
			IEN1 |= BIT6;						//重新开启LPD中断
		}
	}

	_pop_(INSCON);  							//出栈
}

/*-----------------------------------------------------------------------------
Description:		LPD中断子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void IntLPD(void) interrupt 13
{
	_push_(INSCON);
    BANK0_SET;

	if((LPDCON & BIT4) == BIT4)					//有中断挂起
	{
		LPDCON &= ~BIT4;						//清除中断请求标志
		
		IEN1 &= ~BIT6;							//取消LPD中断

		flgPwoeroff = 1;						//掉电标志置1
		
		/*有中断后进行滤波处理，通过计时器读电平，判断是否是真掉电*/
		testCnt = 0;							//单个周期开始计数
		highCnt = 0;							//高电平数量清零
	}
	else
	{
		LPDCON &= ~BIT4;						//清除中断请求标志
	}

	_pop_(INSCON);  							//出栈
}

/*-----------------------------------------------------------------------------
Description:		串口中断子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void IntUart1(void) interrupt 15
{
	unsigned char uartRecvTmp;
	unsigned char i;
	
	_push_(INSCON);								//压栈
    BANK1_SET;
	
	if(RI1)
	{
		RI1 = 0;								//清零接收中断标志
		
		UartData.uartRecvErrCnt = 0;			//接收超时 清零		
		uartRecvTmp = SBUF1;
		if(UartData.uartRevcCnt == 0)
		{
			if(uartRecvTmp == 0x55)				//头码正确
			{
				UartData.RecvData_Buff[0] = uartRecvTmp;
				UartData.uartRevcCnt = 1;
			}
		}
		else if(UartData.uartRevcCnt <= RECV_MAX)
		{
			UartData.RecvData_Buff[UartData.uartRevcCnt] = uartRecvTmp;
			UartData.uartRevcCnt++;
			if(UartData.uartRevcCnt >= RECV_MAX)//一帧数据接收完成
			{
				/*此处若调用TmpDataGive()函数会报警，原因：正常执行时如果在执行该函数，进入中断又执行该程序,发生重入*/
				for(i=0;i<RECV_MAX;i++)
				{
					UartData.RecvDataTmp_Buff[i] = UartData.RecvData_Buff[i];	//将接收的值放入缓存区
				}
				
				UartData.uartRevcCnt = 0;
				g_uartRecvType = recvOver;		//串口接收状态为完成状态
			}
		}
	}
	else if(TI1)								//发送中断
	{
		TI1 = 0;								//清零发送中断标志	
		
		UartData.uartSendCnt++;
		if(UartData.uartSendCnt >= SEND_MAX)
		{
			UartData.uartSendCnt = SEND_MAX;
			g_uartSendType = sendOver;			//一帧数据发送完毕
		}
		else
		{
			SBUF1 = UartData.SendData_Buff[UartData.uartSendCnt];
		}
	}
	
	BANK0_SET;
	_pop_(INSCON);  							//出栈
}

/*-----------------------------------------------------------------------------
Description:		触摸按键中断子程序
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void IntTouchKey(void)	interrupt 1
{
	_push_(INSCON);
    BANK0_SET;

	if(TKF0 & BIT4)
	{
		TKF0 = 0;
		g_scanDataTk[0] = ((unsigned int)(TK01H << 8)) | ((unsigned int)TK01L);
		g_scanDataTk[1] = ((unsigned int)(TK02H << 8)) | ((unsigned int)TK02L);
		g_scanDataTk[2] = ((unsigned int)(TK03H << 8)) | ((unsigned int)TK03L);
		g_scanDataTk[3] = ((unsigned int)(TK04H << 8)) | ((unsigned int)TK04L);
		g_scanDataTk[4] = ((unsigned int)(TK05H << 8)) | ((unsigned int)TK05L);
		g_scanDataTk[5] = ((unsigned int)(TK06H << 8)) | ((unsigned int)TK06L);
		g_scanDataTk[6] = ((unsigned int)(TK07H << 8)) | ((unsigned int)TK07L);
		
		g_keyScanEndFlag = 1;
	}
	else
	{
		if(TKF0 & BIT6)           	//运算结果溢出应该减小放大系数寄存器
		{
			TKF0 = 0;
            g_keyScanEndFlag = 0;
			Delay1us(10);			//延时10us
			TKCON1 |= BIT5;    		//启动按键扫描
		}
		else if(TKF0&BIT5)          //启动信号错误，延时10us后，重新启动扫描
		{
			TKF0 = 0;
            g_keyScanEndFlag = 0;
			Delay1us(10);			//延时10us
			TKCON1 |= BIT5;    		//启动按键扫描
		}
		else if(TKF0&BIT3)      	//计数按键扫描溢出，应减小C1值
		{
			TKF0 = 0;
            g_keyScanEndFlag = 0;
			Delay1us(10);			//延时10us
			TKCON1 |= BIT5;    		//启动按键扫描
		}
	}
	
	_pop_(INSCON);  				//出栈
}