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

/**********************
函数名称：void delay(unsigned int xms)
功能描述：延时
入口参数：xms：输入需要延时的毫秒值
出口参数：无
备注：

***********************/
void delay(unsigned int xms)
{
	uint i,j;						   
	 for(i=xms;i>0;i--)
	 	for(j=124;j>0;j--);
}
/**********************
函数名称：uchar quyin(uchar tem)
功能描述：在quzi数组中，找到music数组定义的简谱音符的重装值，并返回其在quzi数组中的位置
入口参数：tem：music数组中定义的简谱音符
出口参数：返回的是tem音符在quzi数组中的位置值
备注：

***********************/
uchar quyin(uchar tem)
{
	uchar qudiao,jp,weizhi;		  //定义曲调，音符和位置
	qudiao=tem/16;				  //高4位是曲调值
	jp=tem%16;					  //低4位是音符
	if(qudiao==1)				  //当曲调值为1时，即是低八度，低八度在quzi数组中基址为0
		qudiao=0;
	else if(qudiao==2)			  //当曲调值为2时，即是中八度，中八度在quzi数组中基址为14
		qudiao=14;
	else if(qudiao==3)			  //当曲调值为3时，即是高八度，高八度在quzi数组中，基址为28
		qudiao=28;
	weizhi=qudiao+(jp-1)*2;		  //通过基址加上音符作为偏移量，即可定位此音符在quzi数组中的位置
	return weizhi;				  //返回这一个位置值
}

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