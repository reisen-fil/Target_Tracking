/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "usart.h"
//#include "tim.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern uint8_t Key_GetCnt,Key_GetCntFlag,Key_Start;					/* 按键消抖时间计次  按键消抖计时标志位  按键启动位 */
extern uint8_t KeyNum;
extern uint8_t sid_1,sid_2;

extern uint8_t Data_FinishGet,Data_StartGet;			/* 接收完成标志位  启动接收标志位 */

extern uint8_t Monitor_Flag,MovingReset_flag;				/* 在任意位置均可复位部分 */
extern uint16_t My_CoordinateData[10];						/* 传过来的计算坐标值 */
extern uint8_t ShijueRecv_Buffer[20],FinishRectangle_Flag;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key_Getting_4_Pin GPIO_PIN_3
#define Key_Getting_4_GPIO_Port GPIOC
#define Key_Getting_3_Pin GPIO_PIN_0
#define Key_Getting_3_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_1
#define LED_1_GPIO_Port GPIOA
#define Test_Tx_Pin GPIO_PIN_2
#define Test_Tx_GPIO_Port GPIOA
#define Test_Rx_Pin GPIO_PIN_3
#define Test_Rx_GPIO_Port GPIOA
#define Laser_IO_Pin GPIO_PIN_5
#define Laser_IO_GPIO_Port GPIOA
#define Key_Getting_1_Pin GPIO_PIN_6
#define Key_Getting_1_GPIO_Port GPIOA
#define Key_Getting_2_Pin GPIO_PIN_7
#define Key_Getting_2_GPIO_Port GPIOA
#define Key_Setting_3_Pin GPIO_PIN_4
#define Key_Setting_3_GPIO_Port GPIOC
#define Key_Setting_4_Pin GPIO_PIN_5
#define Key_Setting_4_GPIO_Port GPIOC
#define Buzz_Pin GPIO_PIN_0
#define Buzz_GPIO_Port GPIOB
#define Key_Setting_1_Pin GPIO_PIN_1
#define Key_Setting_1_GPIO_Port GPIOB
#define Key_Setting_2_Pin GPIO_PIN_2
#define Key_Setting_2_GPIO_Port GPIOB
#define shijue_Tx_Pin GPIO_PIN_10
#define shijue_Tx_GPIO_Port GPIOB
#define shijue_Rx_Pin GPIO_PIN_11
#define shijue_Rx_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_8
#define LED_2_GPIO_Port GPIOA
#define uartServo_Tx_Pin GPIO_PIN_9
#define uartServo_Tx_GPIO_Port GPIOA
#define uartServo_Rx_Pin GPIO_PIN_10
#define uartServo_Rx_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
