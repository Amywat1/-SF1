#ifndef		_WIFI_H_ 
#define		_WIFI_H_

#define SN_start0							0XFA  
#define SN_start1							0XFB

#define Contract_Verios						0X00 //ͨѶ�汾
#define Contract_FrameNum					0X00 //ͨѶ֡����
#define Contract_FrameNumber				0X00 //ͨѶ֡���
#define Contract_ReturnCtl00          	 	0x00  //��������Ҫ��Է��ظ�
#define Contract_ReturnCtl01				0x80  //����ָ��
#define Contract_ReturnCtl10				0x40  //������������Է��ظ�

#define KXModel_H							0x94  //�豸�ͺ�
#define KXModel_L							0xfc  //�豸�ͺ�

#define  PROGRAM_VER_GB						0x00 //�豸����汾

#define SEND_INTERVAL_TIMER 				300			//2msһ��
#define RECV_OUT_TIMER						500
#define ACK_TIMER							250	

#define CMD_NUM_MAX             10

typedef struct
{
   
	unsigned char CCC5GoUpAck;
	unsigned char CCC9ReBootAck;
//****************************************************

	//����״̬     0x00B3

	unsigned char McuMumeNumber_0;  //�˰����� 
	unsigned char McuMumeNumber_1;  //�˰����� 
 	unsigned char McuMumeNumber_2;  //�˰�����
	unsigned char McuMumeNumber_3;  //�˰����� 
	unsigned char McuyuyueEn; 		 //ԤԼʹ�ܣ�0x01 ʹ��
	unsigned char McuyuyueFlag; 		 //ԤԼ��־λ 0x01 Ĭ����ԤԼ
	unsigned char McuyuyueTimer_H; 		 //ԤԼʱ��
	unsigned char McuyuyueTimer_L; 		 //ԤԼʱ��
	unsigned char McuyureEn; 		 //Ԥ��ʹ�ܣ�0x01 ʹ��
	unsigned char McuyureFlag; 		 //Ԥ�ȱ�־λ 0x01 Ĭ����Ԥ��
	unsigned char McuyureTimer_H; 		 //Ԥ��ʱ��
	unsigned char McuyureTimer_L; 		 //Ԥ��ʱ��
	unsigned char McuyureKeepFlag; 		 //Ԥ�ȱ���
	unsigned char McuyureKeepTimer; 	 //Ԥ�ȱ���ʱ��
	unsigned char WorkStep1_Top_Temp; 	 	//������һ���Ϲܵ��¶�
	unsigned char WorkStep1_Bot_Temp;  		//������һ���¹ܵ��¶�
	unsigned char WorkStep1_work_Time;  	//������һ������ʱ��
	unsigned char WorkStep2_Top_Temp;  		//������2���Ϲܵ��¶�
	unsigned char WorkStep2_Bot_Temp;	  	//������2���¹ܵ��¶�
	unsigned char WorkStep2_work_Time;  	//������2������ʱ��
	unsigned char WorkStep3_Top_Temp;		//������3���Ϲܵ��¶�
	unsigned char WorkStep3_Bot_Temp;		//������3���¹ܵ��¶�
	unsigned char WorkStep3_work_Time;		//������3������ʱ��
	unsigned char Temp_Ajust_Line;    		//�¶ȵ��� �ֽ��
	unsigned char Temp_Ajust_small;    		//�¶ȵ��� С����
	unsigned char Temp_Ajust_large;    		//�¶ȵ��� �󲽳�
	unsigned char Temp_Ajust_samllest;    	//�¶ȵ��� ��С�¶�
	unsigned char Temp_Ajust_largeest;    	//�¶ȵ��� ����¶�
	unsigned char Timer_Ajust_Min;    		//�ɵ���С
	unsigned char Timer_Ajust_Max;    		//�ɵ����
	unsigned char zhuanchaSta;              //ת��״̬ 1�� 0��
	unsigned char LudengSta;              	//¯��״̬	1�� 0��
	unsigned char Resver_1;					//Ԥ��
	unsigned char Resver_2;					//Ԥ��
	unsigned char Resver_3;					//Ԥ��
	unsigned char Resver_4;					//Ԥ��
	unsigned char Resver_5;					//Ԥ��
	unsigned char Resver_6;					//Ԥ��

//********************************************************************

	unsigned char LudengONOFF;	
	unsigned char ZhuanchaONOFF;	
	unsigned char WorkRevise_Top_temp;
	unsigned char WorkRevise_Bot_temp;
	unsigned char WorkRevise_time;
	unsigned char Yurejump;
	unsigned char YureKeep;
	unsigned char YureKeeptime;
	unsigned char CamerSta;	
	unsigned char CamerErr;
	unsigned char Camerlast;
	unsigned char FoodMsg;	
	unsigned char FoodMsgLast;	
	unsigned char AutoLoseCause;
	unsigned char AutoLoseCause_0;	
	unsigned char AutoLoseCause_1;	
	unsigned char AutoLoseCause_3;	
	
}MCURevWifi_str;

