#include<reg52.h>
#include"i2c.h"
#define uchar unsigned char
#define uint unsigned int
#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址


extern bit ack;

sbit rs=P3^5;
sbit wr=P3^6;
sbit lcden=P3^4;    //1602使能端口
sbit dula=P2^6;     //数码管段选，位选
sbit wela=P2^7;

uchar key,num,figure,voltage;  //figure选择波形
uchar table_1[]="U=V";
uchar table_2[]="f=Hz";
uchar table_3[]="T=s";
uchar code  sinWave[]={    //正弦波波表
127,152,176,198,217,233,245,252,
255,252,245,233,217,198,176,152,
127,102,78,56,37,21,9,2,
0,2,9,21,37,56,78,102
};
uchar code triangleWave[]={   //三角波波表
0,16,32,48,64,80,96,112,
128,144, 160, 176, 192, 208,224, 240,
255,240,224,208,192,176,160,144,
128,112,96,80,64,48,32,16
};
uchar code sawtoothWave[]={   //锯齿波表
0,8,16,24,32,40,48,56,
64,72,80,88,96,104,112,120,
128,136,144,152,160,168,176,184,
192,200,208,216,224,232,240,248,
};
uchar code voltage_num[5][3]={//电压数组
"5  ","4  ","2.5","  2","  1"};	 
uchar code freq_num[10][3]={
"50","30","2.5"," 20","10"}; 				
uchar code circle_num[10][5]={
"0.02","0.10","0.20","0.50","0.60"};


uchar j;
uchar array_num1;
uchar delay_time;


void keyscan();	//矩阵键盘
void delay(uchar); //延时函数
void init();	   //屏幕初始化
void write_data(uchar);	 //写数据
void write_com(uchar);	 //写命令
void display_1();		 //显示1
void display_2();		 //显示2
bit WriteDAC(unsigned char);	//完成AD转换

void delay10ms(){
	int i = 980;
	while(i--);
}

void delay5us(){
    _nop_();
}



void main()
{
    int i;
	init();     //液晶屏初始化
	dula=0;     //关闭数码管
	wela=0;
	while(1)
	{
		keyscan();
		if(num==32) num=0;
		switch(figure)
		{	
			case 1:			            //显示直流电压
				WriteDAC(voltage);
				write_com(0x80+0x02);
				for(j = 0; j <3; j++){	//在液晶屏上显示伏特
					write_data(voltage_num[array_num1][j]);
				}
				break;
			case 2:			            //显示正弦电压
				WriteDAC(sinWave[num]);
				write_com(0x80+0x02);
				for(j = 0;j < 3; j++){  //在液晶屏上显示频率
					write_data(freq_num[array_num1][j]);
				}
				write_com(0x80+0x42);
				for(j = 0;j < 4;j++){   //在液晶屏上显示周期
					write_data(circle_num[array_num1][j]);
				}
				//delay(delay_time);
                //delay10ms();
                //for(i = 0; i < 10; i++){
                    delay5us();
                //}
				num++;
				break;
			case 3:			            //显示三角波
				WriteDAC(triangleWave[num]);
				write_com(0x80+0x02);
				for(j = 0;j < 3;j++){   //在液晶屏上显示频率
					write_data(freq_num[array_num1][j]);
				}
				write_com(0x80+0x42);
				for(j = 0;j < 4;j++){   //在液晶屏上显示周期
					write_data(circle_num[array_num1][j]);
				}
				delay(delay_time);
				num++;
				break;
			case 4:			            //显示锯齿波
				WriteDAC(sawtoothWave[num]);
				write_com(0x80+0x02);
				for(j = 0;j < 3;j++){   //在液晶屏上显示频率
					write_data(freq_num[array_num1][j]);
				}
				write_com(0x80+0x42);
				for(j = 0;j < 4;j++){//在液晶屏上显示周期
					write_data(circle_num[array_num1][j]);
				}
				delay(delay_time);
				num++;
				break;
		}
	}
}


