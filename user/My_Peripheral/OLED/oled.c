/*
 * oled.c
 *
 *  Created on: 2022��7��24��
 *      Author: Unicorn_Li
 */
#include "oled.h"
 
/**********************************************************
 * ��ʼ������,����оƬ�ֲ���д����ϸ�������ͼ�Լ�ע������
 ***********************************************************/
uint8_t CMD_Data[]={
0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,0xA1, 0xC8, 0xDA,
 
0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,0x8D, 0x14,
 
0xAF};
 



/**
 * @function: void OLED_Init(void)
 * @description: OLED��ʼ��
 * @return {*}
 */
void OLED_Init(void)
{
	HAL_Delay(200);
 
	uint8_t i = 0;
	for(i=0; i<23; i++)
	{
		OLED_WR_CMD(CMD_Data[i]);
	}
	
}
 
/**
 * @function: void OLED_WR_CMD(uint8_t cmd)
 * @description: ���豸д��������
 * @param {uint8_t} cmd оƬ�ֲ�涨������
 * @return {*}
 */
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}
 
/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: ���豸д��������
 * @param {uint8_t} data ����
 * @return {*}
 */
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}
 
/**
 * @function: void OLED_On(void)
 * @description: ������ʾ
 * @return {*}
 */
void OLED_On(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WR_CMD(0x00);      //������ʾλ�á��е͵�ַ
		OLED_WR_CMD(0x10);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
			OLED_WR_DATA(1);
	}
}
 
 
/**
 * @function: OLED_Clear(void)
 * @description: ����,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @return {*}
 */
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WR_CMD(0x00);      //������ʾλ�á��е͵�ַ
		OLED_WR_CMD(0x10);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	}
}
 
/**
 * @function: void OLED_Display_On(void)
 * @description: ����OLED��ʾ
 * @return {*}
 */
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC����
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON,����ʾ
}
 
 
/**
 * @function: void OLED_Display_Off(void)
 * @description: �ر�OLED��ʾ
 * @return {*}
 */
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC����
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF���ر���ʾ
}
 
/**
 * @function: void OLED_Set_Pos(uint8_t x, uint8_t y)
 * @description: ��������
 * @param {uint8_t} x,y
 * @return {*}
 */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_CMD(0xb0+y);	//����ҳ��ַ��0~7��
	OLED_WR_CMD(((x&0xf0)>>4)|0x10); //������ʾλ�á��иߵ�ַ
	OLED_WR_CMD(x&0x0f);	//������ʾλ�á��е͵�ַ
}
 
 
/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n����
 * @param {uint8_t} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}
 
