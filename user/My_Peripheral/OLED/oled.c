/*
 * oled.c
 *
 *  Created on: 2022年7月24日
 *      Author: Unicorn_Li
 */
#include "oled.h"
 
/**********************************************************
 * 初始化命令,根据芯片手册书写，详细步骤见上图以及注意事项
 ***********************************************************/
uint8_t CMD_Data[]={
0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,0xA1, 0xC8, 0xDA,
 
0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,0x8D, 0x14,
 
0xAF};
 



/**
 * @function: void OLED_Init(void)
 * @description: OLED初始化
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
 * @description: 向设备写控制命令
 * @param {uint8_t} cmd 芯片手册规定的命令
 * @return {*}
 */
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}
 
/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: 向设备写控制数据
 * @param {uint8_t} data 数据
 * @return {*}
 */
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}
 
/**
 * @function: void OLED_On(void)
 * @description: 更新显示
 * @return {*}
 */
void OLED_On(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(1);
	}
}
 
 
/**
 * @function: OLED_Clear(void)
 * @description: 清屏,整个屏幕是黑色的!和没点亮一样!!!
 * @return {*}
 */
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	}
}
 
/**
 * @function: void OLED_Display_On(void)
 * @description: 开启OLED显示
 * @return {*}
 */
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON,打开显示
}
 
 
/**
 * @function: void OLED_Display_Off(void)
 * @description: 关闭OLED显示
 * @return {*}
 */
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF，关闭显示
}
 
/**
 * @function: void OLED_Set_Pos(uint8_t x, uint8_t y)
 * @description: 坐标设置
 * @param {uint8_t} x,y
 * @return {*}
 */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_CMD(0xb0+y);	//设置页地址（0~7）
	OLED_WR_CMD(((x&0xf0)>>4)|0x10); //设置显示位置—列高地址
	OLED_WR_CMD(x&0x0f);	//设置显示位置—列低地址
}
 
 
/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n函数
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
 * @description: 在OLED12864特定位置开始显示一个字符
 * @param {uint8_t} x字符开始显示的横坐标
 * @param {uint8_t} y字符开始显示的纵坐标
 * @param {uint8_t} chr待显示的字符
 * @param {uint8_t} Char_Size待显示字符的字体大小,选择字体 16/12
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t Color_Turn)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
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
 * @description: 在OLED12864特定位置开始显示字符串
 * @param {uint8_t} x待显示字符串的开始横坐标x:0~127
 * @param {uint8_t} y待显示字符串的开始纵坐标 y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {uint8_t} *chr待显示的字符串
 * @param {uint8_t} Char_Size待显示字符串的字体大小,选择字体 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowString(uint8_t x,uint8_t y,char*chr,uint8_t Char_Size, uint8_t Color_Turn)
{
	uint8_t  j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size, Color_Turn);
			if (Char_Size == 12) //6X8的字体列加6，显示下一个字符
				x += 6;
			else  //8X16的字体列加8，显示下一个字符
				x += 8;
 
			if (x > 122 && Char_Size==12) //TextSize6x8如果一行不够显示了，从下一行继续显示
			{
				x = 0;
				y++;
			}
			if (x > 120 && Char_Size== 16) //TextSize8x16如果一行不够显示了，从下一行继续显示
			{
				x = 0;
				y++;
			}
			j++;
	}
}
 
