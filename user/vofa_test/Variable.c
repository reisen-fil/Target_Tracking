#include "Variable.h"

uint8_t Rx_Byte;			/* ���յ��ֽڵĴ�ű��� */
uint8_t Data_BitNum;		/* ���յ���ʵ������λ�� */

uint8_t id_Flag_1;		/* �������ݰ�ȷ�����ζ���ı�־1 */
uint8_t id_Flag_2;		/* �������ݰ�ȷ�����ζ���ı�־2 */
uint8_t PID_RxFlag;		/* PID������ȡ��־λ */
uint8_t On_or_off_RxFlag;				/* ����ֹͣλ��ȡ��־λ */
uint8_t PID_index_1,PID_index_2;

uint8_t Get_Vofa_RxData[100];			/* ����PID���������ݰ� */
uint8_t Get_On_or_off[100];						/* ��ȡ����λ/ֹͣλ */

uint8_t Car_on_off;					/* С������ֹͣλ */

VofaReceive_PID PID_Mode[2];			/* �ٶȻ�/λ�û� */