/**
 * @function: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size,uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾһ���ַ�
 * @param {uint8_t} x�ַ���ʼ��ʾ�ĺ�����
 * @param {uint8_t} y�ַ���ʼ��ʾ��������
 * @param {uint8_t} chr����ʾ���ַ�
 * @param {uint8_t} Char_Size����ʾ�ַ��������С,ѡ������ 16/12
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t Color_Turn)
{
	unsigned char c=0,i=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				{
				  if(Color_Turn)
					  OLED_WR_DATA(~F8X16[c*16+i]);
				  else
					  OLED_WR_DATA(F8X16[c*16+i]);
				}
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~F8X16[c*16+i+8]);
				  else
					  OLED_WR_DATA(F8X16[c*16+i+8]);
			    }
 
			}
	     else
	     {
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~F6x8[c][i]);
				  else
					  OLED_WR_DATA(F6x8[c][i]);
			    }
		  }
}
 
/**
 * @function: void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_tChar_Size, uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾ�ַ���
 * @param {uint8_t} x����ʾ�ַ����Ŀ�ʼ������x:0~127
 * @param {uint8_t} y����ʾ�ַ����Ŀ�ʼ������ y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {uint8_t} *chr����ʾ���ַ���
 * @param {uint8_t} Char_Size����ʾ�ַ����������С,ѡ������ 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowString(uint8_t x,uint8_t y,char*chr,uint8_t Char_Size, uint8_t Color_Turn)
{
	uint8_t  j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size, Color_Turn);
			if (Char_Size == 12) //6X8�������м�6����ʾ��һ���ַ�
				x += 6;
			else  //8X16�������м�8����ʾ��һ���ַ�
				x += 8;
 
			if (x > 122 && Char_Size==12) //TextSize6x8���һ�в�����ʾ�ˣ�����һ�м�����ʾ
			{
				x = 0;
				y++;
			}
			if (x > 120 && Char_Size== 16) //TextSize8x16���һ�в�����ʾ�ˣ�����һ�м�����ʾ
			{
				x = 0;
				y++;
			}
			j++;
	}
}
 
/**
 * @function: void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, Color_Turn)
 * @description: ��ʾ����
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������, y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {unsigned int} num:���������
 * @param {uint8_t } len:���������λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}else enshow=1;
 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
}
 
 
/**
 * @function: void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
 * @description: ��ʾ����������
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������, y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {float} num:����ĸ���������
 * @param {uint8_t } z_ len:�������ֵ�λ��
 * @param {uint8_t } f_len: С�����ֵ�λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp,i=0;//iΪ������־λ
	uint8_t enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//��������
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
	//С����
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2, Color_Turn);
 
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //С������
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2, Color_Turn);
	}
	if(i==1)//���Ϊ�����ͽ���ǰ��һλ��ֵ��-��
	{
		OLED_ShowChar(x,y,'-',size2, Color_Turn);
		i=0;
	}
}
 
 
 
/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: ��OLED�ض�λ�ÿ�ʼ��ʾ16X16����
 * @param {uint8_t} x����ʾ�ĺ�����ʼ������x: 0~112�����к���֮����Ҫ���16
 * @param {uint8_t} y����ʾ�ĺ�����ʼ������ y: 0~6 , ���к���֮����Ҫ���2
 * @param {uint8_t} no����ʾ�ĺ��ֱ��
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no][t]); //��ʾ���ֵ��ϰ벿��
				else
					OLED_WR_DATA(Hzk[2*no][t]); //��ʾ���ֵ��ϰ벿��
        }
 
		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no+1][t]); //��ʾ���ֵ��ϰ벿��
				else
					OLED_WR_DATA(Hzk[2*no+1][t]);//��ʾ���ֵ��ϰ벿��
 
         }
}
 
/**
 * @function: void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: ��OLED�ض�������ʾBMPͼƬ
 * @param {uint8_t} x0ͼ��ʼ��ʾ������  x0:0~127
 * @param {uint8_t} y0ͼ��ʼ��ʾ������  y0:0~7
 * @param {uint8_t} x1ͼ�������ʾ������  x1:1~128
 * @param {uint8_t} y1ͼ�������ʾ������  y1:1~8
 * @param {uint8_t} *BMP����ʾ��ͼ������
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
{
   uint32_t j = 0;
   uint8_t x = 0, y = 0;
 
  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
		{
			if (Color_Turn)
				OLED_WR_DATA(~BMP[j++]);//��ʾ����ͼƬ
			else
				OLED_WR_DATA(BMP[j++]);//��ʾͼƬ
 
		}
	}
}
 
 
/**
 * @function: void OLED_HorizontalShift(uint8_t direction)
 * @description: ��Ļ����ˮƽȫ����������
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(uint8_t direction)
 
{
	OLED_WR_CMD(0x2e);//ֹͣ����
	OLED_WR_CMD(direction);//���ù�������
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0x00);//���ÿ�ʼҳ��ַ
	OLED_WR_CMD(0x07);//����ÿ����������֮���ʱ������֡Ƶ
    //  0x00-5֡�� 0x01-64֡�� 0x02-128֡�� 0x03-256֡�� 0x04-3֡�� 0x05-4֡�� 0x06-25֡�� 0x07-2֡��
	OLED_WR_CMD(0x07);//���ý���ҳ��ַ
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	OLED_WR_CMD(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}
 
/**
 * @function: void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
 * @description: ��Ļ��������ˮƽ��������
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {uint8_t} start ��ʼҳ��ַ  0x00~0x07
 * @param {uint8_t} end  ����ҳ��ַ  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
{
	OLED_WR_CMD(0x2e);//ֹͣ����
	OLED_WR_CMD(direction);//���ù�������
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(start);//���ÿ�ʼҳ��ַ
	OLED_WR_CMD(0x07);//����ÿ����������֮���ʱ������֡Ƶ,0x07�������ٶ�2֡
	OLED_WR_CMD(end);//���ý���ҳ��ַ
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	OLED_WR_CMD(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
 
}
 
/**
 * @function: void OLED_VerticalAndHorizontalShift(uint8_t direction)
 * @description: ��Ļ���ݴ�ֱˮƽȫ����������
 * @param {uint8_t} direction				���Ϲ���	 0x29
 *                                                            ���Ϲ���   0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	OLED_WR_CMD(0x2e);//ֹͣ����
	OLED_WR_CMD(direction);//���ù�������
	OLED_WR_CMD(0x01);//�����ֽ�����
	OLED_WR_CMD(0x00);//���ÿ�ʼҳ��ַ
	OLED_WR_CMD(0x07);//����ÿ����������֮���ʱ������֡Ƶ���������ٶ�
	OLED_WR_CMD(0x07);//���ý���ҳ��ַ
	OLED_WR_CMD(0x01);//��ֱ����ƫ����
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	OLED_WR_CMD(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}
 
/**
 * @function: void OLED_DisplayMode(uint8_t mode)
 * @description: ��Ļ����ȡ����ʾ
 * @param {uint8_t} direction			ON	0xA7  ��
 *                                                          OFF	0xA6	Ĭ�ϴ�ģʽ���������ص���
 * @return {*}
 */
