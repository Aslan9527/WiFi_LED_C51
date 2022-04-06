/*

ͨ�Ų����ʣ�9600Mbps

���Ŷ��壺
		led1-2.0
		led2-2.1
		led3=2.2



*/
#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
//�����ǰ�����LED�����ã���Px_x�ĳ��Լ���Ӧ�Ľš�
//���������51��Ƭ���ľ����С
#define FOSC_110592M 
//ע�⣬���۽��յ��źŻ��Ƿ������źţ�������жϷ�������

sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
/*��ʼ�����򣨱���ʹ�ã������޷��շ������γ��򽫻�ʹ�ö�ʱ��1*/
void SerialInti()//��ʼ�����򣨱���ʹ�ã������޷��շ���
{
	TMOD=0x20;//��ʱ��1����ģʽ2:8λ�Զ����ض�ʱ��
#ifdef FOSC_12M		   //��������ݾ����С���ò�ͬ����ֵ��ʼ������
	TH1=0xf3;//װ���ֵ��������2400
	TL1=0xf3;	
#else 	
	TH1=0xfd;//װ���ֵ��������9600
	TL1=0xfd;
#endif //end of SOC_12M
	
	TR1=1;//�򿪶�ʱ��
	SM0=0;//���ô���ͨѶ����ģʽ����10Ϊһ�����ͣ������ʿɱ䣬�ɶ�ʱ��1������ʿ��ƣ�
	SM1=1;//(ͬ��)�ڴ�ģʽ�£���ʱ�����һ�ξͷ���һ��λ������
	REN=1;//���н�������λ��Ҫ������sm0sm1�ٿ���������
	EA=1;//�����ж�
	ES=1;//�����п��ж�	
}



//���п���������char�����飬������ֹ��/0��ֹͣ
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//�ȴ���������źţ�TI=1������
		TI=0;
		str++;
	}
}




//��ʱ����
void delay(uint ttt)
{
while(ttt--);
}					 


//ESP8266�ϵ��ʼ��
void esp8266_init()
{
	Uart1Sends("AT+CIPMUX=1\r\n");
	delay(50000);
	Uart1Sends("AT+CIPSERVER=1,8080\r\n");
}


void main()
{	 	delay(50000);
	delay(5000);
	SerialInti();
	esp8266_init();
	P2=0x00;
	while(1)
	{    


	}
		 

}

/*����ͨѶ�жϣ��շ���ɽ�������ж�*/
void Serial_interrupt() interrupt 4 
{
if(RI)
{
	RI=0;//�����ж��ź����㣬��ʾ����������
	switch(SBUF)
	{
	  case 'a':led1=~led1;break;
	   case 'b':led2=~led2;break;

	   	  case 'c':led3=~led3;break;

	}
}
} 
