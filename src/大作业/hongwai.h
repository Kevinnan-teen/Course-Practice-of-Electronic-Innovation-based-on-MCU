#ifndef __HONGWAI_H_
#define __HONGWAI_H_


#include<reg52.h>

extern bit irpro_ok;						//解码后4个字节数据接收完成标志位 
extern bit irok;
extern bit flag1;
extern unsigned int flag3;	
extern int flag_way;


void time0INIT();

void time0 (void);


void EX0INIT();

void EX0_ISR ();

void Ir_work(void); //红外键值散转程序

void Ircordpro(void); //红外码值处理函数



#endif



