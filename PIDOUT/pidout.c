#include	"pid.h"
#include "pidout.h"
#include	"MM32f103.h"

void	PIDOUT_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOA3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
}

void	PID_out()			//输出pid运算结果到负载
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
