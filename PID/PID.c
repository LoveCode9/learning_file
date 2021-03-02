#include "pid.h"

PID pid1; //�洢pid�㷨����Ҫ�ĵ�����,����ǰ��
PID pid2; //�洢pid�㷨����Ҫ�ĵ�����,���Ⱥ��

void	PID_Init()
{//ǰ�˳�ʼ��
//	pid1.setValue=100;//�û�Ĭ���趨�¶�
//	pid1.Kp=20;
//	pid1.T=1000;//PID��������1000ms
//	pid1.Ti=5000;//����ʱ��
//	pid1.Td=1200;//΢��ʱ��
//	pid1.currpwm=0;//��ʼ���pwm
//	pid1.pwm_cycle=1000;//pwm�������
//	pid1.c1ms=0;
//	//��˳�ʼ��
//	pid2.setValue=100;//�û�Ĭ���趨�¶�
//	pid2.Kp=20;
//	pid2.T=1000;//PID��������1000ms
//	pid2.Ti=5000;//����ʱ��
//	pid2.Td=1200;//΢��ʱ��
//	pid2.currpwm=0;//��ʼ���pwm
//	pid2.pwm_cycle=1000;//pwm�������
//	pid2.c1ms=0;
	pid1.setValue=100;//�û�Ĭ���趨�¶�
	pid1.Kp=150;
	pid1.T=1500;//PID��������1000ms
	pid1.Ti=3500;//����ʱ��
	pid1.Td=500;//΢��ʱ��
	pid1.currpwm=0;//��ʼ���pwm
	pid1.pwm_cycle=1500;//pwm�������
	pid1.c1ms=0;
	//��˳�ʼ��
	pid2.setValue=pid1.setValue;//�û�Ĭ���趨�¶�
	pid2.Kp=120;
	pid2.T=pid1.T;//PID��������1000ms
	pid2.Ti=5000;//����ʱ��
	pid2.Td=1200;//΢��ʱ��
	pid2.currpwm=0;//��ʼ���pwm
	pid2.pwm_cycle=pid1.pwm_cycle;//pwm�������
	pid2.c1ms=0;
}
//pid1����
void	PID_Calc1()	//pid����
{
	float	ti,td,ki,kd;
	float	delEk,More_delEk;
	float	Iout_add;
	float	Pout_add;
	float	Dout_add;	//pid�����ֵ�����
	float	out=pid1.currpwm;//�������м����
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
	
	out+=Pout_add+Iout_add+Dout_add;	//pwm�������
	
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

//pid2����
void	PID_Calc2()	//pid����
{
	float	ti,td,ki,kd;
	float	delEk,More_delEk;
	float	Iout_add;
	float	Pout_add;
	float	Dout_add;	//pid�����ֵ�����
	float	out=pid2.currpwm;//�������м����
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
	
	out+=Pout_add+Iout_add+Dout_add;	//pwm�������
	
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