void OLED_DisplayMode(uint8_t mode)
{
	OLED_WR_CMD(mode);
}
 
/**
 * @function: void OLED_IntensityControl(uint8_t intensity)
 * @description: ��Ļ���ȵ���
 * @param  {uint8_t} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(uint8_t intensity)
{
	OLED_WR_CMD(0x81);
	OLED_WR_CMD(intensity);
}


/*************************************************************************/

/* OLED�༶�˵����� */

int menu2,menu3,menu4;

/* һ���˵���ʾ */

int menu1(void)
{
		/* һ���˵���ʼ��ʾ */
		int menu1_flag=1;				/* ѡ���־ */
	
		OLED_ShowString(16,0,"LOCATE_MODE",16,0);	
		OLED_ShowString(16,2,"LASER_RESET",16,0);
		OLED_ShowString(16,4,"LASER_MOVE",16,0);
		
		while(1)
		{
				Key_Scan(&KeyNum);			/* ��ȡ����ֵ */
				
				if(KeyNum == 1)		/* *�����ƶ� */
				{
						menu1_flag++;
						if(menu1_flag>=4) menu1_flag=1;
				}
				if(KeyNum == 2)		/* *�����ƶ� */
				{
						menu1_flag--;
						if(menu1_flag<=0) menu1_flag=1;		
				}
				if(KeyNum == 3)		/* ѡ��ȷ��-->��������˵� */
				{
						OLED_Clear();			/* ���һ���˵�����(Ϊ��������˵���׼��) */
						return(menu1_flag);			/* ���ݷ���ֵ����ѡ�� */
				}
				
				switch(menu1_flag)			/* ���ݸ����ı�־λ����*���ƶ� */
				{
						case 1:
						{
								OLED_ShowChar(0,0,'*',16,0);
								OLED_ShowChar(0,2,' ',16,0);
								OLED_ShowChar(0,4,' ',16,0);
								break;
						}
						
						case 2:
						{
								OLED_ShowChar(0,0,' ',16,0);
								OLED_ShowChar(0,2,'*',16,0);
								OLED_ShowChar(0,4,' ',16,0);	
								break;
						}
						case 3:
						{
								OLED_ShowChar(0,0,' ',16,0);
								OLED_ShowChar(0,2,' ',16,0);
								OLED_ShowChar(0,4,'*',16,0);
								break;
						}					
				}
		}
}

/* �����˵����� */

Show_pot point_1;
Show_pot point_2;
Show_pot point_3;
Show_pot point_4;


uint8_t GetOriginData_Flag;

/* ���붨λģʽ */

