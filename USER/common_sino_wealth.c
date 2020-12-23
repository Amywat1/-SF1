#include "common_sino_wealth.h"
#include <ABSACC.H>											//用于CBYTE的宏定义操作（操作绝对地址）
#include <intrins.h>

// /*-----------------------------------------------------------------------------
// Description:		初始化系统

// Input:				divNum：			Foscs时钟的分频系数
// 					div16Enable：		16位除法器选择位
// 					mul16Enable：		16位乘法器选择位

// Return:				void

// Remark:				如果选择32.768kHz振荡器为OSCSCLK， 该函数系统时钟配置无效（仅对内建24MHzRC做过测试）

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void SYSTEM_Init(CLOCK_SYS_TypeDef divNum, FUNCTION_State div16Enable, FUNCTION_State mul16Enable)
// {
// 	unsigned char divValue,temp;

// 	BANK0_SET;													//指向Bank0

// 	divValue= (unsigned char)(divNum);
// 	temp	= CLKCON;											//暂存寄存器值

// 	CLKCON	= (temp & 0x9f) | (divValue << 5);					//更新BIT5/BIT6位，配置系统时钟

// 	if(div16Enable)
// 	{
// 		INSCON |= (BIT3);										//选择16位除法
// 	}
// 	else
// 	{
// 		INSCON &= (~BIT3);										//选择8位除法
// 	}

// 	if(mul16Enable)
// 	{
// 		INSCON |= (BIT2);										//选择16位乘法
// 	}
// 	else
// 	{
// 		INSCON &= (~BIT2);										//选择8位乘法
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		看门狗时间配置

// Input:				wdtOverTime：		看门狗溢出时间

// Return:				void

// Remark:				看门狗复位需要在代码选项中开启

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void WDT_Init(WDT_OVER_TIME_TypeDef wdtOverTime)
// {
// 	unsigned char overTime,temp;

// 	BANK0_SET;													//指向Bank0

// 	overTime= (unsigned char)(wdtOverTime);
// 	temp	= RSTSTAT;											//暂存寄存器的值

// 	RSTSTAT = (temp & 0xF8) | overTime;							//更新寄存器的BIT0/BIT1/BIT2位
// }

// /*-----------------------------------------------------------------------------
// Description:		初始化GPIO口，配置各引脚模式

// Input:				Px：				配置哪一组GPIO口
// 					*GPIO_InitStruct：	需要配置的IO口信息（传递参数为地址）
// 										PinNumber:	需要配置的引脚
// 										PinMode：	输入/输出、带上拉/不带上拉
// Return:				void

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void GPIO_Init(GPIO_GROUP_TypeDef Px, GPIO_INIT_MemberDef *GPIO_InitStruct)
// {
// 	unsigned char operatingPin;									//需要操作的引脚

// 	unsigned char temp;

// 	BANK0_SET;													//指向Bank0

// 	operatingPin = (unsigned char)(GPIO_InitStruct->PinNumber);	//赋值需要操作的引脚

// 	/*赋值操作引脚的输入输出状态*/
// 	if((GPIO_InitStruct->PinMode == GPIO_MODE_OUT_NUP) || (GPIO_InitStruct->PinMode == GPIO_MODE_OUT_UP))	//输出状态时，置输出
// 	{
// 		temp = operatingPin;									//赋值（操作引脚位为1，非操作引脚位为0）
// 	}
// 	else if((GPIO_InitStruct->PinMode == GPIO_MODE_IN_NUP) || (GPIO_InitStruct->PinMode == GPIO_MODE_IN_UP))//输入状态时，置输入
// 	{
// 		temp = 0x00;											//赋值（操作引脚位为0，非操作引脚位为0）
// 	}
// 	else														//无效模式
// 	{
// 		temp = 0xFF;											//全部做输出
// 	}

// 	if(Px == GPIO_P0)
// 	{
// 		REGISTER_OUT_IN_GPIO_0 = (REGISTER_OUT_IN_GPIO_0 & (~operatingPin)) | temp;		//寄存器操作引脚位清零后，做输出的则置1
// 	}
// 	else if(Px == GPIO_P1)
// 	{
// 		REGISTER_OUT_IN_GPIO_1 = (REGISTER_OUT_IN_GPIO_1 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P2)
// 	{
// 		REGISTER_OUT_IN_GPIO_2 = (REGISTER_OUT_IN_GPIO_2 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P3)
// 	{
// 		REGISTER_OUT_IN_GPIO_3 = (REGISTER_OUT_IN_GPIO_3 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P4)
// 	{
// 		REGISTER_OUT_IN_GPIO_4 = (REGISTER_OUT_IN_GPIO_4 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P5)
// 	{
// 		/*BANK1*/
// 		BANK1_SET;												//指向Bank1
// 		REGISTER_OUT_IN_GPIO_5 = (REGISTER_OUT_IN_GPIO_5 & (~operatingPin)) | temp;
// 		BANK0_SET;												//指向Bank0
// 	}
// 	else
// 	{
// 		//无效GPIO口，不做处理
// 	}

