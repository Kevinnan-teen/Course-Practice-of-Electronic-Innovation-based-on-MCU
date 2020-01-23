#include"use1602.h"


sbit rs=P3^5;
sbit rw=P3^6;
sbit lcden=P3^4;

void write_com(uchar com)
{
	rs=0;		//rsΪ�͵�ƽ��д��� Ϊ�ߵ�ƽ�� д����
	rw=0;	   	//rwΪ�͵�ƽ��д���ݣ� Ϊ�ߵ�ƽ�� ������
	lcden=0;   	//���ݲ���ʱ��ͼ�� ��Ҫ�Ȱ�ʹ�ܶ���Ϊ0�� Ȼ������ݸ�P0�ڣ� �ٰ�ʹ�ܶ���Ϊ1
	P0=com;
	lcden=1;	//ʹ�ܶ˿�
	delayms(5);
	lcden=0;
}
void write_data(uchar dat)
{
	rs=1;
	rw=0;
	lcden=0;
	P0=dat;
	lcden=1;
	delayms(5);
	lcden=0;

}

void _1602INIT()
{
	write_com(0x38);
	write_com(0x08);
	write_com(0x01);
	write_com(0x06);
	write_com(0x0f);
}

unsigned char speed[] = "Speed:";

void displaySpeed(char s){
	int num;
	write_com(0X88);
	for( num = 0; num < 6; num++ ){
		write_data( speed[num] );
	}
	write_data(s);
}