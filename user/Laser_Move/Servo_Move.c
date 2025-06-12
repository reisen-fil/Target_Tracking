#include "Servo_Move.h"


uint16_t Locate_angleX[4],Locate_angleY[4];

uint16_t CenterLocate_angleX[4],CenterLocate_angleY[4];

uint16_t Delay_time,MoveStop_Flag=0,MoveStart_Flag=0;

posi_pid X_Pid;		/* �¶��pid */
posi_pid Y_Pid;		/* �϶��pid */

float RelaServo_Angle[2];


/* Ǧ���߿��ĵ��¼ */
void Penstring_Locate(void)
{
//		Key_Scan(&KeyNum);			/* ��ȡ����ֵ */
		
		if(KeyNum>=5 && KeyNum<=8)		
		{
				Locate_angleX[KeyNum-5] = USL_GETPositionVal(servoUsart,sid_1);		/* �¶����ת�ǶȻ�ȡ */
				Locate_angleY[KeyNum-5] = USL_GETPositionVal(servoUsart,sid_2);		/* �϶����ת�ǶȻ�ȡ */
		}
		
		if(KeyNum>=13 && KeyNum<=16)
		{
				CenterLocate_angleX[KeyNum-13] = USL_GETPositionVal(servoUsart,sid_1);		/* �¶����ת�ǶȻ�ȡ */
				CenterLocate_angleY[KeyNum-13] = USL_GETPositionVal(servoUsart,sid_2);		/* �϶����ת�ǶȻ�ȡ */				
		}
}

/* Ǧ�����˶�-����һ(�ĶԽǲɵ�)*/
void Penstring_Moving_1(void)
{
			for(uint16_t cnt=0;cnt<(Locate_angleY[1]-Locate_angleY[0])*0.2;cnt++)		/* ��߿� */
			{
					USL_SetServoAngle(servoUsart, sid_1, (Locate_angleX[0]+Locate_angleX[1]+CenterLocate_angleX[0])/3, 0);										
				  USL_SetServoAngle(servoUsart, sid_2, Locate_angleY[0]+5*(cnt+1), 0);
					LASER_RESET_Start();
					if(cnt<=0) HAL_Delay(250);
					else HAL_Delay(50);
			}
			for(uint16_t cnt=0;cnt<(Locate_angleX[1]-Locate_angleX[2])*0.2;cnt++)		/* �ϱ߿� */		
			{
					USL_SetServoAngle(servoUsart, sid_1, Locate_angleX[1]-5*(cnt+1), 0);										
				  USL_SetServoAngle(servoUsart, sid_2, (Locate_angleY[1]+Locate_angleY[2]+CenterLocate_angleY[1])/3, 0);
					LASER_RESET_Start();
					if(cnt<=0) HAL_Delay(250);
					else HAL_Delay(50);
			}	
			for(uint16_t cnt=0;cnt<(Locate_angleY[2]-Locate_angleY[3])*0.2;cnt++)		/* �ұ߿� */
			{
					USL_SetServoAngle(servoUsart, sid_1, (Locate_angleX[2]+Locate_angleX[3]+CenterLocate_angleX[2])/3, 0);										
				  USL_SetServoAngle(servoUsart, sid_2, Locate_angleY[2]-5*(cnt+1), 0);
					LASER_RESET_Start();
					if(cnt<=0) HAL_Delay(250);
					else HAL_Delay(50);
			}
			for(uint16_t cnt=0;cnt<(Locate_angleX[0]-Locate_angleX[3])*0.2;cnt++)		/* �±߿� */
			{
					USL_SetServoAngle(servoUsart, sid_1, Locate_angleX[3]+5*(cnt+1), 0);										
				  USL_SetServoAngle(servoUsart, sid_2, (Locate_angleY[3]+Locate_angleY[0]+CenterLocate_angleY[3])/3, 0);
					LASER_RESET_Start();
					if(cnt<=0) HAL_Delay(250);
					else HAL_Delay(50);
			}
					
}

/* ���λ�û�pid */
float Servo_Position_PID(float current_position, float target_position,float posi_kp,float posi_ki,float posi_kd) {
    static float last_error,before_error = 0;
    static int integral = 0;  // ������
    float error = target_position - current_position; // ��ǰ���
    integral += error;                              // �������ۼ�

	float derivative = error - last_error;		/* ΢���� */

    float output = posi_kp * -error + posi_ki * integral - posi_kd * derivative; // PID���
	
    last_error = error;                             // ������һ�����

		if(output>500) output=500;				/* �޷����� */
		else if(output<-500) output=-500;
		
    return output;
}

uint16_t Servo_ZeroXY[2];
uint16_t Servo_OriginXY[2];


