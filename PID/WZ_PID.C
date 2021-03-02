float temp = 0.0f;                   //�������¶�,ȫ�ֱ���
void PidTempControl(void)
{
	float ee;
	float TempSet = 45.0f;           //Ŀ���¶�

	float PidE0 = 0.0f;
	float PidKp = 0.0f, PidKi = 0.0f, PidKd = 0.0f;
	float PidOut = 0.0f;
	float PidSum = 0.0f, I_out = 0.0f;
	static float PidE1 = 0.0f;

	PidKp = 15.0f;
	PidKi = 25.0f;
	PidKd = 0.0f;                         //��ʼ������

	PidE0 = TempSet - temp;       //����ƫ��
	ee = (PidE0 - PidE1);        //����һ��ƫ��

	if (ee > 2.0f) ee = 2.0f;
	else if (ee < -2.0f) ee = -2.0f;

	PidSum += PidE0;                               //ƫ��֮��

	if (PidSum > 5.0f) PidSum = 5.0f;
	else if (PidSum < -3.0f) PidSum = -3.0f;

	PidOut = PidKp * PidE0 + PidKd * ee;       //����PID�ı�����΢�����

	if (fabs(PidE0) < 3.0f)                     //����¶����С��3�������PID�������
	{
		if (PidSum > 5.0f) PidSum = 5.0f;
		else if (PidSum < -2.0f) PidSum = -2.0f;

		I_out = PidKi * PidSum;                      //�������


		if (fabs(PidE0) < -1.0f)                       //��ǰ�¶ȸ����趨�¶�0.5��ʱ���ۼƻ�������
		{
			if (PidSum > 5.5f) PidSum = 5.5f;
			if (PidSum > 0)    I_out -= 1;               //��ǰ�¶ȸ����趨�¶�0.5��ʱ,�������������
		}

		PidOut += I_out;
	}
	else
	{
		PidSum = 0.0f;
	}

	PidOut *= 500;                                                 //��������ı���ϵ��
	if (PidOut > 2500) PidOut = 2500;
	if (PidOut < 0)    PidOut = 400;
	if (PidE0 > 5.0f) PidOut = 2500;
	if (PidE0 < -2) PidOut = 0;

	HeaterSet(PidOut);                              //���������
	PidE1 = PidE0;
}