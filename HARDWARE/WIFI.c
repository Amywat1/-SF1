#include "header.h"

TYPE_BYTE16 RevCmdflag_byte02;//
TYPE_BYTE16 SendCmdflag_byte02;//
TYPE_BYTE16 MCUSendCmdflag_byte02;//

#define  BUF_LENGTH_NUM    80

unsigned char xdata Send_Buf[BUF_LENGTH_NUM];
unsigned char xdata Recv_Buf[BUF_LENGTH_NUM];

MCURevWifi_str xdata MCURevWifiDate;
MCUSendWifi_str xdata MCUSendWifiDate;

MCUSend_hands xdata	MCUSendDate_hands;
MCURev_hands xdata	MCURevDate_hands;

MCUSend_SmartLink xdata MCUSendDate_SmartLink;
MCURev_SmartLink xdata MCURevDate_SmartLink;

MCUSend_SoftGoUp xdata MCUSendDate_SoftGoUp;
MCURev_SoftGoUp xdata MCURevDate_SoftGoUp;

MCUSend_ReBoot xdata MCUSendDate_ReBoot;
MCURev_ReBoot xdata  MCURevDate_ReBoot;

//**********************************************************

unsigned int  xdata recvFrameLen   = 0;				//�������ݳ���
unsigned int  xdata recvDataCount  = 0;				//��������
unsigned int  xdata recvOutTime    = 0;				//���ճ�ʱ������

unsigned int  xdata sendIntervalTime = 0;				//���ͼ��������
unsigned int  xdata ackTime        = 0;				//Ӧ����ʱ������� 

unsigned int  xdata sendFrameLen   = 0;				//����֡����
unsigned int  xdata sendDataCount  = 0;				//����������

unsigned int  xdata CmdCode_Buf[16];   				//���û�����
unsigned char xdata cmdNum         = 0;
unsigned int  xdata cmdCode        = 0x0000;			//ָ����

unsigned char xdata firmWareHandle_flag = 0x00;
unsigned char xdata recvFirmWareEdit = 0x00;			//�̼�

//**********************************************************

#define   N_TXD P0_0
#define   N_RXD P2_7

void CmdHandVariableInit(void)
{
//send

	MCUSendDate_hands.Cmdcode_H=0xcc;			//ָ��߰�λ
	MCUSendDate_hands.Cmdcode_L=0xc0;			//ָ��Ͱ�λ
	MCUSendDate_hands.LengthDate_H=0x00;		//���ݳ���
	MCUSendDate_hands.LengthDate_L=0x08;		//���ݳ���
	MCUSendDate_hands.EquNumber_H=0x94;			//�豸��Ÿ߰�λ
	MCUSendDate_hands.EquNumber_L=0xfc;			//�豸��ŵͰ�λ
	MCUSendDate_hands.ProtcolVersion=0x00;		//ͨѶЭ��汾
	MCUSendDate_hands.EquVersion=0x00;			//�豸�̼��汾	
	MCUSendDate_hands.Datelarge_H=0x00;			//Date����󳤶ȸ߰�λ
	MCUSendDate_hands.Datelarge_L=0xff;			//Date����󳤶ȵͰ�λ
	MCUSendDate_hands.HardWareVersion=0x00;		//�豸Ӳ���汾
	MCUSendDate_hands.Remove_sta=0x00;			//����ģ���״̬��0��Ĭ��ģʽ 1������APģʽ	 2:����sarmlink  3���������� 

//rev

	MCURevDate_hands.Aanser_H=0x00;	 		//Ӧ�����߰�λ 0x0000�ɹ�      ��0001ʧ��
	MCURevDate_hands.Aanser_L=0x00; 			//Ӧ�����Ͱ�λ
 	MCURevDate_hands.WIFISta=0x00;			//wifi״̬ 
 	/*
		00��δ����
		01���޷����ӷ�����
		02���Ѿ�����·����
		03���Ѿ����ӷ�����
		04���Ѿ�����smartlink
		05��Ԥ��
		06��Ԥ��
		07��sartlink���óɹ�
		08��ap״̬
		

 	*/
}

void CmdSmartLinkVariableInit(void)
{
	//send

	MCUSendDate_SmartLink.Cmdcode_H=0xcc;		//ָ��߰�λ
	MCUSendDate_SmartLink.Cmdcode_L=0xc2;		//ָ��Ͱ�λ
	MCUSendDate_SmartLink.CmdDtae_H=0x00;		//00
	MCUSendDate_SmartLink.CmdDtae_L=0x00;		//00

	//rev
	MCURevDate_SmartLink.Aanser_H=0x00;	 		//����߰�λ
	MCURevDate_SmartLink.Aanser_L=0x00; 		//����Ͱ�λ
}

void CmdSoftGoUpVariableInit(void)
{
	//send

	MCUSendDate_SoftGoUp.Cmdcode_H=0xcc;	 	//ָ��߰�λ
	MCUSendDate_SoftGoUp.Cmdcode_L=0xc5; 		//ָ��Ͱ�λ
 	MCUSendDate_SoftGoUp.LengthDate_H=0x00;		//���ݳ���
	MCUSendDate_SoftGoUp.LengthDate_L=0x02;		//���ݳ���
	MCUSendDate_SoftGoUp.WIFISendDateLength_H=0x00;  //ÿһ֡�����ݳ����ݶ� 128���ֽ�  һ֡
	MCUSendDate_SoftGoUp.WIFISendDateLength_L=0x80;

	//rev
	MCURevDate_SoftGoUp.Aanser_H=0x00;	 		//����߰�λ
	MCURevDate_SoftGoUp.Aanser_L=0x00; 			//����Ͱ�λ
}

void CmdReBootVariableInit(void)
{
	//send

	MCUSendDate_ReBoot.Cmdcode_H=0xcc;	 	//ָ��߰�λ
	MCUSendDate_ReBoot.Cmdcode_L=0xc9; 		//ָ��Ͱ�λ
 
	MCUSendDate_ReBoot.SendDate_H=0x00;  		//����
	MCUSendDate_ReBoot.SendDate_L=0x00;

	//rev
	MCURevDate_ReBoot.Aanser_H=0x00;	 		//����߰�λ
	MCURevDate_ReBoot.Aanser_L=0x00; 			//����Ͱ�λ
}

