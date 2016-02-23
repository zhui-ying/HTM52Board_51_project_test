#include "KeyScan.h"

#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz
//#define FOSC 12000000L //晶振设置，使用12M Hz
//#define FOSC 24000000L //晶振设置，使用24M Hz

//IO接口定义
#define KEY_PORT P3


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

/*******************************************************************************
* 函 数 名 ：KeyScan
* 函数功能 ：4*4键盘扫描
* 输    入 ：无
* 输    出 ：num 获取的键值，若没有键值则返回 0xff
*******************************************************************************/
unsigned char KeyScan()
{
	unsigned char num;
	num = 0xff;
	if(key1==0)  //判断是否按下键盘
	{
		Delayms(10); //延时,软件去干扰
		if(key1==0)   //确认按键按下
		{
			num = 1;
		}
		while(key1==0);//按键锁定,每按一次count只加1.
	}
	else if(key2==0)  //判断是否按下键盘
	{
		Delayms(10); //延时,软件去干扰
		if(key2==0)   //确认按键按下
		{
			num = 2;
		}
		while(key2==0);//按键锁定,每按一次count只加1.
	}
	else if(key3==0)  //判断是否按下键盘
	{
		Delayms(10); //延时,软件去干扰
		if(key3==0)   //确认按键按下
		{
			num = 3;
		}
		while(key3==0);//按键锁定,每按一次count只加1.
	}
	
return num;
}
