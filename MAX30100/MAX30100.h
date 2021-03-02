/*
   血氧仪引脚定义：
	SCL——PB6
	SDA——PB7
	INT——PB8
*/
#include "sys.h"
#include <stdio.h>
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int

//IO方向设置
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define I2C_SCL    PBout(6) //SCL
#define I2C_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 
//max30100地址
#define max10300_WR_address 0xAE
#define I2C_WR 0
#define I2C_RD 1

#define SAMPLE_50 //采样次数为50次/秒，注释掉为100次/秒
int rda;//读取到的I2C数据
//I2C所有操作函数
void I2C_Init(void);                //初始化I2C的IO口				 
void I2C_Start(void);				//发送I2C开始信号
void I2C_Stop(void);	  			//发送I2C停止信号
void I2C_Send_Byte(u8 txd);			//I2C发送一个字节
u8 I2C_Read_Byte(unsigned char ack);//I2C读取一个字节
u8 I2C_Wait_Ack(void); 				//I2C等待ACK信号
void I2C_Ack(void);					//I2C发送ACK信号
void I2C_NAck(void);				//I2C不发送ACK信号

void max30100_init();	//max30100初始化函数
u8 max30100_Bus_Write(u8 Register_Address , u8 Word_Data);//向max30100的Register_Address寄存器中写入Word_Data（1字节）
u8 max30100_Bus_Read(u8 Register_Address);//向Register_Address寄存器中读取1字节数据
#endif