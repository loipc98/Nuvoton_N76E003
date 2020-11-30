#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "Function_Define.h"
#include "SFR_Macro.h"

#define POLY  0x07 //CRC-8 (8bit)

unsigned char data1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
unsigned char c1;

/*---------------------Ham tinh crc8 cho tep du lieu-------------------------------*/
uint8_t crc8(unsigned char *addr, int num)
{
	  uint8_t crc = 0x00;  //khoi tao gia tri crc ban dau
    int i;
    for (; num > 0; num--)              //chay lan luot cac byte du lieu
    {
        crc = crc ^ (*addr++);          // Xor crc voi cac byte du lieu
        for (i = 0; i < 8; i++)             /* chuan bi de xoay 8 Bit */
        {
            if (crc & 0x80)            
                crc = (crc << 1) ^ POLY;    /* dich va xor voi poly */
            else                          
                crc <<= 1;                  
        }                             
        crc &= 0xFF;                  
    }                               
    return(crc);                    
}

void main()
{
	InitialUART0_Timer1(9600);
	while(1)
	{
		c1 = crc8(data1, 9);
		Send_Data_To_UART0(c1);
		Timer0_Delay1ms(1000);
	}
	
}