// 	/*赋值操作引脚的上拉使能状态*/
// 	if((GPIO_InitStruct->PinMode == GPIO_MODE_OUT_UP) || (GPIO_InitStruct->PinMode == GPIO_MODE_IN_UP))	//上拉使能时，使能上拉
// 	{
// 		temp = operatingPin;									//赋值（操作引脚位为1，非操作引脚位为0）
// 	}
// 	else if((GPIO_InitStruct->PinMode == GPIO_MODE_OUT_NUP) || (GPIO_InitStruct->PinMode == GPIO_MODE_IN_NUP))//上拉无效时，无效上拉
// 	{
// 		temp = 0x00;											//赋值（操作引脚位为0，非操作引脚位为0）
// 	}
// 	else														//无效模式
// 	{
// 		temp = 0x00;											//全部不带上拉
// 	}
	
// 	if(Px == GPIO_P0)
// 	{
// 		REGISTER_UP_NUL_GPIO_0 = (REGISTER_UP_NUL_GPIO_0 & (~operatingPin)) | temp;		//寄存器操作引脚位清零后，带上拉的则置1
// 	}
// 	else if(Px == GPIO_P1)
// 	{
// 		REGISTER_UP_NUL_GPIO_1 = (REGISTER_UP_NUL_GPIO_1 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P2)
// 	{
// 		REGISTER_UP_NUL_GPIO_2 = (REGISTER_UP_NUL_GPIO_2 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P3)
// 	{
// 		REGISTER_UP_NUL_GPIO_3 = (REGISTER_UP_NUL_GPIO_3 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P4)
// 	{
// 		REGISTER_UP_NUL_GPIO_4 = (REGISTER_UP_NUL_GPIO_4 & (~operatingPin)) | temp;
// 	}
// 	else if(Px == GPIO_P5)
// 	{
// 		/*BANK1*/
// 		BANK1_SET;												//指向Bank1
// 		REGISTER_UP_NUL_GPIO_5 = (REGISTER_UP_NUL_GPIO_5 & (~operatingPin)) | temp;
// 		BANK0_SET;												//指向Bank0
// 	}
// 	else
// 	{
// 		//无效GPIO口，不做处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		设置GPIO引脚电平

// Input:				Px：				配置哪一组GPIO口
// 					pinNumber：			需要配置的引脚
// 					pinLevel：			引脚电平状态

// Return:				void

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void GPIO_SetBits(GPIO_GROUP_TypeDef Px, unsigned char pinNumber, GPIO_LEVEL_State pinLevel)
// {
// 	unsigned char temp;

// 	BANK0_SET;													//指向Bank0

// 	if(pinLevel == LEVEL_HIGH)									//需要置高电平时，对应引脚置高
// 	{
// 		temp = pinNumber;										//赋值（操作引脚位为1，非操作引脚位为0）
// 	}
// 	else if(pinLevel == LEVEL_LOW)								//需要置低电平时，对应引脚置低
// 	{
// 		temp = 0x00;											//赋值（操作引脚位为0，非操作引脚位为0）
// 	}
// 	else														//无效电平
// 	{
// 		temp = 0x00;											//无效电平，输出低
// 	}

// 	/*赋值操作引脚的电平状态*/
// 	if(Px == GPIO_P0)
// 	{
// 		REGISTER_PIN_LEVEL_GPIO_0 = (REGISTER_PIN_LEVEL_GPIO_0 & (~pinNumber)) | temp;		//寄存器操作引脚位清零后，高电平的则置1
// 	}
// 	else if(Px == GPIO_P1)
// 	{
// 		REGISTER_PIN_LEVEL_GPIO_1 = (REGISTER_PIN_LEVEL_GPIO_1 & (~pinNumber)) | temp;
// 	}
// 	else if(Px == GPIO_P2)
// 	{
// 		REGISTER_PIN_LEVEL_GPIO_2 = (REGISTER_PIN_LEVEL_GPIO_2 & (~pinNumber)) | temp;
// 	}
// 	else if(Px == GPIO_P3)
// 	{
// 		REGISTER_PIN_LEVEL_GPIO_3 = (REGISTER_PIN_LEVEL_GPIO_3 & (~pinNumber)) | temp;
// 	}
// 	else if(Px == GPIO_P4)
// 	{
// 		REGISTER_PIN_LEVEL_GPIO_4 = (REGISTER_PIN_LEVEL_GPIO_4 & (~pinNumber)) | temp;
// 	}
// 	else if(Px == GPIO_P5)
// 	{
// 		/*BANK1*/
// 		BANK1_SET;												//指向Bank1
// 		REGISTER_PIN_LEVEL_GPIO_5 = (REGISTER_PIN_LEVEL_GPIO_5 & (~pinNumber)) | temp;
// 		BANK0_SET;												//指向Bank0
// 	}
// 	else
// 	{
// 		//无效GPIO口，不做处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		UART的引脚映射

// Input:				uartNum：			配置哪一组UART
// 					txdPin：			TXD映射到哪个引脚
// 					rxdPin：			RXD映射到哪个引脚

// Return:				void

// Remark:				必须在同一组UART里映射才有效

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void LCM_UART_Config(UART_TypeDef uartNum, LCM_UART_PIN_TypeDef txdPin, LCM_UART_PIN_TypeDef rxdPin)
// {
// 	unsigned char mappingPinTXD;
// 	unsigned char mappingPinRXD;

