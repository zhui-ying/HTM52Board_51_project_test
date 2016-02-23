/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：综合实验
* 实验说明 : 1.方波发生器
*            2.DS1302
*            3.红外
*            同时串口也会发送具体信息到上位机
* 实验平台 ：航太51单片机开发板 V1.1
* 连接方式 ：
* 注    意 ：
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <stdarg.h>
#include "KeyScan.h"
#include "beep.h"
#include "hexian.h"
#include "NE555.h"
#include "DS1302.h"
#include "redctrol.h"

#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz
//#define FOSC 12000000L //晶振设置，使用12M Hz
//#define FOSC 24000000L //晶振设置，使用24M Hz
#define TIME_MS 10 //设定定时时间 ms ,在11.0592M晶振下，不易超过60ms

#define BAUD 9600

//IO接口定义
#define LED_SEG P1
#define LED_PORT P0
sbit wela = P2^7;
sbit dula=P2^6;
sbit diola = P2^5;

//全局变量定义
volatile unsigned int update_count = 0;
volatile unsigned int key_value = 0;
volatile unsigned int flag = 0;
volatile unsigned long freq_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int red_value = 0;

//LED显示字模 0-F 共阴模式
unsigned code table[]= {0Xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

//函数声明
void Delayms(unsigned int ms);
char putchar(char ch);
//void UsartConfiguration();

void Timer0Init();
void LEDdisplay(unsigned int index,unsigned int num);
void KeyTest();
void BeepTest(void);
void HexianTest();
void DS1302Test();

/*******************************************************************************
* 函 数 名 ：main
* 函数功能 ：主函数
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void main()
{
	
//	UsartConfiguration();
	flag = 0;
	BeepTest();//蜂鸣器测试
	EA = 0;	
	flag = 1;
	HexianTest();//和弦音测试	
	
	EA = 0;
	flag = 2;
	Delayms(100);
	InitDs1302();
	Timer0Init();
	NE555Timer1Init();
	RedExit0Init();
	
	
	TR1=1;
	
	key_value = 1;
	
	while(1)
	{
		if(update_count >= 10)//100ms 更新一次数据
		{
			update_count = 0;
			switch(key_value)
			{
				case 1: flag = 2; TR1=1;EX0 = 0; break;
				case 2: flag = 3; TR1=0;EX0 = 0; DS1302Test(); break;
				case 3: flag = 4; TR1=0;EX0 = 1;break;
				default : break;
			}
	
		}
		KeyTest();//键盘扫描测试
		switch(key_value)
		{
			case 1: 
				LEDdisplay(1,freq_value);
			  break;
			case 2:
				LEDdisplay(2,second_value);
				break;
			case 3: 
				LEDdisplay(3,red_value);
			  break;
				break;	
      default: 
				LEDdisplay(key_value,key_value);
				break;			
		}
	}	
}

/*******************************************************************************
* 函 数 名 ：BeepTest
* 函数功能 ：蜂鸣器测试,仅开机执行一次
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void BeepTest(void)
{
//	BeepTime0Init();
//	PlaySong(1);
	
	beep1Time();
	Delayms(200);
	beep1Time();
	Delayms(200);
	beep1Time();	
	Delayms(200);
	beep1Time();	
}

/*******************************************************************************
* 函 数 名 ：HexianTest
* 函数功能 ：和弦音测试
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void HexianTest()
{
		PWR_CTRL = 0;
	BUZZER_IO = 0;
	Delayms(700);
	HeXianInitTime();
	Delayms(500);
	BeepOn(S_POWERON);
	Delayms(1000);
	//InitTime();
	
}

/*******************************************************************************
* 函 数 名 ：
* 函数功能 ：
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/ 
void DS1302Test()
{
	ReadTime();
	second_value = (int)second;
}

/*******************************************************************************
* 函 数 名 ：KeyTest
* 函数功能 ：按键扫描，获取按下的键值，若无键值按下，保持原有值
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/ 
void KeyTest()
{
	unsigned char key_temp;
	
	key_temp = KeyScan();
	if(key_temp !=0xff)  key_value = key_temp;
}

/*******************************************************************************
* 函 数 名 ：Timer0Int
* 函数功能 ：定时器0中断函数 ， 每隔TIME_MS ms进入
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Timer0Int() interrupt 1 using 1
{
	static unsigned int count1s = 0;
	
	if(flag == 1)
	{
			TH0=Beep.CountFR/256;	 
			TL0=Beep.CountFR%256;	
			BUZZER_IO = ~BUZZER_IO;	
			return;
	}
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	update_count++;
	count1s++;
	beep_count++;

	if(count1s == 50) //0.5s
	{
		count1s = 0;
		if(flag == 2)
		{
			TR1=0;		
			//计算频率
			Freq = Freq + TL1;         //读取TL的值
			Freq = Freq + (TH1 * 256); //读取TH的值
			freq_value = Freq*2;
			Freq = 0;
			TH1 = 0; //将计数器的值清零
			TL1 = 0;
			TR1 = 1; //开启计数器	
		}
	}
		
}

/*******************************************************************************
* 函 数 名 ：putchar
* 函数功能 ：代替系统自带的putchar函数，实现printf功能
* 输    入 ：要发送的参数
* 输    出 ：发送成功的参数
*******************************************************************************/
char putchar(char ch)
{ 
	/* Place your implementation of fputc here */ 
	SBUF=(unsigned char)ch; //将接收到的数据放入到发送寄存器
	while(!TI);		  //等待发送数据完成
	TI=0;		 //清除发送完成标志位	
	return ch;
}

/*******************************************************************************
* 函 数 名 ：Delayms
* 函数功能 ：实现 ms级的延时
* 输    入 ：ms
* 输    出 ：无
*******************************************************************************/
void Delayms(unsigned int ms)
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

/*******************************************************************************
* 函 数 名 ：UsartConfiguration
* 函数功能 ：串口设置
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
//void UsartConfiguration()
//{
//	SCON = 0X50;			//设置为工作方式1	10位异步收发器
//	TMOD = 0x20; //设置计数器工作方式2  8位自动重装计数器	
//	PCON = 0X80;//波特率加倍	SMOD = 1  28800
//	TH1 = 256 -(FOSC/12/32/(BAUD/2)); //计算溢出率
//	TL1 = 256 -(FOSC/12/32/(BAUD/2));
//	TR1 = 1; //打开定时器	
////	ES=1;//必须关闭，否则会影响定时器
//}

/*******************************************************************************
* 函 数 名 ：Timer0Init
* 函数功能 ：定时器0初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Timer0Init()
{	
	TMOD=  0x01; //设置定时器0工作方式为1
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	ET0=1; //开启定时器0中断
	TR0=1;	//开启定时器	
	EA=1;  //打开总中断
}

/*******************************************************************************
* 函 数 名 ：Timer1Int
* 函数功能 ：定时器1中断服务函数， 控制节拍时长 10ms
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Timer1Int()	interrupt 3	using 3 //采用中断3 控制频率	
{	
	if(flag == 1)
	{
		TH1=(65536-10000)/256;
		TL1=(65536-10000)%256;	//10ms定时
		BeepControl(); 
	//	Beep_control(); 
		return;
	}
	
		//--进入一次中断，表明计数到了65536--//
	Freq=Freq+65536;	
	
}	 

/*******************************************************************************
* 函 数 名 ：Exit0Int
* 函数功能 ：外部中断0 ISR
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Exit0Int() interrupt 0 
{
	unsigned char temp;
	temp = ReadRedData();
	if(temp != 0xff)
	{
		red_value = (int)temp;
	}
}

/*******************************************************************************
* 函 数 名 ：LEDdisplay
* 函数功能 ：循环显示各个位上的数据
* 输    入 ：index 标号   num 要显示的数据
* 输    出 ：无
*******************************************************************************/
void LEDdisplay(unsigned int index,unsigned int num)
{
	unsigned char bai,shi,ge;
	index = index%10;
	bai=num/100%10;
	shi=num%100/10;
	ge=num%10;

	wela=1;//显示标号
	LED_PORT=0x01;
	wela=0;	
	dula=1;			   
	LED_PORT=table[index];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x08;
	wela=0;	
	dula=1;			   //显示百位
	LED_PORT=table[bai];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x10;
	wela=0;
	dula=1;			   //显示十位
	LED_PORT=table[shi];	 
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x20;
	wela=0;
	dula=1;			 // 显示个位
	LED_PORT=table[ge];
	dula=0;
	Delayms(1);
}