/**
 * @function: void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, Color_Turn)
 * @description: 显示数字
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {unsigned int} num:输入的数据
 * @param {uint8_t } len:输入的数据位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
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
 * @description: 显示正负浮点数
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {float} num:输入的浮点型数据
 * @param {uint8_t } z_ len:整数部分的位数
 * @param {uint8_t } f_len: 小数部分的位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp,i=0;//i为负数标志位
	uint8_t enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//整数部分
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
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2, Color_Turn);
 
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2, Color_Turn);
	}
	if(i==1)//如果为负，就将最前的一位赋值‘-’
	{
		OLED_ShowChar(x,y,'-',size2, Color_Turn);
		i=0;
	}
}
 
 
 
/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: 在OLED特定位置开始显示16X16汉字
 * @param {uint8_t} x待显示的汉字起始横坐标x: 0~112，两列汉字之间需要间隔16
 * @param {uint8_t} y待显示的汉字起始纵坐标 y: 0~6 , 两行汉字之间需要间隔2
 * @param {uint8_t} no待显示的汉字编号
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no][t]); //显示汉字的上半部分
				else
					OLED_WR_DATA(Hzk[2*no][t]); //显示汉字的上半部分
        }
 
		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no+1][t]); //显示汉字的上半部分
				else
					OLED_WR_DATA(Hzk[2*no+1][t]);//显示汉字的上半部分
 
         }
}
 
/**
 * @function: void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域显示BMP图片
 * @param {uint8_t} x0图像开始显示横坐标  x0:0~127
 * @param {uint8_t} y0图像开始显示纵坐标  y0:0~7
 * @param {uint8_t} x1图像结束显示横坐标  x1:1~128
 * @param {uint8_t} y1图像结束显示纵坐标  y1:1~8
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
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
				OLED_WR_DATA(~BMP[j++]);//显示反相图片
			else
				OLED_WR_DATA(BMP[j++]);//显示图片
 
		}
	}
}
 
 
/**
 * @function: void OLED_HorizontalShift(uint8_t direction)
 * @description: 屏幕内容水平全屏滚动播放
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(uint8_t direction)
 
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0x00);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频
    //  0x00-5帧， 0x01-64帧， 0x02-128帧， 0x03-256帧， 0x04-3帧， 0x05-4帧， 0x06-25帧， 0x07-2帧，
	OLED_WR_CMD(0x07);//设置结束页地址
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}
 
/**
 * @function: void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
 * @description: 屏幕部分内容水平滚动播放
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {uint8_t} start 开始页地址  0x00~0x07
 * @param {uint8_t} end  结束页地址  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(start);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频,0x07即滚动速度2帧
	OLED_WR_CMD(end);//设置结束页地址
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
 
}
 
/**
 * @function: void OLED_VerticalAndHorizontalShift(uint8_t direction)
 * @description: 屏幕内容垂直水平全屏滚动播放
 * @param {uint8_t} direction				右上滚动	 0x29
 *                                                            左上滚动   0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x01);//虚拟字节设置
	OLED_WR_CMD(0x00);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频，即滚动速度
	OLED_WR_CMD(0x07);//设置结束页地址
	OLED_WR_CMD(0x01);//垂直滚动偏移量
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}
 
/**
 * @function: void OLED_DisplayMode(uint8_t mode)
 * @description: 屏幕内容取反显示
 * @param {uint8_t} direction			ON	0xA7  ，
 *                                                          OFF	0xA6	默认此模式，设置像素点亮
 * @return {*}
 */
void OLED_DisplayMode(uint8_t mode)
{
	OLED_WR_CMD(mode);
}
 
/**
 * @function: void OLED_IntensityControl(uint8_t intensity)
 * @description: 屏幕亮度调节
 * @param  {uint8_t} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(uint8_t intensity)
{
	OLED_WR_CMD(0x81);
	OLED_WR_CMD(intensity);
}


/*************************************************************************/

/* OLED多级菜单部分 */

int menu2,menu3,menu4;

/* 一级菜单显示 */

