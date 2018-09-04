#include <STC15F2K60S2.h>
#include <stdlib.h>
#define uint unsigned int
#define uchar unsigned char

/**********************
函数名称：void init_sys()
功能描述：系统初始化，功能是配置IO口
***********************/
void init_sys()
{
	P0M0=0xff;
	P0M1=0x00;
	P2M0=0x08;
	P2M1=0x00;
	P3M0=0x10;
	P3M1=0x00;
	P4M0=0x00;
	P4M1=0x00;
	P5M0=0x00;
	P5M1=0x00;
}
void init()						  
{
	TMOD=0x11;
	TH0=0xD8;
	TL0=0xEF;
	EA=1;
	ET0=1;
	TR0=0;
	ET1=1;
	TR1=1;
}
uchar flag = 0;  //数码管选择
uchar i = 0;
uchar tmp = 0;
uchar next = 0;
uint count = 0;
uint shift_time = 530;
uchar show_b[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  	//barrier 8-bit segments show
uchar show_f[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  	//final 8-bit show
uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};	
void gameScreen();

/**********************
void main() main function
***********************/
void main()
{
	init_sys();
	init();
	P0=0x00;
	gameScreen();
	while(1);
}

void gameScreen(){
	P0 = 0;
	flag = flag%8;
	P2 = weixuan[flag];
	P0 = show_f[flag];
	if(count == shift_time){
		for(i=7;i>0;i--){
			show_b[i] = show_b[i-1];
			show_f[i] = show_b[i];
		}
		tmp =  rand()%3;
		switch(tmp){
			case 0: next = 0x01; break;		//show_b[0] is an original resource used to copy musicode to next location.
			case 1: next = 0x40; break;
			case 2: next = 0x08; break;
		}
		show_b[0] = next;
		show_f[0] = show_b[0];
		count = 0;
	}
}

void timer0() interrupt 1{
	TH0=(65535-1000)/256;     //重新装载定时器0的初始值，为了下一次定时器溢出准备
	TL0=(65535-1000)%256;
	gameScreen();
	flag++;
}

void tim2() interrupt 3		
{
	TH1=(65536-925)/256;
	TL1=(65536-925)%256;

	
	gameScreen();
	flag++;
	count++;
}