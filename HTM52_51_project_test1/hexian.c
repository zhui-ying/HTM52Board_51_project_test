#include "hexian.h"
#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz

//ȫ�ֱ�������
bit Beep_EN;
data struct BE Beep;//����ʹ��data�����Σ���������ִ�д���
unsigned char *pSoundSel;//ѡ��ָ�� (*pSoundSel)[10]
unsigned char time_h;
unsigned char time_l;

/*=======================��������������======================*/
#define	 TONE_L1	1
#define	 TONE_L2	2
#define	 TONE_L3	3
#define	 TONE_L4	4
#define	 TONE_L5	5
#define	 TONE_L6	6
#define	 TONE_L7	7
#define	 TONE_M1	8
#define	 TONE_M2	9
#define	 TONE_M3	10
#define	 TONE_M4	11
#define	 TONE_M5	12
#define	 TONE_M6	13
#define	 TONE_M7	14
#define	 TONE_H1	15
#define	 TONE_H2	16
#define	 TONE_H3	17
#define	 TONE_H4	18
#define	 TONE_H5	19
#define	 TONE_H6	20
#define	 TONE_H7	21

#define END 0//��Ƶ����λ 

/*=========================����Ƶ�ʱ�=======================*/
unsigned int code Tune_Tbl[21]=
{
	523	,//	����1	,TONE_L1
	587	,//	����2	,TONE_L2
	659	,//	����3	,TONE_L3
	698	,//	����4	,TONE_L4
	784	,//	����5	,TONE_L5
	880	,//	����6	,TONE_L6
	988	,//	����7	,TONE_L7
	1046	,//	����1	,TONE_M1
	1175	,//	����2	,TONE_M2
	1318	,//	����3	,TONE_M3
	1397	,//	����4	,TONE_M4
	1568	,//	����5	,TONE_M5
	1760	,//	����6	,TONE_M6
	1976	,//	����7	,TONE_M7
	2093	,//	����1	,TONE_H1
	2394	,//	����2	,TONE_H2
	2637	,//	����3	,TONE_H3
	2794	,//	����4	,TONE_H4
	3136	,//	����5	,TONE_H5
	3520	,//	����6	,TONE_H6
	3951	,//	����7	,TONE_H7
};

/*======================�ػ�����1,2,3...====================*/
unsigned char code Sound_ShutDown[10]=		//Ƶ�ʣ����ģ���Ƶʱ��
{
	 TONE_H1,2,14,TONE_H2,2,6,TONE_H3,6,60,END
};
/*======================��������3,2,1...====================*/

unsigned char code Sound_PowerOn[10]=		
{
	TONE_H3,2,6,TONE_H2,2,14, TONE_H1,6,60,END
};
/*======================��������2...=======================*/
unsigned char code Sound_Single[10]=		
{
	TONE_H2,12,60,END,END,END,END,END,END,END,
};

unsigned char code TONE_DefTone4[]=//��ֻ�ϻ�,��ֻ�ϻ�,�ܵÿ죬�ܵÿ�
{
	TONE_H1,2,50,//1
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H1,2,70,//1
	TONE_H1,2,50,//1
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H1,2,70,//1
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H5,2,80,//4
	TONE_H2,2,50,//2
	TONE_H3,2,50,//3
	TONE_H5,2,100,//4
	END,0,0//
};

//�������е���
unsigned char code Sound_all[]=		
{
	1	,	2,30,
	2	,	2,30,
	3	,	2,30,
	4	,	2,30,
	5	,	2,30,
	6	,	2,30,
	7	,	2,30,
	8	,	2,30,
	9	,	2,30,
	10 ,	2,30,
	11	,	2,30,
	12	,	2,30,
	13	,	2,30,
	14	,	2,30,
	15	,	2,30,
	16	,	2,30,
	17	,	2,30,
	18	,	2,30,
	19	,	2,30,
	20	,	2,30,
	21	,	2,30,
	END,
};

#define S_SHUTDOWN 1//�ػ���
#define S_POWERON  2//������
#define S_SINGLE   3//����
#define S_MUSIC    4//��ֻ�ϻ�
#define S_ALL      5//��������

/*******************************************************************************
* �� �� �� ��BeepOn
* �������� ��ѡ���������࣬������������Ӧ�ĸ���
* ��    �� ��SoundSel 
* ��    �� ����
*******************************************************************************/
void BeepOn(unsigned char SoundSel)
{
	Beep_EN=1;
	Beep.index=0;
	
	switch(SoundSel)
	{
	case S_SHUTDOWN:
		pSoundSel=&Sound_ShutDown;
		break;
	
	case S_POWERON:		
		pSoundSel=&Sound_PowerOn;	
		break;
	
	case S_SINGLE:		
		pSoundSel=&Sound_Single;		
		break;
	
		case S_MUSIC:	
		pSoundSel=&TONE_DefTone4;
		break;
		
	case S_ALL:
		pSoundSel=&Sound_all;
	break;
	
	default:
		break;
	}	
}

/*******************************************************************************
* �� �� �� ��BeepSet
* �������� ����������������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void BeepSet()
{	 
	
	if (pSoundSel[Beep.index]!=END)
	{
		Beep.CountFR = 65535-1000000/(Tune_Tbl[pSoundSel[Beep.index]-1] * 2);//��Ҫ������Ƶ��*2
		Beep.PWR_time=pSoundSel[Beep.index+1];
		
		Beep.FREQ_time=pSoundSel[Beep.index+2];
		
		Beep.index=Beep.index+3;
		
	}else {Beep.index=0; PWR_CTRL=1;Beep_EN=0;}
	return;
}

/*******************************************************************************
* �� �� �� ��BeepControl
* �������� ������������ , ÿ10ms����һ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void BeepControl()//ÿ10mS����һ��
{
	
	if (Beep_EN)
	{
		
		if (Beep.PWR_time) 
		{
			PWR_CTRL=0;//����	
			Beep.PWR_time--;
		}
		else 
		{
			PWR_CTRL=1;//����,�������ݷŵ�ʵ�ֺ������� 	
			
		}
		
		if (Beep.FREQ_time)
		{
			Beep.FREQ_time--; 
			TR0 = 1;//������ʱ������
		}
		else 
		{
			TR0 = 0;//ֹͣ��ʱ������
			BeepSet();
			PWR_CTRL=0;//������ϣ��򿪵�Դ�����ݳ��	
		}
	}
	else
	{
		BUZZER_IO=0;
		TR0 = 0;//ֹͣ��ʱ������
		
	}
	return;
}

/*******************************************************************************
* �� �� �� ��InitTime
* �������� ����ʱ����ʼ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void HeXianInitTime()
{ 
	TMOD|=0x11;	//��ʱ��0�Ͷ�ʱ��1������ͬ���� 
	TH0=0xff;
	TL0=0xef;	 
	TH1=(65536-10000)/256;
	TL1=(65536-10000)%256;	//10ms��ʱ
	ET0 = 1; 
	ET1 = 1; 
	TR0=0;
	TR1 = 1;
	EA = 1;
}

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

