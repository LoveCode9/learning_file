#ifndef	__PIDOUT_H
#define	__PIDOUT_H
#include	"sys.h"

//加热开关
#define	open	0	//加热打开
#define	close	1	//加热关闭

//IO操作函数	 
#define heat_before   	PAout(0) 
#define heat_after    	PAout(1) 	


void	PIDOUT_Init(void);	//pid输出引脚初始化
void	PID_out(void);			//输出pid运算结果到负载

#endif
