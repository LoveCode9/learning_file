#include	"pt100font.h"
#include	"pt100.h"
#include	"adc.h"
#include	"sys.h"
__IO uint16_t    ADC_RegularConvertedValueTab[9];
u8 ADCflag = 0;	//��ȡ�¶ȱ�־λ	0��ʧ�ܣ�1�ɹ�
//pt100������ʼ��
void	pt100_Init()
{
//	ADC1_SingleChannel(ADC_Channel_2);
//	ADC1_SingleChannel(ADC_Channel_3);
//	Adc_Init();
	
	//����adc1Ϊ��ɨ��ģʽ��ͨ��2��3ʹ��
		ADC1_ContinuousScan();
		GPIO_Configuration();
		DMA_Configuration();
}

//��ȡ��ѹֵ,3.0vΪ����
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

/*ͬ������Ŵ�vo=��1+Rf/R1)v-=��1+Rf/R1)vs
	��ԭ��ͼ��Ӧ�Ĺ�ʽ��R=2000/(36/Vout-1)
*/
//��ѹ-�¶�ת��
signed	int	pt100_v_T(float	*V_Data)
{
	
	signed	int	temp;	//�¶�
	temp=35+((*V_Data-1.9985348)*166.7);
	return	temp;
	
		
}

//��ȡ��ǰ�¶�ֵ��T_before��ǰ���¶ȡ�T_after������¶�
//0����ȡʧ�ܡ�1:ǰ�˻�ȡ�ɹ�����2����˻�ȡ�ɹ���3��ȫ����ȡ�ɹ�
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

//��ȡ���times�ε��¶�ƽ��ֵ
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