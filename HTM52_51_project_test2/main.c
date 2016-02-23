/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：综合实验
* 实验说明 ：开机点阵和流水灯会闪动，然后按键显示
*            1.外部存储IC的值，-1/s
*            2.AD/DA电路电位器AD的值
*            3.DS18B20温度检测的值
*            其他.矩阵键盘的键值
*            同时串口也会发送具体信息到上位机
* 实验平台 ：航太51单片机开发板 V1.1
* 连接方式 ：
* 注    意 ：
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
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

//继电器


//全局变量定义
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
//函数声明
void At24c02Test();
void AD_DATest(void);
void KeyTest();
void LEDdisplayint(unsigned int index,unsigned int num);
void TrafficLEDTest(void);
void RelayTest(void);
void Timer2Init(void);
void StepMotorTest();

/*******************************************************************************
* 函 数 名 ：main
* 函数功能 ：主函数
* 输    入 ：无
* 输    出 ：无
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
	X24c02WriteAdd(2,100);//先写100
	Delayms(10);//延时一段时间
	at24c02_value=X24c02ReadAdd(2);//读出保存的数据赋于at24c02_value
	Read_RomCord();
	
	PcfInit();
	Timer2Init();
	key_value = 1;
	
	while(1)
	{
		if(update_count >= 10)//100ms 更新一次数据
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
		KeyTest();//键盘扫描测试
		if(TR2 == 1) continue;//步进电机测试不需要显示
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

void Timer2Init(void)		//1000微秒@11.0592MHz
{
	//T2MOD = 0;		//初始化模式寄存器
	//T2CON = 0;		//初始化控制寄存器
	TL2 = RCAP2L = 65536-FOSC/12/1000*2;
	TH2 = RCAP2H = (65536-FOSC/12/1000*2)>>8;
	TR2 = 0;		//定时器2开始计时
	ET2 = 1;
}

void StepMotorTest()
{
	TR2 = 1;//打开timer2定时器
	dula = 1;
	LED_PORT = 0xff;
	dula = 0;
	wela = 1;
}

/*******************************************************************************
* 函 数 名 ：KeyTest
* 函数功能 ：按键扫描，获取按下的键值，若无键值按下，保持原有值
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/ 
void KeyTest()
{
	unsigned char key_temp;
	
	key_temp = KeyScan();
	if(key_temp !=0xff)  key_value = key_temp;
}
/*******************************************************************************
* 函 数 名 ：At24c02Test
* 函数功能 ：At24c02测试
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void At24c02Test()
{
	if(at24c02_flag == 1)
	{
		at24c02_flag = 0;
		if(at24c02_value != X24c02ReadAdd(2)) return;
		at24c02_value--;
		Delayms(10);//延时一段时间
		X24c02WriteAdd(2,at24c02_value);	//在24c02的地址2中写入数据sec_count
	}
}

/*******************************************************************************
* 函 数 名 ：AD_DATest
* 函数功能 ：PCF8591测试
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void AD_DATest(void)
{
	//问题：不可以同时读多个通道，会出现顺序打乱的问题，如电位器的AD值在光敏上读取出来
	resist_vaue = (int)PcfReadAdd(0x40);//RESIST_ADD 
	Delayms(1);
	PcfWriteAdd(LED_ADD,resist_vaue);
	Delayms(1);
}

/*******************************************************************************
* 函 数 名 ：Timer0Int
* 函数功能 ：定时器0中断函数 ， 每隔TIME_MS ms进入
* 输    入 ：无
* 输    出 ：无
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
		
		send_flag = 1;//串口发送数据
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
	

