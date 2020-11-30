#ifndef INTERRUPT_H
#define INTERRUPT_H

#define T2MS_OVER_CNT 			16					//2ms计数值
#define T500MS_OVER_CNT 		4000				//500ms计数值(250us*2000=500ms)

extern bit						g_flg2ms;			//2ms计时标志位
extern bit						g_flg500ms;			//500ms计时标志位

#endif