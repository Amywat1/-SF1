#include "header.h"

/*-----------------------------------------------------------------------------
Description:		初始化系统时钟
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitOscilator(void)
{
	BANK0_SET;									//指向Bank0
	INSCON &= (~(BIT2 | BIT3)); 				//8位乘除法
	CLKCON = 0x00; 								//时钟设置，24M内部振荡器，系统时钟不分频
}

/*-----------------------------------------------------------------------------
Description:		初始化看门狗
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitWdt(void)
{
	RSTSTAT = 0x01;								//设置看门狗时间为1024ms
}

/*-----------------------------------------------------------------------------
Description:		初始化所有IO口（先将所有IO设置输出/无上拉/低电平，再配置使用的端口）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitIo(void)
{
	/*BANK0*/
	BANK0_SET;										//指向Bank0

	/*端口做输出*/
	P0CR = 0xFF;
	P1CR = 0xFF;
	P2CR = 0xFF;
	P4CR = 0xFF;
	
	/*掉电检测AD口上电就做输入*/
	P2CR &= ~(BIT6);
	P2CR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
	
	/*端口无上拉*/
	P0PCR = 0;
	P1PCR = 0;
	P2PCR = 0;
	P3PCR = 0;
	P4PCR = 0;
	
	P0 = 0;
	P1 = 0;
	P2 = 0;
	P3 = 0;
	P4 = 0;
	
	/*BANK1*/
	BANK1_SET;
	P5CR	= 0xFF;
	P5PCR	= 0;
	P5		= 0;
	BANK0_SET;										//指向Bank0
	
	/*使用的端口初始化配置*/
	InitUartIo();									//初始化通讯引脚IO
	InitDispIo();									//显示相关IO
	InitTouchKeyIo();								//触摸按键引脚IO
	InitMeasureIo();								//信号检测IO
	InitLoadIo();									//负载IO
	
	#ifdef DEBUG
	InitDebugUartIo();								//初始化调试串口IO
	#endif
}

/*-----------------------------------------------------------------------------
Description:		初始化定时器配置；每250us产生中断
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitTimer5(void)
{
	BANK1_SET;
	
	T5CON = 0x00;									//16位自动重装，计数时钟不分频
	
	TL5 = (65536-3000)%256;							//必须先写低位，再写高位，不然时间误差较大
	TH5 = (65536-3000)/256;  						//24M，每次计时0.041667US *3000=125us

	TF5 = 0;										//清中断标志位（T5由硬件清零）
	TR5 = 1;										//开定时器5
	
	BANK0_SET;
	ET5 = 1;										//开定时器5中断
}

/*-----------------------------------------------------------------------------
Description:		对数组进行清零操作，unsigned char型
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitCharArrayData(unsigned char *buf,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
		*buf = 0;
		buf++;	
	}
}

/*-----------------------------------------------------------------------------
Description:		对数组进行清零操作，unsigned int型
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
//void InitIntArrayData(unsigned int *buf,unsigned char len)
//{
//	unsigned char i;
//	for(i=0;i<len;i++)
//	{
//		*buf = 0;
//		buf++;	
//	}
//}

/*-----------------------------------------------------------------------------
Description:		对扇区进行擦除
Input:				sectorAddr:	16位，[15:9]表示扇区号，范围：0~63（FLSH最大64个扇区，类EEPROM最大8个扇区）;[8]表示存储单元的高位地址；[7:0]表示存储单元的地位地址
					如：sectorAddr = 0x0345；表示选择1号扇区，地址为0x0145;

					sectorSelect:	选择对FLSH或EEPROM编程
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SectorErase(unsigned int sectorAddr,bit sectorSelect)
{
	bit EAtemp;
	
	EAtemp = EA;
	EA = 0;
	
	if(sectorSelect == 0)							//对FLASH操作时FAC位清零
	{
	  FLASHCON &= ~BIT0;
	}
	else											//对类EEPROM操作时FAC位置1
	{
	  FLASHCON |= BIT0;
	}
	
	XPAGE = ((sectorAddr >> 9) << 1);				//赋值扇区号
	if(((sectorAddr>>8) % 2) == 0)					//赋值存储单元的高位地址
	{
		XPAGE |= 0x00;
	}
	else
	{
		XPAGE |= 0x01;
	}
	
	//存储单元删除
	IB_CON1=0xE6;
	IB_CON2=0x05;
	IB_CON3=0x0A;
	IB_CON4=0x09;
	IB_CON5=0x06;
	_nop_();
	_nop_();
	_nop_(); 
	_nop_();
	
	XPAGE=0x00;
	FLASHCON &= ~BIT0;								//当不需对类EEPROM操作时，必须将FAC位清0。
	
	EA = EAtemp;
}

/*-----------------------------------------------------------------------------
Description:		对扇区进行编程
Input:				sectorAddr:	16位，[15:9]表示扇区号，范围：0~63（FLSH最大64个扇区，类EEPROM最大8个扇区）;[8]表示存储单元的高位地址；[7:0]表示存储单元的地位地址
					如：sectorAddr = 0x0345；表示选择1号扇区，地址为0x0145;
					
					byteData:		需要编程的数据
					
					sectorSelect:	选择对FLSH或EEPROM编程
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void SectorWrite(unsigned int sectorAddr,unsigned char byteData,bit sectorSelect)
{
	bit EAtemp;
	
	EAtemp = EA;
	EA = 0;
	
	if(sectorSelect == FLASH_CRL)					//对FLASH操作时FAC位清零
	{
	  FLASHCON &= ~BIT0;
	}
	else											//对类EEPROM操作时FAC位置1
	{
	  FLASHCON |= BIT0;
	}
	
	XPAGE = ((sectorAddr >> 9) << 1);				//赋值扇区号
	if(((sectorAddr>>8) % 2) == 0)					//赋值存储单元的高位地址
	{
		XPAGE |= 0x00;
	}
	else
	{
		XPAGE |= 0x01;
	}
	IB_OFFSET = (unsigned char)(sectorAddr & 0x00ff);	//赋值存储单元的低8位地址
	
	IB_DATA = byteData;
	
	//存储单元编程
	IB_CON1=0x6E;
	IB_CON2=0x05;
	IB_CON3=0x0A;
	IB_CON4=0x09;
	IB_CON5=0x06;
	_nop_();
	_nop_();
	_nop_(); 
	_nop_();
	
	XPAGE=0x00;
	FLASHCON &= ~BIT0;								//当不需对类EEPROM操作时，必须将FAC位清0。
	
	EA = EAtemp;
}

/*-----------------------------------------------------------------------------
Description:		对扇区数据进行读取
Input:				sectorAddr:	16位，[15:9]表示扇区号，范围：0~63（FLSH最大64个扇区，类EEPROM最大8个扇区）;[8]表示存储单元的高位地址；[7:0]表示存储单元的地位地址
					如：sectorAddr = 0x0345；表示选择1号扇区，地址为0x0145;

					sectorSelect:	选择对FLSH或EEPROM编程
Return:				byteData:		对应地址的数据
History:			无
-----------------------------------------------------------------------------*/
unsigned char SectorRead(unsigned int sectorAddr,bit sectorSelect)
{
	unsigned char byteData;
	bit EAtemp;
	
	EAtemp = EA;
	EA = 0;
	
	if(sectorSelect == 0)							//对FLASH操作时FAC位清零
	{
	  FLASHCON &= ~BIT0;
	}
	else											//对类EEPROM操作时FAC位置1
	{
	  FLASHCON |= BIT0;
	}
	
	byteData = CBYTE[sectorAddr];
	FLASHCON &= ~BIT0;								//当不需对类EEPROM操作时，必须将FAC位清0。
	
	EA = EAtemp;
	
	return(byteData);
}