int rectangle_XY[8],Set_rectangle_XY[32];		/* ���ο�����(���ϡ����ϡ����¡�����)  ϸ�ֳ���12���� */


int Laser_XY[2];		/* ��������� */


uint16_t Servo_OriginAngle[2];

int Rectangle_TargetAngle[2];

/* ����������У׼ */
void Servo_SetZeroAngle(uint16_t *X,uint16_t *Y)
{
	
			if(Data_FinishGet)			/* ���ڽ�����һ�����ݺ� */
			{
						if(GetOriginData_Flag)
						{
								/* 16λ���ݴ���(�ϳ�����ֵ) */
								for(uint8_t time=0;time<4;time++)
								{
										My_CoordinateData[time] = (ShijueRecv_Buffer[time*2+1]<<8)|ShijueRecv_Buffer[time*2];
								}
		
								*X = My_CoordinateData[0];			/* X���� */
								*Y = My_CoordinateData[1];			/* Y���� */						
								
								Data_FinishGet=0;			/* ��ʱ���������ɱ�־λ */
								Data_StartGet=0;
								
								GetOriginData_Flag=0;
						}
						else 
						{
								/* 16λ���ݴ���(�ϳ�����ֵ) */
								for(uint8_t time=0;time<10;time++)
								{
										My_CoordinateData[time] = (ShijueRecv_Buffer[time*2+1]<<8)|ShijueRecv_Buffer[time*2];
								}
		
								*X = My_CoordinateData[0];			/* X���� */
								*Y = My_CoordinateData[1];			/* Y���� */

								for(uint8_t time=0;time<8;time++)		/* ���ο��ĵ������ȡ */
								{
										rectangle_XY[time] = My_CoordinateData[time+2];
								}
								
								Data_FinishGet=0;			/* ��ʱ���������ɱ�־λ */
								Data_StartGet=0;
																
						}
						
			}
	
}

void Get_RectangleXY(int* RectangleXY)			/* ��ȡһ�ξ��ο��ĵ������ */
{
	
			if(Data_FinishGet)			/* ���ڽ�����һ�����ݺ� */
			{
						/* 16λ���ݴ���(�ϳ�����ֵ) */
						for(uint8_t time=2;time<10;time++)
						{
								RectangleXY[time-2] = (ShijueRecv_Buffer[time*2+1]<<8)|ShijueRecv_Buffer[time*2];
						}	
						
						Data_FinishGet=0;			/* ��ʱ���������ɱ�־λ */
						Data_StartGet=0;
						
			}
	
}


