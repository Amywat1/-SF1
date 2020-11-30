#ifndef _KEYTOUCH_H
#define _KEYTOUCH_H

#define VALID_TOUCHKEY_NUMBER       7                              //触摸按键个数

#define VALID_THRESHOLD_MAX			30000							//最大按键阈值，限幅滤波
#define ELIM_TREMBLING_CNT			35								//消抖时间

#define KEY_NULL					0x0000							//无按键
#define KEY_TEMP_ADD				0x0001							//按键温度加
#define KEY_TEMP_SUB				0x0002							//按键温度减
#define KEY_TIME_ADD				0x0004							//按键时间加
#define KEY_TIME_SUB				0x0008							//按键时间减
#define KEY_ORDER   				0x0010							//按键预约
#define KEY_CHANGE_FUN				0x0020							//按键功能切换
#define KEY_ON_OFF					0x0040							//按键启动取消

#define KEY_PCB_CHECK				0x0005							//PCB自检按键【温度+】+【时间+】
#define KEY_MACHINE_CHECK			0x0030							//整机自检按键【预约】+【功能】
#define KEY_DISP_PID				0x000A							//PID 按键【时间-】+【温度-】
#define KEY_CLEAN_INCRUSTANT		0x000C							//除水垢按键【时间+】+【时间-】

extern unsigned int	    			g_scanDataTk[VALID_TOUCHKEY_NUMBER];//触摸按键的扫描值

extern bit							g_keyScanEndFlag;				//按键扫描完成标志
extern bit							g_adjPressFlag;					//加减键按下标志

void InitTouchKeyIo(void);
void InitTouchKeyVariable(void);
void InitTouchKey(void);
void TouchKeyAdScan(void);
void TouchKeyScan(void);
void TouchKeyDeal(void);
void TouchKeyJudge(void);
void TouchKeyDealSubroutine(void);

#endif