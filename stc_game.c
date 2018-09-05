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
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P1^7;
sbit led_sel=P2^3;

uint hit = 0;  //击中与否
uint no_hit = 0;
uint no_hit_t = 0;
uchar led = 0x3f;  //初始六条命
uchar gameState = 0; //游戏状态
uchar flag = 0;  //数码管选择
uchar i = 0;
uchar tmp = 0;
uchar next = 0;  
uint count = 0;         //数码管移动计数
uint shift_time = 530;  //数码管移动时间
uchar show_b[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  	//barrier 8-bit segments show
uchar show_f[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  	//final 8-bit show
uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};	
void gameScreen();
void gameOver();
uchar seg7encorder(uint num);
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
	
	//判断点击
	if(gameState == 0){
		if(KEY3 == 0){ show_b[6] &= 0xfe; }		 //敲击上中下三个位置的音符
		if(KEY2 == 0){ show_b[6] &= 0xbf; }
		if(KEY1 == 0){ show_b[6] &= 0xf7; } 
		
		show_f[6] |= 0x36;  //敲击提示线
		
	}
	
	//数码管显示
	P0 = 0;
	flag = flag%9;
	led_sel = 0;
	if(flag < 8){
		P2 = weixuan[flag];
		P0 = show_f[flag];
	}else{
		led_sel=1;
		P0 = led;
		flag = -1;
	}
	
	flag++;
	count++;
	

	
	if(count == shift_time){ //移动
		switch(gameState){
			case 0:
					for(i=7;i>0;i--){
						show_b[i] = show_b[i-1];
						show_f[i] = show_b[i];
					}
					
					//生成随机数
					tmp =  rand()%3;
					switch(tmp){
						case 0: next = 0x01; break;		//show_b[0] is an original resource used to copy musicode to next location.
						case 1: next = 0x40; break;
						case 2: next = 0x08; break;
					}
					show_b[0] = next;
					show_f[0] = show_b[0];
					count = 0;
					
					//判断是否击中
					if(show_b[7] != 0x00){
							no_hit_t ++;
							no_hit ++;
							if(no_hit_t == 20){
								no_hit_t = 0;
								led = led>>1;
								if(led == 0x00){
									gameState = 1;
									gameOver();
								}
						}
					}else{
						hit++;
					}
					break;
				default:
					gameOver();
		}
	}
}

void gameOver(){
	hit = hit%1000-7;  //减去移动前多算的七次
	no_hit = no_hit%1000;
	show_f[0] = 0x76;
	show_f[1] = seg7encorder(hit/100);
	show_f[2] = seg7encorder((hit%100)/10);
	show_f[3] = seg7encorder(hit%10);
	show_f[4] = 0x38;
	show_f[5] = seg7encorder(no_hit/100);
	show_f[6] = seg7encorder((no_hit%100)/10);
	show_f[7] = seg7encorder(no_hit%10);
}
/*
void timer0() interrupt 1{
	TH0=(65535-1000)/256;     //重新装载定时器0的初始值，为了下一次定时器溢出准备
	TL0=(65535-1000)%256;
	gameScreen();
	flag++;
}
*/
void tim2() interrupt 3		
{
	TH1=(65536-925)/256;
	TL1=(65536-925)%256;
	gameScreen();
}

/***********************
FUNCTION: 		void seg7encorder()
DESCRIPTION:	to translate a number into 7-segment code.
************************/
uchar seg7encorder(uint num){
	switch(num){
		case 0: return 0x3f;
		case 1: return 0x06;
		case 2: return 0x5b;
		case 3: return 0x4f;
		case 4: return 0x66;
		case 5: return 0x6d;
		case 6: return 0x7d;
		case 7: return 0x07;
		case 8: return 0x7f;
		case 9: return 0x6f;
		default: return 0xff;
	}
}