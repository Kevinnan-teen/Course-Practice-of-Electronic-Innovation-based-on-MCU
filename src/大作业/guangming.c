#include"guangming.h"
#include"delay.h"
#include"use1602.h"
#include<reg52.h>
#include<intrins.h>
#define	I2cRead 1		 //I2C������λ
#define	I2cWrite 0		 //I2Cд����λ
#define	CH0 0			 //ADͨ��0
#define	CH1	1			 //ADͨ��1
#define	CH2	2			 //ADͨ��2
#define	CH3	3			 //ADͨ��3
#define	DAout	0x40	 //DA�������

#define PCF8591ADDR 0X90 //PCF8591Ӳ����ַ

sbit SCL = P2^1; 		 //I2Cʱ������
sbit SDA = P2^0; 		 //I2C��������

uchar i, j;
uchar num;
uchar target;	//��λ����ʾ
uchar number;	//�¶Ⱥ͹���ʾ��


//bit write=0;
bit AckFlag;//Ӧ���־λ

uchar code light[]="Lig:";



void I2cStart()
{
//ʱ������Ϊ�ߵ�ƽ�ڼ����������ָ߱�Ͳ�����ʼ�ͺ�
	SCL = 1; 
	SDA = 1;
	delay5us(1);//״̬����5us
	SDA = 0;
	delay5us(1);//״̬����5us 
}


void I2cStop()
{
//ʱ������Ϊ�ߵ�ƽ�ڼ䣬�������ߴӸ߱�Ͳ�����ֹ�ź�
	SCL = 0;
	SDA = 0;
	SCL = 1;
	delay5us(1);//״̬����5us
	SDA = 1;
	delay5us(1);//״̬����5us	
}

bit ReadACK()
{
	SCL = 0;//����ʱ�����ߣ�����ӻ�����SDA
	SCL = 1;//���ߣ���SDA
	delay5us(1);
	if(SDA)//NOACK
	{
		SCL = 0;
		return(1);//����1
	}
	else//ACK
	{
		SCL = 0;
		return(0);//����0
	}
}



void SendACK(bit i)
{
	SCL = 0;//����ʱ�����ߣ�������������SDA
	if(i)	//����Ӧ��
		SDA = 1;
	else	//��Ӧ��
		SDA = 0;
	SCL = 1;  //�������ߣ��ôӻ���SDA
	delay5us(1);//����5us
	SCL = 0; //����ʱ�����ߣ�����SDA�ͷ�
	SDA = 1;//�ͷ���������
}



void I2cSendByte(uchar DAT)
{
	for(i=0; i<8; i++) //�ֱ�д8�Σ�ÿ��д1λ
	{
		SCL = 0;//����ʱ�����ߣ�����SDA�仯
		if(DAT & 0x80)//��д�������λ
			SDA = 1;  //д1
		else
			SDA = 0;  //д0
		SCL = 1;	  //����ʱ�ӣ��ôӻ���SDA
		DAT <<= 1;	  //Ϊ������һλ����1λ
	}
	SCL = 0; //����ʱ�����ߣ�����SDA�ͷ�
	SDA = 1;//�ͷ���������
}


uchar I2cReadByte()
{
	uchar i, DAT;
	for(i=0; i<8; i++)//�ֱ��8�Σ�ÿ�ζ�һλ
	{
		DAT <<= 1; //��������1λ��׼������һλ
		SCL = 0;   //����ʱ�����ߣ�����ӻ�����SDA�仯
		SCL = 1;   //����ʱ�����ߣ���ȡSDA�ϵ�����
		if(SDA)
			DAT |= 0X01;//Ϊ1��д1��������ִ��д1��ͨ�����Ʋ�0
	}
	return(DAT); //���ض���������
}

uchar PCF8591Read(uchar Ctrl)
{
	uchar DAT;
	I2cStart();//I2C��ʼ�ź�
	I2cSendByte(PCF8591ADDR + I2cWrite);//����������ַ�Ӷ�д����λ
	if(ReadACK())//���ӻ�Ӧ��
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	I2cSendByte(Ctrl);//���Ϳ����ֽ�
	ReadACK();//���ӻ�Ӧ��
	I2cStart();//�ٴβ���I2C��ʼ�ź�
	I2cSendByte(PCF8591ADDR + I2cRead);//����������ַ�Ӷ�д����λ��
	if(ReadACK())//���ӻ�Ӧ��
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	DAT = I2cReadByte();//��һ�ֽ�
	SendACK(1);//�������ͷ�Ӧ��
	I2cStop(); //I2Cֹͣ�ź�
	return(DAT);//���ض�������
			
}


uint getGuangmingValue(){
	uint a, b, c;
	write_com(0X80);		//Ĭ��Ϊ0x80
	for(num=0;num<4;num++)
	{
		write_data(light[num]);		
	}
	target =  PCF8591Read(CH0);
	PCF8591Read(CH1);
	PCF8591Read(CH2);
	PCF8591Read(CH3);
	delayms(300);
	a = target/100;
	b = (target%100)/10;
	c = target%10;
	write_data( target/100 + '0' );
	write_data((target%100)/10 + '0');
	write_data( target%10 + '0' );
	delayms(1000);
	return a * 100 + b * 10 + c;
}

unsigned char code temperature1[]="Temp:";

uint getReMinValue(){
	uint a, b, c;
	write_com(0X80 + 0X40);		//Ĭ��Ϊ0x80
	for(num=0;num<5;num++)
	{
		write_data(temperature1[num]);		
	}
	PCF8591Read(CH0);
	PCF8591Read(CH1);
	PCF8591Read(CH2);
	target =  PCF8591Read(CH3);
	delayms(300);
	a = target/100;
	b = (target%100)/10;
	c = target%10;
	write_data( target/100 + '0' );
	write_data((target%100)/10 + '0');
	write_data( target%10 + '0' );
	delayms(1000);
	return a * 100 + b * 10 + c;
}