// 	mappingPinTXD = (unsigned char)(txdPin);
// 	mappingPinRXD = (unsigned char)(rxdPin);

// 	BANK1_SET;

// 	if(uartNum == CONFIG_UART0)
// 	{
// 		if(((mappingPinTXD & 0x80) == 0) && ((mappingPinRXD & 0x80) == 0))		//最高位均为0，配置的是UART0
// 		{
// 			UART0CR = (mappingPinTXD | mappingPinRXD);
// 		}
// 		else
// 		{
// 			//引脚配置错误，不做处理
// 		}
// 	}
// 	else if(uartNum == CONFIG_UART1)
// 	{
// 		if(((mappingPinTXD & 0x80) == BIT7) && ((mappingPinRXD & 0x80) == BIT7))//最高位均为1，配置的是UART1
// 		{
// 			mappingPinTXD &= 0x7F;												//清最高位
// 			mappingPinRXD &= 0x7F;												//清最高位
// 			UART1CR = (mappingPinTXD | mappingPinRXD);
// 		}
// 		else
// 		{
// 			//引脚配置错误，不做处理
// 		}
// 	}
// 	else
// 	{
// 		//无效配置口，不做处理
// 	}

// 	BANK0_SET;
// }

// /*-----------------------------------------------------------------------------
// Description:		TWI的引脚映射

// Input:				sckPin：			SCK映射到哪个引脚
// 					sdaPin：			SDA映射到哪个引脚
					
// Return:				void

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void LCM_TWI_Config(LCM_TWI_PIN_TypeDef sckPin, LCM_TWI_PIN_TypeDef sdaPin)
// {
// 	unsigned char mappingPinSCK;
// 	unsigned char mappingPinSDA;

// 	mappingPinSCK = (unsigned char)(sckPin);
// 	mappingPinSDA = (unsigned char)(sdaPin);

// 	BANK1_SET;

// 	TWICR = (mappingPinSCK | mappingPinSCK);

// 	BANK0_SET;
// }

// /*-----------------------------------------------------------------------------
// Description:		PWM的引脚映射

// Input:				pwmNum：			配置哪一组PWM
// 					pwmPin：			PWM映射到哪个引脚
					
// Return:				void

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void LCM_PWM_Config(PWM_TypeDef pwmNum, LCM_PWM_PIN_TypeDef pwmPin)
// {
// 	unsigned char temp,mappingPinPWM;

// 	mappingPinPWM = (unsigned char)(pwmPin);

// 	BANK1_SET;

// 	temp = PWMCR;											//暂存PWMCR寄存器的值

// 	if(pwmNum == CONFIG_PWM0)								//配置PWM0
// 	{
// 		PWMCR = (temp & 0xF8) | mappingPinPWM;				//更新BIT0/BIT1/BTI2位的值
// 	}
// 	else if(pwmNum == CONFIG_PWM1)
// 	{
// 		PWMCR = (temp & 0x8F) | mappingPinPWM;				//更新BIT4/BIT5/BTI6位的值
// 	}
// 	else
// 	{
// 		//无效配置口，不做处理
// 	}

// 	BANK0_SET;
// }

// /*-----------------------------------------------------------------------------
// Description:		PWM初始化

// Input:				pwmNum：			配置哪一组PWM
// 					*PWM_InitStruct：	需要配置的PWM信息（传递参数为地址）
// 										OutEnable：	模块输出使能
// 										IntEnable：	周期中断允许位（IEN2寄存器中的EPWMx位置1时有效）
// 										Clock：		时钟源
// 										Polarity：	输出极性
// 										FunEnable：	功能使能
					
// Return:				void

// Remark:				该初始化函数会主动清PWMxIF位（PWMx周期计数器溢出位）

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void PWM_Init(PWM_TypeDef pwmNum, PWM_INIT_MemberDef *PWM_InitStruct)
// {
// 	unsigned char pwmxSS,pwmxIF,pwmxIE,pwmxCK,pwmxS,pwmxEN;

// 	pwmxSS = (((unsigned char)(PWM_InitStruct->OutEnable)) & (0x01));		/*	0:PWMx输出禁止，用作I/O等功能
// 																			注： 如果此位为0而PWMxEN = 1，则整个PWMx模块仍然正常运行，只是波形输出被禁
// 																			止， PWMx模块可以做一个定时器来使用。
// 																				1:PWMx输出允许
// 																			注： 如果此位设为1而PWMxEN位为0，则PWMx输出非选电平（选为高电平有效则出低
// 																			电平，低电平有效则出高电平）。*/

// 	pwmxIF = 0 << 1;														//清PWM中断标志

// 	pwmxIE = (((unsigned char)(PWM_InitStruct->IntEnable)) & 0x01) << 2;	//PWM周期中断允许位

// 	pwmxCK = (((unsigned char)(PWM_InitStruct->Clock)) & 0x07) << 3;		//PWM时钟源选择位

// 	pwmxS  = (((unsigned char)(PWM_InitStruct->Polarity)) & 0x01) << 6;		/*PWM极性选择
// 																			0： PWMx占空比期间输出高电平，占空比溢出后输出低电平
// 																			1： PWMx占空比期间输出低电平，占空比溢出后输出高电平*/
	
