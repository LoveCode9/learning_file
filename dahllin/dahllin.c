#include "dahllin.h"
���ȶ����м������Ȼ�����ǰ�����õ��Ĵ����㷨��ʽ����C����ʵ�֣�ͬʱ���н����޷������ⳬ��PWM�����ڡ�
long DaLin(float mubiao,float shiji)
{
	int b0=0.4,b1=0.2,a0=12,a1=8;
	static float error,last_error;
	static long DaLin_u,last_DaLin_u,last_DaLin_u1,DaLin_result;
	error = mubiao - shiji;
	DaLin_u = b0*last_DaLin_u + b1*last_DaLin_u1 + a0*error+a1*last_error;�����㷨���ʽ
	last_error = error;
	last_DaLin_u1 = last_DaLin_u;
	last_DaLin_u = DaLin_u;
	DaLin_result = DaLin_u;
	if(error>=-0.6&&error<=0.6)				 //��ʼ���¼�ʱ
	{
	    if(wendu<=49.5)		flag=1;
	    if(wendu>49.5&&wendu<=59.5)		flag=3;
			if(wendu>59.5)		flag=5;
	}
	if(DaLin_result > 60)	 		//��Ŀ���¶�̫��ʵ���¶ȹ�С��
		DaLin_result = 60;       //�޷�
	if(DaLin_result < 0)			//����Ŀ���¶�
		DaLin_result = 0;
		DaLin_result=(int)DaLin_result;      //ǿ��ת��������
	return DaLin_result;
	
}