void Cmd0x5000VariableIint(void)
{



	MCURevWifiDate.LudengONOFF = 0;





	MCUSendWifiDate.McuWorkSta=0; //����״̬     0x5000

	MCUSendWifiDate.McuMumeNumber_0=0;  //�˰����� 

	MCUSendWifiDate.McuMumeNumber_1=0;  //�˰����� 

	MCUSendWifiDate.McuMumeNumber_2=0;  //�˰����� 

	MCUSendWifiDate.McuMumeNumber_3=0;  //�˰����� 

	MCUSendWifiDate.McuyuyueEn=0; 		 //ԤԼʹ�ܣ�0x01 ʹ��

	MCUSendWifiDate.McuyuyueFlag=0; 	 //ԤԼ��־λ 0x01 Ĭ����ԤԼ

	MCUSendWifiDate.McuyuyueTimer_H=0; 	  //ԤԼʱ��

	MCUSendWifiDate.McuyuyueTimer_L=0; 	  //ԤԼʱ��

	MCUSendWifiDate.McuyureEn=0; 		 //Ԥ��ʹ�ܣ�0x01 ʹ��

	MCUSendWifiDate.McuyureFlag=0; 		 //Ԥ�ȱ�־λ 0x01 Ĭ����Ԥ��

	MCUSendWifiDate.McuyureTimer_H=0; 		 //Ԥ��ʱ��

	MCUSendWifiDate.McuyureTimer_L=0; 		 //Ԥ��ʱ��

	MCUSendWifiDate.McuyureKeepFlag=0; 		 //Ԥ�ȱ���

	MCUSendWifiDate.McuyureKeepTimer=0; 	 //Ԥ�ȱ���ʱ��

	MCUSendWifiDate.WorkStep1_Top_Temp=0; 	 	//������һ���Ϲܵ��¶�

	MCUSendWifiDate.WorkStep1_Bot_Temp=0;  		//������һ���¹ܵ��¶�

	MCUSendWifiDate.WorkStep1_work_Time=0;  	//������һ������ʱ��

	MCUSendWifiDate.WorkStep2_Top_Temp=0;  		//������2���Ϲܵ��¶�

	MCUSendWifiDate.WorkStep2_Bot_Temp=0;	  	//������2���¹ܵ��¶�

	MCUSendWifiDate.WorkStep2_work_Time=0;  	//������2������ʱ��


	MCUSendWifiDate.WorkStep3_Top_Temp=0;		//������3���Ϲܵ��¶�

	MCUSendWifiDate.WorkStep3_Bot_Temp=0;		//������3���¹ܵ��¶�
		
	MCUSendWifiDate.WorkStep3_work_Time=0;		//������3������ʱ��

	MCUSendWifiDate.Temp_Ajust_Line=0;    		//�¶ȵ��� �ֽ��

	MCUSendWifiDate.Temp_Ajust_small=0;    		//�¶ȵ��� С����

	MCUSendWifiDate.Temp_Ajust_large=0;    		//�¶ȵ��� �󲽳�

	MCUSendWifiDate.Temp_Ajust_samllest=0;    	//�¶ȵ��� ��С�¶�

	MCUSendWifiDate.Temp_Ajust_largeest=0;    	//�¶ȵ��� ����¶�

	MCUSendWifiDate.Timer_work_leftMin=0;    	//ʣ�๤��ʱ��

	MCUSendWifiDate.Timer_work_leftsecond=0;    //ʣ�๤��ʱ������

	MCUSendWifiDate.Timer_Ajust_Min=0;    		//�ɵ���С

	MCUSendWifiDate.Timer_Ajust_Max=0;    		//�ɵ����

	MCUSendWifiDate.zhuanchaSta=0;              //ת��״̬ 1�� 0��

	MCUSendWifiDate.LudengSta=0;              	//¯��״̬	1�� 0��

	MCUSendWifiDate.RealTemp_Top=0;				//�Ϲ�ʵʱ�¶�

	MCUSendWifiDate.RealTemp_Bot=0;				//�¹�ʵʱ�¶�

	MCUSendWifiDate.Resver_1=0;					//Ԥ��
	MCUSendWifiDate.Resver_2=0;					//Ԥ��
	MCUSendWifiDate.Resver_3=0;					//Ԥ��
	MCUSendWifiDate.Resver_4=0;					//Ԥ��
	MCUSendWifiDate.Resver_5=0;					//Ԥ��
	MCUSendWifiDate.Resver_6=0;					//Ԥ��

	MCUSendWifiDate.ErroDate_H=0;              //���ϴ���

	MCUSendWifiDate.ErroDate_L=0;              //���ϴ���		

	MCUSendWifiDate.KeyNumber=0;

//	TemCharDataInt(&MCUSendWifiDate.KeyCountN[0],10);

	MCUSendCmdflag_Date=0;

	SendCmdflag_Date=0;

	RevCmdflag_Date=0;
}

/*-----------------------------------------------------------------------------
Description:		��ʼ������IO
					��Դ��ͨѶ��TXD��P0.0��RXD��P2.7
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void Uart0IOSet(void)
{
	BANK0_SET;										//ָ��Bank0

	/*��Դ��*/
	/*TXD�������������������ߵ�ƽ*/
	P0CR |= BIT0;
	P0PCR &= (~BIT0);
	N_TXD = 1;
	
	/*RXD���룬��������*/
	P2CR &= (~BIT7);
	P2PCR &= (~BIT7);
}


/*-----------------------------------------------------------------------------
Description:		��ʼ����������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void Uart0Init(void)
{	

	//UartDataInit();       //UART ���ݸ�λ  
	Uart0IOSet();	

	/*BANK1*/
	BANK1_SET;	
	UART0CR = 0x05;							//TXD0ӳ�䵽P0.0��RXD0ӳ�䵽P2.7
	BANK0_SET;	
	SCON = 0x50;							//0101 0000����ʽ1��8λ�첽���ɱ䲨���ʣ���������
	
											//24000000/16/9600 = 156.25
											//SBRT = 32768 - 156 = 32612 = 0x7F64
											//9600 = 24000000/(16*156+BFINE)
											//BFINE = 4��ʵ�ʲ�����Ϊ24000000/(16*156+4)=9600
	SBRTH = 0x7F;							//EUART�����ʷ�����������
	SBRTL = 0x64;
	SFINE = 0x04;
	
	SBRTH |= BIT7;							//��EUART0������
	
	BANK0_SET;	
	
	UTOS |= BIT0;							//RXD0��ƽΪTTL�߼�
	
	IEN0 |= BIT4;							//��EUART0�ж�	
}