// 	pwmxEN = (((unsigned char)(PWM_InitStruct->FunEnable)) & 0x01) << 7;	//PWM模块使能位

// 	BANK0_SET;																//指向Bank0

// 	if(pwmNum == CONFIG_PWM0)
// 	{
// 		PWM0CON = (pwmxSS | pwmxIF | pwmxIE | pwmxCK | pwmxS | pwmxEN);
// 	}
// 	else if(pwmNum == CONFIG_PWM1)
// 	{
// 		BANK1_SET;
// 		PWM1CON = (pwmxSS | pwmxIF | pwmxIE | pwmxCK | pwmxS | pwmxEN);
// 		BANK0_SET;
// 	}
// 	else
// 	{
// 		//无效配置口，不做处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		PWM的输出周期和占空比调整

// Input:				pwmNum：			配置哪一组PWM
// 					pwmCycle：			输出周期配置（PWMx输出周期 = [PWMxPH， PWMxPL] X PWM时钟周期）
// 										PWMx的计数器计满至PWMxPH/L中的值后归零，若PWMxPH/L为0时，如果PWMxS为0，
// 										则PWMx引脚输出低电平；如果PWMxS为1，则PWMx引脚输出高电平。

// 					pwmDuty：		1. 	当PWMxP ≤ PWMxD时
// 										如果PWMxS = 0，则PWMx引脚输出高电平
// 										如果PWMxS = 1，则PWMx引脚输出低电平
// 									2. 	当PWMxD = 00H时
// 										如果PWMxS = 0，则PWMx引脚输出低电平
// 										如果PWMxS = 1，则PWMx引脚输出高电平
					
// Return:				void

// Remark:				修改寄存器PWMxPH将使得PWMx的输出在下一个周期生效。用户需先修改PWMxPL，再修改PWMxPH以修改PWM
// 					周期。无论PWMxPH是否被修改，都需要将PWMxPH再写一次。

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void PWM_WaveformAdj(PWM_TypeDef pwmNum, unsigned int pwmCycle, unsigned int pwmDuty)
// {
// 	BANK0_SET;																//指向Bank0

// 	if((pwmCycle < 0x0FFF) && (pwmDuty < 0x0FFF))							//配置值在配置范围内
// 	{
// 		if(pwmNum == CONFIG_PWM0)
// 		{
// 			PWM0PL = (unsigned char)((pwmCycle & 0x00ff));					//取pwmCycle低八位（先写低八位）
// 			PWM0PH = (unsigned char)((pwmCycle >> 8) & 0x0f);				//取pwmCycle高八位的后4位
// 			PWM0DL = (unsigned char)((pwmDuty & 0x00ff));					//取pwmCycle低八位
// 			PWM0DH = (unsigned char)((pwmDuty >> 8) & 0x0f);				//取pwmDuty高八位的后4位
// 		}
// 		else if(pwmNum == CONFIG_PWM1)
// 		{
// 			BANK1_SET;
// 			PWM1PL = (unsigned char)((pwmCycle & 0x00ff));					//取pwmCycle低八位（先写低八位）
// 			PWM1PH = (unsigned char)((pwmCycle >> 8) & 0x0f);				//取pwmCycle高八位的后4位
// 			PWM1DL = (unsigned char)((pwmDuty & 0x00ff));					//取pwmCycle低八位
// 			PWM1DH = (unsigned char)((pwmDuty >> 8) & 0x0f);				//取pwmDuty高八位的后4位
// 			BANK0_SET;
// 		}
// 		else
// 		{
// 			//无效配置口，不做处理
// 		}
// 	}
// 	else
// 	{
// 		//配置值超过范围，不做处理
// 	}
	
// }

// /*-----------------------------------------------------------------------------
// Description:		PWM模块使能

// Input:				pwmNum：			配置哪一组PWM
// 					state：				PWM模块及输出使能状态
					
// Return:				void

// Remark:				该函数同时禁止或允许PWM模块和引脚输出功能

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void PWM_ModuleEnable(PWM_TypeDef pwmNum, FUNCTION_State state)
// {
// 	if(pwmNum == CONFIG_PWM0)
// 	{
// 		if(state)
// 		{
// 			PWM0CON |= (BIT7 | BIT0);								//使能PWM模块，允许引脚输出
// 		}
// 		else
// 		{
// 			PWM0CON &= ~(BIT7 | BIT0);								//禁止PWM模块，禁止引脚输出
// 		}
// 	}
// 	else if(pwmNum == CONFIG_PWM1)
// 	{
// 		BANK1_SET;
// 		if(state)
// 		{
// 			PWM1CON |= (BIT7 | BIT0);
// 		}
// 		else
// 		{
// 			PWM1CON &= ~(BIT7 | BIT0);
// 		}
// 		BANK0_SET;
// 	}
// 	else
// 	{
// 		//无效配置口，不做处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		UART初始化

// Input:				uartNum：			配置哪一组UART
// 					*UART_InitStruct：	需要配置的UART信息（传递参数为地址）
// 										Mode：		工作方式
// 										CheckBit：	第9位“1”校验器
// 										LevelLogic：RXD输出电平的逻辑
					
