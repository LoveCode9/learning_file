#include "pid.h"

PID pid1; //存储pid算法所需要的的数据,加热前端
PID pid2; //存储pid算法所需要的的数据,加热后端

void	PID_Init()
{//前端初始化
//	pid1.setValue=100;//用户默认设定温度
//	pid1.Kp=20;
//	pid1.T=1000;//PID计算周期1000ms
//	pid1.Ti=5000;//积分时间
//	pid1.Td=1200;//微分时间
//	pid1.currpwm=0;//初始输出pwm
//	pid1.pwm_cycle=1000;//pwm输出周期
//	pid1.c1ms=0;
//	//后端初始化
//	pid2.setValue=100;//用户默认设定温度
//	pid2.Kp=20;
//	pid2.T=1000;//PID计算周期1000ms
//	pid2.Ti=5000;//积分时间
//	pid2.Td=1200;//微分时间
//	pid2.currpwm=0;//初始输出pwm
//	pid2.pwm_cycle=1000;//pwm输出周期
//	pid2.c1ms=0;
	pid1.setValue=100;//用户默认设定温度
	pid1.Kp=150;
	pid1.T=1500;//PID计算周期1000ms
	pid1.Ti=3500;//积分时间
	pid1.Td=500;//微分时间
	pid1.currpwm=0;//初始输出pwm
	pid1.pwm_cycle=1500;//pwm输出周期
	pid1.c1ms=0;
	//后端初始化
	pid2.setValue=pid1.setValue;//用户默认设定温度
	pid2.Kp=120;
	pid2.T=pid1.T;//PID计算周期1000ms
	pid2.Ti=5000;//积分时间
	pid2.Td=1200;//微分时间
	pid2.currpwm=0;//初始输出pwm
	pid2.pwm_cycle=pid1.pwm_cycle;//pwm输出周期
	pid2.c1ms=0;
}
//pid1计算
void	PID_Calc1()	//pid计算
{
	float	ti,td,ki,kd;
	float	delEk,More_delEk;
	float	Iout_add;
	float	Pout_add;
	float	Dout_add;	//pid各部分的增量
	float	out=pid1.currpwm;//输出结果中间变量
	if(pid1.c1ms<pid1.T)
	{
		return	;
	}
	
	pid1.Ek=pid1.setValue-pid1.presentValue;
	
	delEk=pid1.Ek-pid1.Ek_1;
	More_delEk=pid1.Ek-2*pid1.Ek_1+pid1.Ek_2;
	
	ti=pid1.T/pid1.Ti;
	ki=ti*pid1.Kp;
	
	td=pid1.Td/pid1.T;
	kd=td*pid1.Kp;
	
	Pout_add=pid1.Kp*delEk;
	Iout_add=ki*pid1.Ek;
	Dout_add=kd*More_delEk;
	
	out+=Pout_add+Iout_add+Dout_add;	//pwm输出增量
	
	if(out>pid1.pwm_cycle)
	{
		pid1.currpwm=pid1.pwm_cycle;
	}else if(out<0)
	{
		pid1.currpwm=0;
	}else
	{
		pid1.currpwm=out;
	}
	
	pid1.Ek_2=pid1.Ek_1;
	pid1.Ek_1=pid1.Ek;

	pid1.c1ms=0;
}

//pid2计算
void	PID_Calc2()	//pid计算
{
	float	ti,td,ki,kd;
	float	delEk,More_delEk;
	float	Iout_add;
	float	Pout_add;
	float	Dout_add;	//pid各部分的增量
	float	out=pid2.currpwm;//输出结果中间变量
	if(pid2.c1ms<pid2.T)
	{
		return	;
	}
	
	pid2.Ek=pid2.setValue-pid2.presentValue;
	
	delEk=pid2.Ek-pid2.Ek_1;
	More_delEk=pid2.Ek-2*pid2.Ek_1+pid2.Ek_2;
	
	ti=pid2.T/pid2.Ti;
	ki=ti*pid2.Kp;
	
	td=pid2.Td/pid2.T;
	kd=td*pid2.Kp;
	
	Pout_add=pid2.Kp*delEk;
	Iout_add=ki*pid2.Ek;
	Dout_add=kd*More_delEk;
	
	out+=Pout_add+Iout_add+Dout_add;	//pwm输出增量
	
	if(out>pid2.pwm_cycle)
	{
		pid2.currpwm=pid2.pwm_cycle;
	}else if(out<0)
	{
		pid2.currpwm=0;
	}else
	{
		pid2.currpwm=out;
	}
	
	pid2.Ek_2=pid2.Ek_1;
	pid2.Ek_1=pid2.Ek;

	pid2.c1ms=0;
}

