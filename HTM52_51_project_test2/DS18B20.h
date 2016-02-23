#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <reg52.h>
#include <intrins.h>

sbit DQ = P2^2;  //����DS18B20�˿�DQ  

bit Init_DS18B20(void);
unsigned char Read_Temperature(void);
void Read_RomCord(void);

#endif
