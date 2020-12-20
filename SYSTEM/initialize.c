#include "header.h"

/*-----------------------------------------------------------------------------
Description:		��ʼ��ϵͳʱ��
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitOscilator(void)
{
	BANK0_SET;									//ָ��Bank0
	INSCON &= (~(BIT2 | BIT3)); 				//8λ�˳���
	CLKCON = 0x00; 								//ʱ�����ã�24M�ڲ�������ϵͳʱ�Ӳ���Ƶ
}

/*-----------------------------------------------------------------------------
Description:		��ʼ�����Ź�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitWdt(void)
{
	RSTSTAT = 0x01;								//���ÿ��Ź�ʱ��Ϊ1024ms
}

/*-----------------------------------------------------------------------------
Description:		��ʼ������IO�ڣ��Ƚ�����IO�������/������/�͵�ƽ��������ʹ�õĶ˿ڣ�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitIo(void)
{
	/*BANK0*/
	BANK0_SET;										//ָ��Bank0

	/*�˿������*/
	P0CR = 0xFF;
	P1CR = 0xFF;
	P2CR = 0xFF;
	P4CR = 0xFF;
	
	/*������AD���ϵ��������*/
	P2CR &= ~(BIT6);
	P2CR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
	
	/*�˿�������*/
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
	BANK0_SET;										//ָ��Bank0
	
	/*ʹ�õĶ˿ڳ�ʼ������*/
	InitUartIo();									//��ʼ��ͨѶ����IO
	InitDispIo();									//��ʾ���IO
	InitTouchKeyIo();								//������������IO
	InitMeasureIo();								//�źż��IO
	InitLoadIo();									//����IO
	
	#ifdef DEBUG
	InitDebugUartIo();								//��ʼ�����Դ���IO
	#endif
}

/*-----------------------------------------------------------------------------
Description:		��ʼ����ʱ�����ã�ÿ250us�����ж�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitTimer5(void)
{
	BANK1_SET;
	
	T5CON = 0x00;									//16λ�Զ���װ������ʱ�Ӳ���Ƶ
	
	TL5 = (65536-3000)%256;							//������д��λ����д��λ����Ȼʱ�����ϴ�
	TH5 = (65536-3000)/256;  						//24M��ÿ�μ�ʱ0.041667US *3000=125us

	TF5 = 0;										//���жϱ�־λ��T5��Ӳ�����㣩
	TR5 = 1;										//����ʱ��5
	
	BANK0_SET;
	ET5 = 1;										//����ʱ��5�ж�
}

/*-----------------------------------------------------------------------------
Description:		������������������unsigned char��
Input:				void
Return:				void
History:			��
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
Description:		������������������unsigned int��
Input:				void
Return:				void
History:			��
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
Description:		���������в���
Input:				sectorAddr:	16λ��[15:9]��ʾ�����ţ���Χ��0~63��FLSH���64����������EEPROM���8��������;[8]��ʾ�洢��Ԫ�ĸ�λ��ַ��[7:0]��ʾ�洢��Ԫ�ĵ�λ��ַ
					�磺sectorAddr = 0x0345����ʾѡ��1����������ַΪ0x0145;

					sectorSelect:	ѡ���FLSH��EEPROM���
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void SectorErase(unsigned int sectorAddr,bit sectorSelect)
{
	bit EAtemp;
	
	EAtemp = EA;
	EA = 0;
	
	if(sectorSelect == 0)							//��FLASH����ʱFACλ����
	{
	  FLASHCON &= ~BIT0;
	}
	else											//����EEPROM����ʱFACλ��1
	{
	  FLASHCON |= BIT0;
	}
	
	XPAGE = ((sectorAddr >> 9) << 1);				//��ֵ������
	if(((sectorAddr>>8) % 2) == 0)					//��ֵ�洢��Ԫ�ĸ�λ��ַ
	{
		XPAGE |= 0x00;
	}
	else
	{
		XPAGE |= 0x01;
	}
	
	//�洢��Ԫɾ��
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
	FLASHCON &= ~BIT0;								//���������EEPROM����ʱ�����뽫FACλ��0��
	
	EA = EAtemp;
}

/*-----------------------------------------------------------------------------
Description:		���������б��
Input:				sectorAddr:	16λ��[15:9]��ʾ�����ţ���Χ��0~63��FLSH���64����������EEPROM���8��������;[8]��ʾ�洢��Ԫ�ĸ�λ��ַ��[7:0]��ʾ�洢��Ԫ�ĵ�λ��ַ
					�磺sectorAddr = 0x0345����ʾѡ��1����������ַΪ0x0145;
					
					byteData:		��Ҫ��̵�����
					
					sectorSelect:	ѡ���FLSH��EEPROM���
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void SectorWrite(unsigned int sectorAddr,unsigned char byteData,bit sectorSelect)
{
	bit EAtemp;
	
	EAtemp = EA;
	EA = 0;
	
	if(sectorSelect == FLASH_CRL)					//��FLASH����ʱFACλ����
	{
	  FLASHCON &= ~BIT0;
	}
	else											//����EEPROM����ʱFACλ��1
	{
	  FLASHCON |= BIT0;
	}
	
	XPAGE = ((sectorAddr >> 9) << 1);				//��ֵ������
	if(((sectorAddr>>8) % 2) == 0)					//��ֵ�洢��Ԫ�ĸ�λ��ַ
	{
		XPAGE |= 0x00;
	}
	else
	{
		XPAGE |= 0x01;
	}
	IB_OFFSET = (unsigned char)(sectorAddr & 0x00ff);	//��ֵ�洢��Ԫ�ĵ�8λ��ַ
	
	IB_DATA = byteData;
	
	//�洢��Ԫ���
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
	FLASHCON &= ~BIT0;								//���������EEPROM����ʱ�����뽫FACλ��0��
	
	EA = EAtemp;
}

/*-----------------------------------------------------------------------------
Description:		���������ݽ��ж�ȡ
Input:				sectorAddr:	16λ��[15:9]��ʾ�����ţ���Χ��0~63��FLSH���64����������EEPROM���8��������;[8]��ʾ�洢��Ԫ�ĸ�λ��ַ��[7:0]��ʾ�洢��Ԫ�ĵ�λ��ַ
					�磺sectorAddr = 0x0345����ʾѡ��1����������ַΪ0x0145;

					sectorSelect:	ѡ���FLSH��EEPROM���
Return:				byteData:		��Ӧ��ַ������
History:			��
-----------------------------------------------------------------------------*/
unsigned char SectorRead(unsigned int sectorAddr,bit sectorSelect)
{
	unsigned char byteData;
	bit EAtemp;
	
	EAtemp = EA;
	EA = 0;
	
	if(sectorSelect == 0)							//��FLASH����ʱFACλ����
	{
	  FLASHCON &= ~BIT0;
	}
	else											//����EEPROM����ʱFACλ��1
	{
	  FLASHCON |= BIT0;
	}
	
	byteData = CBYTE[sectorAddr];
	FLASHCON &= ~BIT0;								//���������EEPROM����ʱ�����뽫FACλ��0��
	
	EA = EAtemp;
	
	return(byteData);
}

