#ifndef __PWM_H_
#define __PWM_H_

#include<reg52.h>

sbit _4IN = P1^6;					//pwm输出高低电平引脚
extern unsigned char pwm_motor_val;	//左右电机占空比值
extern unsigned char pwm_t;				//周期


void time1INIT();


void time1();

#endif