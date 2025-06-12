/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "oled.h"

#include "My_KeyBoard.h"

#include "uart_servo_lite.h"
#include "Tick.h"

#include "Master_PC.h"
#include "Servo_Move.h"

/* 运动云台
			
			下舵机初始位置角度：2048  编号：1		(左+,右-)
			上舵机初始位置角度：2048  编号：2	  (上+,下-)
						
*/



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint8_t RxData=1,shijue_RxData;
uint8_t Key_GetCnt,Key_GetCntFlag,Key_Start;					/* 按键消抖时间计次  按键消抖计时标志位  按键启动位 */

uint8_t sid_1=1,sid_2=2;

uint8_t pRxPacket;			/* 摄像头数据接收缓冲数组 */

uint8_t KeyNum;

uint8_t Monitor_Flag,MovingReset_flag;				/* 在任意位置均可复位部分 */
uint16_t My_CoordinateData[10];						/* 传过来的计算坐标值 */
uint8_t ShijueRecv_Buffer[20];						/* 传过来的原始数据 */

uint8_t Rectangle_Send=0x66,FinishRectangle_Flag;				
/* 接收到四点矩形框数据后向摄像头发送的数据  四点矩形框数据接收完成标志位*/

float My_Angle;
uint8_t Data_FinishGet,Data_StartGet=0;			/* 接收完成标志位  启动接收标志位 */

//状态码
JOHO_STATUS statusCode;

uint16_t Test;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	
	OLED_Init();
	OLED_Clear();
	Key_Init();
	
	/* 串行舵机初始化部分 */
//	HAL_TIM_Base_Start_IT(&htim6);
  	HAL_UART_Receive_IT(&huart1,&RxData, sizeof(RxData));				
	Tick_DelayMs(1000);

//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);			/* 点亮LED灯 */
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);		/* 蜂鸣器使能 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);		/* 激光使能 */

	HAL_UART_Receive_IT(&huart2,(uint8_t*)&Rx_Byte,sizeof(Rx_Byte));		/* 调试串口 */
	HAL_UART_Receive_DMA(&huart3,(uint8_t*)&shijue_RxData,1);
	
	// USL_SetServoAngle(servoUsart,sid_1,1848,0);
	// USL_SetServoAngle(servoUsart,sid_2,1848,0);
	
	
//	Test = USL_GETPositionVal(servoUsart,sid_1);
//	HAL_Delay(100);
//	USL_SetServoAngle(servoUsart,sid_1,2080,0);	
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
			menu2 = menu1();		/* 从一级菜单中获取 */
			if(menu2 == 1) 
			{
					LOCATE_MODE_Menu();	
			}
			else if(menu2 == 2) 
			{
					LASER_RESET_Menu();			
			}
			else if(menu2 == 3)	
			{
					while(1)
					{
							menu3 = LASER_MOVE_Menu();
							if(MenuReturn_Flag)		/* 返回上一级菜单 */
							{
								MenuReturn_Flag=0;
								break;
							}

							if(menu3 == 1)		/* 铅笔框运动 */
							{
										Monitor_Flag=1;				/* 运动过程中复位功能使能 */
								
										Penstring_Moving_1();
								
										Monitor_Flag=0;				/* 运动过程中复位功能失能 */								
								
							}
							else if(menu3 == 2)		/* 矩形框运动 */
							{
									menu4 = RECTANGLE_MOVE_Menu();
									
									if(menu4 == 1)		/* 校准模式 */
									{
											GET_RECTANGLE_menu();
									}
									else if(menu4 == 2)			/* 启动矩形框运动模式 */
									{
											Set_RectangleXY(rectangle_XY,Set_rectangle_XY);			/* 获取细分坐标 */
											Data_StartGet=1;		/* 实时接收激光坐标 */
										
//											HAL_TIM_Base_Start_IT(&htim7);
//											Tracking_Start_Flag=1;
										
												
//											for(uint8_t Rec_time=0;Rec_time<16;Rec_time++)
//											{
//													while(1)
//													{
////															Set_LaserTracking(Set_rectangle_XY[Rec_time*2],Set_rectangle_XY[Rec_time*2+1]);			/* 启动矩形框巡线 */
////														
////															if( 	((Set_rectangle_XY[Rec_time*2]<Laser_XY[0]+1)&&(Set_rectangle_XY[Rec_time*2+1]<Laser_XY[1]+1))|| 
////																		((Set_rectangle_XY[Rec_time*2]+1>Laser_XY[0])&&(Set_rectangle_XY[Rec_time*2+1]+1>Laser_XY[1]))|| 
////																		((Set_rectangle_XY[Rec_time*2]<Laser_XY[0]+1)&&(Set_rectangle_XY[Rec_time*2+1]+1>Laser_XY[1]))||
////																		((Set_rectangle_XY[Rec_time*2]+1>Laser_XY[0])&&(Set_rectangle_XY[Rec_time*2+1]<Laser_XY[1]+1)) )
////															{
////																		break;
////															}
//																
//																Set_LaserTracking(Set_rectangle_XY[Rec_time*2],Set_rectangle_XY[Rec_time*2+1]);
//																if(Tracking_Enable) 
//																{
//																		Tracking_Enable=0;
//																		break;
//																}
//													}	
//											}
//											
//											HAL_TIM_Base_Stop_IT(&htim7);	
//											Tracking_Start_Flag=0;
//											Tracking_CNT=0;

												while(1)
												{
												Set_LaserTracking(Set_rectangle_XY[0],Set_rectangle_XY[1]);
												}

									}
									
							}
							// else if(menu3 == 0)		/* 回到前级菜单 */
							// {
							// 		OLED_Clear();	
							// 		break;
							// }
					}
			}			

			/* 调参部分 */
			
			/****************************************************************************************/
		
				/* 调参数据接收部分 */