void LOCATE_MODE_Menu(void)
{
		while(1)
		{
				Penstring_Locate();		/* ȡ���ĵ��ӦXY�Ƕ� */			

				/* ������ʾ */
				memset(point_1.XY_Pos,'\0',9);
				sprintf(point_1.XY_Pos,"(%4d,%4d)",Locate_angleX[0],Locate_angleY[0]);
				OLED_ShowString(0,0,point_1.XY_Pos,16, 0);			

				memset(point_2.XY_Pos,'\0',9);
				sprintf(point_2.XY_Pos,"(%4d,%4d)",Locate_angleX[1],Locate_angleY[1]);
				OLED_ShowString(0,2,point_2.XY_Pos,16, 0);

				memset(point_3.XY_Pos,'\0',9);
				sprintf(point_3.XY_Pos,"(%4d,%4d)",Locate_angleX[2],Locate_angleY[2]);
				OLED_ShowString(0,4,point_3.XY_Pos,16, 0);

				memset(point_4.XY_Pos,'\0',9);
				sprintf(point_4.XY_Pos,"(%4d,%4d)",Locate_angleX[3],Locate_angleY[3]);
				OLED_ShowString(0,6,point_4.XY_Pos,16, 0);			
			
			
				Key_Scan(&KeyNum);			/* ��ȡ����ֵ */	
			
				if(KeyNum == 4)			/* �����ϼ��˵����� */
				{
						OLED_Clear();
						break;					/* �ص���ѭ�� */
				}
				
//				else if(KeyNum == 1)			/* ��ȡ�Զ�ԭʼ��������ֵ */
//				{
//						Servo_OriginAngle[0] = USL_GETPositionVal(servoUsart,sid_1);			/* �¶��X�� */
//						Servo_OriginAngle[1] = USL_GETPositionVal(servoUsart,sid_2);			/* �϶��Y�� */
//					
//						Data_StartGet=1;
//						GetOriginData_Flag=1;		/* ��ԭʼ���ݻ�ȡ��־λ */				
//				}
				
				else if(KeyNum == 9)		/* �¶��+ */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_1);		/* �¶����ת�ǶȻ�ȡ */
						Now_Angle=Now_Angle+17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_1,Now_Angle,0);
				}
				else if(KeyNum == 10)		/* �¶��- */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_1);		/* �¶����ת�ǶȻ�ȡ */
						Now_Angle=Now_Angle-17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_1,Now_Angle,0);
				}
				else if(KeyNum == 11)		/* �϶��+ */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_2);		/* �϶����ת�ǶȻ�ȡ */
						Now_Angle=Now_Angle+17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_2,Now_Angle,0);
				}
				else if(KeyNum == 12)		/* �϶��- */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_2);		/* �϶����ת�ǶȻ�ȡ */
						Now_Angle=Now_Angle-17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_2,Now_Angle,0);
				}

//				if(Data_StartGet)
//				{
//						Servo_SetZeroAngle(&Servo_OriginXY[0],&Servo_OriginXY[1]);
//				}				
				
		}
		
}

uint16_t My_ResetXY[2];


void LASER_RESET(void)
{
		My_ResetXY[0] = 0.5*(Locate_angleX[0]+Locate_angleX[3]);
		My_ResetXY[1] = 0.5*(Locate_angleY[0]+Locate_angleY[1]);
	
		USL_SetServoAngle(servoUsart,sid_1,My_ResetXY[0],0);
		USL_SetServoAngle(servoUsart,sid_2,My_ResetXY[1],0);		
}

void LASER_RESET_Start(void)
{
		if(Monitor_Flag)			/* �жϸ�λ���ܱ�־λ�Ƿ�ʹ�� */
		{
				while(1)
				{
						static uint8_t Restart_Flag;
						Key_Scan(&KeyNum);			/* ��ȡ����ֵ */	
						if(KeyNum == 13)
						{
								MovingReset_flag = ~MovingReset_flag;
								if(!MovingReset_flag) 
								{
										Restart_Flag=0;
										break;
								}
								else 
								{
										LASER_RESET();
										Restart_Flag=1;
								}
						}
						else 
						{
								if(!Restart_Flag) break;
						}
				}
		}
		else LASER_RESET();
}
 

/* ���뼤�⸴λģʽ */
void LASER_RESET_Menu(void)
{
		char Reset_Point[10];
	
		LASER_RESET_Start();
	
		while(1)
		{
				OLED_ShowString(0,0,"Reset_Point:",16,0);
			
				memset(Reset_Point,'\0',10);
				sprintf(Reset_Point,"(%4d,%4d)",My_ResetXY[0],My_ResetXY[1]);
				OLED_ShowString(0,2,Reset_Point,16, 0);

				Key_Scan(&KeyNum);			/* ��ȡ����ֵ */				
			
				if(KeyNum == 4)		/* �����ϼ��˵� */
				{
						OLED_Clear();
						break;
				}
		}	
		
}

