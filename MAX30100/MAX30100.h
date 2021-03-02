/*
   Ѫ�������Ŷ��壺
	SCL����PB6
	SDA����PB7
	INT����PB8
*/
#include "sys.h"
#include <stdio.h>
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int

//IO��������
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define I2C_SCL    PBout(6) //SCL
#define I2C_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 
//max30100��ַ
#define max10300_WR_address 0xAE
#define I2C_WR 0
#define I2C_RD 1

#define SAMPLE_50 //��������Ϊ50��/�룬ע�͵�Ϊ100��/��
int rda;//��ȡ����I2C����
//I2C���в�������
void I2C_Init(void);                //��ʼ��I2C��IO��				 
void I2C_Start(void);				//����I2C��ʼ�ź�
void I2C_Stop(void);	  			//����I2Cֹͣ�ź�
void I2C_Send_Byte(u8 txd);			//I2C����һ���ֽ�
u8 I2C_Read_Byte(unsigned char ack);//I2C��ȡһ���ֽ�
u8 I2C_Wait_Ack(void); 				//I2C�ȴ�ACK�ź�
void I2C_Ack(void);					//I2C����ACK�ź�
void I2C_NAck(void);				//I2C������ACK�ź�

void max30100_init();	//max30100��ʼ������
u8 max30100_Bus_Write(u8 Register_Address , u8 Word_Data);//��max30100��Register_Address�Ĵ�����д��Word_Data��1�ֽڣ�
u8 max30100_Bus_Read(u8 Register_Address);//��Register_Address�Ĵ����ж�ȡ1�ֽ�����
#endif