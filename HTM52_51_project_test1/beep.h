#ifndef __BEEP_H__
#define __BEEP_H__
#include <reg52.h>
#include <intrins.h>

//IO�ӿڶ���
sbit speak_IO =P1^5 ;               //���������ƽš�

extern unsigned char beep_count;
extern unsigned char code SONG[];

void BeepTime0Init();
void beep1Time();
void PlaySong(unsigned char i);

#endif
