#include "PCF8591.h"

#define FOSC 11059200L //晶振设置，默认使用11.0592M Hz
//#define FOSC 12000000L //晶振设置，使用12M Hz
//#define FOSC 24000000L //晶振设置，使用24M Hz
#define  PCF8591 0x90    //PCF8591 地址

//全局变量定义

/*******************************************************************************
* 函 数 名 ：PcfStart
* 函数功能 ：I2C开始信号
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void PcfStart()
{
	PCF_SDA=1;
	_nop_();
	PCF_SCL=1;
	_nop_();
	PCF_SDA=0;
	_nop_();
}

/*******************************************************************************
* 函 数 名 ：PcfStop
* 函数功能 ：I2C停止信号
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void PcfStop()      
{
	PCF_SDA=0;
	_nop_();
	PCF_SCL=1;
	_nop_();
	PCF_SDA=1;
	_nop_();  
}

/*******************************************************************************
* 函 数 名 ：PcfRespons
* 函数功能 ：I2C应答信号
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void PcfRespons()
{
	unsigned char i;
	PCF_SCL=1;
	_nop_();
	while((PCF_SDA==1)&&(i<250))
	i++;
	PCF_SCL=0;
	_nop_();
}

/*******************************************************************************
* 函 数 名 ：PcfInit
* 函数功能 ：I2C初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void PcfInit()     
{
	PCF_SDA=1;
	_nop_();
	PCF_SCL=1;
	_nop_();    
}

/*******************************************************************************
* 函 数 名 ：PcfReadByte
* 函数功能 ：读取一个字节
* 输    入 ：无
* 输    出 ：k 读取到的字节
*******************************************************************************/
unsigned char PcfReadByte()
{
	unsigned char i,k;
	PCF_SCL=0;
	_nop_();
	PCF_SDA=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		PCF_SCL=1;
		_nop_();
		k=(k<<1)|PCF_SDA;//先左移一位，再在最低位接受当前位
		PCF_SCL=0;
		_nop_();
	}
	return k;

}

/*******************************************************************************
* 函 数 名 ：PcfWriteByte
* 函数功能 ：写入一个字节
* 输    入 ：data 要写如的字节
* 输    出 ：无
*******************************************************************************/
void PcfWriteByte(unsigned char date)       //写一字节数据
{
	unsigned char i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1; //左移一位 移出的一位在CY中
		PCF_SCL=0;                      //只有在scl=0时sda能变化值
		_nop_();
		PCF_SDA=CY;
		_nop_();
		PCF_SCL=1;
		_nop_();            
	}   
	PCF_SCL=0;
	_nop_();
	PCF_SDA=1;
	_nop_();
}


/*******************************************************************************
* 函 数 名 ：PcfWriteAdd
* 函数功能 ：在某个地址写入数据
* 输    入 ：control 控制字，地址  date 要写入的数据
* 输    出 ：无
*******************************************************************************/
void PcfWriteAdd(unsigned char control,unsigned char date)
{
	PcfStart();
	PcfWriteByte(PCF8591);        //10010000  前四位固定 接下来三位全部被接地了 所以都是0 最后一位是写 所以为低电平
	PcfRespons();
	PcfWriteByte(control);
	PcfRespons();
	PcfWriteByte(date);
	PcfRespons();
	PcfStop();

}

/*******************************************************************************
* 函 数 名 ：PcfReadAdd
* 函数功能 ：在某个地址读取数据
* 输    入 ：control 控制字，地址 
* 输    出 ：date 读到的数据
*******************************************************************************/
unsigned char PcfReadAdd(unsigned char control)
{
	unsigned char date;
	PcfStart();
	PcfWriteByte(PCF8591);
	PcfRespons();
	PcfWriteByte(control);
	PcfRespons();
	PcfStart();
	PcfWriteByte(PCF8591+1);       //把最后一位变成1，读
	PcfRespons();
	date=PcfReadByte();
	PcfStop();
	return date;
}


