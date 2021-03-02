#include "dahllin.h"
首先定义中间变量，然后根据前面计算得到的大林算法公式，用C语言实现，同时还有进行限幅，以免超过PWM的周期。
long DaLin(float mubiao,float shiji)
{
	int b0=0.4,b1=0.2,a0=12,a1=8;
	static float error,last_error;
	static long DaLin_u,last_DaLin_u,last_DaLin_u1,DaLin_result;
	error = mubiao - shiji;
	DaLin_u = b0*last_DaLin_u + b1*last_DaLin_u1 + a0*error+a1*last_error;大林算法表达式
	last_error = error;
	last_DaLin_u1 = last_DaLin_u;
	last_DaLin_u = DaLin_u;
	DaLin_result = DaLin_u;
	if(error>=-0.6&&error<=0.6)				 //开始保温计时
	{
	    if(wendu<=49.5)		flag=1;
	    if(wendu>49.5&&wendu<=59.5)		flag=3;
			if(wendu>59.5)		flag=5;
	}
	if(DaLin_result > 60)	 		//离目标温度太大（实际温度过小）
		DaLin_result = 60;       //限幅
	if(DaLin_result < 0)			//超过目标温度
		DaLin_result = 0;
		DaLin_result=(int)DaLin_result;      //强制转换成整数
	return DaLin_result;
	
}