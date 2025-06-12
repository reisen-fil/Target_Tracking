#ifndef SERVO_MOVE_H_
#define SERVO_MOVE_H_

#include "main.h"
#include "My_KeyBoard.h"
#include "uart_servo_lite.h"
#include "oled.h"
#include "Master_PC.h"


#include "math.h"

#define pi  3.1415926f

typedef struct
{
		float Kp;
		float Ki;
		float Kd;
	
}posi_pid;

extern uint16_t Locate_angleX[4],Locate_angleY[4];
extern uint16_t CenterLocate_angleX[4],CenterLocate_angleY[4];
extern int Rectangle_TargetAngle[2];

extern uint16_t MoveStop_Flag,MoveStart_Flag;

extern uint16_t Servo_ZeroXY[2];
extern uint16_t Servo_OriginXY[2];
extern int rectangle_XY[8],Set_rectangle_XY[32];

extern uint16_t Servo_OriginAngle[2];

extern uint16_t Tracking_CNT,Tracking_Start_Flag,Tracking_Enable;

extern float RelaServo_Angle[2];

extern int Laser_XY[2];		/* 激光点坐标 */


/////////////////////////////////

//extern float Now_RelaXYDistan[2],DistanCoordi_Rate[2];


////////////////////////////////

void Penstring_Locate(void);
void Penstring_Moving(void);

void Penstring_Moving_1(void);

void Servo_SetZeroAngle(uint16_t *X,uint16_t *Y);
void Get_RectangleXY(int *RectangleXY);			/* 获取一次矩形框四点的坐标 */
void Set_RectangleXY(int* RectangleXY,int* Set_RectangleXY);			/* 四点坐标细分 */
void Set_LaserTracking(int RECTANG_X,int RECTANG_Y);


//void Get_RelativeRate(void);
//float Get_XY_RelaDistan(float XY_Rate,int Now_XY,int Target_XY);

#endif 
