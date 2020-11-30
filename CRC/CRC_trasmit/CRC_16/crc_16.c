#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "Function_Define.h"
#include "SFR_Macro.h"

#define POLY        0x1021  // CRC-16 
unsigned char data1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
unsigned char data2[] = {'5', '6', '7', '8', '9'};
unsigned short c1, c2;
/*--------------------------------Ham tinh crc-16--------------------------------------*/
/*Dau vao: add- mang du lieu can tinh crc, num- do dai mang, crc- du lieu khoi tao crc */
uint16_t crc16(unsigned char *addr, int num, uint16_t crc)  
{
    int i;
    for (; num > 0; num--)              //chay lan luot cac byte du lieu
    {
        crc = crc ^ (*addr++ << 8);     //dich 8 Bit cac byte du lieu roi XOR voi crc
        for (i = 0; i < 8; i++)         /// chuan bi de xoay 8 Bit
        {
            if (crc & 0x8000)           
                crc = (crc << 1) ^ POLY;    //Dich Bit va XOR voi poly
            else                          
                crc <<= 1;                  //Chi dich Bit
        }                             
        crc &= 0xFFFF;                  
    }                               
    return(crc);                    
}

void main()
{
	InitialUART0_Timer1(9600);
	while(1)
	{
		c1 = crc16(data1, 9, 0xffff);
		c2 = crc16(data1, 4, 0xffff);
		c2 = crc16(data2, 5, c2);
		printf("%04x\n", c1);
		printf("%04x\n", c2);
		Timer0_Delay1ms(1000);
	}
	
}