void init()
{
	write_com(0x38);//显示模式设置
	write_com(0x08);//显示关闭
	write_com(0x01);//显示清屏
	write_com(0x06);//显示光标移动位置
	write_com(0x0c);//显示开及光标设置
}


void keyscan()
{
    	uchar temp;							
		P3=0xf0;//低四位为低电平，高四位为高电平
		temp=P3;//按键列上的值
		temp=temp&0xf0;//和原来对比，保持temp的高电位的电平保持原来的，低四位保持0
		if(temp!=0xf0)//说明有按键按下
		{			
			delay(10);//抖动处理
			if(temp!=0xf0)//再次判断
			{
				temp=P3;
				temp=temp|0X0f;//按位或得到想要值
				P3=temp;
				key=P3;
				switch(key)
				{	
					case 0xee:			  //按键S6, 直流波形， 电压为5v
						init();
						display_1();
						figure=1;       
						num=0;
						voltage=255;
						array_num1=0;
						break;
					case 0xde:			  //按键S7，正弦波
						init();
						display_2();
						figure=2;
						delay_time=0;
						array_num1=0;
						num=0;
						break;
					case 0xbe:			  //按键S8，三角波
						init();
						display_2();
						figure=3;
						delay_time=0;
						array_num1=0;
						num=0;
						break;
					case 0x7e:			  //按键S9，锯齿波
						init();
						display_2();
						figure=4;
						delay_time=0;
						array_num1=0;
						num=0;
						break;
			
					case 0xed:			  //按键S10, 调节直流电压为4v
						voltage=204;
						delay_time=10;
						array_num1=1;
						num=0;
						break;
					case 0x7D:			  //按键S13, 调节直流电压为2.5v
						voltage=128;
						array_num1=2;
						delay_time=50;
						num=0;
						break;
					case 0xEB:			  //按键S14, 调节直流电压为2v
						voltage=102;
						array_num1=3;
						delay_time=200;
						num=0;
						break;
					case 0xE7:	  		   //按键S18, 调节直流电压为1v
						voltage=51;
						array_num1=4;
						delay_time=500;
						num=0;
						break;
				}
                while(!key);
			}
		}   
}
void display_1()//显示“U=  V”
{
//uchar table_1[]="U=V";//液晶屏上需要的字符
//uchar table_2[]="f=Hz";
//uchar table_3[]="T=s";
	write_com(0x80+0x00);
	for(j = 0;j < 2;j++){
		write_data(table_1[j]);
		delay(3);
	}
	write_com(0x80+0x05);
	write_data(table_1[2]);
		delay(3);		
}
void display_2()//第一行显示“f=  Hz”,第二行显示“T=  s”
{
	write_com(0x80+0x00);//液晶屏第1行第1格显示的位置
	for(j = 0;j < 2;j++){
		write_data(table_2[j]);
		delay(3);
	}
	write_com(0x80+0x05);//液晶屏第1行第6格显示的位置
	for(j = 2;j < 4;j++){
		write_data(table_2[j]);
		delay(3);
	}
	write_com(0x80+0x40);//液晶屏第2行第1格显示的位置
	for(j=0;j<2;j++){
		write_data(table_3[j]);
		delay(3);
	}	
	write_com(0x80+0x46);//液晶屏第2行第7格显示的位置
	write_data(table_3[2]);
	delay(3);
}

//延时函数
void delay(uchar x)
{
	uchar a,b;
	for(a=x;a>0;a--)
		for(b=30;b>0;b--);

}
//写命令的函数
void write_com(uchar com)
{
	rs=0;
	wr=0;
	lcden=0;
	P0=com;
	lcden=1;
//	delay(30);
	delay(10);
	lcden=0;

}
//写数据的函数
void write_data(uchar dat)
{
	rs=1;
	wr=0;
	lcden=0;
	P0=dat;
	lcden=1;
//	delay(30);
    delay(10);
	lcden=0;

}



bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
   Stop_I2c(); 
}