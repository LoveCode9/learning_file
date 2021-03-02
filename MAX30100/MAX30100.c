#include "MAX30100.h"
#include "delay.h"
/*
  MAX30100驱动测试程序
*/
 
//初始化I2C
void I2C_Init(void)
{		     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
}
//产生I2C起始信号
void I2C_Start(void)
{
	SDA_OUT();     //sda线输出
	I2C_SCL=1;
	I2C_SDA=1;	  	  
	delay_us(4);
 	I2C_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生I2C停止信号
void I2C_Stop(void)
{
	SDA_OUT();//sda线输出
	I2C_SCL=0;
	I2C_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	I2C_SCL=1; 
	I2C_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	I2C_SDA=1;delay_us(1);	
	SDA_IN();      //SDA设置为输入     
	I2C_SCL=1;delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void I2C_Ack(void)
{
	I2C_SCL=0;
	SDA_OUT();
	I2C_SDA=0;
	delay_us(2);
	I2C_SCL=1;
	delay_us(2);
	I2C_SCL=0;
}
//不产生ACK应答		    
void I2C_NAck(void)
{
	I2C_SCL=0;
	SDA_OUT();
	I2C_SDA=1;
	delay_us(2);
	I2C_SCL=1;
	delay_us(2);
	I2C_SCL=0;
}					 				     
//I2C发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void I2C_Send_Byte(u8 txd)
{                        
    	u8 t;   
	SDA_OUT(); 	    
    I2C_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //I2C_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			I2C_SDA=1;
		else
			I2C_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		I2C_SCL=1;
		delay_us(2); 
		I2C_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        I2C_SCL=0; 
        delay_us(2);
		I2C_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        I2C_NAck();//发送nACK
    else
        I2C_Ack(); //发送ACK   
    return receive;
}
/*
Register_Address	max30100写寄存器地址
Word_Data		待写入的数据
*/

u8 max30100_Bus_Write(u8 Register_Address , u8 Word_Data)
{

        /* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */
	_nop();
        /* 第1步：发起I2C总线启动信号 */
        I2C_Start();
	_nop();
        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        I2C_Send_Byte(max30100_WR_address | I2C_WR);        /* 此处是写指令 */
	_nop();
        /* 第3步：发送ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM器件无应答 */
        }
	_nop();
        /* 第4步：发送寄存器地址 */
        I2C_Send_Byte(Register_Address);
	_nop();
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM器件无应答 */
        }
        _nop();
        /* 第5步：开始写入数据 */
        I2C_Send_Byte(Word_Data);
	_nop();
        /* 第6步：发送ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM器件无应答 */
        }
	_nop();
        /* 发送I2C总线停止信号 */
        I2C_Stop();
	_nop();
        return 1;        /* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
        /* 发送I2C总线停止信号 */
        I2C_Stop();
        return 0;
}

u8 max30100_Bus_Read(u8 Register_Address)
{
	u8 data;
	_nop();
        /* 第1步：发起I2C总线启动信号 */
        I2C_Start();
	_nop();
        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        I2C_Send_Byte(max30100_WR_address | I2C_WR);        /* 此处是写指令 */
	_nop();
        /* 第3步：发送ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM器件无应答 */
        }
	_nop();
        /* 第4步：发送字节地址， */
        I2C_Send_Byte((uint8_t)Register_Address);
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM器件无应答 */
        }
        
	_nop();
        /* 第6步：重新启动I2C总线。下面开始读取数据 */
        I2C_Start();
	_nop();
        /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        I2C_Send_Byte(max30100_WR_address | I2C_RD);        /* 此处是读指令 */
	_nop();
        /* 第8步：发送ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM器件无应答 */
        }
	_nop();
        /* 第9步：读取数据 */
        {
                data = I2C_Read_Byte();        /* 读1个字节 */

                I2C_NAck();        /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        }
	_nop();
        /* 发送I2C总线停止信号 */
        I2C_Stop();
	_nop();
        return data;        /* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
        /* 发送I2C总线停止信号 */
        I2C_Stop();
        return 0;
}
//max30100初始化函数
void max30100_init()
{
        max30100_Bus_Write(0x06, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
        //max30100_Bus_Write(0x06, 0x0a);  //MODE[2:0]=010 HR only enabled     when used is mode ,the red led is not used.
        max30100_Bus_Write(0x01, 0xF0); //open all of interrupt
        max30100_Bus_Write(INTERRUPT_REG, 0x00); //all interrupt clear
        max30100_Bus_Write(0x09, 0x33); //r_pa=3,ir_pa=3

#ifdef SAMPLE_50
        max30100_Bus_Write(0x07, 0x43); //SPO2_SR[4:2]=000   50 per second    LED_PW[1:0]=11  16BITS
#else
        max30100_Bus_Write(0x07, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
#endif
        
        max30100_Bus_Write(0x02, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
        max30100_Bus_Write(0x03, 0x00);        //set Over Flow Counter  reg = 0x00 for clear it
        max30100_Bus_Write(0x04, 0x0f);        //set FIFO Read Pointer  reg = 0x0f for   
                                                                                        //waitting  write pointer eq read pointer   to   interrupts  INTERRUPT_REG_A_FULL
}