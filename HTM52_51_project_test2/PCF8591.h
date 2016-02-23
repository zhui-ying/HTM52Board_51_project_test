#ifndef __PCF8591_H__
#define __PCF8591_H__
#include <reg52.h>
#include <intrins.h>

#define RESIST_ADD 0x40
#define HOT_ADD 0x41
#define LIGHT_ADD 0x42
#define OUT_ADD 0x43
#define LED_ADD 0x40

//IO�ӿڶ���
sbit PCF_SCL=P2^3;         //����ʱ�������
sbit PCF_SDA=P2^4;   //�������������

void PcfInit();
unsigned char PcfReadAdd(unsigned char control);
void PcfWriteAdd(unsigned char control,unsigned char date);

#endif