void Set_RectangleXY(int* RectangleXY,int* Set_RectangleXY)			/* ���ο�߿�������ϸ��(�����ֶ�������) */
{
		/* ���ϱ߿� */
		Set_RectangleXY[0] = rectangle_XY[0];				/* ��ʼ��һ����X */
		Set_RectangleXY[1] = rectangle_XY[1];				/* ��ʼ��һ����Y */
	
		Set_RectangleXY[4] = (rectangle_XY[0]+rectangle_XY[2])/2;				/* ��ʼ�м��X */
		Set_RectangleXY[5] = (rectangle_XY[1]+rectangle_XY[3])/2;				/* ��ʼ�м��Y */

		Set_RectangleXY[2] = (rectangle_XY[0]+Set_RectangleXY[4])/2;		/* ������м��X */
		Set_RectangleXY[3] = (rectangle_XY[1]+Set_RectangleXY[5])/2;		/* ������м��Y */

		Set_RectangleXY[6] = (rectangle_XY[2]+Set_RectangleXY[4])/2;		/* ���ҵ��м��X */
		Set_RectangleXY[7] = (rectangle_XY[3]+Set_RectangleXY[5])/2;		/* ���ҵ��м��Y */

	
		/* ���ұ߿� */	
		Set_RectangleXY[8] = rectangle_XY[2];				/* ��ʼ�ڶ�����X */
		Set_RectangleXY[9] = rectangle_XY[3];				/* ��ʼ�ڶ�����Y */	
	
		Set_RectangleXY[12] = (rectangle_XY[2]+rectangle_XY[6])/2;				/* ��ʼ�м��X */
		Set_RectangleXY[13] = (rectangle_XY[3]+rectangle_XY[7])/2;				/* ��ʼ�м��Y */

		Set_RectangleXY[10] = (rectangle_XY[2]+Set_RectangleXY[12])/2;		/* ������м��X */
		Set_RectangleXY[11] = (rectangle_XY[3]+Set_RectangleXY[13])/2;		/* ������м��Y */

		Set_RectangleXY[14] = (rectangle_XY[6]+Set_RectangleXY[12])/2;		/* ���ҵ��м��X */
		Set_RectangleXY[15] = (rectangle_XY[7]+Set_RectangleXY[13])/2;		/* ���ҵ��м��Y */


		/* ���±߿� */
		Set_RectangleXY[16] = rectangle_XY[4];				/* ��ʼ��������X */
		Set_RectangleXY[17] = rectangle_XY[5];				/* ��ʼ��������Y */					
		
		Set_RectangleXY[20] = (rectangle_XY[6]+rectangle_XY[4])/2;				/* ��ʼ�м��X */
		Set_RectangleXY[21] = (rectangle_XY[7]+rectangle_XY[5])/2;				/* ��ʼ�м��Y */

		Set_RectangleXY[18] = (rectangle_XY[6]+Set_RectangleXY[20])/2;		/* ������м��X */
		Set_RectangleXY[19] = (rectangle_XY[7]+Set_RectangleXY[21])/2;		/* ������м��Y */

		Set_RectangleXY[22] = (rectangle_XY[4]+Set_RectangleXY[20])/2;		/* ���ҵ��м��X */
		Set_RectangleXY[23] = (rectangle_XY[5]+Set_RectangleXY[21])/2;		/* ���ҵ��м��Y */		


		/* ����߿� */
		Set_RectangleXY[24] = rectangle_XY[6];				/* ��ʼ���ĸ���X */
		Set_RectangleXY[25] = rectangle_XY[7];				/* ��ʼ���ĸ���Y */			
		
		Set_RectangleXY[28] = (rectangle_XY[4]+rectangle_XY[0])/2;				/* ��ʼ�м��X */
		Set_RectangleXY[29] = (rectangle_XY[5]+rectangle_XY[1])/2;				/* ��ʼ�м��Y */

		Set_RectangleXY[26] = (rectangle_XY[4]+Set_RectangleXY[28])/2;		/* ������м��X */
		Set_RectangleXY[27] = (rectangle_XY[5]+Set_RectangleXY[29])/2;		/* ������м��Y */

		Set_RectangleXY[30] = (rectangle_XY[0]+Set_RectangleXY[28])/2;		/* ���ҵ��м��X */
		Set_RectangleXY[31] = (rectangle_XY[1]+Set_RectangleXY[29])/2;		/* ���ҵ��м��Y */	

}


/* ʵʱ��ȡ��������� */
void Get_LaserXY_ALLtime(int* MyXY)
{
		if(Data_FinishGet==1 && FinishRectangle_Flag==1)			/* ���������һ�ν��պ� */
		{
				/* 16λ���ݴ���(�ϳ�����ֵ) */
				for(uint8_t time=0;time<2;time++)
				{
						MyXY[time] = (ShijueRecv_Buffer[time*2+1]<<8)|ShijueRecv_Buffer[time*2];
				}						
				
				Data_FinishGet=0;			/* ��ʱ���������ɱ�־λ */
//				Data_StartGet=0;				
		}
}

uint16_t Tracking_CNT,Tracking_Start_Flag,Tracking_Enable;			/* ������ٲ��ֵļ�ʱ */


float limit_SetAngle(int Now_Angle)
{
		if(Now_Angle<1348) Now_Angle=1348;
		else if(Now_Angle>2748) Now_Angle=2748;
	
		return Now_Angle;
}


/* ������پ��ο򲿷� */

