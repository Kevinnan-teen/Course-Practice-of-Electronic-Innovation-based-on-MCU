#include<reg52.h>    	
#include"delay.h"
#include"guangming.h"
#include"use1602.h"
#include"hongwai.h"
#include"pwm.h"
#include"useDS18B20.h"


#define DataPort P0 				//���ڹر������
sbit dula = P2^6;
sbit wela = P2^7;


sbit led1 = P1^0;					//ͨ���������
bit flag1 = 1;						//������ƵƱ�־λ
int flag_way = 0;
unsigned int flag3 = 100; 			//�������ֱ�������־λ
bit irpro_ok;						//�����4���ֽ����ݽ�����ɱ�־λ 
bit irok;


unsigned char pwm_motor_val = 255;	//���ҵ��ռ�ձ�ֵ
unsigned char pwm_t;				//����


//sbit beep = P2^3;					//������


sbit led2 = P1^1;					//ͨ�����������ڵ�

sbit led3 = P1^2;


/*void beep_voice(){
	beep = 0;
	delayms(500);
	beep = 1;
	delayms(1000);
}*/


void main(void)
{
	EX0INIT(); //��ʼ���ⲿ�ж�
	time0INIT();//��ʼ����ʱ��
	time1INIT();
	DataPort = 0;
	dula = 0;
	wela = 0;		//�ر������
	_1602INIT();	//1602��ʼ��*/
	while(1)//��ѭ��
	{
		if(irok)                        //������պ��˽��к��⴦��
		{   
			Ircordpro();
			irok=0;
		}
		if(irpro_ok)                   //�������ú���й��������簴��Ӧ�İ�������ʾ��Ӧ�����ֵ�
		{
			Ir_work();
		}
		if( flag1 == 1 ){
			led1 = 0;		//����
		}else if(flag1 == 0){
			led1 = 1;		//�ص�
		}
		
		if( flag_way == 1 ){
			//��������
			if( flag3 == 1 ){
				pwm_motor_val = 200;	//����
				displaySpeed('S');
			}else if( flag3 == 2 ){
				pwm_motor_val = 120;	//����
				displaySpeed('M');
			}else if( flag3 == 3 ){
				pwm_motor_val = 30;		//����
				displaySpeed('F');
			}else if( flag3 == 0 ){
				pwm_motor_val = 255;
				displaySpeed('P');
				//beep_voice();//ֹͣ
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
		//�õ���������ֵ��С�� ����1602��ʾ
		if ( getGuangmingValue() > 200 ){
			led2 = 0;
		}else{
			led2 = 1;
		}
		//�õ��¶�ֵ�� ��1602��ʾ
		/*if (ReadTemperature() > 250 ){
			led3 = 0;
		}else{
			led3 = 1;
		}*/
			
	}
}