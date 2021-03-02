float temp = 0.0f;                   //传感器温度,全局变量
void PidTempControl(void)
{
	float ee;
	float TempSet = 45.0f;           //目标温度

	float PidE0 = 0.0f;
	float PidKp = 0.0f, PidKi = 0.0f, PidKd = 0.0f;
	float PidOut = 0.0f;
	float PidSum = 0.0f, I_out = 0.0f;
	static float PidE1 = 0.0f;

	PidKp = 15.0f;
	PidKi = 25.0f;
	PidKd = 0.0f;                         //初始化参数

	PidE0 = TempSet - temp;       //本次偏差
	ee = (PidE0 - PidE1);        //计算一阶偏差

	if (ee > 2.0f) ee = 2.0f;
	else if (ee < -2.0f) ee = -2.0f;

	PidSum += PidE0;                               //偏差之和

	if (PidSum > 5.0f) PidSum = 5.0f;
	else if (PidSum < -3.0f) PidSum = -3.0f;

	PidOut = PidKp * PidE0 + PidKd * ee;       //计算PID的比例和微分输出

	if (fabs(PidE0) < 3.0f)                     //如果温度相差小于3度则计入PID积分输出
	{
		if (PidSum > 5.0f) PidSum = 5.0f;
		else if (PidSum < -2.0f) PidSum = -2.0f;

		I_out = PidKi * PidSum;                      //积分输出


		if (fabs(PidE0) < -1.0f)                       //当前温度高于设定温度0.5度时，累计积分限制
		{
			if (PidSum > 5.5f) PidSum = 5.5f;
			if (PidSum > 0)    I_out -= 1;               //当前温度高于设定温度0.5度时,消弱积分正输出
		}

		PidOut += I_out;
	}
	else
	{
		PidSum = 0.0f;
	}

	PidOut *= 500;                                                 //与控制量的比例系数
	if (PidOut > 2500) PidOut = 2500;
	if (PidOut < 0)    PidOut = 400;
	if (PidE0 > 5.0f) PidOut = 2500;
	if (PidE0 < -2) PidOut = 0;

	HeaterSet(PidOut);                              //输出控制量
	PidE1 = PidE0;
}