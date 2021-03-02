#include	"pt100font.h"
#include	"pt100.h"
#include	"adc.h"
#include	"sys.h"
__IO uint16_t    ADC_RegularConvertedValueTab[9];
u8 ADCflag = 0;	//读取温度标志位	0：失败，1成功
//pt100参数初始化
void	pt100_Init()
{
//	ADC1_SingleChannel(ADC_Channel_2);
//	ADC1_SingleChannel(ADC_Channel_3);
//	Adc_Init();
	
	//配置adc1为单扫描模式，通道2，3使能
		ADC1_ContinuousScan();
		GPIO_Configuration();
		DMA_Configuration();
}

//获取电压值,3.0v为量程
u8	pt100_Get_v(float *Data_v_before,float *Data_v_after)
{
	unsigned int	V_before;
	unsigned	int	V_after;
	float	Del_v;
	if(1==ADCflag)
	{
		V_before=ADC_RegularConvertedValueTab[0];
		V_after=ADC_RegularConvertedValueTab[1];
		Del_v	=(float)(3.0/4096);
		*Data_v_before	=	(float)(V_before)*Del_v;
		*Data_v_after	=	(float)(V_after)*Del_v;
		return	1;
	}
		return	0;

}
u8	pt100_Get_v_before(float *Data_v_before)
{
	
//		if(1	==	ADCflag)
//			{
//				ADCflag	=	0;
//				T_before=ADC_RegularConvertedValueTab[0];
//				T_after=ADC_RegularConvertedValueTab[1];
//			}
	unsigned int	V_before;
	float	Del_v;
	if(1==ADCflag)
	{
		V_before=ADC_RegularConvertedValueTab[0];
		Del_v	=(float)(3.0/4096);
		Del_v	=	(float)(V_before)*Del_v;
		*Data_v_before	=	Del_v;
		return	1;
	}
		return	0;		
}

u8	pt100_Get_v_after(float *Data_v_after)
{
	
	unsigned int	V_after;
	float	Del_v;
	if(1==ADCflag)
	{
		V_after=ADC_RegularConvertedValueTab[1];
		Del_v	=(float)(3.0/4096);
		Del_v	=	(float)(V_after)*Del_v;
		*Data_v_after	=	Del_v;
		return	1;
	}
	return	0;
}

/*同向比例放大vo=（1+Rf/R1)v-=（1+Rf/R1)vs
	与原理图对应的公式：R=2000/(36/Vout-1)
*/
//电压-温度转换
signed	int	pt100_v_T(float	*V_Data)
{
	
	signed	int	temp;	//温度
	temp=35+((*V_Data-1.9985348)*166.7);
	return	temp;
	
		
}

//获取当前温度值，T_before：前端温度、T_after：后端温度
//0：获取失败、1:前端获取成功、、2：后端获取成功、3：全部获取成功
u8	pt100_Get_T(signed	int *T_before,signed	int *T_after)
{
	float Data_v_before;
	float Data_v_after;
	int	sign=0;	
//	pt100_Get_v(&Data_v_before,&Data_v_after);
		sign	+=	pt100_Get_v_before(&Data_v_before);
		*T_before	=	pt100_v_T(&Data_v_before);
		sign	+=	pt100_Get_v_after(&Data_v_after)+1;
		*T_after	=	pt100_v_T(&Data_v_after);
	return	sign;
}
u8	pt100_Get_T_before(signed	int *T_before)
{
	float Data_v_before;
	int	sign=0;
	
	sign=pt100_Get_v_before(&Data_v_before);
	if(sign)
	{
		*T_before	=	pt100_v_T(&Data_v_before);
		
	}
	
	return	sign;
}

u8	pt100_Get_T_after(signed	int *T_after)
{
	float Data_v_after;
	int	sign=0;
	
	if(pt100_Get_v_after(&Data_v_after))
	{
		*T_after	=	pt100_v_T(&Data_v_after);
		sign++;
	}
	
	return	sign;
}

//获取最近times次的温度平均值
u8	pt100_Get_Taverage(signed	int *T_before,signed	int *T_after,u8	times)
{
	int	sign=0;
	int	t_before,t_after;
	u8	i,j,k;
	t_before=0;
	t_after=0;
	i=0;
	j=0;
	k=0;
	for(;i<times;i++)
	{
		sign=pt100_Get_T(T_before,T_after);
		switch(sign)
		{	
					case	1:
					{
						j++;
						t_before	+=	*T_before;
					}
						break;
					case	2:
					{
						k++;
						t_after		+=	*T_after;
					}
						break;
					case	3:
					{
						j++;
						t_before	+=	*T_before;
						k++;
						t_after		+=	*T_after;
					}
					break;
					default	:
						
					break;
					
		}
	}
	sign	=	0;
	if(0	!=	j)
	{
	*T_before	=	t_before/j;
		sign	+=	1;
	}
	if(0	!=	k)
	{
	*T_after	=	t_after/k;
		sign	+=	2;
	}
	return	sign;
	
}