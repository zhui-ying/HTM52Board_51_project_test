#ifndef __NE555_H__
#define __NE555_H__
#include <reg52.h>
#include <intrins.h>

//IO接口定义

extern unsigned long   Freq;        //用来存放要显示的频率值
void NE555Timer1Init();
#endif
