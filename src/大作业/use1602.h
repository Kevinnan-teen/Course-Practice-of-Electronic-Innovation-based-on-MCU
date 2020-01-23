#ifndef __USE1602_H_
#define __USE1602_H_
#include<reg52.h>
#include"delay.h"

void write_com(uchar com);

void write_data(uchar dat);

void _1602INIT();

void displaySpeed(char s);


#endif