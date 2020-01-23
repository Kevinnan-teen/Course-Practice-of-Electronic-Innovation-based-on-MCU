#include"hongwai.h"




unsigned char  irtime;				//�洢������ߵ͵�ƽ����ʱ�� 
						//32λ���ݽ�����ɱ�־ 
unsigned char IRcord[4];			//�洢������4�ֽ����� 
unsigned char irdata[33];			//������ʼ�����ڵ�33λ����



/*********************************
��������time0INIT
���ܣ���ʱ��0��ʼ������
��������
����ֵ����
*********************************/
void time0INIT()//��ʱ��0��ʼ��
{
	TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
	TH0=0x00; //����ֵ
	TL0=0x00; //��ʼ��ֵ�� ��ʱ��0���һ��ʱ��Ϊ256���������� 
	ET0=1;    //����ʱ��0�ж� 
	TR0=1;    //������ʱ��0  
}


/*********************************
��������time0()
���ܣ���ʱ��0�жϺ���
��������
����ֵ����
*********************************/
void time0 (void) interrupt 1 
{
  irtime++;  //���ڼ���2���½���֮���ʱ��
}



/*********************************
��������EX0INIT()
���ܣ��ⲿ�ж�0��ʼ������
��������
����ֵ����
*********************************/
void EX0INIT()
{
 	IT0 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
 	EX0 = 1;   //ʹ���ⲿ�ж�
 	EA = 1;    //�����ж�
}


/*********************************
��������EX0_ISR()
���ܣ��ⲿ�ж�0�жϺ���
��������
����ֵ����
*********************************/
void EX0_ISR () interrupt 0 //�ⲿ�ж�0������
{
	static unsigned char  i;             //���պ����źŴ��� ���ڴ���33�����ݼ��� 
	static bit startflag;                //��ʼ���������־λ 

	if(startflag)                         
	{
		if(irtime<63&&irtime>=33)//������ TC9012��ͷ�룬9ms+4.5ms
            i=0;
		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
    	irtime=0;  
    	i++;
   		if(i==33)
      	{
	  		irok=1;
			i=0;//����  
	  	} 
	}
	else
	{
		irtime=0;
		startflag=1;
	}
}


void Ir_work(void)//�����ֵɢת����
{
	//�жϵ���������ֵ
	switch(IRcord[2]){
		case 0X45:
			flag_way = 1;		//ʹ�ð������Ʒ���
			break;
		case 0X47:
			flag_way = 2;		//����������Ʒ���
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
			
void Ircordpro(void)//������ֵ������
{ 
	unsigned char i, j, k,cord,value;
	k=1;
	for(i=0;i<4;i++)      //����4���ֽ�
	{
		for(j=1;j<=8;j++) //����1���ֽ�8λ
		{
			cord=irdata[k];
			if(cord>7)//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
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
	irpro_ok=1;		//������ϱ�־λ��1
}
