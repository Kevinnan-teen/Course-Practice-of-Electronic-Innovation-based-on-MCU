#include<reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit s2=P3^4;
sbit s3=P3^5;
sbit s4=P3^6;
sbit s5=P3^7;
sbit dula=P2^6;  
sbit wela=P2^7;  

uchar i;
uchar j;
uchar k;
uint count=0;

uchar du[]={0x00,0x00,0x00,0x00,0x00,0x5b,0x3f,0x06,0x7f,0x5b,0x06,
				0x5b,0x06,0x06,0x66,0x00,0x00,0x00,0x00,0x00,0x00};			   
uchar we[]={0x7e,0x7d,0x7b,0x77,0x6f,0x5f};

/*1MS为单位的延时程序*/
void delay(uchar x)
{
    uchar j;
    while(x--){
        for(j=0;j<125;j++);
        }   
}

void delay10ms(){
	int i = 980;
	while(i--);
}



void Key1(){
   if( s2==0 ){
   	delay10ms();
	if( s2 == 0 ){
		for(j=0;j<15;j++){ 
			for(i=0;i<6;i++){ 
		
				P0=0XFF;
				delay(1);
				wela=1;
				delay(1);
				wela=0;	

				dula=0;		
				P0 = du[20-i-count]; 
				dula=1;
				dula=0;

				wela=0;
				P0=we[5-i];  
				wela=1;
				wela=0;	

    			delay(1);
				}
			}
		count++;
		if(count>=16) count=0; //重新循环	
	}
   }
}

void Key2(){
   if( s3==0 ){
   		delay10ms();
		if( s3 == 0 ){
			for(j=0;j<7;j++){ 
			for(i=0;i<6;i++){
		
				P0=0XFF;//关掉
				delay(1);
				wela=1;
				delay(1);
				wela=0;	

				dula=0;		
				P0 = du[20-i-count]; 
				dula=1;
				dula=0;

				wela=0;
				P0=we[5-i];   
				wela=1;
				wela=0;	

    			delay(1);
				}
			}
		count++;
		if(count>=16) count=0; //重新循环		
		}
	}
}

void Key3(){
	if( s4 == 0 ){
		delay10ms();
		if( s4 == 0 ){
			for(j=0;j<15;j++){ 
			for(i=0;i<6;i++){ 
		
				P0=0XFF;//关掉
				delay(1);
				wela=1;
				delay(1);
				wela=0;	

				dula=0;		
				P0 = du[i+count]; 
				dula=1;
				dula=0;

				wela=0;
				P0=we[i];   
				wela=1;
				wela=0;	

    			delay(1);
				}
			}
		count++;
		if(count>=16) count=0; //重新循环	
		}
	}
}

void Key4(){
	if( s5 ==0 ){
		delay10ms();
		if( s5 == 0 ){
			for(j=0;j<7;j++){ 
			for(i=0;i<6;i++){ 	
		
				P0=0XFF;//关掉
				delay(1);
				wela=1;
				delay(1);
				wela=0;

				dula=0;		
				P0 = du[i+count]; 
				dula=1;
				dula=0;

				wela=0;
				P0=we[i];   
				wela=1;
				wela=0;	

    			delay(1);
				}
			}
		count++;
		if(count>=16) count=0; //重新循环	
		}
	}
}

void main()
{
	while(1){	
		Key1();
		Key2();
		Key3();
		Key4();
	}	
}
