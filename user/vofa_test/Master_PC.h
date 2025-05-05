#ifndef __MASTER_PC_H
#define __MASTER_PC_H

#include "main.h"
#include "Variable.h"

extern UART_HandleTypeDef huart1;

uint8_t Get_vofa_PID_Rxflag(void);  /* 获取数据接收成功标志位 */
uint8_t Get_On_or_off_Rxflag(void);	/* 获取启动停止位成功标志位 */
uint8_t Get_id_Flag_1(void);		/* 获取接收标志1 */
uint8_t Get_id_Flag_2(void);		/* 获取接收标志2 */

float RxPacket_Data_Handle(uint8_t* Handle_DataBag);		/* 数据包换算处理 */

#endif

