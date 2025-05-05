
	
#ifndef __tick_H
#define __tick_H

#include "main.h"
#include "tim.h"

	
extern __IO uint32_t TickCnt;

void Tick_Wait();
void Tick_DelayUs(__IO uint32_t nTime);
void Tick_DelayMs(__IO uint32_t nTime);
void Tick_CountdownBegin(__IO uint32_t nTime);
void Tick_CountdownCancel();
uint8_t Tick_CountdownIsTimeout();



#endif




