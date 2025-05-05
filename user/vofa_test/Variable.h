#ifndef __VARIABLE_H
#define __VARIABLE_H

#include "main.h"


/* Vofa */
extern uint8_t Rx_Byte;			/* 接收到字节的存放变量 */
extern uint8_t Data_BitNum;		/* 接收到的实际数据位数 */

extern uint8_t id_Flag_1;		/* 参数数据包确定传参对象的标志1 */
extern uint8_t id_Flag_2;		/* 参数数据包确定传参对象的标志2 */
extern uint8_t PID_RxFlag;		/* PID参数获取标志位 */
extern uint8_t On_or_off_RxFlag;				/* 启动停止位获取标志位 */
extern uint8_t PID_index_1,PID_index_2;

extern uint8_t Get_Vofa_RxData[100];			/* 接收PID参数的数据包 */
extern uint8_t Get_On_or_off[100];						/* 获取启动位/停止位 */

extern uint8_t Car_on_off;					/* 小车启动停止位 */


typedef struct
{
		float PID[3];
		uint8_t Car_Enable;
    
}VofaReceive_PID;


//typedef union 
//{
//		uint32_t uintData[3];
//		float floatData[3];		/* float类型数据为4个字节 */	
//}MyData;			


extern VofaReceive_PID PID_Mode[2];			/* 速度环/位置环 */




#endif
