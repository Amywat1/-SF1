#ifndef UART_H
#define UART_H

#define P_TXD		P4_0
#define P_RXD		P4_2
//#define N_TXD		P0_0
//#define N_RXD		P0_1

#define SEND_MAX				12				//�������ݵ�������
#define RECV_MAX				26				//�������ݵ�������

/*���ư���յ���Ϣ*/
#define RECV_HARD 				UartData.RecvDataSave_Buff[0]		//ͷ��
#define RECV_SN					UartData.RecvDataSave_Buff[1]		//SN��

#define RECV_STEAM_H_TEM_H8		UartData.RecvDataSave_Buff[2]		//��¯�¶� ������ �߰�λ
#define RECV_STEAM_H_TEM_L8		UartData.RecvDataSave_Buff[3]		//��¯�¶� ������ �Ͱ�λ
#define RECV_STEAM_L_TEM_H8		UartData.RecvDataSave_Buff[4]		//��¯�¶� ������ �߰�λ
#define RECV_STEAM_L_TEM_L8		UartData.RecvDataSave_Buff[5]		//��¯�¶� ������ �Ͱ�λ

#define RECV_CORE_H_TEM_H8		UartData.RecvDataSave_Buff[6]		//�����¶� ������ �߰�λ��Ԥ����
#define RECV_CORE_H_TEM_L8		UartData.RecvDataSave_Buff[7]		//�����¶� ������ �Ͱ�λ��Ԥ����
#define RECV_CORE_L_TEM_H8		UartData.RecvDataSave_Buff[8]		//�����¶� ������ �߰�λ��Ԥ����
#define RECV_CORE_L_TEM_L8		UartData.RecvDataSave_Buff[9]		//�����¶� ������ �Ͱ�λ��Ԥ����

#define RECV_PUMP_CURRENT_H8	UartData.RecvDataSave_Buff[10]		//ˮ�õ���ADֵ�߰�λ
#define RECV_PUMP_CURRENT_L8	UartData.RecvDataSave_Buff[11]		//ˮ�õ���ADֵ�Ͱ�λ

#define RECV_PUMP_WORK_FLAG		UartData.RecvDataSave_Buff[12]		//�Ƿ��³���һ��ˮ

#define RECV_POT_TYPE			UartData.RecvDataSave_Buff[13]		//�ڹ�
#define RECV_ZERO_TYPE			UartData.RecvDataSave_Buff[14]		//����

#define RECV_POWER_VERSION		UartData.RecvDataSave_Buff[15]		//��Դ��汾

#define FCHUANG_HUO_OCDE_1		UartData.RecvDataSave_Buff[16]		//�������� 1
#define FCHUANG_HUO_OCDE_2		UartData.RecvDataSave_Buff[17]		//�������� 2
#define FCHUANG_HUO_OCDE_3		UartData.RecvDataSave_Buff[18]		//�������� 3
#define FCHUANG_HUO_OCDE_4		UartData.RecvDataSave_Buff[19]		//�������� 4
#define FCHUANG_HUO_OCDE_5		UartData.RecvDataSave_Buff[20]		//�������� 5
#define FCHUANG_HUO_OCDE_6		UartData.RecvDataSave_Buff[21]		//�������� 6
#define FCHUANG_HUO_OCDE_7		UartData.RecvDataSave_Buff[22]		//�������� 7
#define FCHUANG_HUO_OCDE_8		UartData.RecvDataSave_Buff[23]		//�������� 8	

#define RECV_DATA_CHECK_SUM		UartData.RecvDataSave_Buff[24]		//У���
#define RECV_SUM_RVS_CODE		UartData.RecvDataSave_Buff[25]		//У��ͷ���

