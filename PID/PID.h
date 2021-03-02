#ifndef	__PID_H
#define	__PID_H
#include	"HAL_conf.h"

/*
λ��ʽPID��OUT=(Kp*Ek)+(Kp*(T/Ti)��Ek)+(Kp*(Td/T)(Ek-E(k-1)))+OUT0
		Kp:����ϵ��
		T����������
		Ti��
		Td��
		Ek����ǰ���
		E(k-1):ǰ�����
		OUT0�����鳣��
����ʽPID:��OUT=Kp(Ek-E(k-1))+((Kp*T)/Ti)Ek+(((Kp*Td)/T)*(Ek-2E(k-1)+E(k-2)))
		E(k-2):���ϴ����

*/
typedef struct
{
	float setValue ; 			//�趨ֵ
	float	presentValue;		//��ǰʵ��ֵ
	float	pv_view;	//��ǰ��ʾǰ���¶�
	float	pwm_cycle;			//pwm����
	
	float	Kp;
	float	T;							//��������
	float	Ti;
	float	Td;
	
	float	Ek;							
	float	Ek_1;
	float	Ek_2;
	
	float	currpwm;				//����ֵ

	
	u32		c1ms;
}PID;

extern	PID	pid1;
extern	PID	pid2;

void	PID_Init(void);
void	PID_Calc1(void);
void	PID_Calc2(void);


#endif
