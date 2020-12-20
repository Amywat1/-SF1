
#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_



typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;


typedef char          int8;
typedef int           int16;
typedef long          int32;


typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;


#define	BIT0		0x01		//Bit0
#define	BIT1		0x02		//Bit1
#define	BIT2		0x04		//Bit2
#define	BIT3		0x08		//Bit3
#define	BIT4		0x10		//Bit4
#define	BIT5		0x20		//Bit5
#define	BIT6		0x40		//Bit6
#define	BIT7		0x80		//Bit7

#define BIT8    	0X0100
#define BIT9		0x0200
#define	BIT10		0x0400
#define	BIT11		0x0800		
#define	BIT12		0x1000
#define	BIT13		0x2000
#define	BIT14		0x4000
#define	BIT15		0x8000


typedef  unsigned int   Byte16;		//
typedef struct
{	
	Byte16  b0	:1;
	Byte16  b1	:1;
	Byte16  b2	:1;
	Byte16  b3	:1;
	Byte16  b4	:1;
	Byte16  b5	:1;
	Byte16  b6	:1;
	Byte16  b7	:1;
	Byte16  b8	:1;
	Byte16  b9	:1;
	Byte16  b10	:1;
	Byte16  b11	:1;
	Byte16  b12	:1;
	Byte16  b13	:1;
	Byte16  b14	:1;
	Byte16  b15	:1;

} BYTE16_FIELD;
//*****************************************//
typedef union 
{
	unsigned int byte16;
	BYTE16_FIELD 	b1;
} TYPE_BYTE16;


//bit浣嶅畾涔?
typedef  unsigned char   Byte;
typedef struct
{	
	Byte  b0	:1;
	Byte  b1	:1;
	Byte  b2	:1;
	Byte  b3	:1;
	Byte  b4	:1;
	Byte  b5	:1;
	Byte  b6	:1;
	Byte  b7	:1;
} BYTE_FIELD;
//*****************************************//
typedef union 
{
	unsigned char 		byte;
	BYTE_FIELD 		b;
} TYPE_BYTE;
 





#define ENABLE       1
#define DISABLE      0

#define FINISH       1
#define NO_FINISH    0

#define TURN_ON      1
#define TURN_OFF     0

#define TURE  1

#define FALSE 0

//P_X_CON:开头为IO的输入输出配置寄存器
#define SET_IO_OUTPUT(P_X_CON,BIT_XX) 		(P_X_CON |= BIT_XX)					//设置端口输出
#define SET_IO_INPUT(P_X_CON,BIT_XX) 		(P_X_CON &= (~(BIT_XX)))			//设置端口输入

//P_X :开头为IO口的数据寄存器
#define SET_IO_DATA_H_1(P_X,BIT_XX) 		(P_X |= BIT_XX)					//设置端口值1
#define SET_IO_DATA_L_0(P_X,BIT_XX) 		(P_X &= (~BIT_XX))				//设置端口值0

//P_X_PH:开头为IO上拉配置寄存器
#define SET_IO_PULL(P_X_PH,BIT_XX) 			(P_X_PH |= BIT_XX)					//设置端口上拉
#define SET_IO_NO_PULL(P_X_PH,BIT_XX) 		(P_X_PH &= (~(BIT_XX)))				//设置端口无上拉






#endif




