int menu1(void)
{
		/* 一级菜单初始显示 */
		int menu1_flag=1;				/* 选择标志 */
	
		OLED_ShowString(16,0,"LOCATE_MODE",16,0);	
		OLED_ShowString(16,2,"LASER_RESET",16,0);
		OLED_ShowString(16,4,"LASER_MOVE",16,0);
		
		while(1)
		{
				Key_Scan(&KeyNum);			/* 获取键码值 */
				
				if(KeyNum == 1)		/* *向下移动 */
				{
						menu1_flag++;
						if(menu1_flag>=4) menu1_flag=1;
				}
				if(KeyNum == 2)		/* *向上移动 */
				{
						menu1_flag--;
						if(menu1_flag<=0) menu1_flag=1;		
				}
				if(KeyNum == 3)		/* 选择确认-->进入二级菜单 */
				{
						OLED_Clear();			/* 清除一级菜单内容(为进入二级菜单做准备) */
						return(menu1_flag);			/* 根据返回值进行选择 */
				}
				
				switch(menu1_flag)			/* 根据给到的标志位进行*的移动 */
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

/* 二级菜单部分 */

Show_pot point_1;
Show_pot point_2;
Show_pot point_3;
Show_pot point_4;


uint8_t GetOriginData_Flag;

/* 进入定位模式 */

void LOCATE_MODE_Menu(void)
{
		while(1)
		{
				Penstring_Locate();		/* 取出四点对应XY角度 */			

				/* 定点显示 */
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
			
			
				Key_Scan(&KeyNum);			/* 获取键码值 */	
			
				if(KeyNum == 4)			/* 返回上级菜单按键 */
				{
						OLED_Clear();
						break;					/* 回到主循环 */
				}
				
//				else if(KeyNum == 1)			/* 获取自定原始坐标坐标值 */
//				{
//						Servo_OriginAngle[0] = USL_GETPositionVal(servoUsart,sid_1);			/* 下舵机X轴 */
//						Servo_OriginAngle[1] = USL_GETPositionVal(servoUsart,sid_2);			/* 上舵机Y轴 */
//					
//						Data_StartGet=1;
//						GetOriginData_Flag=1;		/* 置原始数据获取标志位 */				
//				}
				
				else if(KeyNum == 9)		/* 下舵机+ */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_1);		/* 下舵机旋转角度获取 */
						Now_Angle=Now_Angle+17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_1,Now_Angle,0);
				}
				else if(KeyNum == 10)		/* 下舵机- */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_1);		/* 下舵机旋转角度获取 */
						Now_Angle=Now_Angle-17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_1,Now_Angle,0);
				}
				else if(KeyNum == 11)		/* 上舵机+ */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_2);		/* 上舵机旋转角度获取 */
						Now_Angle=Now_Angle+17;
						HAL_Delay(100);
						USL_SetServoAngle(servoUsart,sid_2,Now_Angle,0);
				}
				else if(KeyNum == 12)		/* 上舵机- */
				{
						uint16_t Now_Angle=USL_GETPositionVal(servoUsart,sid_2);		/* 上舵机旋转角度获取 */
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
		if(Monitor_Flag)			/* 判断复位功能标志位是否使能 */
		{
				while(1)
				{
						static uint8_t Restart_Flag;
						Key_Scan(&KeyNum);			/* 获取键码值 */	
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
 

/* 进入激光复位模式 */
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

				Key_Scan(&KeyNum);			/* 获取键码值 */				
			
				if(KeyNum == 4)		/* 返回上级菜单 */
				{
						OLED_Clear();
						break;
				}
		}	
		
}

/* 进入运动菜单选择 */
int LASER_MOVE_Menu(void)
{
		int menu2_flag=1;			/* 模式选择标志 */
	
		OLED_ShowString(16,0,"PENSTRING_MOVE",16,0);
		OLED_ShowString(16,2,"RECTANGLE_MOVE",16,0);

		while(1)
		{
			Key_Scan(&KeyNum);			/* 获取键码值 */	
		
			if(KeyNum == 1)			/* *下移 */
			{
					menu2_flag++;
					if(menu2_flag>=3) menu2_flag=1;
			}
			
			if(KeyNum == 3)			/* 选择确定 */
			{
					OLED_Clear();
					return(menu2_flag);
			}
			
			if(KeyNum == 4)			/* 返回上一级菜单 */
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



/* 三级菜单部分 */

/* 矩形框运动控制菜单 */
int RECTANGLE_MOVE_Menu(void)			
{
		int menu3_flag=1;			/* 模式选择标志 */ 

		OLED_ShowString(16,0,"SET_RECTANGLE",16,0);
		OLED_ShowString(16,2,"START_MOVE",16,0);

		while(1)
		{
			Key_Scan(&KeyNum);			/* 获取键码值 */	
		
			if(KeyNum == 1)			/* *下移 */
			{
					menu3_flag++;
					if(menu3_flag>=3) menu3_flag=1;
			}
			
			if(KeyNum == 3)			/* 选择确定 */
			{
					OLED_Clear();
					return(menu3_flag);
			}
			
			if(KeyNum == 4)			/* 返回上一级菜单 */
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

/* 四级菜单部分 */

void GET_RECTANGLE_menu(void)
{
	
		char Point1_XY[10],Point2_XY[10],Point3_XY[10],Point4_XY[10];	
		
		Data_StartGet=1;
		
		while(1)
		{
				Get_RectangleXY(rectangle_XY);			/* 获取四点原始坐标 */
					
				if(Data_FinishGet == 0 && Data_StartGet == 0)		/* 接收完成后 */
				{
						HAL_UART_Transmit(&huart3,(uint8_t*)&Rectangle_Send,sizeof(Rectangle_Send),HAL_MAX_DELAY);		/* 发送接收完成标志数据 */
					
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
		
						Key_Scan(&KeyNum);			/* 获取键码值 */
					
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

//				Key_Scan(&KeyNum);			/* 获取键码值 */
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

