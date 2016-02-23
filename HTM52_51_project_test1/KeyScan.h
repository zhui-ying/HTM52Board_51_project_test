#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__
#include <reg52.h>
#include <intrins.h>

//IO接口定义
sbit key1 = P3^4;
sbit key2 = P3^6;
sbit key3 = P3^7;
unsigned char KeyScan();

#endif
