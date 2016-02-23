#ifndef __HEXIAN_H__
#define __HEXIAN_H__
#include <reg52.h>
#include <intrins.h>

#define S_SHUTDOWN 1//�ػ���
#define S_POWERON  2//������
#define S_SINGLE   3//����
#define S_MUSIC    4//��ֻ�ϻ�
#define S_ALL      5//��������

#define BUZZER_IO	BEEP2		//BEEP IO
#define PWR_CTRL	beer		//������������ƽ�

struct BE
{
	unsigned int	CountFR;	//��ʱ������ֵ��ͨ���ı����ֵ�ı���ƵƵ��
	unsigned char PWR_time; 	//����ʱ��,��λΪ10mS
	unsigned char FREQ_time; 	//��Ƶ����ʱ��,��λΪ10mS
	unsigned char index;	 	//����������ָʾ��ǰ��Ҫ���������
};

//IO�ӿڶ���
sbit BEEP2 = P1^4;
sbit beer = P1^7;               //���������ƽš�

extern unsigned char beep_count;
extern unsigned char code SONG[];
extern data struct BE Beep;

void HeXianInitTime();
void PlaySong(unsigned char i);
void BeepOn(unsigned char SoundSel);
void BeepControl();//ÿ10mS����һ��

#endif
