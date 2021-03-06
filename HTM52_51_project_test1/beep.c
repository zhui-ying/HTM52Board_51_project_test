#include "beep.h"
#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz
#define T10MS (65536-FOSC/12/100)

unsigned char beep_count;

unsigned char code SONG[] ={      //祝你平安
0x60,0x30,0x4c,0x30,0x40,0x30,0x33,0x30,0x2b,0x30,0x24,0x30,0x1c,0x30,
0x18,0x30,0x13,0x30,0x10,0x30,0x00,
	
//	0x26,0x20,0x20,0x20,0x20,0x20,0x26,0x10,0x20,0x10,0x20,0x80,
//	0x26,0x20,0x30,0x20,0x30,0x20,0x39,0x10,0x30,0x10,0x30,0x80,
//	0x26,0x20,0x20,0x20,0x20,0x20,0x1c,0x20,0x20,0x80,0x2b,0x20,
//	0x26,0x20,0x20,0x20,0x2b,0x10,0x26,0x10,0x2b,0x80,0x26,0x20,
//	0x30,0x20,0x30,0x20,0x39,0x10,0x26,0x10,0x26,0x60,0x40,0x10,
//	0x39,0x10,0x26,0x20,0x30,0x20,0x30,0x20,0x39,0x10,0x26,0x10,
//	0x26,0x80,0x26,0x20,0x2b,0x10,0x2b,0x10,0x2b,0x20,0x30,0x10,
//	0x39,0x10,0x26,0x10,0x2b,0x10,0x2b,0x20,0x2b,0x40,0x40,0x20,
//	0x20,0x10,0x20,0x10,0x2b,0x10,0x26,0x30,0x30,0x80,0x18,0x20,
//	0x18,0x20,0x26,0x20,0x20,0x20,0x20,0x40,0x26,0x20,0x2b,0x20,
//	0x30,0x20,0x30,0x20,0x1c,0x20,0x20,0x20,0x20,0x80,0x1c,0x20,
//	0x1c,0x20,0x1c,0x20,0x30,0x20,0x30,0x60,0x39,0x10,0x30,
//	0x10,0x20,0x20,0x2b,0x10,0x26,0x10,0x2b,0x10,0x26,0x10,0x26,
//	0x10,0x2b,0x10,0x2b,0x80,0x18,0x20,0x18,0x20,0x26,0x20,0x20,
//	0x20,0x20,0x60,0x26,0x10,0x2b,0x20,0x30,0x20,0x30,0x20,0x1c,
//	0x20,0x20,0x20,0x20,0x80,0x26,0x20,0x30,0x10,0x30,0x10,0x30,
//	0x20,0x39,0x20,0x26,0x10,0x2b,0x10,0x2b,0x20,0x2b,0x40,0x40,
//	0x10,0x40,0x10,0x20,0x10,
//	0x20,0x10,0x2b,0x10,0x26,0x30,0x30,0x80,0x00,
//	//路边的野华不要采
//	0x30,0x1C,0x10,0x20,0x40,0x1C,0x10,0x18,0x10,0x20,0x10,0x1C,
//	0x10,0x18,0x40,0x1C,0x20,0x20,0x20,0x1C,0x20,0x18,0x20,0x20,
//	0x80,0xFF,0x20,0x30,0x1C,0x10,0x18,0x20,0x15,0x20,0x1C,0x20,
//	0x20,0x20,0x26,0x40,0x20,0x20,0x2B,0x20,0x26,0x20,0x20,0x20,
//	0x30,0x80,0xFF,0x20,0x20,0x1C,0x10,0x18,0x10,0x20,0x20,0x26,
//	0x20,0x2B,0x20,0x30,
//	0x20,0x2B,0x40,0x20,0x20,0x1C,0x10,0x18,0x10,0x20,0x20,0x26,
//	0x20,0x2B,0x20,0x30,0x20,0x2B,0x40,0x20,0x30,0x1C,0x10,0x18,
//	0x20,0x15,0x20,0x1C,0x20,0x20,0x20,0x26,0x40,0x20,0x20,0x2B,
//	0x20,0x26,0x20,0x20,0x20,0x30,0x80,0x20,0x30,0x1C,0x10,0x20,
//	0x10,0x1C,0x10,0x20,0x20,0x26,0x20,0x2B,0x20,0x30,0x20,0x2B,
//	0x40,0x20,0x15,0x1F,
//	0x05,0x20,0x10,0x1C,0x10,0x20,0x20,0x26,0x20,0x2B,0x20,0x30,
//	0x20,0x2B,0x40,0x20,0x30,0x1C,0x10,0x18,0x20,0x15,0x20,0x1C,
//	0x20,0x20,0x20,0x26,0x40,0x20,0x20,0x2B,0x20,0x26,0x20,0x20,
//	0x20,0x30,0x30,0x20,0x30,0x1C,0x10,0x18,0x40,0x1C,0x20,0x20,
//	0x20,0x26,0x40,0x13,0x60,0x18,0x20,0x15,0x40,0x13,0x40,0x18,
//	0x80,0x00,
};

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
* 函 数 名 ：Delayus
* 函数功能 ：us级延时
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Delayus(unsigned int us)
{
	  unsigned int i,j;

    for( i =0;i < us;i++ )

    {
        for( j =0;j<3;j++ );
    }	
}


/*******************************************************************************
* 函 数 名 ：Time0_Init
* 函数功能 ：定时器0初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void BeepTime0Init()  
{
	TMOD = 0x01;  //工作模式选择
	IE = 0x82;  //中断设置
	TH0  = T10MS>>8;   //装初值,延时10ms
	TL0  = T10MS; 
}

/*******************************************************************************
* 函 数 名 ：PlaySong
* 函数功能 ：歌曲播放子程序i为播放哪一段曲目
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void PlaySong(unsigned char i)
{
	unsigned char Temp1,Temp2;
	unsigned int Addr;
	beep_count = 0;                               //中断计数器清0
	Addr = i * 217;
	while(1)
	{
		Temp1 = SONG[Addr++];  //读取频率
		if ( Temp1 == 0xFF )          //休止符
		{
			TR0 = 0;
			Delayus(100);
		}
		else if ( Temp1 == 0x00 )   //歌曲结束符
		{
			return;
		}
		else
		{
			Temp2 = SONG[Addr++];//读取一个音的时长
			TR0 = 1;
			while(1)
			{
				speak_IO = ~speak_IO;
				Delayus(Temp1);
				if ( Temp2 == beep_count )//一个音结束
				{
					beep_count = 0;
					break;
				}
			}
		}
	}
}

/*******************************************************************************
* 函 数 名 ：beep
* 函数功能 ：蜂鸣器响一声
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void beep1Time()
{
  unsigned char i;
  for (i=0;i<180;i++)
   {
     Delayms(1);
     speak_IO=!speak_IO;                 //BEEP取反
   } 
  speak_IO=1;                      //关闭蜂鸣器
}