extern MCURevWifi_str xdata MCURevWifiDate;

typedef struct
{
	unsigned char McuWorkSta; //����״̬     0x5000
	unsigned char McuMumeNumber_0;  //�˰����� 
	unsigned char McuMumeNumber_1;  //�˰����� 
 	unsigned char McuMumeNumber_2;  //�˰����� 
	unsigned char McuMumeNumber_3;  //�˰����� 
	unsigned char McuyuyueEn; 		 //ԤԼʹ�ܣ�0x01 ʹ��
	unsigned char McuyuyueFlag; 		 //ԤԼ��־λ 0x01 Ĭ����ԤԼ
	unsigned char McuyuyueTimer_H; 		 //ԤԼʱ��
	unsigned char McuyuyueTimer_L; 		 //ԤԼʱ��
	unsigned char McuyureEn; 		 //Ԥ��ʹ�ܣ�0x01 ʹ��
	unsigned char McuyureFlag; 		 //Ԥ�ȱ�־λ 0x01 Ĭ����Ԥ��
	unsigned char McuyureTimer_H; 		 //Ԥ��ʱ��
	unsigned char McuyureTimer_L; 		 //Ԥ��ʱ��
	unsigned char McuyureKeepFlag; 		 //Ԥ�ȱ���
	unsigned char McuyureKeepTimer; 	 //Ԥ�ȱ���ʱ��
	unsigned char WorkStep1_Top_Temp; 	 	//������һ���Ϲܵ��¶�
	unsigned char WorkStep1_Bot_Temp;  		//������һ���¹ܵ��¶�
	unsigned char WorkStep1_work_Time;  	//������һ������ʱ��
	unsigned char WorkStep2_Top_Temp;  		//������2���Ϲܵ��¶�
	unsigned char WorkStep2_Bot_Temp;	  	//������2���¹ܵ��¶�
	unsigned char WorkStep2_work_Time;  	//������2������ʱ��
	unsigned char WorkStep3_Top_Temp;		//������3���Ϲܵ��¶�
	unsigned char WorkStep3_Bot_Temp;		//������3���¹ܵ��¶�
	unsigned char WorkStep3_work_Time;		//������3������ʱ��
	unsigned char Temp_Ajust_Line;    		//�¶ȵ��� �ֽ��
	unsigned char Temp_Ajust_small;    		//�¶ȵ��� С����
	unsigned char Temp_Ajust_large;    		//�¶ȵ��� �󲽳�
	unsigned char Temp_Ajust_samllest;    	//�¶ȵ��� ��С�¶�
	unsigned char Temp_Ajust_largeest;    	//�¶ȵ��� ����¶�
	unsigned char Timer_work_leftMin;    	//ʣ�๤��ʱ��
	unsigned char Timer_work_leftsecond;    //ʣ�๤��ʱ������
	unsigned char Timer_Ajust_Min;    		//�ɵ���С
	unsigned char Timer_Ajust_Max;    		//�ɵ����
	unsigned char zhuanchaSta;              //ת��״̬ 1�� 0��
	unsigned char LudengSta;              	//¯��״̬	1�� 0��
	unsigned char RealTemp_Top;				//�Ϲ�ʵʱ�¶�
	unsigned char RealTemp_Bot;				//�¹�ʵʱ�¶�
	unsigned char Resver_1;					//Ԥ��
	unsigned char Resver_2;					//Ԥ��
	unsigned char Resver_3;					//Ԥ��
	unsigned char Resver_4;					//Ԥ��
	unsigned char Resver_5;					//Ԥ��
	unsigned char Resver_6;					//Ԥ��
	unsigned char ErroDate_H;              //���ϴ���
	unsigned char ErroDate_L;              //���ϴ���
//****************************************************************************0x6003

	unsigned char KeyNumber; //��������
	unsigned char KeyCountN[10];
}MCUSendWifi_str;

