#ifndef _COMMON_SINO_WEALTH_H
#define _COMMON_SINO_WEALTH_H

#define SH79F328										//此处配置芯片型号

#if defined SH79F326
	#include <SH79F326.h>
#elif defined SH79F328
	#include <SH79F328.h>
#endif

// #define SYSTEM_FOSC					24000000			//系统时钟定义（用于串口波特率配置计算）系统时钟实际取决于代码选项配置和系统时钟寄存器配置

// /*GPIO的每个引脚宏定义*/
// #define	GPIO_PIN_0					0x01
// #define	GPIO_PIN_1					0x02
// #define	GPIO_PIN_2					0x04
// #define	GPIO_PIN_3					0x08
// #define	GPIO_PIN_4					0x10
// #define	GPIO_PIN_5					0x20
// #define	GPIO_PIN_6					0x40
// #define	GPIO_PIN_7					0x80
// #define	GPIO_PIN_ALL				0xFF

/*1个字节的每个位宏定义*/   
#define	BIT0					    0x01
#define	BIT1					    0x02
#define	BIT2					    0x04
#define	BIT3					    0x08
#define	BIT4					    0x10
#define	BIT5					    0x20
#define	BIT6					    0x40
#define	BIT7					    0x80

/*操作FLAH和EEPROM选择*/
#define FLASH_CRL				0						//操作FLASH
#define EEPROM_CRL				1						//操作EEPROM

/*BANK区切换宏定义*/    
#define BANK0_SET			        (INSCON &= (~BIT6))
#define BANK1_SET			        (INSCON |= BIT6)

// /*输入/输出，带上拉/不带上拉，输出电平寄存器宏定义*/
// #define REGISTER_OUT_IN_GPIO_0		P0CR
// #define REGISTER_OUT_IN_GPIO_1		P1CR
// #define REGISTER_OUT_IN_GPIO_2		P2CR
// #define REGISTER_OUT_IN_GPIO_3		P3CR
// #define REGISTER_OUT_IN_GPIO_4		P4CR
// #define REGISTER_OUT_IN_GPIO_5		P5CR
// #define REGISTER_UP_NUL_GPIO_0		P0PCR
// #define REGISTER_UP_NUL_GPIO_1		P1PCR
// #define REGISTER_UP_NUL_GPIO_2		P2PCR
// #define REGISTER_UP_NUL_GPIO_3		P3PCR
// #define REGISTER_UP_NUL_GPIO_4		P4PCR
// #define REGISTER_UP_NUL_GPIO_5		P5PCR
// #define REGISTER_PIN_LEVEL_GPIO_0	P0
// #define REGISTER_PIN_LEVEL_GPIO_1	P1
// #define REGISTER_PIN_LEVEL_GPIO_2	P2
// #define REGISTER_PIN_LEVEL_GPIO_3	P3
// #define REGISTER_PIN_LEVEL_GPIO_4	P4
// #define REGISTER_PIN_LEVEL_GPIO_5	P5

// /*中断优先级配置寄存器宏定义*/
// #define PRIORITY_REG_NULL			0
// #define PRIORITY_REG_IP0			1
// #define PRIORITY_REG_IP1			2

// /**************************************************************************************************************
//  ---------------------------------------------------通用定义---------------------------------------------------
//  **************************************************************************************************************/
/*枚举功能状态*/
typedef enum {
	DISABLE				= 0,				//禁止
	ENABLE				= !DISABLE,			//使能
} FUNCTION_State;

// /*枚举输出极性状态*/
// typedef enum {
// 	POSITIVE			= 0,				//正极性
// 	NEGATIVE			= !POSITIVE,		//负极性
// } POLARITY_State;

// /*枚举扇区编程对象*/
// typedef enum {
// 	FLASH_CRL			= 0,				//SSP访问对象FLASH
// 	EEPROM_CRL			= 1,				//SSP访问对象EEPROM
// } SSP_OBJECT_State;

// /**************************************************************************************************************
//  ------------------------------------------------系统时钟相关定义------------------------------------------------
//  **************************************************************************************************************/
// /*枚举系统时钟频率分频器类型*/
// typedef enum {
// 	FOSCS_1				= 0x00,				//系统时钟不分频
// 	FOSCS_2				= 0x01,				//系统时钟 fOSCS/2 分频
// 	FOSCS_4				= 0x02,				//系统时钟 fOSCS/4 分频
// 	FOSCS_12			= 0x03,				//系统时钟 fOSCS/12 分频
// } CLOCK_SYS_TypeDef;

