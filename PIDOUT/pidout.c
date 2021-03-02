#include	"pid.h"
#include "pidout.h"
#include	"MM32f103.h"

void	PIDOUT_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOA3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
}

void	PID_out()			//���pid������������
{
	static	u16	pw_before	;
	static	u16	pw_after	;
	pw_before++;
	pw_after++;
	
	if(pw_before>=pid1.pwm_cycle)
	{
		pw_before=0;
	
	}//0	-	pid.pwm_cucle-1
	if(pw_after>=pid2.pwm_cycle)
	{
		pw_after=0;
	
	}//0	-	pid.pwm_cucle-1
	if(pw_before<pid1.currpwm)
	{
		heat_before =open;
	}else
	{
		heat_before =close;
	}
	if(pw_after<pid2.currpwm)
	{
		heat_after	=open;
	}else
	{
		heat_after	=close;
	}
}
