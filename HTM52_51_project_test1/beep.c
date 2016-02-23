#include "beep.h"
#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
#define T10MS (65536-FOSC/12/100)

unsigned char beep_count;

unsigned char code SONG[] ={      //ף��ƽ��
0x60,0x30,0x4c,0x30,0x40,0x30,0x33,0x30,0x2b,0x30,0x24,0x30,0x1c,0x30,
0x18,0x30,0x13,0x30,0x10,0x30,0x00,
	
//	0x26,0x20,0x20,0x20,0x20,0x20,0x26,0x10,0x20,0x10,0x20,0x80,
//	0x26,0x20,0x30,0x20,0x30,0x20,0x39,0x10,0x30,0x10,0x30,0x80,
//	0x26,0x20,0x20,0x20,0x20,0x20,0x1c,0x20,0x20,0x80,0x2b,0x20,
//	0x26,0x20,0x20,0x20,0x2b,0x10,0x26,0x10,0x2b,0x80,0x26,0x20,
//	0x30,0x20,0x30,0x20,0x39,0x10,0x26,0x10,0x26,0x60,0x40,0x10,
//	0x39,0x10,0x26,0x20,0x30,0x20,0x30,0x20,0x39,0x10,0x26,0x10,
//	0x26,0x80,0x26,0x20,0x2b,0x10,0x2b,0x10,0x2b,0x20,0x30,0x10,
//	0x39,0x10,0x26,0x10,0x2b,0x10,0x2b,0x20,0x2b,0x40,0x40,0x20,
//	0x20,0x10,0x20,0x10,0x2b,0x10,0x26,0x30,0x30,0x80,0x18,0x20,
//	0x18,0x20,0x26,0x20,0x20,0x20,0x20,0x40,0x26,0x20,0x2b,0x20,
//	0x30,0x20,0x30,0x20,0x1c,0x20,0x20,0x20,0x20,0x80,0x1c,0x20,
//	0x1c,0x20,0x1c,0x20,0x30,0x20,0x30,0x60,0x39,0x10,0x30,
//	0x10,0x20,0x20,0x2b,0x10,0x26,0x10,0x2b,0x10,0x26,0x10,0x26,
//	0x10,0x2b,0x10,0x2b,0x80,0x18,0x20,0x18,0x20,0x26,0x20,0x20,
//	0x20,0x20,0x60,0x26,0x10,0x2b,0x20,0x30,0x20,0x30,0x20,0x1c,
//	0x20,0x20,0x20,0x20,0x80,0x26,0x20,0x30,0x10,0x30,0x10,0x30,
//	0x20,0x39,0x20,0x26,0x10,0x2b,0x10,0x2b,0x20,0x2b,0x40,0x40,
//	0x10,0x40,0x10,0x20,0x10,
//	0x20,0x10,0x2b,0x10,0x26,0x30,0x30,0x80,0x00,
//	//·�ߵ�Ұ����Ҫ��
//	0x30,0x1C,0x10,0x20,0x40,0x1C,0x10,0x18,0x10,0x20,0x10,0x1C,
//	0x10,0x18,0x40,0x1C,0x20,0x20,0x20,0x1C,0x20,0x18,0x20,0x20,
//	0x80,0xFF,0x20,0x30,0x1C,0x10,0x18,0x20,0x15,0x20,0x1C,0x20,
//	0x20,0x20,0x26,0x40,0x20,0x20,0x2B,0x20,0x26,0x20,0x20,0x20,
//	0x30,0x80,0xFF,0x20,0x20,0x1C,0x10,0x18,0x10,0x20,0x20,0x26,
//	0x20,0x2B,0x20,0x30,
//	0x20,0x2B,0x40,0x20,0x20,0x1C,0x10,0x18,0x10,0x20,0x20,0x26,
//	0x20,0x2B,0x20,0x30,0x20,0x2B,0x40,0x20,0x30,0x1C,0x10,0x18,
//	0x20,0x15,0x20,0x1C,0x20,0x20,0x20,0x26,0x40,0x20,0x20,0x2B,
//	0x20,0x26,0x20,0x20,0x20,0x30,0x80,0x20,0x30,0x1C,0x10,0x20,
//	0x10,0x1C,0x10,0x20,0x20,0x26,0x20,0x2B,0x20,0x30,0x20,0x2B,
//	0x40,0x20,0x15,0x1F,
//	0x05,0x20,0x10,0x1C,0x10,0x20,0x20,0x26,0x20,0x2B,0x20,0x30,
//	0x20,0x2B,0x40,0x20,0x30,0x1C,0x10,0x18,0x20,0x15,0x20,0x1C,
//	0x20,0x20,0x20,0x26,0x40,0x20,0x20,0x2B,0x20,0x26,0x20,0x20,
//	0x20,0x30,0x30,0x20,0x30,0x1C,0x10,0x18,0x40,0x1C,0x20,0x20,
//	0x20,0x26,0x40,0x13,0x60,0x18,0x20,0x15,0x40,0x13,0x40,0x18,
//	0x80,0x00,
};

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

/*******************************************************************************
* �� �� �� ��Delayus
* �������� ��us����ʱ
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Delayus(unsigned int us)
{
	  unsigned int i,j;

    for( i =0;i < us;i++ )

    {
        for( j =0;j<3;j++ );
    }	
}


/*******************************************************************************
* �� �� �� ��Time0_Init
* �������� ����ʱ��0��ʼ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void BeepTime0Init()  
{
	TMOD = 0x01;  //����ģʽѡ��
	IE = 0x82;  //�ж�����
	TH0  = T10MS>>8;   //װ��ֵ,��ʱ10ms
	TL0  = T10MS; 
}

/*******************************************************************************
* �� �� �� ��PlaySong
* �������� �����������ӳ���iΪ������һ����Ŀ
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void PlaySong(unsigned char i)
{
	unsigned char Temp1,Temp2;
	unsigned int Addr;
	beep_count = 0;                               //�жϼ�������0
	Addr = i * 217;
	while(1)
	{
		Temp1 = SONG[Addr++];  //��ȡƵ��
		if ( Temp1 == 0xFF )          //��ֹ��
		{
			TR0 = 0;
			Delayus(100);
		}
		else if ( Temp1 == 0x00 )   //����������
		{
			return;
		}
		else
		{
			Temp2 = SONG[Addr++];//��ȡһ������ʱ��
			TR0 = 1;
			while(1)
			{
				speak_IO = ~speak_IO;
				Delayus(Temp1);
				if ( Temp2 == beep_count )//һ��������
				{
					beep_count = 0;
					break;
				}
			}
		}
	}
}

/*******************************************************************************
* �� �� �� ��beep
* �������� ����������һ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void beep1Time()
{
  unsigned char i;
  for (i=0;i<180;i++)
   {
     Delayms(1);
     speak_IO=!speak_IO;                 //BEEPȡ��
   } 
  speak_IO=1;                      //�رշ�����
}