// /**************************************************************************************************************
//  ------------------------------------------------看门狗相关定义------------------------------------------------
//  **************************************************************************************************************/
// /*枚举系统时钟频率分频器类型*/
// typedef enum {
// 	OVER_TIME_1024		= 0x01,				//000 - 001：溢出周期最小值 = 1024ms
// 	OVER_TIME_256		= 0x02,				//溢出周期最小值 = 256ms
// 	OVER_TIME_128		= 0x03,				//溢出周期最小值 = 128ms
// 	OVER_TIME_64		= 0x04,				//溢出周期最小值 = 64ms
// 	OVER_TIME_16		= 0x05,				//溢出周期最小值 = 16ms
// 	OVER_TIME_4			= 0x06,				//溢出周期最小值 = 4ms
// 	OVER_TIME_1			= 0x07,				//溢出周期最小值 = 1ms
// } WDT_OVER_TIME_TypeDef;

// /**************************************************************************************************************
//  ------------------------------------------------GPIO口相关定义------------------------------------------------
//  **************************************************************************************************************/
// /*枚举GPIO组别*/
// typedef enum {
// 	GPIO_P0				= 0,
// 	GPIO_P1				= 1,
// 	GPIO_P2				= 2,
// 	GPIO_P3				= 3,
// 	GPIO_P4				= 4,
// 	GPIO_P5				= 5,
// } GPIO_GROUP_TypeDef;

// /*枚举GPIO口输入输出模式*/
// typedef enum {
// 	GPIO_MODE_OUT_NUP	= 0, 				//输出不带上拉
// 	GPIO_MODE_OUT_UP	= 1, 				//输出带上拉
// 	GPIO_MODE_IN_NUP	= 2, 				//输入不带上拉
// 	GPIO_MODE_IN_UP		= 3, 				//输入带上拉
// } GPIO_MODE_TypeDef;

// /*枚举GPIO电平状态*/
// typedef enum {
// 	LEVEL_LOW			= 0, 				//低电平
// 	LEVEL_HIGH			= 1,				//高电平
// } GPIO_LEVEL_State;	

// /*定义GPIO的配置寄存器结构体成员*/
// typedef struct
// {
// 	unsigned char		PinNumber;			//GPIO的引脚编号

// 	GPIO_MODE_TypeDef	PinMode;			//引脚的输入输出模式

// }GPIO_INIT_MemberDef;

// /**************************************************************************************************************
//  -------------------------------------------------UART相关定义-------------------------------------------------
//  **************************************************************************************************************/
/*枚举UART发送接收状态类型*/
typedef enum {
	SEND_OVER					=   0,					//发送完成
	SEND_GOING					=   1,					//正在发送

}UATR_SEND_TYPE;

typedef enum {
	RECV_OVER					=   0,					//接收完成
	RECV_GOING					=   1,					//正在接收

}UATR_RECV_TYPE;

// /*枚举UART配置通道类型*/
// typedef enum {
// 	CONFIG_UART0		= 0,				//配置串口0
// 	CONFIG_UART1		= 1,				//配置串口1
// } UART_TypeDef;

// /*枚举UART工作方式类型*/
// typedef enum {
// 	UART_MODE_0			= 0x00,				//工作方式0
// 	UART_MODE_1			= 0x01,				//工作方式1
// 	UART_MODE_2			= 0x02,				//工作方式2
// 	UART_MODE_3			= 0x03,				//工作方式3
// } UART_MODE_TypeDef;

// /*枚举RXD电平逻辑状态（UART相关）*/
// typedef enum {
// 	CMOS				= 0,				//输入高电平阈值为0.8VDD，输入低电平阈值为0.2VDD（CMOS逻辑）
// 	TTL					= 1,				//输入高电平阈值为2.0V， 输入低电平阈值为0.8V（VDD = 4.5 - 5.5V）（TTL逻辑）
// 											//输入高电平阈值为0.25VDD+0.8，输入低电平阈值为0.15VDD（VDD = 2.0V-4.5V）
// } RXD_LEVEL_Logic;

// /*定义UART的配置寄存器结构体*/
// typedef struct
// {
// 	UART_MODE_TypeDef	Mode;				//uart工作方式

// 	FUNCTION_State		CheckBit;			//第9位校验器使能

// 	RXD_LEVEL_Logic		LevelLogic;			//RXD输出电平逻辑控制位