// Return:				void

// Remark:				不适用于帧出错检测（SSTAT=0，工作方式作为SM0， SM1， SM2）仅验证过方式1

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void UART_Init(UART_TypeDef uartNum, UART_INIT_MemberDef *UART_InitStruct)
// {
// 	unsigned char uartMode,uartCheckBit,rxdLevelLogic,temp;

// 	uartMode 	  = (unsigned char)(UART_InitStruct->Mode);		/*	00：方式0，同步方式，固定波特率
// 																	01：方式1，8位异步方式，可变波特率
// 																	10：方式2，9位异步方式，固定波特率
// 																	11：方式3，9位异步方式，可变波特率*/

// 	uartCheckBit  = (unsigned char)(UART_InitStruct->CheckBit);	/*0:在方式0下，波特率是系统时钟的1/12
// 																	在方式1下，禁止停止位确认检验，任何停止位都会置位RI
// 																	在方式2和3下，任何字节都会置位RI
// 																  1:在方式0下，波特率是系统时钟的1/4
// 																	在方式1下，允许停止位确认检验，只有有效的停止位（1）才能置位RI
// 																	在方式2和3下，只有地址字节（第9位 = 1）才能置位RI*/

// 	rxdLevelLogic = (unsigned char)(UART_InitStruct->LevelLogic);/* RXDx输出电平逻辑控制位
// 																	0：输入高电平阈值为0.8VDD，输入低电平阈值为0.2VDD（CMOS逻辑）
// 																	1：输入高电平阈值为2.0V， 输入低电平阈值为0.8V（VDD = 4.5 - 5.5V）（TTL逻辑）
// 																	输入高电平阈值为0.25VDD+0.8，输入低电平阈值为0.15VDD（VDD = 2.0V-4.5V）
// 																	注意： PowerDown模式下， TLL逻辑无效*/

// 	BANK0_SET;														//指向Bank0

// 	if(uartNum == CONFIG_UART0)
// 	{
// 		temp = SCON;												//赋值寄存器值

// 		SCON = (temp & 0x1F) | ((uartMode << 6) | (uartCheckBit << 5));	//更新BIT5/BIT6/BIT7的值

// 		if(rxdLevelLogic == CMOS)
// 		{
// 			UTOS &= ~BIT0;
// 		}
// 		else
// 		{
// 			UTOS |= BIT0;
// 		}
// 	}
// 	else if(uartNum == CONFIG_UART1)
// 	{
// 		BANK1_SET;

// 		temp = SCON1;												//赋值寄存器值

// 		SCON1 = (temp & 0x1F) | ((uartMode << 6) | (uartCheckBit << 5));	//更新BIT5/BIT6/BIT7的值

// 		BANK0_SET;

// 		if(rxdLevelLogic == CMOS)
// 		{
// 			UTOS &= ~BIT1;
// 		}
// 		else
// 		{
// 			UTOS |= BIT1;
// 		}
// 	}
// 	else
// 	{
// 		//无效配置口，不处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		UART的RXD接收器允许初始化

// Input:				uartNum：			配置哪一组UART
// 					state：				状态（使能/禁止）
					
// Return:				void

// Remark:

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void UART_RXD_Cmd(UART_TypeDef uartNum, FUNCTION_State state)
// {
// 	BANK0_SET;

// 	if(uartNum == CONFIG_UART0)
// 	{
// 		if(state)
// 		{
// 			SCON |= BIT4;									//接收允许
// 		}
// 		else
// 		{
// 			SCON &= (~BIT4);								//禁止接收
// 		}
// 	}
// 	else if(uartNum == CONFIG_UART1)
// 	{
// 		BANK1_SET;

// 		if(state)
// 		{
// 			SCON1 |= BIT4;
// 		}
// 		else
// 		{
// 			SCON1 &= (~BIT4);
// 		}

// 		BANK0_SET;
// 	}
// 	else
// 	{
// 		//无效配置口，不处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		UART的波特率配置

// Input:				uartNum：			配置哪一组UART
// 					baud：				波特率
// 					state：				状态（使能，禁止）
					
// Return:				void

// Remark:				注意remainde最大值为256，波特率配置不宜过低（如24Mhz下，波特率不得低于2560）

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void UART_BaudRate_Init(UART_TypeDef uartNum, unsigned long baud, FUNCTION_State state)
// {
// 	unsigned char	remainde;
// 	unsigned int	sbrt;

// 	BANK0_SET;														//指向Bank0

// 	if(uartNum == CONFIG_UART0)
// 	{
// 		if(state)													//使能波特率发生器
// 		{
// 			remainde = (unsigned char)(SYSTEM_FOSC/16/baud);		//计算参考规格书

// 			sbrt	= 32768 - remainde;

// 			SBRTH	= (unsigned char)((sbrt & 0xff00) >> 8);		//赋值波特率发生寄存器
// 			SBRTL	= (unsigned char)(sbrt & 0x00ff);

// 			SFINE	= (unsigned char)(SYSTEM_FOSC/baud - 16*remainde);	//赋值波特率微调寄存器

