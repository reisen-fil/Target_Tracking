#ifndef __MASTER_PC_H
#define __MASTER_PC_H

#include "main.h"
#include "Variable.h"

extern UART_HandleTypeDef huart1;

uint8_t Get_vofa_PID_Rxflag(void);  /* ��ȡ���ݽ��ճɹ���־λ */
uint8_t Get_On_or_off_Rxflag(void);	/* ��ȡ����ֹͣλ�ɹ���־λ */
uint8_t Get_id_Flag_1(void);		/* ��ȡ���ձ�־1 */
uint8_t Get_id_Flag_2(void);		/* ��ȡ���ձ�־2 */

float RxPacket_Data_Handle(uint8_t* Handle_DataBag);		/* ���ݰ����㴦�� */

#endif

