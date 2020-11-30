#ifndef _KEYTOUCH_H
#define _KEYTOUCH_H

#define VALID_TOUCHKEY_NUMBER       7                              //������������

#define VALID_THRESHOLD_MAX			30000							//��󰴼���ֵ���޷��˲�
#define ELIM_TREMBLING_CNT			35								//����ʱ��

#define KEY_NULL					0x0000							//�ް���
#define KEY_TEMP_ADD				0x0001							//�����¶ȼ�
#define KEY_TEMP_SUB				0x0002							//�����¶ȼ�
#define KEY_TIME_ADD				0x0004							//����ʱ���
#define KEY_TIME_SUB				0x0008							//����ʱ���
#define KEY_ORDER   				0x0010							//����ԤԼ
#define KEY_CHANGE_FUN				0x0020							//���������л�
#define KEY_ON_OFF					0x0040							//��������ȡ��

#define KEY_PCB_CHECK				0x0005							//PCB�Լ찴�����¶�+��+��ʱ��+��
#define KEY_MACHINE_CHECK			0x0030							//�����Լ찴����ԤԼ��+�����ܡ�
#define KEY_DISP_PID				0x000A							//PID ������ʱ��-��+���¶�-��
#define KEY_CLEAN_INCRUSTANT		0x000C							//��ˮ��������ʱ��+��+��ʱ��-��

extern unsigned int	    			g_scanDataTk[VALID_TOUCHKEY_NUMBER];//����������ɨ��ֵ

extern bit							g_keyScanEndFlag;				//����ɨ����ɱ�־
extern bit							g_adjPressFlag;					//�Ӽ������±�־

void InitTouchKeyIo(void);
void InitTouchKeyVariable(void);
void InitTouchKey(void);
void TouchKeyAdScan(void);
void TouchKeyScan(void);
void TouchKeyDeal(void);
void TouchKeyJudge(void);
void TouchKeyDealSubroutine(void);

#endif