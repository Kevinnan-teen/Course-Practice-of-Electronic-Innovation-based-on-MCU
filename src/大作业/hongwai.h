#ifndef __HONGWAI_H_
#define __HONGWAI_H_


#include<reg52.h>

extern bit irpro_ok;						//�����4���ֽ����ݽ�����ɱ�־λ 
extern bit irok;
extern bit flag1;
extern unsigned int flag3;	
extern int flag_way;


void time0INIT();

void time0 (void);


void EX0INIT();

void EX0_ISR ();

void Ir_work(void); //�����ֵɢת����

void Ircordpro(void); //������ֵ������



#endif



