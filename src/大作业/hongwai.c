#include"hongwai.h"




unsigned char  irtime;				//存储检测红外高低电平持续时间 
						//32位数据接收完成标志 
unsigned char IRcord[4];			//存储解码后的4字节数据 
unsigned char irdata[33];			//包含起始码在内的33位数据



/*********************************
函数名：time0INIT
功能：定时器0初始化函数
参数：无
返回值：无
*********************************/
void time0INIT()//定时器0初始化
{
	TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
	TH0=0x00; //重载值
	TL0=0x00; //初始化值， 定时器0溢出一次时间为256个机器周期 
	ET0=1;    //开定时器0中断 
	TR0=1;    //启动定时器0  
}


/*********************************
函数名：time0()
功能：定时器0中断函数
参数：无
返回值：无
*********************************/
void time0 (void) interrupt 1 
{
  irtime++;  //用于计数2个下降沿之间的时间
}



/*********************************
函数名：EX0INIT()
功能：外部中断0初始化函数
参数：无
返回值：无
*********************************/
void EX0INIT()
{
 	IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 	EX0 = 1;   //使能外部中断
 	EA = 1;    //开总中断
}


/*********************************
函数名：EX0_ISR()
功能：外部中断0中断函数
参数：无
返回值：无
*********************************/
void EX0_ISR () interrupt 0 //外部中断0服务函数
{
	static unsigned char  i;             //接收红外信号处理， 用于存入33次数据计数 
	static bit startflag;                //开始储存脉宽标志位 

	if(startflag)                         
	{
		if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
            i=0;
		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    	irtime=0;  
    	i++;
   		if(i==33)
      	{
	  		irok=1;
			i=0;//脉宽  
	  	} 
	}
	else
	{
		irtime=0;
		startflag=1;
	}
}


void Ir_work(void)//红外键值散转程序
{
	//判断第三个数码值
	switch(IRcord[2]){
		case 0X45:
			flag_way = 1;		//使用按键控制风扇
			break;
		case 0X47:
			flag_way = 2;		//热敏电阻控制风扇
			break;
		case 0X43:
			flag1 = 1;
			break;
		case 0X09:
			flag1 = 0;
			break;
		case 0X0C:
			flag3 = 1;
			break;
		case 0X18:
			flag3 = 2;
			break;
		case 0X5E:
			flag3 = 3;
			break;
		case 0X16:
			flag3 = 0;
		default:
			break;
	}
}
			
void Ircordpro(void)//红外码值处理函数
{ 
	unsigned char i, j, k,cord,value;
	k=1;
	for(i=0;i<4;i++)      //处理4个字节
	{
		for(j=1;j<=8;j++) //处理1个字节8位
		{
			cord=irdata[k];
			if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
			value=value|0x80;
			if(j<8)
			{
				value>>=1;
			}
			k++;
		}
		IRcord[i]=value;
		value=0;     
	} 
	irpro_ok=1;		//处理完毕标志位置1
}