//�����жϺ������
void INT_EUART0(void) interrupt 4
{
	//unsigned char data UartRevBak;
	//unsigned char data i; 
	unsigned char recvSbuf = 0;
	
	_push_(INSCON);
	BANK0_SET;	
	if(RI)	
	{
		RI = 0;

		if(!RevWifiEnd_flag)
		{
			recvSbuf = SBUF;
			RevWifiworking_flag= 1;
			recvOutTime = RECV_OUT_TIMER;

			if((frameHead_H_Flag==1) && (frameHead_L_Flag== 1))
			{
				recvDataCount++;
				Recv_Buf[recvDataCount]=recvSbuf;

				if(recvDataCount == 9)
				{
					recvFrameLen = Recv_Buf[8];
					recvFrameLen <<= 8;
					recvFrameLen += Recv_Buf[9];
					recvFrameLen += 10;				//����ǰʮλ��֡ͷ
				}
				if(recvDataCount > 9)
				{
					if(recvDataCount >= recvFrameLen)
					{
						recvDataCount = 0;
						frameHead_H_Flag = 0;
						frameHead_L_Flag = 0;
						RevWifiworking_flag= 0;
						RevWifiEnd_flag= 1;	
					}
				}
			}
			
			if((frameHead_H_Flag== 1) && (frameHead_L_Flag== 0))
			{
				if(recvSbuf == 0xFB)
				{
					frameHead_L_Flag=1;
					Recv_Buf[0] = 0xFA;
					Recv_Buf[1] = 0xFB;
					recvDataCount	= 1;
				}
				else
				{
					frameHead_H_Flag= 0;
				}
			}
			
			if((frameHead_H_Flag== 0) && (frameHead_L_Flag== 0))
			{
				if(recvSbuf == 0xFA)
				{
					frameHead_H_Flag= 1;	
				}
			}
		}
	}

	if(TI)
	{
		TI=0;


		sendDataCount++;
		if(sendDataCount == sendFrameLen)
		{
			sendDataCount = 0;
			MCUSendWorking_Flag=0;	//
			sendIntervalTime = SEND_INTERVAL_TIMER;
		}
		else
		{
			SBUF = Send_Buf[sendDataCount];	//
			MCUSendWorking_Flag= 1;
		}


	}
	BANK0_SET;			
	_pop_(INSCON);
}

/****************************
�������ƣ�
�������ܣ������λУ��λ
����������
��������ֵ��
����ʱ�䣺Zhengmingwei-20150728
��    ע��  
�޸ļ�Ҫ:
*******************************/
unsigned char SumMakeVerify(unsigned char *PBuf, unsigned int Len)
{
    unsigned char Verify = 0;
    while(Len--)
    {
    	Verify = Verify + (*PBuf);
      ++PBuf;
    }
    return (~Verify + 1);
}

/****************************
�������ƣ�
�������ܣ����ܰ�λУ��λ
����������
��������ֵ��
����ʱ�䣺Zhengmingwei-20150728
��    ע��  
�޸ļ�Ҫ:
*******************************/
unsigned char SumVerify(unsigned char *PBuf, unsigned int Len)
{
	unsigned char Verify = 0;
	while(Len--)
	{
	  Verify = Verify + (*PBuf);
	  PBuf ++;
	}
	return (Verify == 0); //���verifyΪ0 ����1 ���򷵻�0
}

void KeyAction_buf(uchar lenNumber)
{
	uchar i=0;

	for(i=0;i<lenNumber;i++)
	{

		Send_Buf[11+i]=MCUSendWifiDate.KeyCountN[i];

	}	
	
}

/****************************
�������ƣ�
�������ܣ�
����������
��������ֵ��
����ʱ�䣺
��    ע��  
�޸ļ�Ҫ:
*******************************/
unsigned int SendUartData(unsigned int len)
{
	Send_Buf[0] = SN_start0;        						//֡ͷ��λ
	Send_Buf[1] = SN_start1;				 					//֡ͷ��λ
	Send_Buf[2] = Contract_Verios;						//ͨ�Ű汾
	Send_Buf[3] = 0x00;					 				//֡����
	Send_Buf[4] = 0x00;					 				//֡���
	Send_Buf[5]	= 0x00;					 				//���ӿ�����; 0x00:��������ָ��Ҫ��Է��ظ�
	Send_Buf[len] = SumMakeVerify(Send_Buf,len);
	sendDataCount = 0;
	McuSendWifi_ACK_flag=0;		 				//1��ʾ��ҪӦ���ʶλ��0��ʾ����Ҫ
	ackTime = ACK_TIMER;
	MCUSendWorking_Flag=1;
	SBUF = Send_Buf[0];
	return (len + 1);
}

