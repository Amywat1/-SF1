/*==========================================================================================================================*/
/*=========================================================buzzer.c=========================================================*/
/*==========================================================================================================================*/
/*
/* 版权说明：杭州九阳欧南多小家电有限公司
/*
/* 文件名称：buzzer.c
/*          	 
/* 编制人员：西式电控/冯晓丽、郑明伟、毛树海
/*
/* 编制时间：2018年10月30日
/*			          	 
/* 软件版本： 1、V1.0/20181030（程序首次编制）
/*
/*			  2、修改后在此增加记录
/*
/* 硬件版本： 1、适用于2K/4K压电或电磁式蜂鸣器、单频
/*
/*			  2、修改后在此增加记录
/*
/* 文件描述：蜂鸣器程序配置头文件
/*   
/*           01、变量定义    gb_buzzerDrive_flag 全局变量蜂鸣器电平翻转定义	     
/*
/*           02、变量定义    蜂鸣器类型结构体变量 蜂鸣变量数据	 
/*
/*			 03、变量定义    gU8_buzzerType 枚举变量 蜂鸣类型 根据产品线匹配使用  注意，勿放置在变量定义
/*
/*			 04、蜂鸣时间表  各条产品线蜂鸣时间表格 根据宏定义配置的BUZ_CURRENT 预编译选择产品线表格 设置之后不需要更换	
/*
/*			 05、函数处理    void BuzzerVariableInit(void)        //蜂鸣器变量初始化函数 
/*
/*			 06、函数处理    void BuzzerConfigureInit(void)	      //蜂鸣器端口配置初始化函数  在程序初始化函数中调用 
/*
/*			 07、函数处理    void BuzzerCtrl(void)	              //蜂鸣器处理函数 在程序主函数中调用（调用频率10ms或20ms）
/*			 
/* 注意事项：使用本文件时，请注意如下事宜
/*
/*			 01、不得任意改变标准模板文件，如有更改通知文件管理人员执行
/*
/*			 02、文件、函数、变量、宏定义等请符合九阳西式标准命名格式
/*
/*			 03、需要蜂鸣器发声请给gU8_buzzerType赋值所需的声音类型
/*
/*			 04、赋值完成后BuzzerCtrl函数将运算出声音生成驱动标志位 gb_buzzerDrive_flag，为真则开启PWM驱动或进行电平翻转	
/*
/* 			 05、蜂鸣器器不需要蜂鸣时，需要根据电路将端口电平置位在省电状态，否则电源功耗和板子温升将受影响
/*
/*==========================================================================================================================*/
/*==========================================================================================================================*/
/*==========================================================================================================================*/
/*
/*
/*
/*----------------------------------------------------包含头文件------------------------*/
#include "header.h"								                             //包含所有头文件
/*----------------------------------------------------变量定义--------------------------*/
bit     gb_buzzerDrive_flag;                                                     //蜂鸣器允许标志位  0-允许蜂鸣  1-禁止蜂鸣
BUZZER_DATA idata BuzzData;                                                      //蜂鸣器类型结构体变量 蜂鸣变量数据
enum _Buzz_Type_  gU8_buzzerType;                                                //枚举变量，注意，勿放置在变量定义之前
/*----------------------------------------------------各产品线蜂鸣表格------------------*/
/*----------------------------------煎烤--------------*/                            
#if     BUZ_CURRENT==BUZZER_JK													 //如果当前产品线选择的是煎烤产品线 则使用下述表格
unsigned char code BuzzerTable[7][3]={                                           //蜂鸣器类型表  基于1ms时间  
/*-----次数-------蜂鸣时长--------间隙时长------------*/                         //声音种类 基于10ms时钟配置 其他配置修改BUZ_CALL_FRE参数
{1,         500/BUZ_CALL_FRE,      500/BUZ_CALL_FRE, },				             //上电蜂鸣	
{1,         120/BUZ_CALL_FRE,      120/BUZ_CALL_FRE, },				             //按键有效蜂鸣
{4,         1000/BUZ_CALL_FRE,     1500/BUZ_CALL_FRE,},				             //功能结束蜂鸣	
{20,        500/BUZ_CALL_FRE,      800/BUZ_CALL_FRE, },				             //报警蜂鸣
{5,         500/BUZ_CALL_FRE,      1000/BUZ_CALL_FRE,},				             //预热完成蜂鸣
{4,         300/BUZ_CALL_FRE,      600/BUZ_CALL_FRE,},				             //可恢复报警蜂鸣
{0,         0,                     0, },                                         //静音鸣器声音	
};
#elif  	 BUZ_CURRENT==BUZZER_HP                                
/*----------------------------------烘焙----------------*/                       //如果当前产品线选择的是烘焙产品线
unsigned char code BuzzerTable[6][3]={                                           //蜂鸣器类型表    
/*-----次数-------蜂鸣时长--------间隙时长--------------*/                       //声音种类
{1,		    500/BUZ_CALL_FRE,	   500/BUZ_CALL_FRE, },						     //上电蜂鸣			
{1,		    600/BUZ_CALL_FRE,	   600/BUZ_CALL_FRE, },						     //按键有效和自动执行蜂鸣
{4,		    500/BUZ_CALL_FRE,	   750/BUZ_CALL_FRE, },						     //功能结束蜂鸣	
{20,	    250/BUZ_CALL_FRE,	   400/BUZ_CALL_FRE, },						     //报警蜂鸣
{5,		    250/BUZ_CALL_FRE,	   50/BUZ_CALL_FRE,  },						     //预热完成
{0,         0,                     0, },                                         //静音鸣器声音
};
#else                                                                            //选择面条产品线                             
/*----------------------------------面条--------------*/                                
unsigned char code BuzzerTable[9][3]={                                           //蜂鸣器类型表    
/*-----次数-------蜂鸣时长--------间隙时长------------*/                         //声音种类 
{1,         500/BUZ_CALL_FRE,       1000/BUZ_CALL_FRE,},					     //上电蜂鸣	
{1,         120/BUZ_CALL_FRE,       240/BUZ_CALL_FRE, },					     //按键有效蜂鸣
{1,         120/BUZ_CALL_FRE,       120/BUZ_CALL_FRE, },					     //自动启动	
{4,         1000/BUZ_CALL_FRE,      1500/BUZ_CALL_FRE,},					     //结束蜂鸣
{20,        500/BUZ_CALL_FRE,       800/BUZ_CALL_FRE, },					     //异常报警蜂鸣	
{2,         120/BUZ_CALL_FRE,       240/BUZ_CALL_FRE, },					     //合盖蜂鸣
{100,       500/BUZ_CALL_FRE,       1000/BUZ_CALL_FRE,},					     //检测报警（自检）
{20,        200/BUZ_CALL_FRE,       300/BUZ_CALL_FRE, },					     //缺水蜂鸣
{0,         0,                      0,                },                         //无蜂鸣器声音
};
#endif
/*----------------------------------------------------蜂鸣器变量初始化函数---------------*/									   
/* 
Function             函数名称：   void BuzzerConfigureInit(void) 
Function Parameters  函数参数：   翻转电平、蜂鸣次数、计数器、蜂鸣时间、蜂鸣间隔初始化0
void Return          函数返回值： 无	 	 
void Author          作 者：      冯晓丽 郑明伟 毛树海 
Data                 时 间：      2018-10-30 
Description          函数描述：   蜂鸣器变量初始化 
History              修改纪要:    无 
Remarks              备 注：      无 
*/  
/*---------------------------------------------------------------------------------------*/
void BuzzerVariableInit(void)	 
{				
    gb_buzzerDrive_flag                        =   0;  							  //初始化蜂鸣器电平翻转标志位0
    BuzzData.sU8_buzzerTimes                   =   0;             				  //蜂鸣器次数初始化为0
    BuzzData.sU8_buzzerCount                   =   0;           				  //蜂鸣计数器初始化为0
    BuzzData.sU8_buzzerOnTime                  =   0;       				      //蜂鸣器开时间初始化为0
    BuzzData.sU8_buzzerOffTime                 =   0;							  //蜂鸣器间隔时间初始化0
    gU8_buzzerType                             =   BUZ_JK_POWER_ON;		    	  //蜂鸣器类型初始化为上电蜂鸣 
}
/*----------------------------------------------------蜂鸣器端口配置初始化函数------------*/
/* 
Function             函数名称：   void BuzzerConfigureInit(void) 
Function Parameters  函数参数：   BUZZ_IO_OUTPUT BUZ_PULL_UP BUZ_POWER_SAVE	宏定义配置 无需修改    
void Return          函数返回值： 无	 	 
void Author          作 者：      冯晓丽 郑明伟 毛树海 
Data                 时 间：      2018-10-30 
Description          函数描述：   蜂鸣器IO口初始化 
History              修改纪要:    无 
Remarks              备 注：      无 
*/  
/*---------------------------------------------------------------------------------------*/
void BuzzerConfigureInit(void)	 
{
    BUZZ_IO_OUTPUT;                                                                //设置IO口为输出口，0-输入  1-输出	 
    BUZ_PULL_UP;                                                                   //设置IO口上拉关闭，0-关闭  1-开启   
	BUZ_POWER_SAVE;   					                                           //初始化输出省电模式
}
/*----------------------------------------------------蜂鸣器驱动函数放到中断------------*/
/* 
Function             函数名称：    
Function Parameters  函数参数：      
void Return          函数返回值： 无	 	 
void Author          作 者：      冯晓丽 郑明伟 毛树海 
Data                 时 间：      2018-10-30 
Description          函数描述：   蜂鸣器IO口初始化 
History              修改纪要:    无 
Remarks              备 注：      无 
*/  
/*---------------------------------------------------------------------------------------*/
void BuzzerDrive(void)
{
    if(gb_buzzerDrive_flag)
	{
	 BUZ_GPIO = ~BUZ_GPIO;
	}
	else
	{
	 BUZ_GPIO=1;
	}
}