void Set_LaserTracking(int RECTANG_X,int RECTANG_Y)
{
//		while(1)
//		{
				static float Now_OriginAngle[2];
			
//				if(Tracking_Enable==1 && Data_FinishGet==1)			/* 100ms��ʱ���� */
				if(Data_FinishGet==1)			/* 100ms��ʱ���� */
				{
						Get_LaserXY_ALLtime(Laser_XY);		/* ʵʱ���ռ������� */

					
					
						if(Get_vofa_PID_Rxflag()==1)		/* �жϽ����Ƿ�ɹ� */	
						{			
								PID_index_1=Get_id_Flag_1();		
								PID_index_2=Get_id_Flag_2();
							
								PID_Mode[PID_index_1-1].PID[PID_index_2]=RxPacket_Data_Handle(Get_Vofa_RxData);
						}
						
						// X_Pid.Kp = PID_Mode[0].PID[0];
						// X_Pid.Ki = PID_Mode[0].PID[1];
						// X_Pid.Kd = PID_Mode[0].PID[2];
						
						// Y_Pid.Kp = PID_Mode[1].PID[0];
						// Y_Pid.Ki = PID_Mode[1].PID[1];
						// Y_Pid.Kd = PID_Mode[1].PID[2];
						
						X_Pid.Kp = 0.49;
						// X_Pid.Ki = 0.01;
						X_Pid.Kd = 0.1;
//						
						Y_Pid.Kp = 0.39;
//						Y_Pid.Ki = 0;
						Y_Pid.Kd = 0.1;
						
						/* ����λ�û�pid���� */
						RelaServo_Angle[0] = Servo_Position_PID(Laser_XY[0],RECTANG_X,X_Pid.Kp,X_Pid.Ki,X_Pid.Kd);
						RelaServo_Angle[1] = Servo_Position_PID(Laser_XY[1],RECTANG_Y,Y_Pid.Kp,Y_Pid.Ki,Y_Pid.Kd);
					
						Now_OriginAngle[0] = USL_GETPositionVal(servoUsart,sid_1);
						Now_OriginAngle[1] = USL_GETPositionVal(servoUsart,sid_2);
					
						HAL_Delay(10);
					
						USL_SetServoAngle(servoUsart,sid_1,limit_SetAngle(Now_OriginAngle[0]+RelaServo_Angle[0]),0);
						USL_SetServoAngle(servoUsart,sid_2,limit_SetAngle(Now_OriginAngle[1]+RelaServo_Angle[1]),0);
				}						

						
//						Tracking_Enable=0;
//						TIM7->CNT=0;		/* �����Ĵ������� */
//						HAL_TIM_Base_Start_IT(&htim7);		/* ����ʹ�ܶ�ʱ�ж� */
											
//				}

}


///* ��ȡУ׼��;��ο�ǵ����Ծ��� */
//float Get_RelativeDistance(int Diff_Angle)
//{
//		int Board_s=100;			/* �װ�����̨��� */
//		float Relative_Distance;
//		
//		if(Diff_Angle<0)		/* ���<0ʱ */
//		{
//				Relative_Distance=Board_s*tan(-pi*Diff_Angle/2048);
//		}
//		else		/* ���>0ʱ */
//		{
//				Relative_Distance=Board_s*tan(pi*Diff_Angle/2048);
//		}
//		
//		return Relative_Distance;
//}

//float Get_DistanCoordi_Rate(int Coordinate_Diff,float Reladistance)
//{
//		float Target_Rate;
//		if(Coordinate_Diff<0) Target_Rate = -Reladistance/Coordinate_Diff;
//		else Target_Rate = Reladistance/Coordinate_Diff;
//	
//		return Target_Rate;
//}



//float DistanCoordi_Rate[2],Now_RelaXYDistan[2];


///* ��ȡƽ���ϵľ����������֮��ı�ֵ */

//void Get_RelativeRate(void)
//{
////		int X_Reladistance,Y_Reladistance,RelaAngle[2],Board_s=100;
////		float DistanCoordi_Rate[2],Target_DistanBias[2],Delta_ServoAngle[2];
//		float X_Reladistance,Y_Reladistance;	
//		int	RelaAngle[2];
//	
//		/* ���ԭʼ�����У׼���X��Y�����(��̨��װ�ľ���Ϊ1m) */
//		RelaAngle[0] = Servo_OriginAngle[0]-USL_GETPositionVal(servoUsart,sid_1);
//		RelaAngle[1] = Servo_OriginAngle[1]-USL_GETPositionVal(servoUsart,sid_2);
//	
//		X_Reladistance = Get_RelativeDistance(RelaAngle[0]);		/* X�Ჿ�� */
//		Y_Reladistance = Get_RelativeDistance(RelaAngle[1]);		/* Y�Ჿ�� */
//	
//		/* ����ϵ�� */
//		DistanCoordi_Rate[0] = Get_DistanCoordi_Rate(Servo_OriginXY[0]-Servo_ZeroXY[0],X_Reladistance);
//		DistanCoordi_Rate[1] = Get_DistanCoordi_Rate(Servo_OriginXY[1]-Servo_ZeroXY[1],Y_Reladistance);	
//	
//		/* У׼����Ŀ��λ��X��Y���ϵľ��� */
////		Target_DistanBias[0] = -(Input_XY[0]-Servo_ZeroXY[0])*DistanCoordi_Rate[0];
////		Target_DistanBias[1] = -(Input_XY[1]-Servo_ZeroXY[1])*DistanCoordi_Rate[1];
//			
//				
//}

///* ��ȡ��̨�ﵽĿ��λ����Ҫ��Ŀ�����
//		(�������Ӧ���ο�ǵ���Ҫ����Ծ���) */
//float Get_XY_RelaDistan(float XY_Rate,int Now_XY,int Target_XY)
//{
//		float XY_RelaDistan = XY_Rate*(Target_XY-Now_XY);
//		return XY_RelaDistan;
//}