extern MCUSendWifi_str xdata MCUSendWifiDate;

#define	MCUSendCmdflag_Date				MCUSendCmdflag_byte02.byte16     //
extern TYPE_BYTE16 MCUSendCmdflag_byte02;//

#define	CmdSmartlinkStart_flag					MCUSendCmdflag_byte02.b1.b0//smartlink
#define	CmdRemovelinkStart_flag					MCUSendCmdflag_byte02.b1.b1//���
#define	CmdSbGoUpStart_flag						MCUSendCmdflag_byte02.b1.b2//�豸Ҫ��̼�����
#define	CmdSbReBootStart_flag					MCUSendCmdflag_byte02.b1.b3//�豸Ҫ������wifi
#define	CmdSbReprotStart_flag					MCUSendCmdflag_byte02.b1.b4//�豸�ϱ�״̬
#define	CmdSbAutoBootStart_flag					MCUSendCmdflag_byte02.b1.b5//�豸�������ܺ決
#define	CmdSbKeyActionStart_flag				MCUSendCmdflag_byte02.b1.b6//����

#define	SendCmdflag_Date				SendCmdflag_byte02.byte16     //
extern TYPE_BYTE16 SendCmdflag_byte02;//

#define	HandsCmd_OK_flag				SendCmdflag_byte02.b1.b0//����
#define	McuSendWifi_ACK_flag			SendCmdflag_byte02.b1.b1//�費��ҪӦ��  1��Ҫ��  0����Ҫ

#define	RevCmdflag_Date				RevCmdflag_byte02.byte16     //
extern TYPE_BYTE16 RevCmdflag_byte02;//

#define	RevWifiEnd_flag					RevCmdflag_byte02.b1.b0//�����������
#define	RevWifiworking_flag				RevCmdflag_byte02.b1.b1//�������ݹ�����
#define frameHead_H_Flag				RevCmdflag_byte02.b1.b3//
#define frameHead_L_Flag				RevCmdflag_byte02.b1.b4//
#define MCUSendWorking_Flag				RevCmdflag_byte02.b1.b5//������
#define MCUSendStart_Flag				RevCmdflag_byte02.b1.b6//��ʼ����
#define MCUSmartlinkWorking_Flag		RevCmdflag_byte02.b1.b6//smartlink ��

//***************************************************************************************************

//�豸����>ģ��


