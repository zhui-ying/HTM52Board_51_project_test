#include "DS18B20.h"

unsigned char data  RomCode[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*******************************************************************/
/*                                                                 */
/*us����ʱ����                                                     */
/*                                                                 */
/*******************************************************************/

void Delay(unsigned int num)
{
  while( --num );
}

/*******************************************************************/
/*                                                                 */
/*��ʼ��ds1820                                                     */
/*                                                                 */
/*******************************************************************/
bit Init_DS18B20(void)
{ 
		bit presence;
     DQ = 1;      //DQ��λ
     Delay(8);    //������ʱ

     DQ = 0;      //��Ƭ����DQ����
     Delay(90);   //��ȷ��ʱ ���� 480us

     DQ = 1;       //��������
     Delay(8);

     presence = DQ;    //���=0���ʼ���ɹ� =1���ʼ��ʧ��
     Delay(100);
     DQ = 1; 
     
     return(presence); //�����źţ�0=presence,1= no presence
}

/*******************************************************************/
/*                                                                 */
/* ��һ���ֽ�                                                      */
/*                                                                 */
/*******************************************************************/
unsigned char ReadOneChar(void)
{
   unsigned char i = 0;
   unsigned char dat = 0;

   for (i = 8; i > 0; i--)
   {
     DQ = 0; // �������ź�
     dat >>= 1;
     DQ = 1; // �������ź�

     if(DQ)
     dat |= 0x80;
     Delay(4);
   }
    return (dat);
}

/*******************************************************************/
/*                                                                 */
/* дһ���ֽ�                                                      */
/*                                                                 */
/*******************************************************************/
void  WriteOneChar(unsigned char dat)
{
  unsigned char i = 0;
  for (i = 8; i > 0; i--)
  {
    DQ = 0;
    DQ = dat&0x01;
    Delay(5);

    DQ = 1;
    dat>>=1;
  }
}

/*******************************************************************/
/*                                                                 */
/* �¶ȱ���ֵд��DS18B20                                           */
/*                                                                 */
/*******************************************************************/
void Write_Temperature_alarm(unsigned char Temp_h , unsigned char Temp_l)
{
    Init_DS18B20();
  
    WriteOneChar(0xCC);           //����������кŵĲ��� 
    WriteOneChar(0x4e);           //���趨���¶ȱ���ֵд�� DS18B20
	WriteOneChar(Temp_h);  //дTH
	WriteOneChar(Temp_l);  //дTL
    WriteOneChar(0x7f);           //12λ��ȷ��
 
	Init_DS18B20();
    WriteOneChar(0xCC);           //����������кŵĲ��� 
    WriteOneChar(0x48);           //���ݴ�������¶ȱ���ֵ������EEROM
}

/*******************************************************************/
/*                                                                 */
/* ��ȡ64λ������                                                  */
/*                                                                 */
/*******************************************************************/
void Read_RomCord(void)
{
   unsigned char j;
   Init_DS18B20();
  
   WriteOneChar(0x33);          // ��������Ĳ���
   for (j = 0; j < 8; j++)
   {
     RomCode[j] = ReadOneChar() ; 
   }
}

/*******************************************************************/
/*                                                                 */
/*DS18B20��CRC8У�����                                            */
/*                                                                 */
/*******************************************************************/
unsigned char CRC8() 
{ 
   unsigned char i,x; 
	unsigned char crcbuff,crc;
   
   crc=0;
   for(x = 0; x <8; x++)
   {
    crcbuff=RomCode[x];
    for(i = 0; i < 8; i++) 
     { 
      if(((crc ^ crcbuff)&0x01)==0) 
      crc >>= 1; 
       else { 
              crc ^= 0x18;   //CRC=X8+X5+X4+1
              crc >>= 1; 
              crc |= 0x80; 
            }         
      crcbuff >>= 1;       
	 }
   }
     return crc;	
}

/*******************************************************************/
/*                                                                 */
/* ��ȡ�¶�                                                        */
/*                                                                 */
/*******************************************************************/
unsigned char Read_Temperature(void)
{
	unsigned char  i; 
	unsigned char temp_comp;	
	unsigned char temp_data[2];
	unsigned char temp_alarm[2];
	Init_DS18B20();

	WriteOneChar(0xCC);        //����������кŵĲ���
	WriteOneChar(0x44);        //�����¶�ת��

	Init_DS18B20();

	WriteOneChar(0x55);         //ƥ��ROM����
	for(i=0;i<8;i++)
	WriteOneChar(RomCode[i]);

	WriteOneChar(0xBE);         //��ȡ�¶ȼĴ���

	temp_data[0] = ReadOneChar();   //�¶ȵ�8λ
	temp_data[1] = ReadOneChar();   //�¶ȸ�8λ
	temp_alarm[0] = ReadOneChar();  //�¶ȱ���TH
	temp_alarm[1] = ReadOneChar();  //�¶ȱ���TL

	temp_comp=((temp_data[0]&0xf0)>>4)|((temp_data[1]&0x0f)<<4);//ȡ�¶�����ֵ
	return temp_comp;  
														 
}

