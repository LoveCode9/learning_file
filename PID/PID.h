#ifndef	__PID_H
#define	__PID_H
#include	"HAL_conf.h"

/*
位置式PID：OUT=(Kp*Ek)+(Kp*(T/Ti)∑Ek)+(Kp*(Td/T)(Ek-E(k-1)))+OUT0
		Kp:比例系数
		T：采样周期
		Ti：
		Td：
		Ek：当前误差
		E(k-1):前次误差
		OUT0：经验常数
增量式PID:△OUT=Kp(Ek-E(k-1))+((Kp*T)/Ti)Ek+(((Kp*Td)/T)*(Ek-2E(k-1)+E(k-2)))
		E(k-2):上上次误差

*/
typedef struct
{
	float setValue ; 			//设定值
	float	presentValue;		//当前实际值
	float	pv_view;	//当前显示前端温度
	float	pwm_cycle;			//pwm周期
	
	float	Kp;
	float	T;							//采样周期
	float	Ti;
	float	Td;
	
	float	Ek;							
	float	Ek_1;
	float	Ek_2;
	
	float	currpwm;				//计算值

	
	u32		c1ms;
}PID;

extern	PID	pid1;
extern	PID	pid2;

void	PID_Init(void);
void	PID_Calc1(void);
void	PID_Calc2(void);


#endif
