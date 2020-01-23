#include"useDS18B20.h"
#include"use1602.h"
#include"delay.h"

sbit DQ =P2^2;  //定义DS18B20通信端口

unsigned char code temperature[]="Temp:";



void delay(unsigned int i)
{
	while(i--);
}

//初始化函数
void Init_DS18B20(void)
{
	unsigned char x=0;
	DQ = 1; //DQ复位
	delay(8); //稍做延时
	DQ = 0; //单片机将DQ拉低
	delay(80); //精确延时 大于 480us
	DQ = 1; //拉高总线
	delay(14);
	x=DQ; //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	delay(20);
}

//读一个字节
char ReadOneChar()
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--){
		DQ = 0; // 给脉冲信号
		dat>>=1;
		DQ = 1; // 给脉冲信号
		if(DQ)  dat|=0x80;
		delay(4);
		}
	return(dat);
}

//写一个字节
void WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--){
		DQ = 0;
		DQ = dat&0x01;
		delay(5);
		DQ = 1;
		dat>>=1;
		}
}

int ReadTemperature()
{
	int num;
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	float tt=0;
	Init_DS18B20();
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0x44); // 启动温度转换
	Init_DS18B20();
	WriteOneChar(0xCC); //跳过读序号列号的操作
	WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	a=ReadOneChar();
	b=ReadOneChar();
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625; //将温度的高位与低位合并
	t= tt*10+0.5; //对结果进行4舍5入
	//在1602上显示温度
	write_com(0X80 + 0X40);            
	for(num=0;num<5;num++)
	{
		write_data(temperature[num]);		
	}
	write_data( t/100 + '0' );
	write_data( t%100/10 + '0' );
	write_data( t%10 + '0' );
	delayms(1000);
	return t;
}