// 			SBRTH	|= BIT7;										//使能波特率发生器
// 		}
// 		else
// 		{
// 			SBRTH	&= (~BIT7);										//关闭波特率发生器
// 		}
// 	}
// 	else if(uartNum == CONFIG_UART1)
// 	{
// 		BANK1_SET;

// 		if(state)													//使能波特率发生器
// 		{
// 			remainde = (unsigned char)(SYSTEM_FOSC/16/baud);		//计算参考规格书

// 			sbrt	= 32768 - remainde;

// 			SBRTH1	= (unsigned char)((sbrt & 0xff00) >> 8);		//赋值波特率发生寄存器
// 			SBRTL1	= (unsigned char)(sbrt & 0x00ff);

// 			SFINE1	= (unsigned char)(SYSTEM_FOSC/baud - 16*remainde);	//赋值波特率微调寄存器

// 			SBRTH1	|= BIT7;										//使能波特率发生器
// 		}
// 		else
// 		{
// 			SBRTH1	&= (~BIT7);										//关闭波特率发生器
// 		}
		
// 		BANK0_SET;
// 	}
// 	else
// 	{
// 		//无效配置口，不处理
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		中断优先级及使能配置

// Input:				function：			配置哪一个中断
// 					priority：			中断优先级
// 					state：				状态（使能，禁止）
					
// Return:				void

// Remark:				(1) 打开外部中断4时，相应的端口必须设置为输入状态。
// 					(2) 若要打开外部中断4， EX4和IENC中的EXS4x（x = 0 - 7）相应位必须同时置1（在其它函数中配置）

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void IEN_Init(IEN_TypeDef function, IEN_PRIORITY_TypeDef priority, FUNCTION_State state)
// {
// 	unsigned char priorityCrlRegister 	= PRIORITY_REG_NULL;
// 	unsigned char priorityCrlBit		= 0;

// 	BANK0_SET;												//指向Bank0

// 	switch (function)										//查询需要配置的中断功能
// 	{
// 	case IEN_ADC:
// 		if(state)
// 		{
// 			IEN0 |= BIT6;									//使能
// 		}
// 		else
// 		{
// 			IEN0 &= ~BIT6;									//禁止
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;				//中断优先级需要配置的寄存器为IP0
// 		priorityCrlBit		= BIT6;							//中断优先级需要配置的位为BIT6
// 		break;

// 	case IEN_T3:
// 		if(state)
// 		{
// 			IEN0 |= BIT5;
// 		}
// 		else
// 		{
// 			IEN0 &= ~BIT5;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT5;
// 		break;

// 	case IEN_UART0:
// 		if(state)
// 		{
// 			IEN0 |= BIT4;
// 		}
// 		else
// 		{
// 			IEN0 &= ~BIT4;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT4;
// 		break;

// 	case IEN_T5:
// 		if(state)
// 		{
// 			IEN0 |= BIT3;
// 		}
// 		else
// 		{
// 			IEN0 &= ~BIT3;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT3;
// 		break;

// 	case IEN_TK:
// 		if(state)
// 		{
// 			IEN0 |= BIT1;
// 		}
// 		else
// 		{
// 			IEN0 &= ~BIT1;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT1;
// 		break;

// 	case IEN_SMC:
// 		if(state)
// 		{
// 			IEN1 |= BIT7;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT7;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT7;
// 		break;

// 	case IEN_LPD:
// 		if(state)
// 		{
// 			IEN1 |= BIT6;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT6;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT6;
// 		break;

// 	case IEN_X2:
// 		if(state)
// 		{
// 			IEN1 |= BIT5;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT5;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT5;
// 		break;

// 	case IEN_X3:
// 		if(state)
// 		{
// 			IEN1 |= BIT4;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT4;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT4;
// 		break;

// 	case IEN_X4:
// 		if(state)
// 		{
// 			IEN1 |= BIT3;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT3;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT3;
// 		break;

// 	case IEN_T4:
// 		if(state)
// 		{
// 			IEN1 |= BIT2;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT2;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT2;
// 		break;

// 	case IEN_LED:
// 		if(state)
// 		{
// 			IEN1 |= BIT1;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT1;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT1;
// 		break;

// 	case IEN_TWI:
// 		if(state)
// 		{
// 			IEN1 |= BIT0;
// 		}
// 		else
// 		{
// 			IEN1 &= ~BIT0;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP1;
// 		priorityCrlBit		= BIT0;
// 		break;

// 	case IEN_PWM1:
// 		if(state)
// 		{
// 			IEN2 |= BIT6;
// 		}
// 		else
// 		{
// 			IEN2 &= ~BIT6;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;				//PWM1/EUART1/CRC/PWM0/EPCA0中断优先级共享优先级设置
// 		priorityCrlBit		= BIT7;
// 		break;

// 	case IEN_PCA0:
// 		if(state)
// 		{
// 			IEN2 |= BIT5;
// 		}
// 		else
// 		{
// 			IEN2 &= ~BIT5;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT7;
// 		break;

// 	case IEN_PWM0:
// 		if(state)
// 		{
// 			IEN2 |= BIT3;
// 		}
// 		else
// 		{
// 			IEN2 &= ~BIT3;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT7;
// 		break;

