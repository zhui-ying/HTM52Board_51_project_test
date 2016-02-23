#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include <reg52.h>
#include <intrins.h>

#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz
//#define FOSC 12000000L //晶振设置，使用12M Hz
//#define FOSC 24000000L //晶振设置，使用24M Hz
#define TIME_MS 10 //设定定时时间 ms ,在11.0592M晶振下，不易超过60ms

#define BAUD 9600

//IO接口定义
#define LED_SEG P1
#define LED_PORT P0
sbit wela = P2^7;
sbit dula=P2^6;
sbit diola = P2^5;
sbit RELAY = P0^5;
//sbit WX1 = P0^0;	  //定义管脚
//sbit WX2 = P0^1;
//sbit WX3 = P0^2;
//sbit WX4 = P0^3;
sbit WX1 = P0^4;	  //定义管脚
sbit WX2 = P0^5;
sbit WX3 = P0^0;
sbit WX4 = P0^1;

//交通灯IO接口
sbit led_d20 = P1^7;
sbit led_d19 = P1^6;
sbit led_d13 = P1^0;
sbit led_d14 = P1^1;
sbit led_d15 = P1^2;
sbit led_d21 = P3^6;
sbit led_d22 = P3^7;
sbit led_d16 = P1^3;
sbit led_d17 = P1^4;
sbit led_d18 = P1^5;

void Delayms(unsigned int ms);
char putchar(char ch);
void UsartConfiguration();
void LEDInit();
void LEDTest();
void Timer0Init();
void LEDdisplay(unsigned int index,unsigned int num);
void TrafficLEDTest(void);

#endif