void SendUart(void)
{
	unsigned char i = 0;
	if((McuSendWifi_ACK_flag== 0) && (MCUSendWorking_Flag== 0) && (sendIntervalTime == 0))
	{
		if(cmdNum > 0)
		{
			cmdCode = CmdCode_Buf[0];
			cmdNum--;
			for(i = 0;i < cmdNum;i++)
			{
				CmdCode_Buf[i] = CmdCode_Buf[i + 1];
			}
			MCUSendStart_Flag = 1;		
		}
	}

	if(MCUSendStart_Flag)
	{
		MCUSendStart_Flag= 0;
		switch(cmdCode)
		{
			case 0xCCC0:
				Send_Buf[6] = 0xcc;					//ָ���λ 
				Send_Buf[7] = 0xc0;					//ָ���λ
				Send_Buf[8] = 0x00;					//���ݳ���
				Send_Buf[9] = 0x06;					//���ݳ���
				Send_Buf[10] = KXModel_H; 		 	//�豸�ͺŸ�λ
				Send_Buf[11] = KXModel_L ; 		 	//�豸�ͺŵ�λ �翾��KX-38I95 0x3c02
				Send_Buf[12] = 0x00; 				//�豸ͨ�Ű汾
				Send_Buf[13] = PROGRAM_VER_GB;		//�豸�̼��汾
				Send_Buf[14] = 0x00; 				//ͨ�����ݳ��ȸ�λ
				Send_Buf[15] = 0xFF; 		 		//ͨ�����ݳ��ȵ�λ
				sendFrameLen = 16;
				sendFrameLen = SendUartData(sendFrameLen);	
				McuSendWifi_ACK_flag=1;
				
				break;
			case 0xCCC2:
				MCUSmartlinkWorking_Flag = 1;
				//smarkLinkStatus = 0x00;  			//ȥ�����WiFi״̬����˸������
				Send_Buf[6] = 0xcc;					//ָ���λ 
				Send_Buf[7] = 0xc2;					//ָ���λ
				Send_Buf[8] = 0x00;					//���ݳ���
				Send_Buf[9] = 0x00;					//���ݳ���
				sendFrameLen = 10;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=1;
				break;
			case 0x6001:								//���ָ��
				Send_Buf[6] = 0x60;
				Send_Buf[7] = 0x01;
				Send_Buf[8] = 0;						//���ݳ��ȸ�λ
				Send_Buf[9] = 2;						//���ݳ��ȵ�λ
				Send_Buf[10] = 0x00; 					
				Send_Buf[11] = 0x00;
				sendFrameLen = 12;
				sendFrameLen = SendUartData(sendFrameLen);
				Send_Buf[5] = 0x40;
				McuSendWifi_ACK_flag= 0; 				
				break;
			case 0xCCC5:                             //�豸Ҫ��̼�����   
				Send_Buf[6] = 0xcc;					//ָ���λ 
				Send_Buf[7] = 0xc5;					//ָ���λ
				Send_Buf[8] = 0x00;					//���ݳ���
				Send_Buf[9] = 0x02;					//���ݳ���
				Send_Buf[10] = 0x00;				//ÿ֡���ȸ�λ
				Send_Buf[11] = 0x01;				//ÿ֡���ȵ�λ �ݶ�1
				sendFrameLen = 12;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=1;
				break;

			
			case 0xCCC9:                             //����ģ��   
				Send_Buf[6] = 0xcc;					//ָ���λ 
				Send_Buf[7] = 0xc9;					//ָ���λ
				Send_Buf[8] = 0x00;					//���ݳ���
				Send_Buf[9] = 0x00;					//���ݳ���
				sendFrameLen = 10;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=1;
				break;

			case 0x5000:                            //�����ϱ�״̬   
				Send_Buf[6] = 0x50;					//ָ���λ 
				Send_Buf[7] = 0x00;					//ָ���λ
				Send_Buf[8] = 0x00;					//���ݳ���
				Send_Buf[9] = 45;					//���ݳ���
				Send_Buf[10] =MCUSendWifiDate.McuWorkSta; //����״̬     0x5000

				Send_Buf[11] =MCUSendWifiDate.McuMumeNumber_0;  //�˰����� 

				Send_Buf[12] =MCUSendWifiDate.McuMumeNumber_1;  //�˰����� 
				
			 	Send_Buf[13] =MCUSendWifiDate.McuMumeNumber_2;  //�˰����� 

				Send_Buf[14] =MCUSendWifiDate.McuMumeNumber_3;  //�˰����� 

				Send_Buf[15] =MCUSendWifiDate.McuyuyueEn; 		 //ԤԼʹ�ܣ�0x01 ʹ��

				Send_Buf[16] =MCUSendWifiDate.McuyuyueFlag; 	 //ԤԼ��־λ 0x01 Ĭ����ԤԼ

				Send_Buf[17] =MCUSendWifiDate.McuyuyueTimer_H; 	  //ԤԼʱ��

				Send_Buf[18] =MCUSendWifiDate.McuyuyueTimer_L; 	  //ԤԼʱ��

				Send_Buf[19] =MCUSendWifiDate.McuyureEn; 		 //Ԥ��ʹ�ܣ�0x01 ʹ��

				Send_Buf[20] =MCUSendWifiDate.McuyureFlag; 		 //Ԥ�ȱ�־λ 0x01 Ĭ����Ԥ��

				Send_Buf[21] =MCUSendWifiDate.McuyureTimer_H; 		 //Ԥ��ʱ��

				Send_Buf[22] =MCUSendWifiDate.McuyureTimer_L; 		 //Ԥ��ʱ��

				Send_Buf[23] =MCUSendWifiDate.McuyureKeepFlag; 		 //Ԥ�ȱ���

				Send_Buf[24] =MCUSendWifiDate.McuyureKeepTimer; 	 //Ԥ�ȱ���ʱ��

				Send_Buf[25] =MCUSendWifiDate.WorkStep1_Top_Temp; 	 	//������һ���Ϲܵ��¶�

				Send_Buf[26] =MCUSendWifiDate.WorkStep1_Bot_Temp;  		//������һ���¹ܵ��¶�
				
				Send_Buf[27] =MCUSendWifiDate.WorkStep1_work_Time;  	//������һ������ʱ��

				Send_Buf[28] =MCUSendWifiDate.WorkStep2_Top_Temp;  		//������2���Ϲܵ��¶�

				Send_Buf[29] =MCUSendWifiDate.WorkStep2_Bot_Temp;	  	//������2���¹ܵ��¶�
				
				Send_Buf[30] =MCUSendWifiDate.WorkStep2_work_Time;  	//������2������ʱ��

				
				Send_Buf[31] =MCUSendWifiDate.WorkStep3_Top_Temp;		//������3���Ϲܵ��¶�
				
				Send_Buf[32] =MCUSendWifiDate.WorkStep3_Bot_Temp;		//������3���¹ܵ��¶�
					
				Send_Buf[33] =MCUSendWifiDate.WorkStep3_work_Time;		//������3������ʱ��

				Send_Buf[34] =MCUSendWifiDate.Temp_Ajust_Line;    		//�¶ȵ��� �ֽ��
				
				Send_Buf[35] =MCUSendWifiDate.Temp_Ajust_small;    		//�¶ȵ��� С����

				Send_Buf[36] =MCUSendWifiDate.Temp_Ajust_large;    		//�¶ȵ��� �󲽳�

				Send_Buf[37] =MCUSendWifiDate.Temp_Ajust_samllest;    	//�¶ȵ��� ��С�¶�

				Send_Buf[38] =MCUSendWifiDate.Temp_Ajust_largeest;    	//�¶ȵ��� ����¶�

				Send_Buf[39] =MCUSendWifiDate.Timer_work_leftMin;    	//ʣ�๤��ʱ��

				Send_Buf[40] =MCUSendWifiDate.Timer_work_leftsecond;    //ʣ�๤��ʱ������

				Send_Buf[41] =MCUSendWifiDate.Timer_Ajust_Min;    		//�ɵ���С

				Send_Buf[42] =MCUSendWifiDate.Timer_Ajust_Max;    		//�ɵ����

				Send_Buf[43] =MCUSendWifiDate.zhuanchaSta;              //ת��״̬ 1�� 0��

				Send_Buf[44] =MCUSendWifiDate.LudengSta;              	//¯��״̬	1�� 0��

				Send_Buf[45] =MCUSendWifiDate.RealTemp_Top;				//�Ϲ�ʵʱ�¶�

				Send_Buf[46] =MCUSendWifiDate.RealTemp_Bot;				//�¹�ʵʱ�¶�

				Send_Buf[47] =MCUSendWifiDate.Resver_1;					//Ԥ��
				Send_Buf[48] =MCUSendWifiDate.Resver_2;					//Ԥ��
				Send_Buf[49] =MCUSendWifiDate.Resver_3;					//Ԥ��
				Send_Buf[50] =MCUSendWifiDate.Resver_4;					//Ԥ��
				Send_Buf[51] =MCUSendWifiDate.Resver_5;					//Ԥ��
				Send_Buf[52] =MCUSendWifiDate.Resver_6;					//Ԥ��
			

				Send_Buf[53] =MCUSendWifiDate.ErroDate_H;              //���ϴ���

				Send_Buf[54] =MCUSendWifiDate.ErroDate_L;              //���ϴ���	
							
				sendFrameLen = 55;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=0;
				break;
			
			case 0x820A:
				
				Send_Buf[6] = 0x82;					//ָ���λ 
				Send_Buf[7] = 0x0A;					//ָ���λ
				Send_Buf[8] = 0x00;					//���ݳ���
				Send_Buf[9] = 0x00;					//���ݳ���
				sendFrameLen = 10;
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=0;
				break;


			case 0x6003:
				
				Send_Buf[6] = 0x60;									//ָ���λ 
				Send_Buf[7] = 0x03;									//ָ���λ
				Send_Buf[8] = 0x00;									//���ݳ���
				Send_Buf[9] = (MCUSendWifiDate.KeyNumber+1);		//���ݳ���
				Send_Buf[10] = (MCUSendWifiDate.KeyNumber);	     	//��������
				KeyAction_buf(MCUSendWifiDate.KeyNumber);
				sendFrameLen=(MCUSendWifiDate.KeyNumber+11);
				sendFrameLen = SendUartData(sendFrameLen);
				McuSendWifi_ACK_flag=0;
				break;
				

				
				default:
				break;
		}	
	}
}                  


