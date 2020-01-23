#ifndef __GUANGMING_H_
#define __GUANGMING_H_
#include"delay.h"

typedef unsigned int uint;


void I2cStart();

void I2cStop();

bit ReadACK();

void SendACK(bit i);

void I2cSendByte(uchar DAT);

uchar I2cReadByte();

uchar PCF8591Read(uchar Ctrl);

uint getReMinValue();



uint getGuangmingValue();

#endif

