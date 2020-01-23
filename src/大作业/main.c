#include<reg52.h>    	
#include"delay.h"
#include"guangming.h"
#include"use1602.h"
#include"hongwai.h"
#include"pwm.h"
#include"useDS18B20.h"


#define DataPort P0 				//用于关闭数码管
sbit dula = P2^6;
sbit wela = P2^7;


sbit led1 = P1^0;					//通过红外控制
bit flag1 = 1;						//红外控制灯标志位
int flag_way = 0;
unsigned int flag3 = 100; 			//红外控制直流电机标志位
bit irpro_ok;						//解码后4个字节数据接收完成标志位 
bit irok;


unsigned char pwm_motor_val = 255;	//左右电机占空比值
unsigned char pwm_t;				//周期


//sbit beep = P2^3;					//蜂鸣器


sbit led2 = P1^1;					//通过光敏来调节灯

sbit led3 = P1^2;


/*void beep_voice(){
	beep = 0;
	delayms(500);
	beep = 1;
	delayms(1000);
}*/


void main(void)
{
	EX0INIT(); //初始化外部中断
	time0INIT();//初始化定时器
	time1INIT();
	DataPort = 0;
	dula = 0;
	wela = 0;		//关闭数码管
	_1602INIT();	//1602初始化*/
	while(1)//主循环
	{
		if(irok)                        //如果接收好了进行红外处理
		{   
			Ircordpro();
			irok=0;
		}
		if(irpro_ok)                   //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
		{
			Ir_work();
		}
		if( flag1 == 1 ){
			led1 = 0;		//开灯
		}else if(flag1 == 0){
			led1 = 1;		//关灯
		}
		
		if( flag_way == 1 ){
			//三级调速
			if( flag3 == 1 ){
				pwm_motor_val = 200;	//慢速
				displaySpeed('S');
			}else if( flag3 == 2 ){
				pwm_motor_val = 120;	//中速
				displaySpeed('M');
			}else if( flag3 == 3 ){
				pwm_motor_val = 30;		//快速
				displaySpeed('F');
			}else if( flag3 == 0 ){
				pwm_motor_val = 255;
				displaySpeed('P');
				//beep_voice();//停止
			}
		}else if( flag_way == 2 ){
			if (getReMinValue() < 125 ){
				pwm_motor_val = 30;	
			}else{
				pwm_motor_val = 255;	
			}
		}else{
			pwm_motor_val = 255;	
		}
		//得到光敏电阻值大小， 并在1602显示
		if ( getGuangmingValue() > 200 ){
			led2 = 0;
		}else{
			led2 = 1;
		}
		//得到温度值， 在1602显示
		/*if (ReadTemperature() > 250 ){
			led3 = 0;
		}else{
			led3 = 1;
		}*/
			
	}
}