/****************************

*******************************/
unsigned int SendUartAck(unsigned int len)
{
   	unsigned int i;
	McuSendWifi_ACK_flag= 0; //����ҪӦ��
	//cmdNum = 0;
	for(i = 0; i < len;i++)
	{
		Send_Buf[i] = Recv_Buf[i];
	}
	Send_Buf[5] = 0x80; //mcu���ճɹ��ظ�������wifi��
	Send_Buf[len] = SumMakeVerify(Send_Buf,len);
	sendDataCount = 0;
	MCUSendWorking_Flag= 1;
	SBUF = Send_Buf[0];
	return(len + 1);
}



void CommRecvData(void)//����wifi���ݳɹ�
{
	Recv_Buf[8]  = 0x00;
	Recv_Buf[9]  = 0x02;
	Recv_Buf[10] = 0x00;
	Recv_Buf[11] = 0x00; 	 
	sendFrameLen = 12;
	sendFrameLen = SendUartAck(sendFrameLen);
}

void returnData(void)//����wifi���ݳɹ�
{
	// Recv_Buf[8]  = 0x00;
	// Recv_Buf[9]  = 0x02;
	// Recv_Buf[10] = 0x00;
	// Recv_Buf[11] = 0x00; 	 
	sendFrameLen = 11;
	sendFrameLen = SendUartAck(sendFrameLen);
}


