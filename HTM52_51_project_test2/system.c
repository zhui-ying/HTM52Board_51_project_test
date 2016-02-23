#include "system.h"

//LED显示字模 0-F 共阴模式
unsigned code table[]= {0Xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

/*******************************************************************************
* 函 数 名 ：Delayms
* 函数功能 ：实现 ms级的延时
* 输    入 ：ms
* 输    出 ：无
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
* 函 数 名 ：UsartConfiguration
* 函数功能 ：串口设置
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void UsartConfiguration()
{
	SCON = 0X50;			//设置为工作方式1	10位异步收发器
	TMOD = 0x20; //设置计数器工作方式2  8位自动重装计数器	
	PCON = 0X80;//波特率加倍	SMOD = 1  28800
	TH1 = 256 -(FOSC/12/32/(BAUD/2)); //计算溢出率
	TL1 = 256 -(FOSC/12/32/(BAUD/2));
	TR1 = 1; //打开定时器	
//	ES=1;//必须关闭，否则会影响定时器
}

/*******************************************************************************
* 函 数 名 ：LEDInit
* 函数功能 ：LED测试功能初始化，注意关闭数码管
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void LEDInit()
{
	wela = 1; //关闭数码管相关的位选
	LED_PORT = 0;
	wela = 0;
	
	LED_SEG=0x00;
	LED_PORT=0x55;
	LED_SEG=0xfe;//给初始化值
}

/*******************************************************************************
* 函 数 名 ：Timer0Init
* 函数功能 ：定时器0初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Timer0Init()
{	
	TMOD= TMOD | 0x01; //设置定时器0工作方式为1
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	ET0=1; //开启定时器0中断
	TR0=1;	//开启定时器	
	EA=1;  //打开总中断
}

/*******************************************************************************
* 函 数 名 ：LEDTest
* 函数功能 ：实现点阵扫描，同时流水灯闪烁
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void LEDTest()
{
	int count = 7;//只检查流水灯
	dula=1;			   
	LED_PORT=0xff;
	dula=0;
	
	wela=1;
	LED_PORT=0x0;
//		while(count--)
//	{
//		Delayms(300);
//		LED_SEG<<=1;//左移一位
//		LED_SEG|=0x01;//最后一位补1
//		RELAY = ~RELAY;
//		
////		if(LED_SEG==0x7f)//检测是否移到最左端？
////		{ 
////			Delayms(500);//delay
////			LED_SEG=0xfe;
////			LED_PORT=~LED_PORT;
////		}
//	}
	LED_SEG = 0x55;
	RELAY = ~RELAY;
	Delayms(500);
	LED_SEG = 0xaa;
	RELAY = ~RELAY;
	Delayms(500);
	LED_SEG = 0x55;
	RELAY = ~RELAY;
	Delayms(500);
	LED_SEG = 0xaa;
	RELAY = ~RELAY;
	Delayms(500);
	LED_SEG = 0xff;

	dula=1;			   //关闭点阵
	LED_PORT=0;
	dula=0;
	Delayms(1000);
	
	TrafficLEDTest();//交通灯检查
	
	diola = 1;
	LED_SEG=0xff;//关闭LED
	diola = 0;
	
	wela=1;//数码管测试,全部点亮
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0;
	dula=0;
	Delayms(500);
	
		wela=1;//数码管测试,全部熄灭
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0xff;
	dula=0;
	Delayms(500);
	
		wela=1;//数码管测试,全部点亮
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0;
	dula=0;
	Delayms(500);
	
		wela=1;//数码管测试,全部熄灭
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0xff;
	dula=0;
	Delayms(500);
	
		wela=1;//数码管测试,全部点亮
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0;
	dula=0;
	Delayms(500);	
}


void TrafficLEDTest(void)
{
	char i=2;
	P1 = 0xff;
	led_d21 = 1;
	led_d22 = 1;
//	Delayms(1000);
//	led_d20 = 0;
//	Delayms(500);//delay
//	led_d20 = 1;
//	led_d19 = 0;
//	Delayms(500);//delay
//	led_d19 = 1;
//	led_d13 = 0;
//	Delayms(500);//delay
//	led_d13 = 1;
//	led_d14 = 0;
//	Delayms(500);//delay
//	led_d14 = 1;
//	led_d15 = 0;
//	Delayms(500);//delay
//	led_d15 = 1;
//	led_d21 = 0;
//	Delayms(500);//delay
//	led_d21 = 1;
//	led_d22 = 0;
//	Delayms(500);//delay
//	led_d22 = 1;
//	led_d16 = 0;
//	Delayms(500);//delay
//	led_d16 = 1;
//	led_d17 = 0;
//	Delayms(500);//delay
//	led_d17 = 1;
//	led_d18 = 0;
//	Delayms(500);//delay

while(i--)
{
	led_d13 = 0;//红灯亮
	led_d16 = 0;
	led_d19 = 0;
	led_d21 = 0;
	Delayms(500);
	P1 = 0xff;  //全灭
	led_d21 = 1;
	led_d22 = 1;
	
	led_d14 = 0;//绿灯亮
	led_d17 = 0;
	led_d20 = 0;
	led_d22 = 0;
	Delayms(500);
	P1 = 0xff;  //全灭
	led_d21 = 1;
	led_d22 = 1;	

	led_d15 = 0;//黄灯亮
	led_d18 = 0;
	Delayms(500);
	P1 = 0xff;  //全灭
	led_d21 = 1;
	led_d22 = 1;
}	
	
}


/*******************************************************************************
* 函 数 名 ：LEDdisplay
* 函数功能 ：循环显示各个位上的数据
* 输    入 ：index 标号   num 要显示的数据
* 输    出 ：无
*******************************************************************************/
void LEDdisplay(unsigned int index,unsigned int num)
{
	unsigned char bai,shi,ge;
	index = index%10;
	bai=num/100%10;
	shi=num%100/10;
	ge=num%10;

	wela=1;//显示标号
	LED_PORT =0x01;//保留最高一位，继电器用的，其他清零
	wela=0;	
	dula=1;			   
	LED_PORT=table[index];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x04;
	wela=0;	
	dula=1;			   //显示百位
	LED_PORT=table[bai];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x08;
	wela=0;
	dula=1;			   //显示十位
	LED_PORT=table[shi];	 
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x10;
	wela=0;
	dula=1;			 // 显示个位
	LED_PORT=table[ge];
	dula=0;
	Delayms(1);
}

/*******************************************************************************
* 函 数 名 ：putchar
* 函数功能 ：代替系统自带的putchar函数，实现printf功能
* 输    入 ：要发送的参数
* 输    出 ：发送成功的参数
*******************************************************************************/
char putchar(char ch)
{ 
	/* Place your implementation of fputc here */ 
	SBUF=(unsigned char)ch; //将接收到的数据放入到发送寄存器
	while(!TI);		  //等待发送数据完成
	TI=0;		 //清除发送完成标志位	
	return ch;
}

