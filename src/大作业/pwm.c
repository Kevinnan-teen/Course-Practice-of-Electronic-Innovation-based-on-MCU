#include"pwm.h"

/*********************************
函数名：time1INIT()
功能：定时器1初始化函数
参数：无
返回值：无
*********************************/
void time1INIT(){
	TMOD = TMOD | 0X20;
	TH1 = 220;
	TL1 = 220;
	TR1 = 1;
	ET1 = 1;
	EA = 1;
}


/*********************************
函数名：time1()
功能：定时器1中断函数
参数：无
返回值：无
*********************************/
void time1() interrupt 3
{
	pwm_t++;
	if(pwm_t == 255){
		pwm_t = 0;
		_4IN = 0;
	}
	if(pwm_motor_val == pwm_t)
		_4IN = 1;
}