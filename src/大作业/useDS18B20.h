#ifndef __USEDS18B20_H_
#define __USEDS18B20_H_
#include<reg52.h>

void delay(unsigned int i);

char ReadOneChar();

void WriteOneChar(unsigned char dat);

int ReadTemperature();



#endif