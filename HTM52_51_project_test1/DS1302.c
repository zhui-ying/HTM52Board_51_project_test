#include "DS1302.h"

#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
//#define FOSC 12000000L //�������ã�ʹ��12M Hz
//#define FOSC 24000000L //�������ã�ʹ��24M Hz

#define TIME_MS 50 //�趨��ʱʱ�� ms ,��11.0592M�����£����׳���60ms

//IO�ӿڶ���

//ȫ�ֱ�������
unsigned char count,temp;
volatile unsigned char second,minute,hour,week,day,month,year;		//�롢�֡�ʱ�����ڡ��ա��¡���
unsigned char time[]={0x06,0x03,0x14,0x03,0x15,0x42,0x55};	//��ʼʱ������


/*******************************************************************************
* �� �� �� ��Delayms
* �������� ��ʵ�� ms������ʱ
* ��    �� ��ms
* ��    �� ����
*******************************************************************************/
static void Delayms(unsigned int ms)
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

/*****************************************************************************
* �� �� �� ��InputByte
* �������ܣ���DS1302��һ�ֽ������ӳ���
* ��    �룺byte1
* ��    ����
*****************************************************************************/
void InputByte(unsigned char byte1)
{
	char i;
	for(i=8;i>0;i--)
	{
		DS1302_IO=(bit)(byte1&0x01);
		DS1302_CLK=1;
		_nop_();_nop_();
		DS1302_CLK=0;
		byte1>>=1;
	}
	return;
}

/*****************************************************************************
* �� �� ����outputbyte
* �������ܣ���DS1302һ���ֽ��ӳ���
* ��    �룺
* ��    ������ȡ����ֵ
*****************************************************************************/
unsigned char OutPutByte(void)  
{
	unsigned char i;
	unsigned ucdat=0;
	for(i=8;i>0;i--)
	{
		DS1302_IO=1;
		ucdat>>=1;
		if(DS1302_IO)ucdat|=0x80;
		DS1302_CLK=1;
		_nop_();_nop_();
		DS1302_CLK=0;
	 }
	 return(ucdat);
}

/*****************************************************************************
* �� �� ����write_ds1302
* �������ܣ���DS1302ĳ��ַдһ�ֽ������ӳ���
* ��    �룺addr,TDat
* ��    ����
*****************************************************************************/
void WriteDs1302(unsigned char addr,unsigned char TDat)
{
	DS1302_RST=0;
	_nop_();_nop_();
	DS1302_CLK=0;
	_nop_();_nop_();
	DS1302_RST=1;
 	InputByte(addr);//����8�ֽڵ�ַ����
	_nop_();_nop_();
	InputByte(TDat);//д����������
	DS1302_CLK=1;
	_nop_();_nop_();
	DS1302_RST=0;
}

/*****************************************************************************
* �� �� ����read_ds1302
* �������ܣ���DS1302��ַ�ӳ���
* ��    �룺add
* ��    ����timedata
*****************************************************************************/
unsigned char ReadDs1302(unsigned char addr)
{
	unsigned char timedata,aa;
	DS1302_RST=0;
	_nop_();_nop_();
	DS1302_CLK=0;
	_nop_();_nop_();
	DS1302_RST=1;
	InputByte(addr);//����8�ֽڵ�ַ����
	timedata=OutPutByte();//��ȡ�ֽ�����
	DS1302_CLK=1;
	_nop_();_nop_();
	DS1302_RST=0;
	aa=timedata%16;
	timedata=timedata/16;
	timedata=timedata*10+aa;
	return(timedata);
}

/*****************************************************************************
* �� �� ����initial_ds1302
* �������ܣ���ʼ��DS1302�ӳ���
* ��    �룺time[](ȫ�ֱ���)
* ��    ����
*****************************************************************************/
void InitDs1302()
{
	WriteDs1302(0x8e,0x00);		//д�����Ĵ������ڶ�ʱ�ӻ�RAMдǰWPһ��ҪΪ0
	Delayms(5);
	WriteDs1302(0x8c,time[0]);		//��
	Delayms(5);
	WriteDs1302(0x88,time[1]);		//��
	Delayms(5);
	WriteDs1302(0x86,time[2]);		//��
	Delayms(5);
	WriteDs1302(0x8A,time[3]);		//����
	Delayms(5);
	WriteDs1302(0x84,time[4]);		//ʱ
	Delayms(5);
	WriteDs1302(0x82,time[5]);		//��
	Delayms(5);
	WriteDs1302(0x80,time[6]);		//��
	Delayms(5);
	WriteDs1302(0x8e,0x80);		//д�����Ĵ���
	Delayms(5);
}

/*****************************************************************************
* �� �� ����read_time
* �������ܣ���DS1302ʱ���ӳ���
* ��    �룺
* ��    ����ȫ�ֱ���(second,minute,hour,week,day,month,year)
*****************************************************************************/
void ReadTime()
{
	second=ReadDs1302(0x81);		//��Ĵ���

	minute=ReadDs1302(0x83);		//��
//	Delayms(5);
	hour=ReadDs1302(0x85);			//ʱ
//	Delayms(5);
	week=ReadDs1302(0x8B);			//����
//	Delayms(5);
	day=ReadDs1302(0x87);			//��
//	Delayms(5);
	month=ReadDs1302(0x89);		//��
//	Delayms(5);
	year=ReadDs1302(0x8d);
//	Delayms(5);			//��
}