//				if(Get_vofa_PID_Rxflag()==1)		/* 判断接收是否成功 */	
//				{			
//						PID_index_1=Get_id_Flag_1();		
//						PID_index_2=Get_id_Flag_2();
//					
//						PID_Mode[PID_index_1-1].PID[PID_index_2]=RxPacket_Data_Handle(Get_Vofa_RxData);
//				}
				
				/* 启动停止位接收部分 */
//				if(Get_On_or_off_Rxflag()==1)			/* 判断接受是否成功 */
//				{
//						Car_on_off=Get_On_or_off[0];
//				}		

			/****************************************************************************************/				
				
				
			
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

    if(huart->Instance == USART1){
        HAL_UART_Receive_IT(&huart1,&RxData, sizeof(RxData));
        RingBuffer_Push(usart1.recvBuf, RxData);
    }
		
  
		if(huart->Instance == USART2)		/* 调试用接收中断 */
    {	
				static uint8_t PID_RxState=0;  		/* PID参数接收标志位 */
				static uint8_t PID_pRxPacket=0;			/* 接收数据位数(数组元素索引) */
			
				static uint8_t On_or_off_RxState=0;		/* 启动停止位接收标志位 */
				static uint8_t On_or_off_pRxPacket=0;			/* 接收数据位数(数组元素索引) */
			
//				static uint8_t FlashSave_RxState=0;
			
				HAL_UART_Receive_IT(&huart2,(uint8_t*)&Rx_Byte,sizeof(Rx_Byte));    /* 接受上位机传来的一个字节 */
				
			
				/* PID参数接收部分 */
				if(PID_RxState==0 && Rx_Byte==0x23)			/* 接收"#" */
				{
						PID_RxState=1;
				}
				else if(PID_RxState==1 && Rx_Byte==0x50)			/* 接收"P" */
				{
						PID_RxState=2;
				}
				else if(PID_RxState==2)		/* 接收标志1(_前边) */
				{
						id_Flag_1=Rx_Byte-48;
						PID_RxState=3;
				}
				else if(PID_RxState==3 && Rx_Byte==0x5F)			/* 接收"_" */
				{
						PID_RxState=4;
				}
				else if(PID_RxState==4)		/* 接收标志1(_后边) */
				{
						id_Flag_2=Rx_Byte-48;
						PID_RxState=5;					
				}
				else if(PID_RxState==5 && Rx_Byte==0x3D)		/* 接收"=" */
				{
						PID_RxState=6;
				}
				else if(PID_RxState==6)			/* 此时开始接收参数数据 */
				{
						if(Rx_Byte==0x21)		/* 接收到"!"即帧尾-->作标志位清零处理 */
						{
								Data_BitNum=PID_pRxPacket;		/* 获取接收到的实际位数 */		
								PID_pRxPacket=0;				/* 清除索引方便下次进行接收数据 */
								PID_RxState=0;						/* 标志位清零 */
								PID_RxFlag=1;		/* 获取标志位置1 */
						}
						else			/* 持续接收数据 */
						{
								Get_Vofa_RxData[PID_pRxPacket++]=Rx_Byte;
						}
				}	

				
				/* 启动停止位接收部分 */
				if(On_or_off_RxState==0 && Rx_Byte==0x40)		/* 接收"@" */	
				{
						On_or_off_RxState=1;
				}
				else if(On_or_off_RxState==1 && Rx_Byte==0x46)		/* 接收"F" */
				{
						On_or_off_RxState=2;
				}
				else if(On_or_off_RxState==2 && Rx_Byte==0x6C)			/* 接收"l" */
				{
						On_or_off_RxState=3;
				}
				else if(On_or_off_RxState==3 && Rx_Byte==0x61)					/* 接收"a" */
				{
						On_or_off_RxState=4;
				}
				else if(On_or_off_RxState==4 && Rx_Byte==0x67)					/* 接收"g" */
				{
						On_or_off_RxState=5;
				}
				else if(On_or_off_RxState==5 && Rx_Byte==0x3D)			/* 接收"=" */
				{
						On_or_off_RxState=6;
				}
				else if(On_or_off_RxState==6)			/* 开始接受标志位 */
				{
						if(Rx_Byte==0x21)			/* 判断接收到帧尾"!"后,结束本次启动停止位的获取 */
						{
								On_or_off_RxState=0;				/* 标志位清零 */
								On_or_off_RxFlag=1;				/* 获取标志位置1 */
								On_or_off_pRxPacket=0;
						}
						else
						{
								Get_On_or_off[On_or_off_pRxPacket++]=Rx_Byte-48;		/* 获取对应启动停止位(1/2) */
						}
				}			
			
		}		
	
		if(huart->Instance == USART3)				/* 摄像头串口接收部分 (16位数据接收)*/
		{
				static uint8_t HeadBagRecv_Flag,DataRecv_Flag,Recv_time;
				HAL_UART_Receive_DMA(&huart3,(uint8_t*)&shijue_RxData,1);
				
			if(Data_FinishGet==0 && Data_StartGet==1)
			{
			
				if(shijue_RxData == 0xAA) 
				{
						HeadBagRecv_Flag=1;			
				}
				else if(shijue_RxData == 0x55 && HeadBagRecv_Flag==1)
				{
						HeadBagRecv_Flag=0;		/* 包头接收完毕 */
						DataRecv_Flag=1;
						
				}
				
				if(DataRecv_Flag == 1 && FinishRectangle_Flag == 0)			/* 四点矩形框坐标接收 */
				{
						if(Recv_time++>=1)
						{							
								ShijueRecv_Buffer[pRxPacket++] = shijue_RxData;
								
								if(pRxPacket>=20)		/* 判断数据接收完后 */
								{
										pRxPacket=0;			/* 清零 */
										DataRecv_Flag=0;
										Recv_time=0;
									
										Data_FinishGet=1;
									
										FinishRectangle_Flag=1;			/* 置开始接收激光坐标标志位 */
								}		
						}						
					
				}
				else if(DataRecv_Flag == 1 && FinishRectangle_Flag == 1)	/* 红色激光坐标接收 */	
				{
						if(Recv_time++>=1)
						{							
								ShijueRecv_Buffer[pRxPacket++] = shijue_RxData;
								
								if(pRxPacket>=4)		/* 判断数据接收完后 */
								{
										pRxPacket=0;			/* 清零 */
										DataRecv_Flag=0;
										Recv_time=0;
									
										Data_FinishGet=1;
									
								}		
						}									
				}
			}

				
    }					
		

}

/* 1ms计时中断 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

    if(htim->Instance == TIM6){
         if(TickCnt > 0){
             TickCnt--;
         }
         else{
             TickCnt = 0;
         }
				 

				 
//				 if(MoveStart_Flag==1)
//				 {
//							Penstring_Moving();
//							MoveStart_Flag=0;
//				 }
				 
    }

		if(htim->Instance == TIM7)
		{
				 if(Key_GetCntFlag)		/* 开始消抖计时 */
				 {
						Key_GetCnt++;
						if(Key_GetCnt >= 100)		/* 到100ms时 */
						{
								Key_GetCntFlag=0;			/* 清零 */
								Key_GetCnt=0;
								Key_Start=1;
						}
				 }
				 
				 if(Tracking_Start_Flag)			/* 开始1ms计次 */
				 {
						Tracking_CNT++;
						if(Tracking_CNT>=6000)			/* 记到100ms时 */
						{
								Tracking_CNT=0;
								Tracking_Enable=1;
						}
				 }				
		}			
		
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
