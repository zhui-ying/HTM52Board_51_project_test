#include "DS1302.h"

#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz
//#define FOSC 12000000L //晶振设置，使用12M Hz
//#define FOSC 24000000L //晶振设置，使用24M Hz

#define TIME_MS 50 //设定定时时间 ms ,在11.0592M晶振下，不易超过60ms

//IO接口定义

//全局变量定义
unsigned char count,temp;
volatile unsigned char second,minute,hour,week,day,month,year;		//秒、分、时、星期、日、月、年
unsigned char time[]={0x06,0x03,0x14,0x03,0x15,0x42,0x55};	//初始时间数组


/*******************************************************************************
* 函 数 名 ：Delayms
* 函数功能 ：实现 ms级的延时
* 输    入 ：ms
* 输    出 ：无
*******************************************************************************/
static void Delayms(unsigned int ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
	#if FOSC == 11059200L
		for(j=0;j<114;j++);
	#elif FOSC == 12000000L
	  for(j=0;j<123;j++);
	#elif FOSC == 24000000L
		for(j=0;j<249;j++);
	#else
		for(j=0;j<114;j++);
	#endif
}

/*****************************************************************************
* 函 数 名 ：InputByte
* 函数功能：向DS1302送一字节数据子程序
* 输    入：byte1
* 输    出：
*****************************************************************************/
void InputByte(unsigned char byte1)
{
	char i;
	for(i=8;i>0;i--)
	{
		DS1302_IO=(bit)(byte1&0x01);
		DS1302_CLK=1;
		_nop_();_nop_();
		DS1302_CLK=0;
		byte1>>=1;
	}
	return;
}

/*****************************************************************************
* 函 数 名：outputbyte
* 函数功能：读DS1302一个字节子程序
* 输    入：
* 输    出：读取的数值
*****************************************************************************/
unsigned char OutPutByte(void)  
{
	unsigned char i;
	unsigned ucdat=0;
	for(i=8;i>0;i--)
	{
		DS1302_IO=1;
		ucdat>>=1;
		if(DS1302_IO)ucdat|=0x80;
		DS1302_CLK=1;
		_nop_();_nop_();
		DS1302_CLK=0;
	 }
	 return(ucdat);
}

/*****************************************************************************
* 函 数 名：write_ds1302
* 函数功能：向DS1302某地址写一字节数据子程序
* 输    入：addr,TDat
* 输    出：
*****************************************************************************/
void WriteDs1302(unsigned char addr,unsigned char TDat)
{
	DS1302_RST=0;
	_nop_();_nop_();
	DS1302_CLK=0;
	_nop_();_nop_();
	DS1302_RST=1;
 	InputByte(addr);//传送8字节地址命令
	_nop_();_nop_();
	InputByte(TDat);//写入数据命令
	DS1302_CLK=1;
	_nop_();_nop_();
	DS1302_RST=0;
}

/*****************************************************************************
* 函 数 名：read_ds1302
* 函数功能：读DS1302地址子程序
* 输    入：add
* 输    出：timedata
*****************************************************************************/
unsigned char ReadDs1302(unsigned char addr)
{
	unsigned char timedata,aa;
	DS1302_RST=0;
	_nop_();_nop_();
	DS1302_CLK=0;
	_nop_();_nop_();
	DS1302_RST=1;
	InputByte(addr);//传送8字节地址命令
	timedata=OutPutByte();//读取字节命令
	DS1302_CLK=1;
	_nop_();_nop_();
	DS1302_RST=0;
	aa=timedata%16;
	timedata=timedata/16;
	timedata=timedata*10+aa;
	return(timedata);
}

/*****************************************************************************
* 函 数 名：initial_ds1302
* 函数功能：初始化DS1302子程序
* 输    入：time[](全局变量)
* 输    出：
*****************************************************************************/
void InitDs1302()
{
	WriteDs1302(0x8e,0x00);		//写保护寄存器，在对时钟或RAM写前WP一定要为0
	Delayms(5);
	WriteDs1302(0x8c,time[0]);		//年
	Delayms(5);
	WriteDs1302(0x88,time[1]);		//月
	Delayms(5);
	WriteDs1302(0x86,time[2]);		//日
	Delayms(5);
	WriteDs1302(0x8A,time[3]);		//星期
	Delayms(5);
	WriteDs1302(0x84,time[4]);		//时
	Delayms(5);
	WriteDs1302(0x82,time[5]);		//分
	Delayms(5);
	WriteDs1302(0x80,time[6]);		//秒
	Delayms(5);
	WriteDs1302(0x8e,0x80);		//写保护寄存器
	Delayms(5);
}

/*****************************************************************************
* 函 数 名：read_time
* 函数功能：读DS1302时间子程序
* 输    入：
* 输    出：全局变量(second,minute,hour,week,day,month,year)
*****************************************************************************/
void ReadTime()
{
	second=ReadDs1302(0x81);		//秒寄存器

	minute=ReadDs1302(0x83);		//分
//	Delayms(5);
	hour=ReadDs1302(0x85);			//时
//	Delayms(5);
	week=ReadDs1302(0x8B);			//星期
//	Delayms(5);
	day=ReadDs1302(0x87);			//日
//	Delayms(5);
	month=ReadDs1302(0x89);		//月
//	Delayms(5);
	year=ReadDs1302(0x8d);
//	Delayms(5);			//年
}