// }UART_INIT_MemberDef;

// /**************************************************************************************************************
//  --------------------------------------------------PWM相关定义-------------------------------------------------
//  **************************************************************************************************************/
// /*枚举PWM配置通道*/
// typedef enum {
// 	CONFIG_PWM0			= 0,
// 	CONFIG_PWM1			= 1,
// } PWM_TypeDef;

// /*枚举PWM时钟配置类型*/
// typedef enum {
// 	PWM_FOSC_1			= 0x00,
// 	PWM_FOSC_2			= 0x01,
// 	PWM_FOSC_4			= 0x02,
// 	PWM_FOSC_8			= 0x03,
// 	PWM_FOSC_16			= 0x04,
// 	PWM_FOSC_32			= 0x05,
// 	PWM_FOSC_64			= 0x06,
// 	PWM_FOSC_128		= 0x07,
// } PWM_CLOCK_TypeDef;

// /*定义PWM的配置寄存器结构体*/
// typedef struct
// {
// 	FUNCTION_State		OutEnable;			//模块输出使能

// 	FUNCTION_State		IntEnable;			//中断使能

// 	PWM_CLOCK_TypeDef	Clock;				//时钟源

// 	POLARITY_State		Polarity;			//输出极性

// 	FUNCTION_State		FunEnable;			//功能使能

// }PWM_INIT_MemberDef;

// /**************************************************************************************************************
//  --------------------------------------------------中断相关定义-------------------------------------------------
//  **************************************************************************************************************/
// /*枚举中断配置类型*/
// typedef enum {
// 	IEN_ADC				= 0,
// 	IEN_T3,
// 	IEN_UART0,
// 	IEN_T5,			
// 	IEN_TK,				
// 	IEN_SMC,				
// 	IEN_LPD,				
// 	IEN_X2,				
// 	IEN_X3,				
// 	IEN_X4,				
// 	IEN_T4,				
// 	IEN_LED,				
// 	IEN_TWI,				
// 	IEN_PWM1,			
// 	IEN_PCA0,
// 	IEN_PWM0,			
// 	IEN_CRC,			
// 	IEN_UART1,				
// } IEN_TypeDef;

// /*枚举中断优先级类型*/
// typedef enum {
// 	PRIORITY_LEVEL_0	= 0x00,
// 	PRIORITY_LEVEL_1	= 0x01,
// 	PRIORITY_LEVEL_2	= 0x02,
// 	PRIORITY_LEVEL_3	= 0x03,
// } IEN_PRIORITY_TypeDef;

// /**************************************************************************************************************
//  --------------------------------------------------LCM相关定义-------------------------------------------------
//  **************************************************************************************************************/
// /*枚举LCM中UART允许配置引脚*/
// typedef enum {
// 	TXD0_PIN_P0_0		= 0x00,
// 	TXD0_PIN_P0_1		= 0x10,
// 	TXD0_PIN_P0_2		= 0x20,
// 	TXD0_PIN_P2_6		= 0x40,
// 	TXD0_PIN_P2_7		= 0x50,
// 	TXD0_PIN_P5_2		= 0x60,
// 	RXD0_PIN_P0_0		= 0x00,						//和TXD0_PIN_P0_0相同
// 	RXD0_PIN_P0_1		= 0x01,
// 	RXD0_PIN_P0_2		= 0x02,
// 	RXD0_PIN_P2_6		= 0x04,
// 	RXD0_PIN_P2_7		= 0x05,

// 	/*最高位1为区分UART0的标志*/
// 	TXD1_PIN_P2_6		= 0x80,
// 	TXD1_PIN_P2_7		= 0x90,
// 	TXD1_PIN_P4_0		= 0xA0,
// 	TXD1_PIN_P4_1		= 0xB0,
// 	TXD1_PIN_P4_2		= 0xC0,
// 	TXD1_PIN_P4_3		= 0xD0,
// 	TXD1_PIN_P5_0		= 0xE0,
// 	TXD1_PIN_P5_1		= 0xF0,
// 	RXD1_PIN_P2_6		= 0x80,						//和TXD1_PIN_P2_6相同
// 	RXD1_PIN_P2_7		= 0x81,
// 	RXD1_PIN_P4_0		= 0x82,
// 	RXD1_PIN_P4_1		= 0x83,
// 	RXD1_PIN_P4_2		= 0x84,
// 	RXD1_PIN_P4_3		= 0x85,
// 	RXD1_PIN_P5_0		= 0x86,
// 	RXD1_PIN_P5_1		= 0x87,
// } LCM_UART_PIN_TypeDef;

