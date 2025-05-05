#include "Master_PC.h"

/* 通过Vofa+调参,用上位机上无线通信 */

/* 参数数据包接收部分 */

/*
		速度环:Sp_kp  Sp_ki  Sp_kd
		位置环:Po_kp  Po_ki  Po_kd
		
		启动位:Start

*/

/*  	1.PID参数部分
			->
			#P1=12.123！   	#P为帧头，P1_x(0<=x<=2)为是改变谁的标志位， =是数据收集标志位

			12.123是数据本身  ！是帧尾	
		
		
			2.启动/停止部分
			->
			@Flag=1/2!			#@为帧头，Flag为该模式的标志字符部分，=是标志收集标志位
			
			1/2是标志位本身  ！是帧尾
			
			
			3.Flash闪存保存PID数据部分
			->
			SAVE_DATA			#保存已经调好的两组PID数据
*/


/* 获取PID数据接收成功标志位 */
uint8_t Get_vofa_PID_Rxflag(void)
{
		if(PID_RxFlag==1)		/* 成功时 */
		{
				PID_RxFlag=0;		/* 清零处理 */
				return 1;
		}
		else return 0;
}

/* 获取启动停止位成功标志位 */
uint8_t Get_On_or_off_Rxflag(void)
{
		if(On_or_off_RxFlag==1)
		{
				On_or_off_RxFlag=0;			/* 清零处理 */
				return 1;
		}
		else return 0;
}


/* 获取接收标志1 */
uint8_t Get_id_Flag_1(void)
{
		uint8_t id_temp;
		id_temp=id_Flag_1;
		id_Flag_1=0;			/* 清零 */
		return id_temp;
}

/* 获取接收标志2 */
uint8_t Get_id_Flag_2(void)
{
		uint8_t id_temp;
		id_temp=id_Flag_2;
		id_Flag_1=0;			/* 清零 */
		return id_temp;
}

float Pow_invert(uint8_t X,uint8_t n) /* x除以n次10 */
{
  float result=X;
	while(n--)
	{
		result/=10;
	}
	return result;
}

//uint8_t Get_Vofa_RxData[5]={0x31,0x32,0x2E,0x31,0x33};//可以给数据包直接赋值直接调用一下换算程序，看是否输出为12.13
//Data_BitNum = 5//别忘记数据的长度也要设置
//然后直接在主程序就放  Printf("%f\n",RxPacket_Data_Handle());  Delay_ms(1000);就ok了


/* 数据包换算处理 */
float RxPacket_Data_Handle(uint8_t Handle_DataBag[])
{
  float Data=0.0;
  uint8_t dot_Flag=0;	/* 小数点标志位，能区分小数点后或小数点前 0为小数点前，1为小数点后 */
  uint8_t dot_after_num=1;	/* 小数点后的第几位 */
  int8_t minus_Flag=1;				/* 负号标志位 -1为是负号 1为正号 */
  for(uint8_t i=0;i<Data_BitNum;i++)
  {
    if(Handle_DataBag[i]==0x2D)		/* 如果第一位为负号 */
    {
      minus_Flag=-1;
      continue;		/* 跳过本次循环 */ 
    }
    if(dot_Flag==0)
    {
      if(Handle_DataBag[i]==0x2E) /* 如果识别到小数点，则将dot_Flag置1 */
      {
        dot_Flag=1;
      }
      else  /* 还没遇到小数点前的运算 */
      {
        Data = Data*10 + Handle_DataBag[i]-48;
      }
    }
    else  /* 遇到小数点后的运算 */
    {
      Data = Data + Pow_invert(Handle_DataBag[i]-48,dot_after_num);
      dot_after_num++;
    }
  }
  return Data*minus_Flag; /* 将换算后的数据返回出来 这里乘上负号标志位 */

}