/* �����˶��˵�ѡ�� */
int LASER_MOVE_Menu(void)
{
		int menu2_flag=1;			/* ģʽѡ���־ */
	
		OLED_ShowString(16,0,"PENSTRING_MOVE",16,0);
		OLED_ShowString(16,2,"RECTANGLE_MOVE",16,0);

		while(1)
		{
			Key_Scan(&KeyNum);			/* ��ȡ����ֵ */	
		
			if(KeyNum == 1)			/* *���� */
			{
					menu2_flag++;
					if(menu2_flag>=3) menu2_flag=1;
			}
			
			if(KeyNum == 3)			/* ѡ��ȷ�� */
			{
					OLED_Clear();
					return(menu2_flag);
			}
			
			if(KeyNum == 4)			/* ������һ���˵� */
			{
					OLED_Clear();
					break;
			}
			
			switch(menu2_flag)
			{
				case 1:
				{
						OLED_ShowChar(0,0,'*',16,0);
						OLED_ShowChar(0,2,' ',16,0);	
						break;
				}
				case 2:
				{
						OLED_ShowChar(0,0,' ',16,0);
						OLED_ShowChar(0,2,'*',16,0);	
						break;						
				}
			}
		}
}



/* �����˵����� */

/* ���ο��˶����Ʋ˵� */
int RECTANGLE_MOVE_Menu(void)			
{
		int menu3_flag=1;			/* ģʽѡ���־ */ 

		OLED_ShowString(16,0,"SET_RECTANGLE",16,0);
		OLED_ShowString(16,2,"START_MOVE",16,0);

		while(1)
		{
			Key_Scan(&KeyNum);			/* ��ȡ����ֵ */	
		
			if(KeyNum == 1)			/* *���� */
			{
					menu3_flag++;
					if(menu3_flag>=3) menu3_flag=1;
			}
			
			if(KeyNum == 3)			/* ѡ��ȷ�� */
			{
					OLED_Clear();
					return(menu3_flag);
			}
			
			if(KeyNum == 4)			/* ������һ���˵� */
			{
					OLED_Clear();
					break;
			}			
			
			switch(menu3_flag)
			{
				case 1:
				{
						OLED_ShowChar(0,0,'*',16,0);
						OLED_ShowChar(0,2,' ',16,0);	
						break;
				}
				case 2:
				{
						OLED_ShowChar(0,0,' ',16,0);
						OLED_ShowChar(0,2,'*',16,0);	
						break;						
				}
			}
		}	
		
}

/* �ļ��˵����� */

void GET_RECTANGLE_menu(void)
{
	
		char Point1_XY[10],Point2_XY[10],Point3_XY[10],Point4_XY[10];	
		
		Data_StartGet=1;
		
		while(1)
		{
				Get_RectangleXY(rectangle_XY);			/* ��ȡ�ĵ�ԭʼ���� */
					
				if(Data_FinishGet == 0 && Data_StartGet == 0)		/* ������ɺ� */
				{
						HAL_UART_Transmit(&huart3,(uint8_t*)&Rectangle_Send,sizeof(Rectangle_Send),HAL_MAX_DELAY);		/* ���ͽ�����ɱ�־���� */
					
						memset(Point1_XY,'\0',10);
						sprintf(Point1_XY,"(%4d,%4d)",rectangle_XY[0],rectangle_XY[1]);
						OLED_ShowString(0,0,Point1_XY,16,0);	
					
						memset(Point2_XY,'\0',10);
						sprintf(Point2_XY,"(%4d,%4d)",rectangle_XY[2],rectangle_XY[3]);
						OLED_ShowString(0,2,Point2_XY,16,0);
		
						memset(Point3_XY,'\0',10);
						sprintf(Point3_XY,"(%4d,%4d)",rectangle_XY[4],rectangle_XY[5]);
						OLED_ShowString(0,4,Point3_XY,16,0);
		
						memset(Point4_XY,'\0',10);
						sprintf(Point4_XY,"(%4d,%4d)",rectangle_XY[6],rectangle_XY[7]);
						OLED_ShowString(0,6,Point4_XY,16,0);			
		
						Key_Scan(&KeyNum);			/* ��ȡ����ֵ */
					
						if(KeyNum == 4)
						{
								OLED_Clear();
								break;
						}
				}
			
		}
}

//void Rectangle_Reladistan_menu(void)
//{
//		OLED_ShowString(0,0,"XY_Distance:",16,0);
//	
//		char RECTANGLE_XY[10];
//	
//		while(1)
//		{
//				memset(RECTANGLE_XY,'\0',10);
//				sprintf(RECTANGLE_XY,"(%2.3f,%2.3f)",DistanCoordi_Rate[0],DistanCoordi_Rate[1]);
//				OLED_ShowString(16,2,RECTANGLE_XY,16,0);

//				Key_Scan(&KeyNum);			/* ��ȡ����ֵ */
//			
//				if(KeyNum == 4)
//				{
//						OLED_Clear();
//						break;
//				}
//			
//		}
//		
//}

