#include <STC15F2K60S2.h>
#include <stdlib.h>
#include "music.h"
#include "variable.h"
#include "function.h"
#define uint unsigned int
#define uchar unsigned char

//函数列表
void gameScreen();
void gameOver();
uchar seg7encorder(uint num);
void playmusic();
void delay(unsigned int xms);
uchar quyin(uchar tem);
void init_sys();
void init();

/**********************
void main() main function
***********************/
void main()
{
	init_sys();
	init();
	P0=0x00;
	playmusic();
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
/**********************
函数名称：gameOver()
功能描述：游戏结束，显示击中与未击中次数
入口参数：无
出口参数：无
备注：
***********************/
void gameOver(){
	hit = (hit%1000)-7;  //减去移动前多算的七次
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

/**********************
函数名称：void tim1() interrupt 1
功能描述：定时器0中断处理，重新装值，并把beep值取反，产生方波
入口参数：无
出口参数：无
备注：
***********************/
void tim1() interrupt 1						//计时器控制频率
{
	TH0=timeh;
	TL0=timel;
	beep=~beep;
}

/**********************
函数名称：void tim2() interrupt 3
功能描述：定时器1中断处理,点亮led和数码管
入口参数：无
出口参数：无
备注：
***********************/
void tim2() interrupt 3		
{
	TH1=(65536-925)/256;
	TL1=(65536-925)%256;
	gameScreen();
}

/**********************
函数名称：void playmusic()
功能描述：播放音乐
入口参数：无
出口参数：无
备注：

***********************/
void playmusic()
{
	uchar p,m,tem;   //m为节拍   
	uchar i=0;    
	while(1)   
	{   
		p=music[i];   	 //如果碰到结束符,延时1秒,回到开始再来一遍 
		if(p==0x00)
		{
			i=0;
			delay(1000);
		}        
		else if(p==0xff)   //若碰到休止符,延时100ms,继续取下一音符
		{
			i=i+2;
			delay(100);
			TR0=0;
		}      
		else			   //正常情况下取音符和节拍 
		{
			tem=quyin(music[i]);		//取出当前音符在quzi数组中的位置值
			timeh=quzi[tem];			//把音符相应的计时器重装载值赋予timeh和timel
			timel=quzi[tem+1];
			i++;
			TH0=timeh;					//把timeh和timel赋予计时器
			TL0=timel;
			m=music[i];					 //取得节拍
			i++;
		} 
		TR0=1;                                             //开定时器1    
		delay(m*180);             //等待节拍完成, 通过P3^4口输出音频    
		TR0=0;                                            //关定时器1 
			   
   }
}