/*-----------------------------------------------------------------------------
Description:		��ʼ���͵�ѹ�ж�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitLPD(void)
{
	BANK0_SET;

	LPDCON |= BIT7;									//����͵�ѹ���
	LPDSEL = 0x08;									//LPD��ѹ����Ϊ3.6V
	Delay1ms(10);
	LPDCON &= ~BIT4;								//����ж������־
	IEN1 |= BIT6;									//����LPD�ж�
}

/*-----------------------------------------------------------------------------
Description:		��ʼ�����б���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitVariable(void)
{
	InitUartVariable();								//��ʼ��ͨѶ����
	InitDispVariable();								//��ʼ����ʾ����
	InitTouchKeyVariable();							//��ʼ��������������
	BuzzerVariableInit();							//��ʼ������������
	InitSysModeVariable();							//��ʼ��ϵͳ״̬����
	InitMeasureVariable();							//��ʼ������źű���
	InitLoadVariable();								//��ʼ�����ر���


	CmdHandVariableInit();
	CmdSmartLinkVariableInit();
	CmdSoftGoUpVariableInit();
	CmdReBootVariableInit();
	Cmd0x5000VariableIint();
	
	#ifdef DEBUG
	InitDebugUartVariable();						//��ʼ�����Դ��ڱ���
	#endif
}

/*-----------------------------------------------------------------------------
Description:		��ʼ���ж����ȼ���T2��ʱ�����ȼ���ߣ�
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitPriority(void)
{
	BANK0_SET;

	IPL0 = 0x18;              						//�ж����ȼ� 	
	IPH0 = 0x0A;			  	
	IPL1 = 0x00;				
	IPH1 = 0x00;									//T5	���ȼ�3  ���
													//����	���ȼ�2
													//����	���ȼ�1
													//����	���ȼ�0	 ���
}

/*-----------------------------------------------------------------------------
Description:		�ײ����úͳ�ʼ�����б���
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void InitAll(void)
{
	EA = 0;										//�����ж�/�����ж�
	IEN0 = 0X00;
	IEN1 = 0X00;
	IEN2 = 0X00;
	InitOscilator();							//��ʼ��ϵͳʱ��
	InitWdt();									//��ʼ�����Ź�
	InitIo();									//��ʼ��IO��
	Delay1ms(200);								//��ʱ200ms���ȶ���ƽ
	InitWdt();									//�幷
	InitUart();									//���ڳ�����ʼ��

	Uart0Init();
	
	#ifdef DEBUG
	InitDebugUart();							//���Դ������ó�ʼ��
	#endif
	
	InitTouchKey();								//�����������ó�ʼ��
	InitTimer5();								//��ʼ����ʱ��5
	InitLPD();									//�͵�ѹ�жϳ�ʼ��
	InitVariable();								//��ʼ�����б���
	InitAd();									//��ʼ��AD���ã�������ݼ�⼰������ֵ�������¶ȼ�����ã�
	BuzzerConfigureInit();						//���������ó�ʼ��(����ںͷ����ڹ��ã��ȼ�����ڵ�ƽ)
	InitPriority();								//�ж����ȼ���ʼ��
	EA = 1;										//�����ж�
}