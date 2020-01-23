#include"delay.h"
#include<intrins.h>


void delays(uchar t){
	int k, m;
    while(t != 0){
        for(k = 0; k< 550; k++){
		    for(m = 0; m < 210;m++){}
	    }
        t--;
    }
	
}


void delayms( uchar t ){
    while( t != 0 ){
        int i = 98;
	    while(i--); 
        t--;   
    }
}



void delay5us( uchar t ){
	while( t != 0 ){
    	_nop_();
    	t--;    
    }	
}
