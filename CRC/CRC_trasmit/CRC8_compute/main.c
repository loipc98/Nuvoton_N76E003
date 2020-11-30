#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "Function_Define.h"
#include "SFR_Macro.h"

#define POLY  0x07 //ATM-8 CRC (8bit)
	
uint8_t xdata crcTable[256]; //mang luu du lieu tra cuu CRC
unsigned char data1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
unsigned char c1;
 /*------------------- Ham tinh bang du lieu CRC-----------------------*/
void CalulateTable_CRC8() 
{
	int divident;
  for (divident = 0; divident < 256; divident++)
  {
    uint8_t currByte = divident; 
		uint8_t bitSec;
    //Tinh toan gia tri CRC-8 cho byte hien thoi
    for (bitSec = 0; bitSec < 8; bitSec++)
    {
      if ((currByte & 0x80) != 0)
      {
        currByte <<= 1;
        currByte ^= POLY;
      }
      else
      {
        currByte <<= 1;
      }
    }
    //luu gia tri CRC vao bang tra cuu
    crcTable[divident] = currByte;
  }
}

/*-----------------Ham tinh gia tri CRC-8 cua chuoi du lieu------------*/
uint8_t computeCRC(unsigned char *addr, int nBytes)
{
  uint8_t crc = 0x00;  //khoi tao gia tri crc ban dau
  for ( ; nBytes > 0; nBytes--)
  {
    //XOR in next input byte
    uint8_t Data = crc^(*addr++);
    crc = crcTable[Data];
  }
  return crc;
}


void main()
{
	InitialUART0_Timer1(9600);
	CalulateTable_CRC8();
	while(1)
	{
		c1 = computeCRC(data1, 9);
		Send_Data_To_UART0(c1);
		Timer0_Delay1ms(1000);
	}
	
}