/****************************
�������ƣ�
�������ܣ�
����������
��������ֵ��
����ʱ�䣺
��    ע��  
�޸ļ�Ҫ:
*******************************/
void RecvUart(void)
{
	unsigned char verify;
	unsigned int cmdRecv;

   unsigned char i = 0;
   unsigned char j = 0;
   
	if(RevWifiworking_flag== 0)
	{
		if(RevWifiEnd_flag)
		{
			RevWifiEnd_flag=0;
			verify = SumVerify(Recv_Buf,(recvFrameLen + 1));
			if(verify == 1)
			{
				cmdRecv = Recv_Buf[6];
				cmdRecv = cmdRecv << 8;
				cmdRecv = cmdRecv + Recv_Buf[7];
				switch(cmdRecv)
				{
					case 	0xCCC0:
						if((Recv_Buf[10] == 0x00) && (Recv_Buf[11] == 0x00))	    
						{
							HandsCmd_OK_flag=1;		//���ֳɹ�
							MCURevDate_hands.WIFISta = Recv_Buf[12];
							McuSendWifi_ACK_flag=0;
							cmdNum = 0;
						}
						break;
					case 	0xCCC2:
						if((Recv_Buf[10] == 0x00) && (Recv_Buf[11] == 0x00))
						{
							McuSendWifi_ACK_flag=0;
							CmdSmartlinkStart_flag= 0;	//smartLinkָ��
							cmdNum = 0;
						}
						break;	
					case 0xCCD0:
						if(Recv_Buf[5] == 0x00)
						{
							MCURevDate_hands.WIFISta=Recv_Buf[10];
							CommRecvData();
						}
						else //smartlink ������
						{
							MCURevDate_hands.WIFISta=Recv_Buf[10];
						}
						
						if(MCUSmartlinkWorking_Flag)
						{
							if((MCURevDate_hands.WIFISta != 0) && (MCURevDate_hands.WIFISta != 1))	//����·������ָʾ�Ƴ�����
							{
								MCUSmartlinkWorking_Flag = 0; 
								CmdSmartlinkStart_flag= 0;	//������ķ�ֹһֱ����smartLinkָ��
							}
						}
						break;

					case 0xCCC5:   //CCC5�豸Ҫ��̼�����
						if((Recv_Buf[10] == 0x00)&&(Recv_Buf[10] == 0x00))//�ɹ�
						{
							MCURevWifiDate.CCC5GoUpAck=1;	
						}
						else //
						{
							MCURevWifiDate.CCC5GoUpAck=0;
						}
						
						
						break;
					case 0xCCC9:   //����ģ��
						if((Recv_Buf[10] == 0x00)&&(Recv_Buf[10] == 0x00))//�ɹ�
						{
							MCURevWifiDate.CCC9ReBootAck=1;	
						}
						else //
						{
							MCURevWifiDate.CCC9ReBootAck=0;	
						}
						
						break;		
				
					case 0xCCD1:									//�̼���������ָ��
						if(g_sysType == 1) //����
						{
							recvFirmWareEdit = Recv_Buf[10];
							/*Sector_Erase(0x0300,1); 			//�̼�����������Կ
							Byte_Write1(0x0300,0x55,1); 		//д����Կ0x55
							Byte_Write1(0x0301,0xaa,1); 		//д����Կ0xaa */	
							Recv_Buf[8] = 0;
							Recv_Buf[9] = 4;
							Recv_Buf[10] = 0x00;
							Recv_Buf[11] = 0x00;
							Recv_Buf[12] = 0x01;
							Recv_Buf[13] = 0x00; 
							sendFrameLen = 14;
							sendFrameLen = SendUartAck(sendFrameLen); 
							firmWareHandle_flag = 0x01;				
						}
						break;
					case	0x00B1:										//ȡ��ָ��			
						//g_systemState = SYS_DEFAULT_STAND_BY;
						//g_buzzerType  = BUZ_TYP_KEY_AUTO;
						Recv_Buf[8] = 0;
						Recv_Buf[9] = 6;
						Recv_Buf[10] = 0x00;
						Recv_Buf[11] = 0x00;
						Recv_Buf[12] = MCUSendWifiDate.McuMumeNumber_0;
						Recv_Buf[13] = MCUSendWifiDate.McuMumeNumber_0;
						Recv_Buf[14] = MCUSendWifiDate.McuMumeNumber_0;
						Recv_Buf[15] = MCUSendWifiDate.McuMumeNumber_0;	

						sendFrameLen = 16;
						sendFrameLen = SendUartAck(sendFrameLen);						
						break;
					case 0x00B2:
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						Recv_Buf[8]  = 0x00;  //���ݳ���
						Recv_Buf[9]  = 43;
						
						Recv_Buf[10] =MCUSendWifiDate.McuWorkSta; //����״̬     0x5000

						Recv_Buf[11] =MCUSendWifiDate.McuMumeNumber_0;  //�˰����� 

						Recv_Buf[12] =MCUSendWifiDate.McuMumeNumber_1;  //�˰����� 
				
			 			Recv_Buf[13] =MCUSendWifiDate.McuMumeNumber_2;  //�˰����� 

						Recv_Buf[14] =MCUSendWifiDate.McuMumeNumber_3;  //�˰����� 

						Recv_Buf[15] =MCUSendWifiDate.McuyuyueEn; 		 //ԤԼʹ�ܣ�0x01 ʹ��

						Recv_Buf[16] =MCUSendWifiDate.McuyuyueFlag; 	 //ԤԼ��־λ 0x01 Ĭ����ԤԼ

						Recv_Buf[17] =MCUSendWifiDate.McuyuyueTimer_H; 	  //ԤԼʱ��

						Recv_Buf[18] =MCUSendWifiDate.McuyuyueTimer_L; 	  //ԤԼʱ��

						Recv_Buf[19] =MCUSendWifiDate.McuyureEn; 		 //Ԥ��ʹ�ܣ�0x01 ʹ��

						Recv_Buf[20] =MCUSendWifiDate.McuyureFlag; 		 //Ԥ�ȱ�־λ 0x01 Ĭ����Ԥ��

						Recv_Buf[21] =MCUSendWifiDate.McuyureTimer_H; 		 //Ԥ��ʱ��

						Recv_Buf[22] =MCUSendWifiDate.McuyureTimer_L; 		 //Ԥ��ʱ��

						Recv_Buf[23] =MCUSendWifiDate.McuyureKeepFlag; 		 //Ԥ�ȱ���

						Recv_Buf[24] =MCUSendWifiDate.McuyureKeepTimer; 	 //Ԥ�ȱ���ʱ��

						Recv_Buf[25] =MCUSendWifiDate.WorkStep1_Top_Temp; 	 	//������һ���Ϲܵ��¶�

						Recv_Buf[26] =MCUSendWifiDate.WorkStep1_Bot_Temp;  		//������һ���¹ܵ��¶�
						
						Recv_Buf[27] =MCUSendWifiDate.WorkStep1_work_Time;  	//������һ������ʱ��

						Recv_Buf[28] =MCUSendWifiDate.WorkStep2_Top_Temp;  		//������2���Ϲܵ��¶�

						Recv_Buf[29] =MCUSendWifiDate.WorkStep2_Bot_Temp;	  	//������2���¹ܵ��¶�
						
						Recv_Buf[30] =MCUSendWifiDate.WorkStep2_work_Time;  	//������2������ʱ��

						
						Recv_Buf[31] =MCUSendWifiDate.WorkStep3_Top_Temp;		//������3���Ϲܵ��¶�
						
						Recv_Buf[32] =MCUSendWifiDate.WorkStep3_Bot_Temp;		//������3���¹ܵ��¶�
							
						Recv_Buf[33] =MCUSendWifiDate.WorkStep3_work_Time;		//������3������ʱ��

						Recv_Buf[34] =MCUSendWifiDate.Temp_Ajust_Line;    		//�¶ȵ��� �ֽ��
						
						Recv_Buf[35] =MCUSendWifiDate.Temp_Ajust_small;    		//�¶ȵ��� С����

						Recv_Buf[36] =MCUSendWifiDate.Temp_Ajust_large;    		//�¶ȵ��� �󲽳�

						Recv_Buf[37] =MCUSendWifiDate.Temp_Ajust_samllest;    	//�¶ȵ��� ��С�¶�

						Recv_Buf[38] =MCUSendWifiDate.Temp_Ajust_largeest;    	//�¶ȵ��� ����¶�

						Recv_Buf[39] =MCUSendWifiDate.Timer_work_leftMin;    	//ʣ�๤��ʱ��

						Recv_Buf[40] =MCUSendWifiDate.Timer_work_leftsecond;    //ʣ�๤��ʱ������

						Recv_Buf[41] =MCUSendWifiDate.Timer_Ajust_Min;    		//�ɵ���С

						Recv_Buf[42] =MCUSendWifiDate.Timer_Ajust_Max;    		//�ɵ����

						Recv_Buf[43] =MCUSendWifiDate.zhuanchaSta;              //ת��״̬ 1�� 0��

						Recv_Buf[44] =MCUSendWifiDate.LudengSta;              	//¯��״̬	1�� 0��

						Recv_Buf[45] =MCUSendWifiDate.Resver_1;					//Ԥ��
						Recv_Buf[46] =MCUSendWifiDate.Resver_2;					//Ԥ��
						Recv_Buf[47] =MCUSendWifiDate.Resver_3;					//Ԥ��
						Recv_Buf[48] =MCUSendWifiDate.Resver_4;					//Ԥ��
						Recv_Buf[49] =MCUSendWifiDate.Resver_5;					//Ԥ��
						Recv_Buf[50] =MCUSendWifiDate.Resver_6;					//Ԥ��
					

						Recv_Buf[51] =MCUSendWifiDate.ErroDate_H;              //���ϴ���

						Recv_Buf[52] =MCUSendWifiDate.ErroDate_L;              //���ϴ���	
									
	                  	sendFrameLen = 53;
						sendFrameLen = SendUartAck(sendFrameLen);								
						break;

					case 0x00B3:
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						MCURevWifiDate.McuMumeNumber_0=Recv_Buf[9];  //�˰����� 

						MCURevWifiDate.McuMumeNumber_1=Recv_Buf[10];  //�˰����� 
						
					 	MCURevWifiDate.McuMumeNumber_2=Recv_Buf[11];  //�˰����� 

						MCURevWifiDate.McuMumeNumber_3=Recv_Buf[12];  //�˰����� 

						MCURevWifiDate.McuyuyueEn=Recv_Buf[13]; 		 //ԤԼʹ�ܣ�0x01 ʹ��

						MCURevWifiDate.McuyuyueFlag=Recv_Buf[14]; 		 //ԤԼ��־λ 0x01 Ĭ����ԤԼ

						MCURevWifiDate.McuyuyueTimer_H=Recv_Buf[15]; 		 //ԤԼʱ��

						MCURevWifiDate.McuyuyueTimer_L=Recv_Buf[16]; 		 //ԤԼʱ��

						MCURevWifiDate.McuyureEn=Recv_Buf[17]; 		 //Ԥ��ʹ�ܣ�0x01 ʹ��

						MCURevWifiDate.McuyureFlag=Recv_Buf[18]; 		 //Ԥ�ȱ�־λ 0x01 Ĭ����Ԥ��

						MCURevWifiDate.McuyureTimer_H=Recv_Buf[19]; 		 //Ԥ��ʱ��

						MCURevWifiDate.McuyureTimer_L=Recv_Buf[20]; 		 //Ԥ��ʱ��

						MCURevWifiDate.McuyureKeepFlag=Recv_Buf[21]; 		 //Ԥ�ȱ���

						MCURevWifiDate.McuyureKeepTimer=Recv_Buf[22]; 	 //Ԥ�ȱ���ʱ��

						MCURevWifiDate.WorkStep1_Top_Temp=Recv_Buf[23]; 	 	//������һ���Ϲܵ��¶�

						MCURevWifiDate.WorkStep1_Bot_Temp=Recv_Buf[24];  		//������һ���¹ܵ��¶�
						
						MCURevWifiDate.WorkStep1_work_Time=Recv_Buf[25];  	//������һ������ʱ��

						MCURevWifiDate.WorkStep2_Top_Temp=Recv_Buf[26];  		//������2���Ϲܵ��¶�

						MCURevWifiDate.WorkStep2_Bot_Temp=Recv_Buf[27];	  	//������2���¹ܵ��¶�
						
						MCURevWifiDate.WorkStep2_work_Time=Recv_Buf[28];  	//������2������ʱ��

						
						MCURevWifiDate.WorkStep3_Top_Temp=Recv_Buf[29];		//������3���Ϲܵ��¶�
						
						MCURevWifiDate.WorkStep3_Bot_Temp=Recv_Buf[30];		//������3���¹ܵ��¶�
							
						MCURevWifiDate.WorkStep3_work_Time=Recv_Buf[31];		//������3������ʱ��

						MCURevWifiDate.Temp_Ajust_Line=Recv_Buf[32];    		//�¶ȵ��� �ֽ��
						
						MCURevWifiDate.Temp_Ajust_small=Recv_Buf[33];    		//�¶ȵ��� С����

						MCURevWifiDate.Temp_Ajust_large=Recv_Buf[34];    		//�¶ȵ��� �󲽳�

						MCURevWifiDate.Temp_Ajust_samllest=Recv_Buf[35];    	//�¶ȵ��� ��С�¶�

						MCURevWifiDate.Temp_Ajust_largeest=Recv_Buf[36];    	//�¶ȵ��� ����¶�

						MCURevWifiDate.Timer_Ajust_Min=Recv_Buf[37];    		//�ɵ���С

						MCURevWifiDate.Timer_Ajust_Max=Recv_Buf[38];    		//�ɵ����

						MCURevWifiDate.zhuanchaSta=Recv_Buf[39];              //ת��״̬ 1�� 0��

						MCURevWifiDate.LudengSta=Recv_Buf[40];              	//¯��״̬	1�� 0��

						MCURevWifiDate.Resver_1=Recv_Buf[41];					//Ԥ��
						MCURevWifiDate.Resver_2=Recv_Buf[42];					//Ԥ��
						MCURevWifiDate.Resver_3=Recv_Buf[43];					//Ԥ��
						MCURevWifiDate.Resver_4=Recv_Buf[44];					//Ԥ��
						MCURevWifiDate.Resver_5=Recv_Buf[45];					//Ԥ��
						MCURevWifiDate.Resver_6=Recv_Buf[46];					//Ԥ��

						Recv_Buf[8]  = 0x00;  //���ݳ���
						Recv_Buf[9]  = 6;

						Recv_Buf[10]  = 0x00;  
					    Recv_Buf[11]  = 0x00;

						Recv_Buf[10]  = MCURevWifiDate.McuMumeNumber_0;  
					    Recv_Buf[11]  = MCURevWifiDate.McuMumeNumber_1;

						Recv_Buf[10]  = MCURevWifiDate.McuMumeNumber_2;  
					    Recv_Buf[11]  = MCURevWifiDate.McuMumeNumber_3;
						
	                  	sendFrameLen =12;
						sendFrameLen = SendUartAck(sendFrameLen);								
						break;
					
					// case 0x5001:					//¯������
					// 	//g_buzzerType = BUZ_TYP_KEY_AUTO;
						
					// 	MCURevWifiDate.LudengONOFF=Recv_Buf[10];
					// 	CommRecvData(); 	
					// 	break;

					case 0x820C:					//����ץͼ
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						
						MCURevWifiDate.LudengONOFF = 1;
//						g_pictureCnt = 300;				//10msʱ����ץͼ����ʱ
						returnData(); 	
						break;

					case 0x5002:						//ת������
						//g_buzzerType = BUZ_TYP_KEY_AUTO;
						
						MCURevWifiDate.ZhuanchaONOFF=Recv_Buf[10];
						CommRecvData(); 
						break;

					case 0x5003:						//�����޸�
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.WorkRevise_Top_temp=Recv_Buf[10];
						
						MCURevWifiDate.WorkRevise_Bot_temp=Recv_Buf[11];
						
						MCURevWifiDate.WorkRevise_time=Recv_Buf[12];
						CommRecvData(); 
						break;

					case 0x5004:						//Ԥ����ת
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.Yurejump=1;
						
						CommRecvData(); 
						break;
						
					case 0x5005:						//Ԥ�ȱ���
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.YureKeep=Recv_Buf[10];
						MCURevWifiDate.YureKeeptime=Recv_Buf[11];
						
						CommRecvData(); 
						break;
						
					case 0x5008:						//����ͷ��Ϣ�·�
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						
						MCURevWifiDate.CamerSta=Recv_Buf[10];	

						MCURevWifiDate.CamerErr=Recv_Buf[11];

						MCURevWifiDate.Camerlast=Recv_Buf[12];
						
						CommRecvData(); 
						break;

						
					case 0x5009:						//�·�ʳ����Ϣ
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.FoodMsg=Recv_Buf[10];	

						MCURevWifiDate.FoodMsgLast=Recv_Buf[11];

						CommRecvData(); 
						break;

					case 0x500A:						//�·�ʶ��ʧ��ԭ��
						//g_buzzerType = BUZ_TYP_KEY_AUTO;

						MCURevWifiDate.AutoLoseCause=Recv_Buf[10];	

						MCURevWifiDate.AutoLoseCause_0=Recv_Buf[11];	

						MCURevWifiDate.AutoLoseCause_1=Recv_Buf[12];	

						MCURevWifiDate.AutoLoseCause_3=Recv_Buf[13];	

						CommRecvData(); 
						break;

				
					
               
					default:
						break;
				}
			}
		}
	}
}


