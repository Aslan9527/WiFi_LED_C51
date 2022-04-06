/*

通信波特率：9600Mbps

引脚定义：
		led1-2.0
		led2-2.1
		led3=2.2



*/
#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
//以下是板子上LED的配置，把Px_x改成自己对应的脚。
//以下是你的51单片机的晶振大小
#define FOSC_110592M 
//注意，无论接收到信号还是发送完信号，都会进中断服务程序的

sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
/*初始化程序（必须使用，否则无法收发），次程序将会使用定时器1*/
void SerialInti()//初始化程序（必须使用，否则无法收发）
{
	TMOD=0x20;//定时器1操作模式2:8位自动重载定时器
#ifdef FOSC_12M		   //在这里根据晶振大小设置不同的数值初始化串口
	TH1=0xf3;//装入初值，波特率2400
	TL1=0xf3;	
#else 	
	TH1=0xfd;//装入初值，波特率9600
	TL1=0xfd;
#endif //end of SOC_12M
	
	TR1=1;//打开定时器
	SM0=0;//设置串行通讯工作模式，（10为一部发送，波特率可变，由定时器1的溢出率控制）
	SM1=1;//(同上)在此模式下，定时器溢出一次就发送一个位的数据
	REN=1;//串行接收允许位（要先设置sm0sm1再开串行允许）
	EA=1;//开总中断
	ES=1;//开串行口中断	
}



//串行口连续发送char型数组，遇到终止号/0将停止
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//等待发送完成信号（TI=1）出现
		TI=0;
		str++;
	}
}




//延时函数
void delay(uint ttt)
{
while(ttt--);
}					 


//ESP8266上电初始化
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

/*串行通讯中断，收发完成将进入该中断*/
void Serial_interrupt() interrupt 4 
{
if(RI)
{
	RI=0;//接收中断信号清零，表示将继续接收
	switch(SBUF)
	{
	  case 'a':led1=~led1;break;
	   case 'b':led2=~led2;break;

	   	  case 'c':led3=~led3;break;

	}
}
} 
