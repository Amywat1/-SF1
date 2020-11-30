#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define P_COM1				P3_6
#define P_COM2				P3_4
#define P_COM3				P3_3
#define P_COM4				P3_2
#define P_SEG_A				P5_1
#define P_SEG_B				P0_2
#define P_SEG_C				P0_0
#define P_SEG_D				P3_0
#define P_SEG_E				P3_1
#define P_SEG_F				P3_5
#define P_SEG_G				P1_7
#define P_SEG_H				P0_1

//显示数据符号定义
#define SA					0x01
#define SB					0x02
#define SC					0x04
#define SD					0x08
#define SE					0x10
#define SF					0x20
#define SG					0x40
#define SDP					0x80

#define DISPDATA_0			(SA|SB|SC|SD|SE|SF)			//数据0
#define DISPDATA_1			(SB|SC)						//数据1
#define DISPDATA_3			(SA|SB|SC|SD|SG)			//数据3
#define DISPDATA_2			(SA|SB|SD|SE|SG)			//数据2
#define DISPDATA_4			(SB|SC|SF|SG)				//数据4
#define DISPDATA_5			(SA|SC|SD|SF|SG)			//数据5
#define DISPDATA_6			(SA|SC|SD|SE|SF|SG)			//数据6
#define DISPDATA_7			(SA|SB|SC)					//数据7
#define DISPDATA_8			(SA|SB|SC|SD|SE|SF|SG)		//数据8
#define DISPDATA_9			(SA|SB|SC|SD|SF|SG)			//数据9
#define DISPDATA_A			(SA|SB|SC|SE|SF|SG)			//数据A
#define DISPDATA_b			(SC|SD|SE|SF|SG)			//数据b
#define DISPDATA_C			(SA|SD|SE|SF)				//数据C
#define DISPDATA_d			(SB|SC|SD|SE|SG)			//数据d
#define DISPDATA_E			(SA|SD|SE|SF|SG)			//数据E
#define DISPDATA_F			(SA|SE|SF|SG)				//数据F
#define DISPDATA_G			(SA|SC|SD|SE|SF)			//数据G
#define DISPDATA_H			(SB|SC|SE|SF|SG)			//数据H
#define DISPDATA_h			(SC|SE|SF|SG)				//数据h
#define DISPDATA_J			(SB|SC|SD)					//数据J
#define DISPDATA_L			(SD|SE|SF)					//数据L
#define DISPDATA_n			(SC|SE|SG)					//数据n
#define DISPDATA_o			(SC|SD|SE|SG)				//数据o
#define DISPDATA_P			(SA|SB|SE|SF|SG)			//数据P
#define DISPDATA_r			(SE|SG)						//数据r
#define DISPDATA_S			(SA|SC|SD|SF|SG)			//数据S
#define DISPDATA_y			(SB|SC|SD|SF|SG)			//数据y
#define DISPDATA_ON			(SA|SB|SC|SD|SE|SF|SG|SDP)	//数据ON
#define DISPDATA_OFF		(0x00)						//数据OFF
#define DISPDATA_SA			(SA)						//数据A段
#define DISPDATA_SB			(SB)						//数据B段
#define DISPDATA_SC			(SC)						//数据C段
#define DISPDATA_SD			(SD)						//数据D段
#define DISPDATA_SE			(SE)						//数据E段
#define DISPDATA_SF			(SF)						//数据F段
#define DISPDATA_SG			(SG)						//数据G段
#define DISPDATA_SDP		(SDP)						//数据DP段

#define DIGITAL_FIRST_NUM(x)	(Led_Buff[0].ramDat = x)			//数码管第一位
#define DIGITAL_SECOND_NUM(x)	(Led_Buff[1].ramDat = x)			//数码管第二位
#define DIGITAL_THIRD_NUM(x)	(Led_Buff[2].Led.ledDat = x)		//数码管第三位
#define SYMBOL_FUN_BAKE			Led_Buff[3].Bits.bit1				//空气炸标志
#define SYMBOL_FUN_BA_ST		Led_Buff[3].Bits.bit0				//蒸汽烤标志
#define SYMBOL_FUN_STEAM		Led_Buff[3].Bits.bit3				//原味蒸标志
#define SYMBOL_FUN_WING			Led_Buff[3].Bits.bit4				//鸡翅标志
#define SYMBOL_FUN_RICE			Led_Buff[3].Bits.bit5				//热饭标志
#define SYMBOL_FUN_CHIPS		Led_Buff[3].Bits.bit6				//薯条标志
#define SYMBOL_TEMP				Led_Buff[3].Bits.bit7				//℃标志
#define SYMBOL_MINITE			Led_Buff[3].Bits.bit2				//分标志

typedef union
{
	struct{
		volatile unsigned char bit0		: 1;
		volatile unsigned char bit1		: 1;
		volatile unsigned char bit2		: 1;
		volatile unsigned char bit3		: 1;
		volatile unsigned char bit4		: 1;
		volatile unsigned char bit5		: 1;
		volatile unsigned char bit6		: 1;
		volatile unsigned char bit7		: 1;
	}Bits;
	struct{
		volatile unsigned char ledDat	: 7;
		volatile unsigned char   		: 1;
	}Led;
	volatile unsigned char ramDat;
}LED_MESSAGE;

extern LED_MESSAGE Led_Buff[4];
extern bit						g_flashFlag;						//闪烁标志位

void InitDispIo(void);
void InitDispVariable(void);
void DispAllOn(void);
void DispAllOff(void);
void DisplayDataLoad(void);
void DisplayScan(void);
void DispCrl(void);

#endif