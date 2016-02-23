/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ���ۺ�ʵ��
* ʵ��˵�� : 1.����������
*            2.DS1302
*            3.����
*            ͬʱ����Ҳ�ᷢ�;�����Ϣ����λ��
* ʵ��ƽ̨ ����̫51��Ƭ�������� V1.1
* ���ӷ�ʽ ��
* ע    �� ��
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <stdarg.h>
#include "KeyScan.h"
#include "beep.h"
#include "hexian.h"
#include "NE555.h"
#include "DS1302.h"
#include "redctrol.h"

#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
//#define FOSC 12000000L //�������ã�ʹ��12M Hz
//#define FOSC 24000000L //�������ã�ʹ��24M Hz
#define TIME_MS 10 //�趨��ʱʱ�� ms ,��11.0592M�����£����׳���60ms

#define BAUD 9600

//IO�ӿڶ���
#define LED_SEG P1
#define LED_PORT P0
sbit wela = P2^7;
sbit dula=P2^6;
sbit diola = P2^5;

//ȫ�ֱ�������
volatile unsigned int update_count = 0;
volatile unsigned int key_value = 0;
volatile unsigned int flag = 0;
volatile unsigned long freq_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int red_value = 0;

//LED��ʾ��ģ 0-F ����ģʽ
unsigned code table[]= {0Xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

//��������
void Delayms(unsigned int ms);
char putchar(char ch);
//void UsartConfiguration();

void Timer0Init();
void LEDdisplay(unsigned int index,unsigned int num);
void KeyTest();
void BeepTest(void);
void HexianTest();
void DS1302Test();

/*******************************************************************************
* �� �� �� ��main
* �������� ��������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void main()
{
	
//	UsartConfiguration();
	flag = 0;
	BeepTest();//����������
	EA = 0;	
	flag = 1;
	HexianTest();//����������	
	
	EA = 0;
	flag = 2;
	Delayms(100);
	InitDs1302();
	Timer0Init();
	NE555Timer1Init();
	RedExit0Init();
	
	
	TR1=1;
	
	key_value = 1;
	
	while(1)
	{
		if(update_count >= 10)//100ms ����һ������
		{
			update_count = 0;
			switch(key_value)
			{
				case 1: flag = 2; TR1=1;EX0 = 0; break;
				case 2: flag = 3; TR1=0;EX0 = 0; DS1302Test(); break;
				case 3: flag = 4; TR1=0;EX0 = 1;break;
				default : break;
			}
	
		}
		KeyTest();//����ɨ�����
		switch(key_value)
		{
			case 1: 
				LEDdisplay(1,freq_value);
			  break;
			case 2:
				LEDdisplay(2,second_value);
				break;
			case 3: 
				LEDdisplay(3,red_value);
			  break;
				break;	
      default: 
				LEDdisplay(key_value,key_value);
				break;			
		}
	}	
}

/*******************************************************************************
* �� �� �� ��BeepTest
* �������� ������������,������ִ��һ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void BeepTest(void)
{
//	BeepTime0Init();
//	PlaySong(1);
	
	beep1Time();
	Delayms(200);
	beep1Time();
	Delayms(200);
	beep1Time();	
	Delayms(200);
	beep1Time();	
}

/*******************************************************************************
* �� �� �� ��HexianTest
* �������� ������������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void HexianTest()
{
		PWR_CTRL = 0;
	BUZZER_IO = 0;
	Delayms(700);
	HeXianInitTime();
	Delayms(500);
	BeepOn(S_POWERON);
	Delayms(1000);
	//InitTime();
	
}

/*******************************************************************************
* �� �� �� ��
* �������� ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/ 
void DS1302Test()
{
	ReadTime();
	second_value = (int)second;
}

/*******************************************************************************
* �� �� �� ��KeyTest
* �������� ������ɨ�裬��ȡ���µļ�ֵ�����޼�ֵ���£�����ԭ��ֵ
* ��    �� ����
* ��    �� ����
*******************************************************************************/ 
void KeyTest()
{
	unsigned char key_temp;
	
	key_temp = KeyScan();
	if(key_temp !=0xff)  key_value = key_temp;
}

/*******************************************************************************
* �� �� �� ��Timer0Int
* �������� ����ʱ��0�жϺ��� �� ÿ��TIME_MS ms����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer0Int() interrupt 1 using 1
{
	static unsigned int count1s = 0;
	
	if(flag == 1)
	{
			TH0=Beep.CountFR/256;	 
			TL0=Beep.CountFR%256;	
			BUZZER_IO = ~BUZZER_IO;	
			return;
	}
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	update_count++;
	count1s++;
	beep_count++;

	if(count1s == 50) //0.5s
	{
		count1s = 0;
		if(flag == 2)
		{
			TR1=0;		
			//����Ƶ��
			Freq = Freq + TL1;         //��ȡTL��ֵ
			Freq = Freq + (TH1 * 256); //��ȡTH��ֵ
			freq_value = Freq*2;
			Freq = 0;
			TH1 = 0; //����������ֵ����
			TL1 = 0;
			TR1 = 1; //����������	
		}
	}
		
}

/*******************************************************************************
* �� �� �� ��putchar
* �������� ������ϵͳ�Դ���putchar������ʵ��printf����
* ��    �� ��Ҫ���͵Ĳ���
* ��    �� �����ͳɹ��Ĳ���
*******************************************************************************/
char putchar(char ch)
{ 
	/* Place your implementation of fputc here */ 
	SBUF=(unsigned char)ch; //�����յ������ݷ��뵽���ͼĴ���
	while(!TI);		  //�ȴ������������
	TI=0;		 //���������ɱ�־λ	
	return ch;
}

/*******************************************************************************
* �� �� �� ��Delayms
* �������� ��ʵ�� ms������ʱ
* ��    �� ��ms
* ��    �� ����
*******************************************************************************/
void Delayms(unsigned int ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
	#if FOSC == 11059200L
	for(j=0;j<114;j++);
	#elif FOSC == 12000000L
	for(j=0;j<123;j++);
	#elif FOSC == 24000000L
	for(j=0;j<249;j++);
	#else
	for(j=0;j<114;j++);
	#endif
}

/*******************************************************************************
* �� �� �� ��UsartConfiguration
* �������� ����������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
//void UsartConfiguration()
//{
//	SCON = 0X50;			//����Ϊ������ʽ1	10λ�첽�շ���
//	TMOD = 0x20; //���ü�����������ʽ2  8λ�Զ���װ������	
//	PCON = 0X80;//�����ʼӱ�	SMOD = 1  28800
//	TH1 = 256 -(FOSC/12/32/(BAUD/2)); //���������
//	TL1 = 256 -(FOSC/12/32/(BAUD/2));
//	TR1 = 1; //�򿪶�ʱ��	
////	ES=1;//����رգ������Ӱ�춨ʱ��
//}

/*******************************************************************************
* �� �� �� ��Timer0Init
* �������� ����ʱ��0��ʼ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer0Init()
{	
	TMOD=  0x01; //���ö�ʱ��0������ʽΪ1
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	ET0=1; //������ʱ��0�ж�
	TR0=1;	//������ʱ��	
	EA=1;  //�����ж�
}

/*******************************************************************************
* �� �� �� ��Timer1Int
* �������� ����ʱ��1�жϷ������� ���ƽ���ʱ�� 10ms
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer1Int()	interrupt 3	using 3 //�����ж�3 ����Ƶ��	
{	
	if(flag == 1)
	{
		TH1=(65536-10000)/256;
		TL1=(65536-10000)%256;	//10ms��ʱ
		BeepControl(); 
	//	Beep_control(); 
		return;
	}
	
		//--����һ���жϣ�������������65536--//
	Freq=Freq+65536;	
	
}	 

/*******************************************************************************
* �� �� �� ��Exit0Int
* �������� ���ⲿ�ж�0 ISR
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Exit0Int() interrupt 0 
{
	unsigned char temp;
	temp = ReadRedData();
	if(temp != 0xff)
	{
		red_value = (int)temp;
	}
}

/*******************************************************************************
* �� �� �� ��LEDdisplay
* �������� ��ѭ����ʾ����λ�ϵ�����
* ��    �� ��index ���   num Ҫ��ʾ������
* ��    �� ����
*******************************************************************************/
void LEDdisplay(unsigned int index,unsigned int num)
{
	unsigned char bai,shi,ge;
	index = index%10;
	bai=num/100%10;
	shi=num%100/10;
	ge=num%10;

	wela=1;//��ʾ���
	LED_PORT=0x01;
	wela=0;	
	dula=1;			   
	LED_PORT=table[index];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x08;
	wela=0;	
	dula=1;			   //��ʾ��λ
	LED_PORT=table[bai];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x10;
	wela=0;
	dula=1;			   //��ʾʮλ
	LED_PORT=table[shi];	 
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x20;
	wela=0;
	dula=1;			 // ��ʾ��λ
	LED_PORT=table[ge];
	dula=0;
	Delayms(1);
}
