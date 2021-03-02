#ifndef	__PT100_H
#define	__PT100_H
#include	"sys.h"



void	pt100_Init(void);
//获取电压值
u8	pt100_Get_v_before(float *Data_v_before); 
u8	pt100_Get_v_after(float *Data_v_after);
u8	pt100_Get_v(float *Data_v_before,float *Data_v_after);

signed	int	pt100_v_T(float	*V_Data);

u8	pt100_Get_T_before(signed	int *T_before);
u8	pt100_Get_T_after(signed	int *T_after);

u8	pt100_Get_T(signed	int *T_before,signed	int *T_after);

u8	pt100_Get_Taverage(signed	int *T_before,signed	int *T_after,u8	times);
#endif

