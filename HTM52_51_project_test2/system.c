#include "system.h"

//LED��ʾ��ģ 0-F ����ģʽ
unsigned code table[]= {0Xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

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
void UsartConfiguration()
{
	SCON = 0X50;			//����Ϊ������ʽ1	10λ�첽�շ���
	TMOD = 0x20; //���ü�����������ʽ2  8λ�Զ���װ������	
	PCON = 0X80;//�����ʼӱ�	SMOD = 1  28800
	TH1 = 256 -(FOSC/12/32/(BAUD/2)); //���������
	TL1 = 256 -(FOSC/12/32/(BAUD/2));
	TR1 = 1; //�򿪶�ʱ��	
//	ES=1;//����رգ������Ӱ�춨ʱ��
}

/*******************************************************************************
* �� �� �� ��LEDInit
* �������� ��LED���Թ��ܳ�ʼ����ע��ر������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void LEDInit()
{
	wela = 1; //�ر��������ص�λѡ
	LED_PORT = 0;
	wela = 0;
	
	LED_SEG=0x00;
	LED_PORT=0x55;
	LED_SEG=0xfe;//����ʼ��ֵ
}

/*******************************************************************************
* �� �� �� ��Timer0Init
* �������� ����ʱ��0��ʼ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer0Init()
{	
	TMOD= TMOD | 0x01; //���ö�ʱ��0������ʽΪ1
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	ET0=1; //������ʱ��0�ж�
	TR0=1;	//������ʱ��	
	EA=1;  //�����ж�
}

/*******************************************************************************
* �� �� �� ��LEDTest
* �������� ��ʵ�ֵ���ɨ�裬ͬʱ��ˮ����˸
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void LEDTest()
{
	int count = 7;//ֻ�����ˮ��
	dula=1;			   
	LED_PORT=0xff;
	dula=0;
	
	wela=1;
	LED_PORT=0x0;
//		while(count--)
//	{
//		Delayms(300);
//		LED_SEG<<=1;//����һλ
//		LED_SEG|=0x01;//���һλ��1
//		RELAY = ~RELAY;
//		
////		if(LED_SEG==0x7f)//����Ƿ��Ƶ�����ˣ�
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

	dula=1;			   //�رյ���
	LED_PORT=0;
	dula=0;
	Delayms(1000);
	
	TrafficLEDTest();//��ͨ�Ƽ��
	
	diola = 1;
	LED_SEG=0xff;//�ر�LED
	diola = 0;
	
	wela=1;//����ܲ���,ȫ������
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0;
	dula=0;
	Delayms(500);
	
		wela=1;//����ܲ���,ȫ��Ϩ��
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0xff;
	dula=0;
	Delayms(500);
	
		wela=1;//����ܲ���,ȫ������
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0;
	dula=0;
	Delayms(500);
	
		wela=1;//����ܲ���,ȫ��Ϩ��
	LED_PORT=0xff;
	wela=0;	
	dula=1;			   
	LED_PORT=0xff;
	dula=0;
	Delayms(500);
	
		wela=1;//����ܲ���,ȫ������
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
	led_d13 = 0;//�����
	led_d16 = 0;
	led_d19 = 0;
	led_d21 = 0;
	Delayms(500);
	P1 = 0xff;  //ȫ��
	led_d21 = 1;
	led_d22 = 1;
	
	led_d14 = 0;//�̵���
	led_d17 = 0;
	led_d20 = 0;
	led_d22 = 0;
	Delayms(500);
	P1 = 0xff;  //ȫ��
	led_d21 = 1;
	led_d22 = 1;	

	led_d15 = 0;//�Ƶ���
	led_d18 = 0;
	Delayms(500);
	P1 = 0xff;  //ȫ��
	led_d21 = 1;
	led_d22 = 1;
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
	LED_PORT =0x01;//�������һλ���̵����õģ���������
	wela=0;	
	dula=1;			   
	LED_PORT=table[index];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x04;
	wela=0;	
	dula=1;			   //��ʾ��λ
	LED_PORT=table[bai];
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x08;
	wela=0;
	dula=1;			   //��ʾʮλ
	LED_PORT=table[shi];	 
	dula=0;
	Delayms(1);
	
	wela=1;
	LED_PORT=0x10;
	wela=0;
	dula=1;			 // ��ʾ��λ
	LED_PORT=table[ge];
	dula=0;
	Delayms(1);
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

