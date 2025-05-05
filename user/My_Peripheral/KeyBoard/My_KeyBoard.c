#include "My_KeyBoard.h"


void Key_Init(void)
{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
}



/* ѭ����ʱ��������ɨ�� */
void Key_Scan(uint8_t* key)
{
		*key=0;		/* ��δ����ʱĬ��Ϊ0 */
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
	
		if((!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
			||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))||(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)))
		{
				Key_GetCntFlag=1;			/* ��������ʹ�� */
			
				HAL_TIM_Base_Start_IT(&htim7);
			
				if(Key_Start)
				{			
						HAL_TIM_Base_Stop_IT(&htim7);
					
						if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))
						{
									while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
									*key=1;
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
						{
									while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7));
									*key=2;
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
						{
									while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
									*key=3;
						}
						else if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3))
						{
									while(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3));
									*key=4;
						}
						
						Key_Start=0;
				}
		}
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
	
		/* �ĸ������ֱ����ڼ�¼Ǧ�ʿ���ĸ����Ӧ�Ķ���Ƕ�ֵ */
		if((!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
			||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))||(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)))
		{
				Key_GetCntFlag=1;			/* ��������ʹ�� */
			
				HAL_TIM_Base_Start_IT(&htim7);
			
				if(Key_Start)
				{	
						HAL_TIM_Base_Stop_IT(&htim7);
					
						if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))
						{
								
									while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
									*key=5;
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
						{
									while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7));
									*key=6;
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
						{
									while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
									*key=7;
						}
						else if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3))
						{
									while(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3));
									*key=8;
						}
						
						Key_Start=0;
				}
		}
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);		

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
	
		if((!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
			||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))||(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)))
		{
				Key_GetCntFlag=1;			/* ��������ʹ�� */				

				HAL_TIM_Base_Start_IT(&htim7);
			
				if(Key_Start)
				{
						HAL_TIM_Base_Stop_IT(&htim7);					
					
						if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))
						{
										while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
										*key=9;
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
						{
							
										while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7));
										*key=10;	
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
						{
		
										while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
										*key=11;
						}
						else if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3))
						{
							
										while(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3));
										*key=12;
										
						}
				
				Key_Start=0;		
				
				}
		}
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	


		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
	
		if((!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
			||(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))||(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)))
		{
				Key_GetCntFlag=1;			/* ��������ʹ�� */

				HAL_TIM_Base_Start_IT(&htim7);
			
				if(Key_Start)
				{	
						HAL_TIM_Base_Stop_IT(&htim7);
					
						if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))
						{
							
										while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
										*key=13;
									
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7))
						{				
									
										while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7));
										*key=14;
													
						}
						else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
						{
							
										while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
										*key=15;
							
						}
						else if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3))
						{				
						
										while(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3));
										*key=16;						
						}
						
						Key_Start=0;			/* ����λ���� */				
				}
		}
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	
		
}