/*-----------------------------------------------------------------------------
Description:		初始化低电压中断
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitLPD(void)
{
	BANK0_SET;

	LPDCON |= BIT7;									//允许低电压检测
	LPDSEL = 0x08;									//LPD电压设置为3.6V
	Delay1ms(10);
	LPDCON &= ~BIT4;								//清除中断请求标志
	IEN1 |= BIT6;									//允许LPD中断
}

/*-----------------------------------------------------------------------------
Description:		初始化所有变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitVariable(void)
{
	InitUartVariable();								//初始化通讯变量
	InitDispVariable();								//初始化显示变量
	InitTouchKeyVariable();							//初始化触摸按键变量
	BuzzerVariableInit();							//初始化蜂鸣器变量
	InitSysModeVariable();							//初始化系统状态变量
	InitMeasureVariable();							//初始化检测信号变量
	InitLoadVariable();								//初始化负载变量


	CmdHandVariableInit();
	CmdSmartLinkVariableInit();
	CmdSoftGoUpVariableInit();
	CmdReBootVariableInit();
	Cmd0x5000VariableIint();
	
	#ifdef DEBUG
	InitDebugUartVariable();						//初始化调试串口变量
	#endif
}

/*-----------------------------------------------------------------------------
Description:		初始化中断优先级（T2定时器优先级最高）
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitPriority(void)
{
	BANK0_SET;

	IPL0 = 0x18;              						//中断优先级 	
	IPH0 = 0x0A;			  	
	IPL1 = 0x00;				
	IPH1 = 0x00;									//T5	优先级3  最高
													//触摸	优先级2
													//串口	优先级1
													//其他	优先级0	 最低
}

/*-----------------------------------------------------------------------------
Description:		底层配置和初始化所有变量
Input:				void
Return:				void
History:			无
-----------------------------------------------------------------------------*/
void InitAll(void)
{
	EA = 0;										//关总中断/所有中断
	IEN0 = 0X00;
	IEN1 = 0X00;
	IEN2 = 0X00;
	InitOscilator();							//初始化系统时钟
	InitWdt();									//初始化看门狗
	InitIo();									//初始化IO口
	Delay1ms(200);								//延时200ms以稳定电平
	InitWdt();									//清狗
	InitUart();									//串口初配置始化

	Uart0Init();
	
	#ifdef DEBUG
	InitDebugUart();							//调试串口配置初始化
	#endif
	
	InitTouchKey();								//触摸按键配置初始化
	InitTimer5();								//初始化定时器5
	InitLPD();									//低电压中断初始化
	InitVariable();								//初始化所有变量
	InitAd();									//初始化AD配置（掉电电容检测及变量赋值，中心温度检测配置）
	BuzzerConfigureInit();						//蜂鸣器配置初始化(掉电口和蜂鸣口共用，先检测掉电口电平)
	InitPriority();								//中断优先级初始化
	EA = 1;										//开总中断
}