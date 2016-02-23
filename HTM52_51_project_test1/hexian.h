#ifndef __HEXIAN_H__
#define __HEXIAN_H__
#include <reg52.h>
#include <intrins.h>

#define S_SHUTDOWN 1//关机音
#define S_POWERON  2//开机音
#define S_SINGLE   3//单音
#define S_MUSIC    4//两只老虎
#define S_ALL      5//所有音符

#define BUZZER_IO	BEEP2		//BEEP IO
#define PWR_CTRL	beer		//蜂鸣器供电控制脚

struct BE
{
	unsigned int	CountFR;	//定时器计数值，通过改变计数值改变音频频率
	unsigned char PWR_time; 	//开启时间,单位为10mS
	unsigned char FREQ_time; 	//音频供给时间,单位为10mS
	unsigned char index;	 	//音符计数，指示当前需要演奏的音符
};

//IO接口定义
sbit BEEP2 = P1^4;
sbit beer = P1^7;               //蜂鸣器控制脚　

extern unsigned char beep_count;
extern unsigned char code SONG[];
extern data struct BE Beep;

void HeXianInitTime();
void PlaySong(unsigned char i);
void BeepOn(unsigned char SoundSel);
void BeepControl();//每10mS调用一次

#endif
