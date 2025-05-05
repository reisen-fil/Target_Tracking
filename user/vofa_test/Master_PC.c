#include "Master_PC.h"

/* ͨ��Vofa+����,����λ��������ͨ�� */

/* �������ݰ����ղ��� */

/*
		�ٶȻ�:Sp_kp  Sp_ki  Sp_kd
		λ�û�:Po_kp  Po_ki  Po_kd
		
		����λ:Start

*/

/*  	1.PID��������
			->
			#P1=12.123��   	#PΪ֡ͷ��P1_x(0<=x<=2)Ϊ�Ǹı�˭�ı�־λ�� =�������ռ���־λ

			12.123�����ݱ���  ����֡β	
		
		
			2.����/ֹͣ����
			->
			@Flag=1/2!			#@Ϊ֡ͷ��FlagΪ��ģʽ�ı�־�ַ����֣�=�Ǳ�־�ռ���־λ
			
			1/2�Ǳ�־λ����  ����֡β
			
			
			3.Flash���汣��PID���ݲ���
			->
			SAVE_DATA			#�����Ѿ����õ�����PID����
*/


/* ��ȡPID���ݽ��ճɹ���־λ */
uint8_t Get_vofa_PID_Rxflag(void)
{
		if(PID_RxFlag==1)		/* �ɹ�ʱ */
		{
				PID_RxFlag=0;		/* ���㴦�� */
				return 1;
		}
		else return 0;
}

/* ��ȡ����ֹͣλ�ɹ���־λ */
uint8_t Get_On_or_off_Rxflag(void)
{
		if(On_or_off_RxFlag==1)
		{
				On_or_off_RxFlag=0;			/* ���㴦�� */
				return 1;
		}
		else return 0;
}


/* ��ȡ���ձ�־1 */
uint8_t Get_id_Flag_1(void)
{
		uint8_t id_temp;
		id_temp=id_Flag_1;
		id_Flag_1=0;			/* ���� */
		return id_temp;
}

/* ��ȡ���ձ�־2 */
uint8_t Get_id_Flag_2(void)
{
		uint8_t id_temp;
		id_temp=id_Flag_2;
		id_Flag_1=0;			/* ���� */
		return id_temp;
}

float Pow_invert(uint8_t X,uint8_t n) /* x����n��10 */
{
  float result=X;
	while(n--)
	{
		result/=10;
	}
	return result;
}

//uint8_t Get_Vofa_RxData[5]={0x31,0x32,0x2E,0x31,0x33};//���Ը����ݰ�ֱ�Ӹ�ֱֵ�ӵ���һ�»�����򣬿��Ƿ����Ϊ12.13
//Data_BitNum = 5//���������ݵĳ���ҲҪ����
//Ȼ��ֱ����������ͷ�  Printf("%f\n",RxPacket_Data_Handle());  Delay_ms(1000);��ok��


/* ���ݰ����㴦�� */
float RxPacket_Data_Handle(uint8_t Handle_DataBag[])
{
  float Data=0.0;
  uint8_t dot_Flag=0;	/* С�����־λ��������С������С����ǰ 0ΪС����ǰ��1ΪС����� */
  uint8_t dot_after_num=1;	/* С�����ĵڼ�λ */
  int8_t minus_Flag=1;				/* ���ű�־λ -1Ϊ�Ǹ��� 1Ϊ���� */
  for(uint8_t i=0;i<Data_BitNum;i++)
  {
    if(Handle_DataBag[i]==0x2D)		/* �����һλΪ���� */
    {
      minus_Flag=-1;
      continue;		/* ��������ѭ�� */ 
    }
    if(dot_Flag==0)
    {
      if(Handle_DataBag[i]==0x2E) /* ���ʶ��С���㣬��dot_Flag��1 */
      {
        dot_Flag=1;
      }
      else  /* ��û����С����ǰ������ */
      {
        Data = Data*10 + Handle_DataBag[i]-48;
      }
    }
    else  /* ����С���������� */
    {
      Data = Data + Pow_invert(Handle_DataBag[i]-48,dot_after_num);
      dot_after_num++;
    }
  }
  return Data*minus_Flag; /* �����������ݷ��س��� ������ϸ��ű�־λ */

}




