//**********************Copyright (c)***************************
//
//		���ݾ����ɷ����޹�˾
//
//		���ݾ�������������޹�˾
//
//		���ݾ���ŷ�϶�С�ҵ����޹�˾
//
//		��ʽ������ҵ��.��ز�
//
//*********************��Ŀ��Ϣ*********************************
//
//��Ŀ����:	
//
//�������:	C����		
//
//��ƷƷ��: 
//
//оƬ��Ϣ:	��ӱSH79F328(44PIN 32K_ROM��256BYTE ��RAM��1280BYTE ��RAM��4096BYTE E2PROM)
//
//�񵴷�ʽ:	�ڲ�RC 24M
//
//��ʾ��ʽ:	����ܣ�LED
//
//������ʽ:	��������
//
//���Դ��:	SF1-P
//
//����ʾ��:	SF1-K
//
//������Ա: ����ΰ
//
//��������:	
//
//��ע��Ϣ��
//***********************�汾��Ϣ********************************

//*************��ʷ�汾01*****************************
//����汾: A01
//
//У �� ��:
//
//�� �� ��: ����ΰ
//
//�޸�����: 2020.11.30
//
//���ļ�¼��1���̳�SF1 A10����

#include "header.h"

/*-----------------------------------------------------------------------------
Description:		������
Input:				void
Return:				void
History:			��
-----------------------------------------------------------------------------*/
void main(void)
{
	unsigned char tmp = (PROCESS_NUM - 1);	//ʹ��case 0��ʼִ�г���
	unsigned char cycleCnt = (PROCESS_NUM - 1);
	
	InitAll();								//�������ó�ʼ��

	while(1)
	{
		if(g_flg2ms)
		{
			g_flg2ms = 0;
			tmp++;
			if(tmp >= PROCESS_NUM)
			{
				tmp = 0;
			}
			
			/*ʱЧ�Խ�ǿ����*/
			InitWdt();						//�幷
			TouchKeyAdScan();				//����ADɨ��				//3�������������392us
			TouchKeyScan();					//����ɨ��
			TouchKeyJudge();				//�����ж�
			DisplayScan();					//��ʾɨ��					//7.08us
		}
		
		else
		{

		}
		
		if(cycleCnt != tmp)
		{
			cycleCnt = tmp;
			
			/*�Ƚ������ݣ��ٿ���������أ��ٷ�������*/
			switch(tmp)						//ÿ��ʱ��Ƭ����ѭ������Ϊ   PROCESS_NUM*2ms
			{
				case 0:
					UartRecvDeal();			//���ڽ��մ���				//210.2us
					break;
					
				case 1:
					MeasureDataScan();		//�������ɨ��				//107.5us
					break;
					
				case 2:
					LoadCrl();				//����������ؿ���			//43.6us
					BuzzerCtrl();										//26us
					break;
					
				case 3:
					SysModelCrl();			//ϵͳ״̬����				//23.6us
					DispCrl();				//��ʾ����					//61.2us
					break;
			
				case 4:
					UartSendDeal();			//���ڷ��ʹ���				//202us
					
					#ifdef DEBUG
					DebugUartDeal();		//���Դ��ڴ���
					#endif
					break;
					
				default:
					break;
			}
		}
	}
}