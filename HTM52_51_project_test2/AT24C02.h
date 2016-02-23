#ifndef __AT24C02_H__
#define __AT24C02_H__

#include <reg52.h>
#include <intrins.h>

#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
//#define FOSC 12000000L //�������ã�ʹ��12M Hz
//#define FOSC 24000000L //�������ã�ʹ��24M Hz

//AT24C02�ӿ�
sbit X24_sda=P2^0;			 //IO�ڶ���
sbit X24_scl=P2^1;

void X24c02Init();
void X24c02WriteAdd(unsigned char address,unsigned char info);
unsigned char X24c02ReadAdd(unsigned char address);

#endif
