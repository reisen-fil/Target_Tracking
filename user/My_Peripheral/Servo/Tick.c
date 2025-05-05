#include "Tick.h"

 __IO uint32_t TickCnt;
//等待计时完成
void Tick_Wait(){
    HAL_TIM_Base_Start_IT(&htim6);
    while (TickCnt > 0);
    HAL_TIM_Base_Stop_IT(&htim6);
}
//延时us
void Tick_DelayUs(__IO uint32_t nTime){
    uint32_t nTime_us = nTime / 1000;
    TickCnt = nTime_us;
    Tick_Wait();
}
//延时ms
void Tick_DelayMs(__IO uint32_t nTime){
    TickCnt = nTime;
    Tick_Wait();
}
//设置倒计时（非阻塞式）
void Tick_CountdownBegin(__IO uint32_t nTime){
    TickCnt = nTime;
    HAL_TIM_Base_Start_IT(&htim6);
}
//撤销倒计时
void Tick_CountdownCancel(){
    TickCnt = 0;
    HAL_TIM_Base_Stop_IT(&htim6);
}
//判断倒计时是超时
uint8_t Tick_CountdownIsTimeout(){
    return TickCnt == 0;
}
