#include "MAX30100.h"
#include "delay.h"
/*
  MAX30100�������Գ���
*/
 
//��ʼ��I2C
void I2C_Init(void)
{		     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}
//����I2C��ʼ�ź�
void I2C_Start(void)
{
	SDA_OUT();     //sda�����
	I2C_SCL=1;
	I2C_SDA=1;	  	  
	delay_us(4);
 	I2C_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����I2Cֹͣ�ź�
void I2C_Stop(void)
{
	SDA_OUT();//sda�����
	I2C_SCL=0;
	I2C_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	I2C_SCL=1; 
	I2C_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	I2C_SDA=1;delay_us(1);	
	SDA_IN();      //SDA����Ϊ����     
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
	I2C_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//I2C����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I2C_Send_Byte(u8 txd)
{                        
    	u8 t;   
	SDA_OUT(); 	    
    I2C_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //I2C_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			I2C_SDA=1;
		else
			I2C_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2C_SCL=1;
		delay_us(2); 
		I2C_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        I2C_NAck();//����nACK
    else
        I2C_Ack(); //����ACK   
    return receive;
}
/*
Register_Address	max30100д�Ĵ�����ַ
Word_Data		��д�������
*/

u8 max30100_Bus_Write(u8 Register_Address , u8 Word_Data)
{

        /* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	_nop();
        /* ��1��������I2C���������ź� */
        I2C_Start();
	_nop();
        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        I2C_Send_Byte(max30100_WR_address | I2C_WR);        /* �˴���дָ�� */
	_nop();
        /* ��3��������ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM������Ӧ�� */
        }
	_nop();
        /* ��4�������ͼĴ�����ַ */
        I2C_Send_Byte(Register_Address);
	_nop();
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM������Ӧ�� */
        }
        _nop();
        /* ��5������ʼд������ */
        I2C_Send_Byte(Word_Data);
	_nop();
        /* ��6��������ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM������Ӧ�� */
        }
	_nop();
        /* ����I2C����ֹͣ�ź� */
        I2C_Stop();
	_nop();
        return 1;        /* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
        /* ����I2C����ֹͣ�ź� */
        I2C_Stop();
        return 0;
}

u8 max30100_Bus_Read(u8 Register_Address)
{
	u8 data;
	_nop();
        /* ��1��������I2C���������ź� */
        I2C_Start();
	_nop();
        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        I2C_Send_Byte(max30100_WR_address | I2C_WR);        /* �˴���дָ�� */
	_nop();
        /* ��3��������ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM������Ӧ�� */
        }
	_nop();
        /* ��4���������ֽڵ�ַ�� */
        I2C_Send_Byte((uint8_t)Register_Address);
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM������Ӧ�� */
        }
        
	_nop();
        /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
        I2C_Start();
	_nop();
        /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        I2C_Send_Byte(max30100_WR_address | I2C_RD);        /* �˴��Ƕ�ָ�� */
	_nop();
        /* ��8��������ACK */
        if (0 != I2C_Wait_Ack())
        {
                goto cmd_fail;        /* EEPROM������Ӧ�� */
        }
	_nop();
        /* ��9������ȡ���� */
        {
                data = I2C_Read_Byte();        /* ��1���ֽ� */

                I2C_NAck();        /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        }
	_nop();
        /* ����I2C����ֹͣ�ź� */
        I2C_Stop();
	_nop();
        return data;        /* ִ�гɹ� ����dataֵ */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
        /* ����I2C����ֹͣ�ź� */
        I2C_Stop();
        return 0;
}
//max30100��ʼ������
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