// /*枚举LCM中TWI允许配置引脚*/
// typedef enum {
// 	SCK_PIN_P0_0		= 0x00,
// 	SCK_PIN_P0_1		= 0x10,
// 	SCK_PIN_P4_2		= 0x40,
// 	SCK_PIN_P4_3		= 0x50,
// 	SCK_PIN_P4_4		= 0x60,
// 	SDA_PIN_P0_0		= 0x00,						//和SCK_PIN_P0_0相同
// 	SDA_PIN_P0_1		= 0x01,
// 	SDA_PIN_P0_2		= 0x04,
// 	SDA_PIN_P2_6		= 0x05,
// 	SDA_PIN_P2_7		= 0x06,
// } LCM_TWI_PIN_TypeDef;

// /*枚举LCM中PWM允许配置引脚*/
// typedef enum {
// 	PWM1_PIN_P0_2		= 0x00,
// 	PWM1_PIN_P2_6		= 0x40,
// 	PWM1_PIN_P2_7		= 0x50,
// 	PWM1_PIN_P5_0		= 0x60,
// 	PWM0_PIN_P3_0		= 0x00,						//和PWM1_PIN_P0_2相同
// 	PWM0_PIN_P3_1		= 0x01,
// 	PWM0_PIN_P3_2		= 0x02,
// 	PWM0_PIN_P3_3		= 0x03,
// 	PWM0_PIN_P3_4		= 0x04,
// 	PWM0_PIN_P3_5		= 0x05,
// 	PWM0_PIN_P3_6		= 0x06,
// 	PWM0_PIN_P3_7		= 0x07,
// } LCM_PWM_PIN_TypeDef;


// /**************************************************************************************************************
//  --------------------------------------------------所有配置的函数-------------------------------------------------
//  **************************************************************************************************************/
// //系统时钟相关
// void SYSTEM_Init(CLOCK_SYS_TypeDef divNum, FUNCTION_State div16Enable, FUNCTION_State mul16Enable);

// //看门狗相关
// void WDT_Init(WDT_OVER_TIME_TypeDef wdtOverTime);

// //GPIO相关
// void GPIO_Init(GPIO_GROUP_TypeDef Px, GPIO_INIT_MemberDef *GPIO_InitStruct);
// void GPIO_SetBits(GPIO_GROUP_TypeDef Px, unsigned char pinNumber, GPIO_LEVEL_State pinLevel);

// //LCM相关
// void LCM_UART_Config(UART_TypeDef uartNum, LCM_UART_PIN_TypeDef txdPin, LCM_UART_PIN_TypeDef rxdPin);
// void LCM_TWI_Config(LCM_TWI_PIN_TypeDef sckPin, LCM_TWI_PIN_TypeDef sdaPin);
// void LCM_PWM_Config(PWM_TypeDef pwmNum, LCM_PWM_PIN_TypeDef pwmPin);

// //PWM相关
// void PWM_WaveformAdj(PWM_TypeDef pwmNum, unsigned int pwmCycle, unsigned int pwmDuty);
// void PWM_Init(PWM_TypeDef pwmNum, PWM_INIT_MemberDef *PWM_InitStruct);
// void PWM_ModuleEnable(PWM_TypeDef pwmNum, FUNCTION_State state);

// //UART相关
// void UART_Init(UART_TypeDef uartNum, UART_INIT_MemberDef *UART_InitStruct);
// void UART_RXD_Cmd(UART_TypeDef uartNum, FUNCTION_State state);
// void UART_BaudRate_Init(UART_TypeDef uartNum, unsigned long baud, FUNCTION_State state);

// //IEN（中断使能）相关
// void IEN_Init(IEN_TypeDef function, IEN_PRIORITY_TypeDef priority, FUNCTION_State state);

// //扇区编程（SSP）相关
// void SectorErase(unsigned int sectorAddr, SSP_OBJECT_State sectorSelect);
// void SectorWrite(unsigned int sectorAddr, unsigned char byteData, SSP_OBJECT_State sectorSelect);
// unsigned char SectorRead(unsigned int sectorAddr, SSP_OBJECT_State sectorSelect);

//其它
//void InitCharArrayData(unsigned char *buf, unsigned char len);
//void InitIntArrayData(unsigned int *buf, unsigned char len);

#endif