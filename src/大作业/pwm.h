#ifndef __PWM_H_
#define __PWM_H_

#include<reg52.h>

sbit _4IN = P1^6;					//pwm����ߵ͵�ƽ����
extern unsigned char pwm_motor_val;	//���ҵ��ռ�ձ�ֵ
extern unsigned char pwm_t;				//����


void time1INIT();


void time1();

#endif