#define uint unsigned int
#define uchar unsigned char

sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P1^7;
sbit led_sel=P2^3;
sbit beep=P3^4;			    //蜂鸣器
uchar timeh,timel;		  //定义定时器1的重装值

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
