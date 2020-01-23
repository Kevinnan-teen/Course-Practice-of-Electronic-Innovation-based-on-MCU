#include<reg52.h>
#include"i2c.h"
#define uchar unsigned char
#define uint unsigned int
#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ


extern bit ack;

sbit rs=P3^5;
sbit wr=P3^6;
sbit lcden=P3^4;    //1602ʹ�ܶ˿�
sbit dula=P2^6;     //����ܶ�ѡ��λѡ
sbit wela=P2^7;

uchar key,num,figure,voltage;  //figureѡ����
uchar table_1[]="U=V";
uchar table_2[]="f=Hz";
uchar table_3[]="T=s";
uchar code  sinWave[]={    //���Ҳ�����
127,152,176,198,217,233,245,252,
255,252,245,233,217,198,176,152,
127,102,78,56,37,21,9,2,
0,2,9,21,37,56,78,102
};
uchar code triangleWave[]={   //���ǲ�����
0,16,32,48,64,80,96,112,
128,144, 160, 176, 192, 208,224, 240,
255,240,224,208,192,176,160,144,
128,112,96,80,64,48,32,16
};
uchar code sawtoothWave[]={   //��ݲ���
0,8,16,24,32,40,48,56,
64,72,80,88,96,104,112,120,
128,136,144,152,160,168,176,184,
192,200,208,216,224,232,240,248,
};
uchar code voltage_num[5][3]={//��ѹ����
"5  ","4  ","2.5","  2","  1"};	 
uchar code freq_num[10][3]={
"50","30","2.5"," 20","10"}; 				
uchar code circle_num[10][5]={
"0.02","0.10","0.20","0.50","0.60"};


uchar j;
uchar array_num1;
uchar delay_time;


void keyscan();	//�������
void delay(uchar); //��ʱ����
void init();	   //��Ļ��ʼ��
void write_data(uchar);	 //д����
void write_com(uchar);	 //д����
void display_1();		 //��ʾ1
void display_2();		 //��ʾ2
bit WriteDAC(unsigned char);	//���ADת��

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
	init();     //Һ������ʼ��
	dula=0;     //�ر������
	wela=0;
	while(1)
	{
		keyscan();
		if(num==32) num=0;
		switch(figure)
		{	
			case 1:			            //��ʾֱ����ѹ
				WriteDAC(voltage);
				write_com(0x80+0x02);
				for(j = 0; j <3; j++){	//��Һ��������ʾ����
					write_data(voltage_num[array_num1][j]);
				}
				break;
			case 2:			            //��ʾ���ҵ�ѹ
				WriteDAC(sinWave[num]);
				write_com(0x80+0x02);
				for(j = 0;j < 3; j++){  //��Һ��������ʾƵ��
					write_data(freq_num[array_num1][j]);
				}
				write_com(0x80+0x42);
				for(j = 0;j < 4;j++){   //��Һ��������ʾ����
					write_data(circle_num[array_num1][j]);
				}
				//delay(delay_time);
                //delay10ms();
                //for(i = 0; i < 10; i++){
                    delay5us();
                //}
				num++;
				break;
			case 3:			            //��ʾ���ǲ�
				WriteDAC(triangleWave[num]);
				write_com(0x80+0x02);
				for(j = 0;j < 3;j++){   //��Һ��������ʾƵ��
					write_data(freq_num[array_num1][j]);
				}
				write_com(0x80+0x42);
				for(j = 0;j < 4;j++){   //��Һ��������ʾ����
					write_data(circle_num[array_num1][j]);
				}
				delay(delay_time);
				num++;
				break;
			case 4:			            //��ʾ��ݲ�
				WriteDAC(sawtoothWave[num]);
				write_com(0x80+0x02);
				for(j = 0;j < 3;j++){   //��Һ��������ʾƵ��
					write_data(freq_num[array_num1][j]);
				}
				write_com(0x80+0x42);
				for(j = 0;j < 4;j++){//��Һ��������ʾ����
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
	write_com(0x38);//��ʾģʽ����
	write_com(0x08);//��ʾ�ر�
	write_com(0x01);//��ʾ����
	write_com(0x06);//��ʾ����ƶ�λ��
	write_com(0x0c);//��ʾ�����������
}


void keyscan()
{
    	uchar temp;							
		P3=0xf0;//����λΪ�͵�ƽ������λΪ�ߵ�ƽ
		temp=P3;//�������ϵ�ֵ
		temp=temp&0xf0;//��ԭ���Աȣ�����temp�ĸߵ�λ�ĵ�ƽ����ԭ���ģ�����λ����0
		if(temp!=0xf0)//˵���а�������
		{			
			delay(10);//��������
			if(temp!=0xf0)//�ٴ��ж�
			{
				temp=P3;
				temp=temp|0X0f;//��λ��õ���Ҫֵ
				P3=temp;
				key=P3;
				switch(key)
				{	
					case 0xee:			  //����S6, ֱ�����Σ� ��ѹΪ5v
						init();
						display_1();
						figure=1;       
						num=0;
						voltage=255;
						array_num1=0;
						break;
					case 0xde:			  //����S7�����Ҳ�
						init();
						display_2();
						figure=2;
						delay_time=0;
						array_num1=0;
						num=0;
						break;
					case 0xbe:			  //����S8�����ǲ�
						init();
						display_2();
						figure=3;
						delay_time=0;
						array_num1=0;
						num=0;
						break;
					case 0x7e:			  //����S9����ݲ�
						init();
						display_2();
						figure=4;
						delay_time=0;
						array_num1=0;
						num=0;
						break;
			
					case 0xed:			  //����S10, ����ֱ����ѹΪ4v
						voltage=204;
						delay_time=10;
						array_num1=1;
						num=0;
						break;
					case 0x7D:			  //����S13, ����ֱ����ѹΪ2.5v
						voltage=128;
						array_num1=2;
						delay_time=50;
						num=0;
						break;
					case 0xEB:			  //����S14, ����ֱ����ѹΪ2v
						voltage=102;
						array_num1=3;
						delay_time=200;
						num=0;
						break;
					case 0xE7:	  		   //����S18, ����ֱ����ѹΪ1v
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
void display_1()//��ʾ��U=  V��
{
//uchar table_1[]="U=V";//Һ��������Ҫ���ַ�
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
void display_2()//��һ����ʾ��f=  Hz��,�ڶ�����ʾ��T=  s��
{
	write_com(0x80+0x00);//Һ������1�е�1����ʾ��λ��
	for(j = 0;j < 2;j++){
		write_data(table_2[j]);
		delay(3);
	}
	write_com(0x80+0x05);//Һ������1�е�6����ʾ��λ��
	for(j = 2;j < 4;j++){
		write_data(table_2[j]);
		delay(3);
	}
	write_com(0x80+0x40);//Һ������2�е�1����ʾ��λ��
	for(j=0;j<2;j++){
		write_data(table_3[j]);
		delay(3);
	}	
	write_com(0x80+0x46);//Һ������2�е�7����ʾ��λ��
	write_data(table_3[2]);
	delay(3);
}

//��ʱ����
void delay(uchar x)
{
	uchar a,b;
	for(a=x;a>0;a--)
		for(b=30;b>0;b--);

}
//д����ĺ���
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
//д���ݵĺ���
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
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(0x40);            //���������ӵ�ַ
     if(ack==0)return(0);
   SendByte(dat);             //��������
     if(ack==0)return(0);
   Stop_I2c(); 
}