typedef struct
{
   
	unsigned char Cmdcode_H;	 	//ָ��߰�λ
	unsigned char Cmdcode_L; 		//ָ��Ͱ�λ
 	unsigned char LengthDate_H;		//���ݳ���
 	unsigned char LengthDate_L;
	unsigned char EquNumber_H;		//�豸��Ÿ߰�λ
	unsigned char EquNumber_L;		//�豸��ŵͰ�λ
	unsigned char ProtcolVersion;	//ͨѶЭ��汾
	unsigned char EquVersion;		//�豸�̼��汾	
	unsigned char Datelarge_H;		//Date����󳤶ȸ߰�λ
	unsigned char Datelarge_L;		//Date����󳤶ȵͰ�λ
	unsigned char HardWareVersion;//�豸Ӳ���汾
	unsigned char Remove_sta;		//����ģ���״̬��0��Ĭ��ģʽ 1������APģʽ     2:����sarmlink  3����������    

	
}MCUSend_hands;

extern MCUSend_hands xdata MCUSendDate_hands;

typedef struct
{
   
	unsigned char Aanser_H;	 		//Ӧ�����߰�λ
	unsigned char Aanser_L; 		//Ӧ�����Ͱ�λ
 	unsigned char WIFISta;			//wifi״̬ 
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
   
}MCURev_hands;

extern MCURev_hands xdata MCURevDate_hands;


//***************************************************************************************************



//***************************************************************************************************
//�豸����>ģ��


typedef struct
{
   
	unsigned char Cmdcode_H;	 	//ָ��߰�λ
	unsigned char Cmdcode_L; 		//ָ��Ͱ�λ
 	unsigned char CmdDtae_H;     	//00
	unsigned char CmdDtae_L;		//00
	
}MCUSend_SmartLink;

extern MCUSend_SmartLink xdata MCUSendDate_SmartLink;

typedef struct
{
   
	unsigned char Aanser_H;	 		//����߰�λ
	unsigned char Aanser_L; 		//����Ͱ�λ
 
	
}MCURev_SmartLink;

extern MCURev_SmartLink xdata MCURevDate_SmartLink;

//***************************************************************************************************



//***************************************************************************************************
//�豸Ҫ��̼�����            �豸����>ģ��

typedef struct
{
   
	unsigned char Cmdcode_H;	 	//ָ��߰�λ
	unsigned char Cmdcode_L; 		//ָ��Ͱ�λ
 	unsigned char LengthDate_H;		//���ݳ���
	unsigned char LengthDate_L;		//���ݳ���
	unsigned char WIFISendDateLength_H;  //ÿһ֡�����ݳ���
	unsigned char WIFISendDateLength_L;
	
}MCUSend_SoftGoUp;

extern MCUSend_SoftGoUp xdata MCUSendDate_SoftGoUp;

typedef struct     //FAFB 00 00 00 80 CCC5 0002 0000 xx
{
   
	unsigned char Aanser_H;	 		//����߰�λ
	unsigned char Aanser_L; 		//����Ͱ�λ
 
	
}MCURev_SoftGoUp;

extern MCURev_SoftGoUp xdata MCURevDate_SoftGoUp;

//***************************************************************************************************


//***************************************************************************************************
//�豸����WIFIģ��         �豸����>ģ��

typedef struct
{
   
	unsigned char Cmdcode_H;	 	//ָ��߰�λ
	unsigned char Cmdcode_L; 		//ָ��Ͱ�λ
 
	unsigned char SendDate_H;  		//����
	unsigned char SendDate_L;
	
}MCUSend_ReBoot;

extern MCUSend_ReBoot xdata MCUSendDate_ReBoot;

typedef struct     //FAFB 00 00 00 80 CCC9 0002 0000 xx
{
   
	unsigned char Aanser_H;	 		//����߰�λ
	unsigned char Aanser_L; 		//����Ͱ�λ
 
	
}MCURev_ReBoot;

extern MCURev_ReBoot xdata MCURevDate_ReBoot;

//***************************************************************************************************


#define	HandsCmd_val	0x01  //����

extern void Uart0Init(void);
extern void WifiDealFunction(void);
void CmdHandVariableInit(void);
void CmdSmartLinkVariableInit(void);
void CmdSoftGoUpVariableInit(void);
void CmdReBootVariableInit(void);
extern void Cmd0x5000VariableIint(void);

#endif
