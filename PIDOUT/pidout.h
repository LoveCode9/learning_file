#ifndef	__PIDOUT_H
#define	__PIDOUT_H
#include	"sys.h"

//���ȿ���
#define	open	0	//���ȴ�
#define	close	1	//���ȹر�

//IO��������	 
#define heat_before   	PAout(0) 
#define heat_after    	PAout(1) 	


void	PIDOUT_Init(void);	//pid������ų�ʼ��
void	PID_out(void);			//���pid������������

#endif
