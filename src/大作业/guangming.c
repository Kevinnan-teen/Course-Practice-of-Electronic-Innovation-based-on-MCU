#include"guangming.h"
#include"delay.h"
#include"use1602.h"
#include<reg52.h>
#include<intrins.h>
#define	I2cRead 1		 //I2C读方向位
#define	I2cWrite 0		 //I2C写方向位
#define	CH0 0			 //AD通道0
#define	CH1	1			 //AD通道1
#define	CH2	2			 //AD通道2
#define	CH3	3			 //AD通道3
#define	DAout	0x40	 //DA输出命令

#define PCF8591ADDR 0X90 //PCF8591硬件地址

sbit SCL = P2^1; 		 //I2C时钟总线
sbit SDA = P2^0; 		 //I2C数据总线

uchar i, j;
uchar num;
uchar target;	//电位器显示
uchar number;	//温度和光敏示数


//bit write=0;
bit AckFlag;//应答标志位

uchar code light[]="Lig:";



void I2cStart()
{
//时钟总线为高电平期间数据总线又高变低产生起始型号
	SCL = 1; 
	SDA = 1;
	delay5us(1);//状态保持5us
	SDA = 0;
	delay5us(1);//状态保持5us 
}


void I2cStop()
{
//时钟总线为高电平期间，数据总线从高变低产生终止信号
	SCL = 0;
	SDA = 0;
	SCL = 1;
	delay5us(1);//状态保持5us
	SDA = 1;
	delay5us(1);//状态保持5us	
}

bit ReadACK()
{
	SCL = 0;//拉低时钟总线，允许从机控制SDA
	SCL = 1;//拉高，读SDA
	delay5us(1);
	if(SDA)//NOACK
	{
		SCL = 0;
		return(1);//返回1
	}
	else//ACK
	{
		SCL = 0;
		return(0);//返回0
	}
}



void SendACK(bit i)
{
	SCL = 0;//拉低时钟总线，允许主机控制SDA
	if(i)	//发非应答
		SDA = 1;
	else	//发应答
		SDA = 0;
	SCL = 1;  //拉高总线，让从机读SDA
	delay5us(1);//保持5us
	SCL = 0; //拉低时钟总线，允许SDA释放
	SDA = 1;//释放数据总线
}



void I2cSendByte(uchar DAT)
{
	for(i=0; i<8; i++) //分别写8次，每次写1位
	{
		SCL = 0;//拉低时钟总线，允许SDA变化
		if(DAT & 0x80)//先写数据最高位
			SDA = 1;  //写1
		else
			SDA = 0;  //写0
		SCL = 1;	  //拉高时钟，让从机读SDA
		DAT <<= 1;	  //为发送下一位左移1位
	}
	SCL = 0; //拉低时钟总线，允许SDA释放
	SDA = 1;//释放数据总线
}


uchar I2cReadByte()
{
	uchar i, DAT;
	for(i=0; i<8; i++)//分别读8次，每次读一位
	{
		DAT <<= 1; //数据左移1位，准备接收一位
		SCL = 0;   //拉低时钟总线，允许从机控制SDA变化
		SCL = 1;   //拉高时钟总线，读取SDA上的数据
		if(SDA)
			DAT |= 0X01;//为1则写1，否则不行执行写1，通过左移补0
	}
	return(DAT); //返回读出的数据
}

uchar PCF8591Read(uchar Ctrl)
{
	uchar DAT;
	I2cStart();//I2C起始信号
	I2cSendByte(PCF8591ADDR + I2cWrite);//发送器件地址加读写方向位
	if(ReadACK())//读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	I2cSendByte(Ctrl);//发送控制字节
	ReadACK();//读从机应答
	I2cStart();//再次产生I2C起始信号
	I2cSendByte(PCF8591ADDR + I2cRead);//发送器件地址加读写方向位读
	if(ReadACK())//读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	DAT = I2cReadByte();//读一字节
	SendACK(1);//主机发送非应答
	I2cStop(); //I2C停止信号
	return(DAT);//返回读出数据
			
}


uint getGuangmingValue(){
	uint a, b, c;
	write_com(0X80);		//默认为0x80
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
	write_com(0X80 + 0X40);		//默认为0x80
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