/****************************
�������ƣ�
�������ܣ�
����������
��������ֵ��
����ʱ�䣺
��    ע��  
�޸ļ�Ҫ:
*******************************/
void WifiUartExceptionHandl(void)
{
	if(McuSendWifi_ACK_flag)
	{
		if(ackTime > 0)
		{
			ackTime--;
		}
		if(ackTime == 0)
		{
			MCUSendStart_Flag = 1;
		}
	}

	if((recvOutTime == 0) && (RevWifiworking_flag))
	{
		recvDataCount = 0;
		frameHead_H_Flag= 0;
		frameHead_L_Flag = 0;	
		RevWifiworking_flag = 0;
	}
}


/****************************
�������ƣ�
�������ܣ�
����������
��������ֵ��
����ʱ�䣺
��    ע��  
�޸ļ�Ҫ:
*******************************/
void WifiDealFunction(void)
{
	//����豸MCUû����WIFIģ��������ֳɹ�
	if(HandsCmd_OK_flag==0)
	{
		cmdNum = 0;
		if(cmdNum < CMD_NUM_MAX)
		{
			CmdCode_Buf[cmdNum] = 0xCCC0;
			cmdNum++;
		}
		
		SendUart();
		if(sendIntervalTime > 0)
		{
			sendIntervalTime--;
		}
		
		RecvUart();

		if(recvOutTime > 0)
		{
			recvOutTime--;
		}
		
		WifiUartExceptionHandl();		
	}
	else
	{
		if(CmdSmartlinkStart_flag==1)				//smartLink_start
		{
			CmdSmartlinkStart_flag = 0;
			cmdNum = 0;
			if(cmdNum < CMD_NUM_MAX)
			{
				CmdCode_Buf[cmdNum] = 0xCCC2;
				cmdNum++;
			}	
			SendUart();
			
			if(sendIntervalTime > 0)
			{
				sendIntervalTime--;
			}
			
			RecvUart();
			
			if(recvOutTime > 0)
			{
				recvOutTime--;
			}
			WifiUartExceptionHandl();
		}
		else
		{
			if(CmdSbGoUpStart_flag== 1)
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbGoUpStart_flag=0;
					CmdCode_Buf[cmdNum] = 0xCCC5;
					cmdNum++;
				}	
			}
			else if(CmdSbReBootStart_flag== 1)
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbReBootStart_flag= 0;
					CmdCode_Buf[cmdNum] = 0xCCC9;
					cmdNum++;
				}
			}
			else if(CmdSbReprotStart_flag== 1)				//�����ϱ�
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbReprotStart_flag= 0;
					CmdCode_Buf[cmdNum] = 0x5000;
					cmdNum++;
				}	
			}
			else if(CmdSbAutoBootStart_flag== 1)			//���ܺ決
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbAutoBootStart_flag = 0;
					CmdCode_Buf[cmdNum] = 0x820A;
					cmdNum++;
				}	
			}
			else if(CmdSbKeyActionStart_flag== 1)			//������Ϊ�ϱ�
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdSbKeyActionStart_flag = 0;
					CmdCode_Buf[cmdNum] = 0x1000;
					cmdNum++;
				}	
			}
			
			else if(CmdRemovelinkStart_flag== 1) 	//���ָ��0x6001
			{
				if(cmdNum < CMD_NUM_MAX)
				{
					CmdRemovelinkStart_flag = 0;
					CmdCode_Buf[cmdNum] = 0x6001;
					cmdNum++;
				}	
			}
			else
			{


			}
			
			if((CmdCode_Buf[0] != 0xccc0) && (CmdCode_Buf[0] != 0xccc2))
			{
				SendUart();
			}			
			if(sendIntervalTime > 0)
			{
				sendIntervalTime--;
			}
			RecvUart();
			if(recvOutTime > 0)
			{
				recvOutTime--;
			}
			WifiUartExceptionHandl();
		}
	}
}
