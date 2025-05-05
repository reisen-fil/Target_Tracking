#ifndef __VARIABLE_H
#define __VARIABLE_H

#include "main.h"


/* Vofa */
extern uint8_t Rx_Byte;			/* ���յ��ֽڵĴ�ű��� */
extern uint8_t Data_BitNum;		/* ���յ���ʵ������λ�� */

extern uint8_t id_Flag_1;		/* �������ݰ�ȷ�����ζ���ı�־1 */
extern uint8_t id_Flag_2;		/* �������ݰ�ȷ�����ζ���ı�־2 */
extern uint8_t PID_RxFlag;		/* PID������ȡ��־λ */
extern uint8_t On_or_off_RxFlag;				/* ����ֹͣλ��ȡ��־λ */
extern uint8_t PID_index_1,PID_index_2;

extern uint8_t Get_Vofa_RxData[100];			/* ����PID���������ݰ� */
extern uint8_t Get_On_or_off[100];						/* ��ȡ����λ/ֹͣλ */

extern uint8_t Car_on_off;					/* С������ֹͣλ */


typedef struct
{
		float PID[3];
		uint8_t Car_Enable;
    
}VofaReceive_PID;


//typedef union 
//{
//		uint32_t uintData[3];
//		float floatData[3];		/* float��������Ϊ4���ֽ� */	
//}MyData;			


extern VofaReceive_PID PID_Mode[2];			/* �ٶȻ�/λ�û� */




#endif
