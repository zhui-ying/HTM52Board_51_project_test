#ifndef __REDCTROL_H__
#define __REDCTROL_H__
#include <reg52.h>
#include <intrins.h>

//IO接口定义
sbit IRIN = P3^2;         //红外接收器数据线

unsigned char ReadRedData(void);
void RedExit0Init();

#endif
