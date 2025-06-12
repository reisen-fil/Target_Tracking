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

/* �˶���̨
			
			�¶����ʼλ�ýǶȣ�2048  ��ţ�1		(��+,��-)
			�϶����ʼλ�ýǶȣ�2048  ��ţ�2	  (��+,��-)
						
*/



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint8_t RxData=1,shijue_RxData;
uint8_t Key_GetCnt,Key_GetCntFlag,Key_Start;					/* ��������ʱ��ƴ�  ����������ʱ��־λ  ��������λ */

uint8_t sid_1=1,sid_2=2;

uint8_t pRxPacket;			/* ����ͷ���ݽ��ջ������� */

uint8_t KeyNum;

uint8_t Monitor_Flag,MovingReset_flag;				/* ������λ�þ��ɸ�λ���� */
uint16_t My_CoordinateData[10];						/* �������ļ�������ֵ */
uint8_t ShijueRecv_Buffer[20];						/* ��������ԭʼ���� */

uint8_t Rectangle_Send=0x66,FinishRectangle_Flag;				
/* ���յ��ĵ���ο����ݺ�������ͷ���͵�����  �ĵ���ο����ݽ�����ɱ�־λ*/

float My_Angle;
uint8_t Data_FinishGet,Data_StartGet=0;			/* ������ɱ�־λ  �������ձ�־λ */

//״̬��
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
	
	/* ���ж����ʼ������ */
//	HAL_TIM_Base_Start_IT(&htim6);
  	HAL_UART_Receive_IT(&huart1,&RxData, sizeof(RxData));				
	Tick_DelayMs(1000);

//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);			/* ����LED�� */
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);		/* ������ʹ�� */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);		/* ����ʹ�� */

	HAL_UART_Receive_IT(&huart2,(uint8_t*)&Rx_Byte,sizeof(Rx_Byte));		/* ���Դ��� */
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
			menu2 = menu1();		/* ��һ���˵��л�ȡ */
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
							if(MenuReturn_Flag)		/* ������һ���˵� */
							{
								MenuReturn_Flag=0;
								break;
							}

							if(menu3 == 1)		/* Ǧ�ʿ��˶� */
							{
										Monitor_Flag=1;				/* �˶������и�λ����ʹ�� */
								
										Penstring_Moving_1();
								
										Monitor_Flag=0;				/* �˶������и�λ����ʧ�� */								
								
							}
							else if(menu3 == 2)		/* ���ο��˶� */
							{
									menu4 = RECTANGLE_MOVE_Menu();
									
									if(menu4 == 1)		/* У׼ģʽ */
									{
											GET_RECTANGLE_menu();
									}
									else if(menu4 == 2)			/* �������ο��˶�ģʽ */
									{
											Set_RectangleXY(rectangle_XY,Set_rectangle_XY);			/* ��ȡϸ������ */
											Data_StartGet=1;		/* ʵʱ���ռ������� */
										
//											HAL_TIM_Base_Start_IT(&htim7);
//											Tracking_Start_Flag=1;
										
												
//											for(uint8_t Rec_time=0;Rec_time<16;Rec_time++)
//											{
//													while(1)
//													{
////															Set_LaserTracking(Set_rectangle_XY[Rec_time*2],Set_rectangle_XY[Rec_time*2+1]);			/* �������ο�Ѳ�� */
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
							// else if(menu3 == 0)		/* �ص�ǰ���˵� */
							// {
							// 		OLED_Clear();	
							// 		break;
							// }
					}
			}			

			/* ���β��� */
			
			/****************************************************************************************/
		
				/* �������ݽ��ղ��� */
//				if(Get_vofa_PID_Rxflag()==1)		/* �жϽ����Ƿ�ɹ� */	
//				{			
//						PID_index_1=Get_id_Flag_1();		
//						PID_index_2=Get_id_Flag_2();
//					
//						PID_Mode[PID_index_1-1].PID[PID_index_2]=RxPacket_Data_Handle(Get_Vofa_RxData);
//				}
				
				/* ����ֹͣλ���ղ��� */
