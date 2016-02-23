/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ���ۺ�ʵ��
* ʵ��˵�� �������������ˮ�ƻ�������Ȼ�󰴼���ʾ
*            1.�ⲿ�洢IC��ֵ��-1/s
*            2.AD/DA��·��λ��AD��ֵ
*            3.DS18B20�¶ȼ���ֵ
*            ����.������̵ļ�ֵ
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
#include "AT24C02.h"
#include "DS18B20.h"
#include "PCF8591.h"
#include "KeyScan.h"
#include "system.h"

//�̵���


//ȫ�ֱ�������
volatile unsigned int at24c02_count=0;
volatile unsigned int at24c02_value= 100;
volatile unsigned int Temperature=0;
volatile unsigned int update_count = 0;
volatile unsigned int disp_index=1;
volatile unsigned int resist_vaue = 0;
volatile unsigned int hot_vaue = 0;
volatile unsigned int light_vaue = 0;
volatile unsigned int led_vaue = 0;
volatile unsigned int key_value = 0;
volatile unsigned int send_flag = 0;
volatile unsigned int at24c02_flag = 0;
volatile unsigned int relay_flag = 0;
volatile unsigned int relay_value = 0;

volatile unsigned char MotorStep=0;
//��������
void At24c02Test();
void AD_DATest(void);
void KeyTest();
void LEDdisplayint(unsigned int index,unsigned int num);
void TrafficLEDTest(void);
void RelayTest(void);
void Timer2Init(void);
void StepMotorTest();

/*******************************************************************************
* �� �� �� ��main
* �������� ��������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void main()
{
	
	UsartConfiguration();
	printf("********** Hantech **********\r\n");
	printf("system Init\r\n");
	
	printf("LED Test\r\n");
	LEDInit();
	LEDTest();
	Timer0Init();
	
	printf("AT24C02 Init\r\n");
	X24c02Init();	
	X24c02WriteAdd(2,100);//��д100
	Delayms(10);//��ʱһ��ʱ��
	at24c02_value=X24c02ReadAdd(2);//������������ݸ���at24c02_value
	Read_RomCord();
	
	PcfInit();
	Timer2Init();
	key_value = 1;
	
	while(1)
	{
		if(update_count >= 10)//100ms ����һ������
		{
			update_count = 0;
			TR2 = 0;
			switch(key_value)
			{
				case 1: At24c02Test(); break;
				case 2: AD_DATest(); break; 
				case 3:Temperature = (int)Read_Temperature(); break;
			  case 4:StepMotorTest();break;
				default : break;
			}
	
		}
		KeyTest();//����ɨ�����
		if(TR2 == 1) continue;//����������Բ���Ҫ��ʾ
		switch(key_value)
		{
			case 1: 
				LEDdisplay(1,at24c02_value);
			  break;
			case 2:
				LEDdisplay(2,resist_vaue);
				break;
			case 3: 
				LEDdisplay(3,Temperature);
			  break;
//			case 4:
//				LEDdisplay(4,Temperature);
//				break;	
      default: LEDdisplay(key_value,key_value);
				break;			
		}

//			if(send_flag == 1)
//			{
//				send_flag = 0;
//				printf("at24c02 value: %03d  DS18B20 value: %03d  CF8591 resist: %03d  Key Value: %03d\r\n",at24c02_value,Temperature,resist_vaue,key_value);;
//			}
	}	
}

void Timer2Init(void)		//1000΢��@11.0592MHz
{
	//T2MOD = 0;		//��ʼ��ģʽ�Ĵ���
	//T2CON = 0;		//��ʼ�����ƼĴ���
	TL2 = RCAP2L = 65536-FOSC/12/1000*2;
	TH2 = RCAP2H = (65536-FOSC/12/1000*2)>>8;
	TR2 = 0;		//��ʱ��2��ʼ��ʱ
	ET2 = 1;
}

void StepMotorTest()
{
	TR2 = 1;//��timer2��ʱ��
	dula = 1;
	LED_PORT = 0xff;
	dula = 0;
	wela = 1;
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
* �� �� �� ��At24c02Test
* �������� ��At24c02����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void At24c02Test()
{
	if(at24c02_flag == 1)
	{
		at24c02_flag = 0;
		if(at24c02_value != X24c02ReadAdd(2)) return;
		at24c02_value--;
		Delayms(10);//��ʱһ��ʱ��
		X24c02WriteAdd(2,at24c02_value);	//��24c02�ĵ�ַ2��д������sec_count
	}
}

/*******************************************************************************
* �� �� �� ��AD_DATest
* �������� ��PCF8591����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void AD_DATest(void)
{
	//���⣺������ͬʱ�����ͨ���������˳����ҵ����⣬���λ����ADֵ�ڹ����϶�ȡ����
	resist_vaue = (int)PcfReadAdd(0x40);//RESIST_ADD 
	Delayms(1);
	PcfWriteAdd(LED_ADD,resist_vaue);
	Delayms(1);
}

/*******************************************************************************
* �� �� �� ��Timer0Int
* �������� ����ʱ��0�жϺ��� �� ÿ��TIME_MS ms����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer0Int() interrupt 1 using 2
{
	static unsigned int count1s = 0;
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	update_count++;
	count1s++;
	if(count1s == 100) //1s
	{
		count1s = 0;
		at24c02_flag = 1;
		at24c02_count++;
		relay_flag = 1;
		
		if(at24c02_count >= 120)
		{
			at24c02_count = 0;
		}
		
		disp_index++;
		if(disp_index > 4) disp_index = 1;		
		
		send_flag = 1;//���ڷ�������
	}
		
}

void Timer2Int() interrupt 5 using 1
{
	TF2=0;
			switch(MotorStep)
		{
		   case 0:	
				WX1 = 0;		 // WX1	   
				WX2 = 1;
				WX3 = 1;
				WX4 = 1;
				MotorStep = 1;
		   break;

		   case 1:		 
				WX1 = 0;		 // WX1WX2 
				WX2 = 0;
				WX3 = 1;
				WX4 = 1;
				MotorStep = 2;
		   break;

		   case 2:	   //WX2
				WX1 = 1;
				WX2 = 0;		 
				WX3 = 1;
				WX4 = 1;		   
				MotorStep = 3;
		   break;
		   
		   case 3:		//WX2WX3
				WX1 = 1;
				WX2 = 0;		   
				WX3 = 0;
				WX4 = 1;
				MotorStep = 4;
		   break;
		   case 4:		 //WX3
				WX1 = 1;
				WX2 = 1;		   
				WX3 = 0;
				WX4 = 1;
				MotorStep = 5;
		   break;
		   
		   case 5:			  //WX3WX4
				WX1 = 1;
				WX2 = 1;		    
				WX3 = 0;
				WX4 = 0;
				MotorStep = 6;
		   break;
		   case 6:			  //WX4
				WX1 = 1;
				WX2 = 1;		  
				WX3 = 1;
				WX4 = 0;
				MotorStep = 7;
		   break;
		   case 7:			//WX4WX1
				WX1 = 0;
				WX2 = 1;		   
				WX3 = 1;
				WX4 = 0;
				MotorStep = 0;
		   break;		
		}
	
}
	

