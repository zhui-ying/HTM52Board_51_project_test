#ifndef __DS1302_H__
#define __DS1302_H__
#include <reg52.h>
#include <intrins.h>

//IO�ӿڶ���
sbit DS1302_CLK = P1^1;
sbit DS1302_IO  = P1^2;
sbit DS1302_RST = P1^3;

extern unsigned char second,minute,hour,week,day,month,year;		//�롢�֡�ʱ�����ڡ��ա��¡���

void InitDs1302();
void ReadTime();


#endif
