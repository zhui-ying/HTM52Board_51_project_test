#ifndef __REDCTROL_H__
#define __REDCTROL_H__
#include <reg52.h>
#include <intrins.h>

//IO�ӿڶ���
sbit IRIN = P3^2;         //���������������

unsigned char ReadRedData(void);
void RedExit0Init();

#endif
