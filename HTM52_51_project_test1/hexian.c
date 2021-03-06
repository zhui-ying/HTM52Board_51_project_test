#include "hexian.h"
#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz

//全局变量定义
bit Beep_EN;
data struct BE Beep;//必须使用data型修饰，否则程序会执行错误
unsigned char *pSoundSel;//选曲指针 (*pSoundSel)[10]
unsigned char time_h;
unsigned char time_l;

/*=======================以下是音调定义======================*/
#define	 TONE_L1	1
#define	 TONE_L2	2
#define	 TONE_L3	3
#define	 TONE_L4	4
#define	 TONE_L5	5
#define	 TONE_L6	6
#define	 TONE_L7	7
#define	 TONE_M1	8
#define	 TONE_M2	9
#define	 TONE_M3	10
#define	 TONE_M4	11
#define	 TONE_M5	12
#define	 TONE_M6	13
#define	 TONE_M7	14
#define	 TONE_H1	15
#define	 TONE_H2	16
#define	 TONE_H3	17
#define	 TONE_H4	18
#define	 TONE_H5	19
#define	 TONE_H6	20
#define	 TONE_H7	21

#define END 0//音频结束位 

/*=========================音调频率表=======================*/
unsigned int code Tune_Tbl[21]=
{
	523	,//	低音1	,TONE_L1
	587	,//	低音2	,TONE_L2
	659	,//	低音3	,TONE_L3
	698	,//	低音4	,TONE_L4
	784	,//	低音5	,TONE_L5
	880	,//	低音6	,TONE_L6
	988	,//	低音7	,TONE_L7
	1046	,//	中音1	,TONE_M1
	1175	,//	中音2	,TONE_M2
	1318	,//	中音3	,TONE_M3
	1397	,//	中音4	,TONE_M4
	1568	,//	中音5	,TONE_M5
	1760	,//	中音6	,TONE_M6
	1976	,//	中音7	,TONE_M7
	2093	,//	高音1	,TONE_H1
	2394	,//	高音2	,TONE_H2
	2637	,//	高音3	,TONE_H3
	2794	,//	高音4	,TONE_H4
	3136	,//	高音5	,TONE_H5
	3520	,//	高音6	,TONE_H6
	3951	,//	高音7	,TONE_H7
};

/*======================关机和弦1,2,3...====================*/
unsigned char code Sound_ShutDown[10]=		//频率，节拍，音频时间
{
	 TONE_H1,2,14,TONE_H2,2,6,TONE_H3,6,60,END
};
/*======================开机和弦3,2,1...====================*/

unsigned char code Sound_PowerOn[10]=		
{
	TONE_H3,2,6,TONE_H2,2,14, TONE_H1,6,60,END
};
/*======================单音和弦2...=======================*/
unsigned char code Sound_Single[10]=		
{
	TONE_H2,12,60,END,END,END,END,END,END,END,
};

unsigned char code TONE_DefTone4[]=//两只老虎,两只老虎,跑得快，跑得快
{
	TONE_H1,2,50,//1
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H1,2,70,//1
	TONE_H1,2,50,//1
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H1,2,70,//1
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H5,2,80,//4
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H5,2,100,//4
	END,0,0//
};

//演奏所有的音
unsigned char code Sound_all[]=		
{
	1	,	2,30,
	2	,	2,30,
	3	,	2,30,
	4	,	2,30,
	5	,	2,30,
	6	,	2,30,
	7	,	2,30,
	8	,	2,30,
	9	,	2,30,
	10 ,	2,30,
	11	,	2,30,
	12	,	2,30,
	13	,	2,30,
	14	,	2,30,
	15	,	2,30,
	16	,	2,30,
	17	,	2,30,
	18	,	2,30,
	19	,	2,30,
	20	,	2,30,
	21	,	2,30,
	END,
};

#define S_SHUTDOWN 1//关机音
#define S_POWERON  2//开机音
#define S_SINGLE   3//单音
#define S_MUSIC    4//两只老虎
#define S_ALL      5//所有音符

/*******************************************************************************
* 函 数 名 ：BeepOn
* 函数功能 ：选择声音种类，蜂鸣器唱出相应的歌曲
* 输    入 ：SoundSel 
* 输    出 ：无
*******************************************************************************/
void BeepOn(unsigned char SoundSel)
{
	Beep_EN=1;
	Beep.index=0;
	
	switch(SoundSel)
	{
	case S_SHUTDOWN:
		pSoundSel=&Sound_ShutDown;
		break;
	
	case S_POWERON:		
		pSoundSel=&Sound_PowerOn;	
		break;
	
	case S_SINGLE:		
		pSoundSel=&Sound_Single;		
		break;
	
		case S_MUSIC:	
		pSoundSel=&TONE_DefTone4;
		break;
		
	case S_ALL:
		pSoundSel=&Sound_all;
	break;
	
	default:
		break;
	}	
}

/*******************************************************************************
* 函 数 名 ：BeepSet
* 函数功能 ：蜂鸣器参数设置
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void BeepSet()
{	 
	
	if (pSoundSel[Beep.index]!=END)
	{
		Beep.CountFR = 65535-1000000/(Tune_Tbl[pSoundSel[Beep.index]-1] * 2);//需要对音调频率*2
		Beep.PWR_time=pSoundSel[Beep.index+1];
		
		Beep.FREQ_time=pSoundSel[Beep.index+2];
		
		Beep.index=Beep.index+3;
		
	}else {Beep.index=0; PWR_CTRL=1;Beep_EN=0;}
	return;
}

/*******************************************************************************
* 函 数 名 ：BeepControl
* 函数功能 ：蜂鸣器控制 , 每10ms调用一次
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void BeepControl()//每10mS调用一次
{
	
	if (Beep_EN)
	{
		
		if (Beep.PWR_time) 
		{
			PWR_CTRL=0;//供电	
			Beep.PWR_time--;
		}
		else 
		{
			PWR_CTRL=1;//掉电,依靠电容放电实现和弦拖音 	
			
		}
		
		if (Beep.FREQ_time)
		{
			Beep.FREQ_time--; 
			TR0 = 1;//启动定时器发声
		}
		else 
		{
			TR0 = 0;//停止定时器发声
			BeepSet();
			PWR_CTRL=0;//发声完毕，打开电源给电容充电	
		}
	}
	else
	{
		BUZZER_IO=0;
		TR0 = 0;//停止定时器发声
		
	}
	return;
}

/*******************************************************************************
* 函 数 名 ：InitTime
* 函数功能 ：定时器初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void HeXianInitTime()
{ 
	TMOD|=0x11;	//定时器0和定时器1采用相同配置 
	TH0=0xff;
	TL0=0xef;	 
	TH1=(65536-10000)/256;
	TL1=(65536-10000)%256;	//10ms定时
	ET0 = 1; 
	ET1 = 1; 
	TR0=0;
	TR1 = 1;
	EA = 1;
}

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