// 	case IEN_CRC:
// 		if(state)
// 		{
// 			IEN2 |= BIT2;
// 		}
// 		else
// 		{
// 			IEN2 &= ~BIT2;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT7;
// 		break;

// 	case IEN_UART1:
// 		if(state)
// 		{
// 			IEN2 |= BIT0;
// 		}
// 		else
// 		{
// 			IEN2 &= ~BIT0;
// 		}

// 		priorityCrlRegister = PRIORITY_REG_IP0;
// 		priorityCrlBit		= BIT7;
// 		break;
	
// 	default:
// 		priorityCrlRegister = PRIORITY_REG_NULL;
// 		priorityCrlBit		= BIT0;
// 		break;
// 	}

// 	switch (priority)
// 	{
// 	case PRIORITY_LEVEL_0:										//优先级等级0，IPL和IPH均为0
// 		if(priorityCrlRegister == PRIORITY_REG_IP0)
// 		{
// 			IPL0 &= ~priorityCrlBit;
// 			IPH0 &= ~priorityCrlBit;
// 		}
// 		else if(priorityCrlRegister == PRIORITY_REG_IP1)
// 		{
// 			IPL1 &= ~priorityCrlBit;
// 			IPH1 &= ~priorityCrlBit;
// 		}
// 		else
// 		{
// 			//无优先级配置，不处理
// 		}
// 		break;
// 	case PRIORITY_LEVEL_1:										//优先级等级1，IPL置1，IPH置0
// 		if(priorityCrlRegister == PRIORITY_REG_IP0)
// 		{
// 			IPL0 |=  priorityCrlBit;
// 			IPH0 &= ~priorityCrlBit;
// 		}
// 		else if(priorityCrlRegister == PRIORITY_REG_IP1)
// 		{
// 			IPL1 |=  priorityCrlBit;
// 			IPH1 &= ~priorityCrlBit;
// 		}
// 		else
// 		{
// 			//无优先级配置，不处理
// 		}
// 		break;
// 	case PRIORITY_LEVEL_2:										//优先级等级2，IPL置0，IPH置1
// 		if(priorityCrlRegister == PRIORITY_REG_IP0)
// 		{
// 			IPL0 &= ~priorityCrlBit;
// 			IPH0 |=  priorityCrlBit;
// 		}
// 		else if(priorityCrlRegister == PRIORITY_REG_IP1)
// 		{
// 			IPL1 &= ~priorityCrlBit;
// 			IPH1 |=  priorityCrlBit;
// 		}
// 		else
// 		{
// 			//无优先级配置，不处理
// 		}
// 		break;
// 	case PRIORITY_LEVEL_3:										//优先级等级3，IPL置1，IPH置1
// 		if(priorityCrlRegister == PRIORITY_REG_IP0)
// 		{
// 			IPL0 |=  priorityCrlBit;
// 			IPH0 |=  priorityCrlBit;
// 		}
// 		else if(priorityCrlRegister == PRIORITY_REG_IP1)
// 		{
// 			IPL1 |=  priorityCrlBit;
// 			IPH1 |=  priorityCrlBit;
// 		}
// 		else
// 		{
// 			//无优先级配置，不处理
// 		}
// 		break;
// 	default:
// 		break;
// 	}
// }

// /*-----------------------------------------------------------------------------
// Description:		对扇区进行擦除

// Input:				sectorAddr:			16位，[15:9]表示扇区号，范围：0~63（FLSH最大64个扇区，类EEPROM最大8个扇区）;[8]表示存储单元的高位地址；[7:0]表示存储单元的地位地址
// 										如：sectorAddr = 0x0345；表示选择1号扇区，地址为0x0145;
// 					sectorSelect:		选择对FLSH或EEPROM编程
					
// Return:				void

// Remark:				

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void SectorErase(unsigned int sectorAddr, SSP_OBJECT_State sectorSelect)
// {
// 	unsigned char EAtemp;
	
// 	BANK0_SET;														//指向Bank0

// 	EAtemp = EA;
// 	EA = 0;
	
// 	if(sectorSelect == FLASH_CRL)					//对FLASH操作时FAC位清零
// 	{
// 	  FLASHCON &= ~BIT0;
// 	}
// 	else if(sectorSelect == EEPROM_CRL)				//对类EEPROM操作时FAC位置1
// 	{
// 	  FLASHCON |= BIT0;
// 	}
// 	else
// 	{
// 		//无效编程对象，不处理
// 	}
	
	
// 	XPAGE = ((sectorAddr >> 9) << 1);				//赋值扇区号
// 	if(((sectorAddr >>8 ) % 2) == 0)				//赋值存储单元的高位地址
// 	{
// 		XPAGE |= 0x00;
// 	}
// 	else
// 	{
// 		XPAGE |= 0x01;
// 	}
	
// 	//存储单元删除
// 	IB_CON1=0xE6;
// 	IB_CON2=0x05;
// 	IB_CON3=0x0A;
// 	IB_CON4=0x09;
// 	IB_CON5=0x06;
// 	_nop_();
// 	_nop_();
// 	_nop_(); 
// 	_nop_();
	
