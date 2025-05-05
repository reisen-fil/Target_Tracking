#include "Variable.h"

uint8_t Rx_Byte;			/* 接收到字节的存放变量 */
uint8_t Data_BitNum;		/* 接收到的实际数据位数 */

uint8_t id_Flag_1;		/* 参数数据包确定传参对象的标志1 */
uint8_t id_Flag_2;		/* 参数数据包确定传参对象的标志2 */
uint8_t PID_RxFlag;		/* PID参数获取标志位 */
uint8_t On_or_off_RxFlag;				/* 启动停止位获取标志位 */
uint8_t PID_index_1,PID_index_2;

uint8_t Get_Vofa_RxData[100];			/* 接收PID参数的数据包 */
uint8_t Get_On_or_off[100];						/* 获取启动位/停止位 */

uint8_t Car_on_off;					/* 小车启动停止位 */

VofaReceive_PID PID_Mode[2];			/* 速度环/位置环 */





