#include<reg52.h>
#include<intrins.h>
#include"delay.h"
sbit s2 = P3^4;
sbit s3 = P3^5;
sbit s4 = P3^6;
sbit s5 = P3^7;
#define led P1
int x, j, k;










void Key1(){
 	if(s2 == 0 ){
		delay10ms();
		if( s2 == 0 ){
			//while(1){
			led = 0X00;
			delay500ms();
			led = 0XFF;
			delay500ms();}
		//}
	}
}


void Key2(){
	if(s3 == 0){
		delay10ms();
		if(s3 == 0){	
			//while(1){
				led = 0XFF;
				delay500ms();
				for(k = 0; k < 8 ;k++){
					x = led;
					led = led & (~1<<j++);
					delay500ms();
				}
				delay1s();
				if(k == 8){
					j = 0;
					k = 0;
				}
				
			//}
		}
	}
}

void Key3(){
	int i;
	if(s4 == 0){
		delay10ms();
		if(s4 == 0){
			//while(1){
				led = 0XFE;
				delay500ms();
			
				for(i=1;i<7;i++)
				{
					led=_crol_(led,1);
					delay500ms();
				}
				led=0x7f;//1111 1101
				delay500ms();
				for(i=1;i<7;i++)
				{
					led=_cror_(led,1);
					delay500ms();
				}	
			//}
		}
	}
}


void Key4(){
	
	int i;
	if( s5==0 ){
		delay10ms();
		if( s5==0 ){
			//while(1){
				int x = 0X01;
				int y = 0X80;
				led = ~(x^y);
				delay500ms();
				for( i = 0; i < 3; i++){
					x = _crol_(x, 1);
					y = _cror_(y, 1);
					//led = ~((x^y)^(~led));
					led = ~(x^y);
					delay500ms();
				}
			//}
		}
	} 
	
}




void main(){
	while(1){
		Key1();
		Key2();
		Key3();
		Key4();
	}
}