/*���ư巢�͵���Ϣ*/
#define SEND_HARD				UartData.SendDataSave_Buff[0]		//ͷ��
#define SEND_SN					UartData.SendDataSave_Buff[1]		//SN��
#define SEND_OUT_DATA1			UartData.SendDataSave_Buff[2]		//�����Ϣ1��±�عܡ����ȡ����衢ɢ�ȣ�
#define SEND_OUT_DATA_STEAM		UartData.SendDataSave_Buff[3]		//�����Ϣ2����¯�ɿع���ƣ�
#define SEND_OUT_DATA_PUMP		UartData.SendDataSave_Buff[4]		//�����Ϣ3��ˮ�ó�ˮƵ�ʿ��ƣ�
#define SEND_OUT_DATA_HOT		UartData.SendDataSave_Buff[5]		//�����Ϣ4��Ԥ����
#define SEND_OUT_RVS_CODE1		UartData.SendDataSave_Buff[6]		//�������1
#define SEND_OUT_RVS_CODE2		UartData.SendDataSave_Buff[7]		//�������2
#define SEND_OUT_RVS_CODE3		UartData.SendDataSave_Buff[8]		//�������3
#define SEND_OUT_RVS_CODE4		UartData.SendDataSave_Buff[9]		//�������4
#define SEND_DATA_CHECK_SUM		UartData.SendDataSave_Buff[10]		//У���
#define SEND_SUM_RVS_CODE		UartData.SendDataSave_Buff[11]		//����ͷ���

#define BIT_HOT					BIT0								//±�عܿ���λ
#define BIT_FAN					BIT1								//�ּ�������ȿ���λ
#define BIT_STIR				BIT2								//����������λ
#define BIT_SANRE				BIT3								//ɢ�ȷ��ȿ���λ
#define BIT_NULL_1				BIT4								//Ԥ��
#define BIT_NULL_2				BIT5								//Ԥ��
#define BIT_NULL_3				BIT6								//Ԥ��
#define BIT_NULL_4				BIT7								//Ԥ��

typedef struct
{
	unsigned char SendData_Buff[SEND_MAX];				//���͵�����
    unsigned char SendDataTmp_Buff[SEND_MAX];			//�������ݵ���ʱֵ
	unsigned char SendDataSave_Buff[SEND_MAX];			//�������ݵı���ֵ
	unsigned char RecvData_Buff[RECV_MAX];				//���յ�����
    unsigned char RecvDataTmp_Buff[RECV_MAX];			//�������ݵ���ʱֵ
	unsigned char RecvDataSave_Buff[RECV_MAX];			//�������ݵı���ֵ
	unsigned char uartRevcCnt;							//�������ݵļ���
	unsigned char uartSendCnt;							//�������ݵļ���
	unsigned int  uartRecvErrCnt;						//���ڽ���ʧ�ܼ�ʱ
	unsigned char uartSendCrlCnt;						//���ڷ��ʹ������100msˢ��һ��
	unsigned int  uartRecvOverCnt;						//���ڽ��ճ�ʱ�����ֹͣ
}UART_DATA;

enum _UATR_SEND_TYPE_{                           
    sendOver               		=   0,					//�������
    sendGoing             		=   1,	      		 	//���ڷ���
};
enum _UATR_RECV_TYPE_{                           
    recvOver               		=   0,					//�������
    recvGoing             		=   1,	       			//���ڽ���
};

extern UART_DATA xdata			UartData;				//��������ʹ�õ��Ľṹ��
extern enum _UATR_RECV_TYPE_	g_uartRecvType;			//���ڽ���״̬
extern enum _UATR_SEND_TYPE_	g_uartSendType;			//���ڷ���״̬

extern bit 						g_firstRecvFlag;		//�״ν������ݱ�־

void InitUartIo(void);
void InitUartVariable(void);
void InitUart(void);
unsigned char TmpDataSum(unsigned char *buf,unsigned char len);
void TmpDataGive(unsigned char *recv,unsigned char *give,unsigned char len);
void UartSendDeal(void);
void UartRecvDeal(void);

#endif