// 	XPAGE=0x00;
// 	FLASHCON &= ~BIT0;								//当不需对类EEPROM操作时，必须将FAC位清0。
	
// 	EA = EAtemp;
// }

// /*-----------------------------------------------------------------------------
// Description:		对扇区进行编程

// Input:				sectorAddr：		16位，[15:9]表示扇区号，范围：0~63（FLSH最大64个扇区，类EEPROM最大8个扇区）;[8]表示存储单元的高位地址；[7:0]表示存储单元的地位地址
// 										如：sectorAddr = 0x0345；表示选择1号扇区，地址为0x0145;
// 					byteData：			需要编程的数据
// 					sectorSelect：		选择对FLSH或EEPROM编程
					
// Return:				void

// Remark:				

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// void SectorWrite(unsigned int sectorAddr, unsigned char byteData, SSP_OBJECT_State sectorSelect)
// {
// 	unsigned char EAtemp;
	
// 	BANK0_SET;														//指向Bank0
	
// 	EAtemp = EA;
// 	EA = 0;
	
// 	if(sectorSelect == FLASH_CRL)					//对FLASH操作时FAC位清零
// 	{
// 	  FLASHCON &= ~BIT0;
// 	}
// 	else if(sectorSelect == EEPROM_CRL)				//对类EEPROM操作时FAC位置1
// 	{
// 	  FLASHCON |= BIT0;
// 	}
// 	else
// 	{
// 		//无效编程对象，不处理
// 	}
	
// 	XPAGE = ((sectorAddr >> 9) << 1);				//赋值扇区号
// 	if(((sectorAddr >> 8) % 2) == 0)				//赋值存储单元的高位地址
// 	{
// 		XPAGE |= 0x00;
// 	}
// 	else
// 	{
// 		XPAGE |= 0x01;
// 	}
// 	IB_OFFSET = (unsigned char)(sectorAddr & 0x00ff);	//赋值存储单元的低8位地址
	
// 	IB_DATA = byteData;
	
// 	//存储单元编程
// 	IB_CON1=0x6E;
// 	IB_CON2=0x05;
// 	IB_CON3=0x0A;
// 	IB_CON4=0x09;
// 	IB_CON5=0x06;
// 	_nop_();
// 	_nop_();
// 	_nop_(); 
// 	_nop_();
	
// 	XPAGE=0x00;
// 	FLASHCON &= ~BIT0;								//当不需对类EEPROM操作时，必须将FAC位清0。
	
// 	EA = EAtemp;
// }

// /*-----------------------------------------------------------------------------
// Description:		对扇区数据进行读取

// Input:				sectorAddr：		16位，[15:9]表示扇区号，范围：0~63（FLSH最大64个扇区，类EEPROM最大8个扇区）;[8]表示存储单元的高位地址；[7:0]表示存储单元的地位地址
// 										如：sectorAddr = 0x0345；表示选择1号扇区，地址为0x0145;
// 					sectorSelect：		选择对FLSH或EEPROM编程
					
// Return:				byteData：			对应地址的数据

// Remark:				

// Author:				胡望伟
// History:			V1.0（20200807）：首次编辑
// -----------------------------------------------------------------------------*/
// unsigned char SectorRead(unsigned int sectorAddr, SSP_OBJECT_State sectorSelect)
// {
// 	unsigned char byteData;
// 	unsigned char EAtemp;
	
// 	BANK0_SET;														//指向Bank0
	
// 	EAtemp = EA;
// 	EA = 0;
	
// 	if(sectorSelect == FLASH_CRL)					//对FLASH操作时FAC位清零
// 	{
// 	  FLASHCON &= ~BIT0;
// 	}
// 	else if(sectorSelect == EEPROM_CRL)				//对类EEPROM操作时FAC位置1
// 	{
// 	  FLASHCON |= BIT0;
// 	}
// 	else
// 	{
// 		//无效编程对象，不处理
// 	}
	
// 	byteData = CBYTE[sectorAddr];
// 	FLASHCON &= ~BIT0;								//当不需对类EEPROM操作时，必须将FAC位清0。
	
// 	EA = EAtemp;
	
// 	return(byteData);
// }

///*-----------------------------------------------------------------------------
//Description:		char型数组清零

//Input:				*buf：				数组（传递参数为地址）
//					len：				数组长度
//					
//Return:				void

//Remark:				数组长度不超过255

//Author:				胡望伟
//History:			V1.0（20200807）：首次编辑
//-----------------------------------------------------------------------------*/
//void InitCharArrayData(unsigned char *buf, unsigned char len)
//{
//	unsigned char i;
//	for(i=0;i<len;i++)
//	{
//		*buf = 0;
//		buf++;	
//	}
//}

///*-----------------------------------------------------------------------------
//Description:		int型数组清零

//Input:				*buf：				数组（传递参数为地址）
//					len：				数组长度
//					
//Return:				void

//Remark:				数组长度不超过255

//Author:				胡望伟
//History:			V1.0（20200807）：首次编辑
//-----------------------------------------------------------------------------*/
//void InitIntArrayData(unsigned int *buf, unsigned char len)
//{
//	unsigned char i;
//	for(i=0;i<len;i++)
//	{
//		*buf = 0;
//		buf++;	
//	}
//}

