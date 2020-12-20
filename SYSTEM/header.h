#ifndef HEADER_H
#define HEADER_H

//#define DEBUG

#include <SH79F328.h>
#include <intrins.h>
#include <ABSACC.H>
#include "initialize.h"
#include "delay.h"
#include "interrupt.h"
#include "uart.h"
#include "display.h"
#include "touchkey.h"
#include "buzzer.h"
#include "sysmode.h"
#include "load.h"
#include "measure.h"
#include "TYPE_DEF.h"
#include "WIFI.H"

#ifdef DEBUG
#include "uart_send_data.h"
#endif

#define	BIT0			0x01						//BIT0位
#define	BIT1			0x02						//BIT1位
#define	BIT2			0x04						//BIT2位
#define	BIT3			0x08						//BIT3位
#define	BIT4			0x10						//BIT4位
#define	BIT5			0x20						//BIT5位
#define	BIT6			0x40						//BIT6位
#define	BIT7			0x80						//BIT7位

#define TURN_ON			1							//输出开
#define TURN_OFF		0							//输出关

#define PROCESS_NUM		5							//步骤循环次数

#endif