//				if(Get_On_or_off_Rxflag()==1)			/* �жϽ����Ƿ�ɹ� */
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
		
  
		if(huart->Instance == USART2)		/* �����ý����ж� */
    {	
				static uint8_t PID_RxState=0;  		/* PID�������ձ�־λ */
				static uint8_t PID_pRxPacket=0;			/* ��������λ��(����Ԫ������) */
			
				static uint8_t On_or_off_RxState=0;		/* ����ֹͣλ���ձ�־λ */
				static uint8_t On_or_off_pRxPacket=0;			/* ��������λ��(����Ԫ������) */
			
//				static uint8_t FlashSave_RxState=0;
			
				HAL_UART_Receive_IT(&huart2,(uint8_t*)&Rx_Byte,sizeof(Rx_Byte));    /* ������λ��������һ���ֽ� */
				
			
				/* PID�������ղ��� */
				if(PID_RxState==0 && Rx_Byte==0x23)			/* ����"#" */
				{
						PID_RxState=1;
				}
				else if(PID_RxState==1 && Rx_Byte==0x50)			/* ����"P" */
				{
						PID_RxState=2;
				}
				else if(PID_RxState==2)		/* ���ձ�־1(_ǰ��) */
				{
						id_Flag_1=Rx_Byte-48;
						PID_RxState=3;
				}
				else if(PID_RxState==3 && Rx_Byte==0x5F)			/* ����"_" */
				{
						PID_RxState=4;
				}
				else if(PID_RxState==4)		/* ���ձ�־1(_���) */
				{
						id_Flag_2=Rx_Byte-48;
						PID_RxState=5;					
				}
				else if(PID_RxState==5 && Rx_Byte==0x3D)		/* ����"=" */
				{
						PID_RxState=6;
				}
				else if(PID_RxState==6)			/* ��ʱ��ʼ���ղ������� */
				{
						if(Rx_Byte==0x21)		/* ���յ�"!"��֡β-->����־λ���㴦�� */
						{
								Data_BitNum=PID_pRxPacket;		/* ��ȡ���յ���ʵ��λ�� */		
								PID_pRxPacket=0;				/* ������������´ν��н������� */
								PID_RxState=0;						/* ��־λ���� */
								PID_RxFlag=1;		/* ��ȡ��־λ��1 */
						}
						else			/* ������������ */
						{
								Get_Vofa_RxData[PID_pRxPacket++]=Rx_Byte;
						}
				}	

				
				/* ����ֹͣλ���ղ��� */
				if(On_or_off_RxState==0 && Rx_Byte==0x40)		/* ����"@" */	
				{
						On_or_off_RxState=1;
				}
				else if(On_or_off_RxState==1 && Rx_Byte==0x46)		/* ����"F" */
				{
						On_or_off_RxState=2;
				}
				else if(On_or_off_RxState==2 && Rx_Byte==0x6C)			/* ����"l" */
				{
						On_or_off_RxState=3;
				}
				else if(On_or_off_RxState==3 && Rx_Byte==0x61)					/* ����"a" */
				{
						On_or_off_RxState=4;
				}
				else if(On_or_off_RxState==4 && Rx_Byte==0x67)					/* ����"g" */
				{
						On_or_off_RxState=5;
				}
				else if(On_or_off_RxState==5 && Rx_Byte==0x3D)			/* ����"=" */
				{
						On_or_off_RxState=6;
				}
				else if(On_or_off_RxState==6)			/* ��ʼ���ܱ�־λ */
				{
						if(Rx_Byte==0x21)			/* �жϽ��յ�֡β"!"��,������������ֹͣλ�Ļ�ȡ */
						{
								On_or_off_RxState=0;				/* ��־λ���� */
								On_or_off_RxFlag=1;				/* ��ȡ��־λ��1 */
								On_or_off_pRxPacket=0;
						}
						else
						{
								Get_On_or_off[On_or_off_pRxPacket++]=Rx_Byte-48;		/* ��ȡ��Ӧ����ֹͣλ(1/2) */
						}
				}			
			
		}		
	
		if(huart->Instance == USART3)				/* ����ͷ���ڽ��ղ��� (16λ���ݽ���)*/
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
						HeadBagRecv_Flag=0;		/* ��ͷ������� */
						DataRecv_Flag=1;
						
				}
				
				if(DataRecv_Flag == 1 && FinishRectangle_Flag == 0)			/* �ĵ���ο�������� */
				{
						if(Recv_time++>=1)
						{							
								ShijueRecv_Buffer[pRxPacket++] = shijue_RxData;
								
								if(pRxPacket>=20)		/* �ж����ݽ������ */
								{
										pRxPacket=0;			/* ���� */
										DataRecv_Flag=0;
										Recv_time=0;
									
										Data_FinishGet=1;
									
										FinishRectangle_Flag=1;			/* �ÿ�ʼ���ռ��������־λ */
								}		
						}						
					
				}
				else if(DataRecv_Flag == 1 && FinishRectangle_Flag == 1)	/* ��ɫ����������� */	
				{
						if(Recv_time++>=1)
						{							
								ShijueRecv_Buffer[pRxPacket++] = shijue_RxData;
								
								if(pRxPacket>=4)		/* �ж����ݽ������ */
								{
										pRxPacket=0;			/* ���� */
										DataRecv_Flag=0;
										Recv_time=0;
									
										Data_FinishGet=1;
									
								}		
						}									
				}
			}

				
    }					
		

}

/* 1ms��ʱ�ж� */
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
				 if(Key_GetCntFlag)		/* ��ʼ������ʱ */
				 {
						Key_GetCnt++;
						if(Key_GetCnt >= 100)		/* ��100msʱ */
						{
								Key_GetCntFlag=0;			/* ���� */
								Key_GetCnt=0;
								Key_Start=1;
						}
				 }
				 
				 if(Tracking_Start_Flag)			/* ��ʼ1ms�ƴ� */
				 {
						Tracking_CNT++;
						if(Tracking_CNT>=6000)			/* �ǵ�100msʱ */
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