/*----------------------------------------------------蜂鸣器处理函数---------------------*/									   
/* 
Function             函数名称：   void BuzzerCtrl(void) 
Function Parameters  函数参数：   sU8_buzzerTypeBuf 静态变量蜂鸣器缓存数据
void Return          函数返回值： 无	 	 
void Author          作 者：      冯晓丽 郑明伟 毛树海 
Data                 时 间：      2018-10-30 
Description          函数描述：   蜂鸣器处理过程函数 
History              修改纪要:    无 
Remarks              备 注：      无 
*/  
/*---------------------------------------------------------------------------------------*/
void BuzzerCtrl(void)
{
	static unsigned char     sU8_buzzerTypeBuf= 0;                             //蜂鸣器鸣叫的buf
    sU8_buzzerTypeBuf=gU8_buzzerType;										   //给蜂鸣缓存buf赋值
	if(sU8_buzzerTypeBuf != 0) 
		{
		 BuzzData.sU8_buzzerCount=0;		
         BuzzData.sU8_buzzerTimes    =   BuzzerTable[sU8_buzzerTypeBuf-1][0];	   //蜂鸣器次数赋值 表格第一列
         BuzzData.sU8_buzzerOnTime   =   BuzzerTable[sU8_buzzerTypeBuf-1][1];      // 蜂鸣时间赋值  表格第二列 计数单位为1ms
         BuzzData.sU8_buzzerOffTime  =   BuzzerTable[sU8_buzzerTypeBuf-1][2];      // 间隔时间赋值  表格第三列 计数单位为1ms
	    } 
	    gU8_buzzerType = 0;	
	 		
    if(BuzzData.sU8_buzzerTimes > 0)                                               //蜂鸣器在鸣叫
        {																		   //蜂鸣次数大于0
		BuzzData.sU8_buzzerCount++;												   //蜂鸣计数器累加1
		if(BuzzData.sU8_buzzerCount <= BuzzData.sU8_buzzerOnTime)                  //如果蜂鸣计数器小于蜂鸣时间
            {																	   //则开启翻转电平，蜂鸣器响				
			gb_buzzerDrive_flag = Ture;											   
		    }																				
		else if(BuzzData.sU8_buzzerCount < BuzzData.sU8_buzzerOffTime)             //如果蜂鸣器计数器小于间隔时间  
            {																					
			gb_buzzerDrive_flag = False;										   // 关闭翻转电平 蜂鸣器停止鸣叫 
		    }																					
		else if(BuzzData.sU8_buzzerCount >= BuzzData.sU8_buzzerOffTime)			   //蜂鸣器计数器大于关闭时间
            {																					
			BuzzData.sU8_buzzerTimes--;											   //每次响完后 蜂鸣次数减1
			BuzzData.sU8_buzzerCount = 0;										   //蜂鸣计数器归零
		    }																					
        }																						
     else                                                                          //如果蜂鸣次数为0             
	    {
		BuzzData.sU8_buzzerCount = 0;											   //结束后计数清零
		gb_buzzerDrive_flag = False;											   //关闭蜂鸣器翻转电平	
//		BUZ_POWER_SAVE;		
	    }
}

