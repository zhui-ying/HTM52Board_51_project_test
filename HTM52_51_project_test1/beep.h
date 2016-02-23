#ifndef __BEEP_H__
#define __BEEP_H__
#include <reg52.h>
#include <intrins.h>

//IO接口定义
sbit speak_IO =P1^5 ;               //蜂鸣器控制脚　

extern unsigned char beep_count;
extern unsigned char code SONG[];

void BeepTime0Init();
void beep1Time();
void PlaySong(unsigned char i);

#endif
