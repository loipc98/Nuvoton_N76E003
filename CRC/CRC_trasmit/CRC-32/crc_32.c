#include "Common.h"
#include "Delay.h"
#include "Function_Define.h"
#include "SFR_Macro.h"
#define POLY   	0x04C11DB7  // CRC-32
uint32_t xdata crcTable[256]; 
unsigned char data1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
uint32_t c1;

uint32_t crc32Compute(char * message, unsigned int nBytes)
{
    unsigned int offset;
    unsigned char byte;
    uint32_t remainder = 0xFFFFFFFF;
    /* Divide the message by the polynomial, a byte at a time. */
    for( offset = 0; offset < nBytes; offset++)
    {
        byte = (remainder >> (32 - 8)) ^ message[offset];
        remainder = crcTable[byte] ^ (remainder << 8);
    }
    /* The final remainder is the CRC result. */
    return (remainder ^ 0xFFFFFFFF);
}

void crcInit(void)
{
    uint32_t remainder;
    uint32_t dividend;
    int Bit;
    /* Perform binary long division, a bit at a time. */
    for(dividend = 0; dividend < 256; dividend++)
    {
        /* Initialize the remainder.  */
        remainder = dividend << (32 - 8);
        /* Shift and XOR with the polynomial.   */
        for(Bit = 0; Bit < 8; Bit++)
        {
            /* Try to divide the current data bit.  */
            if(remainder & 0x80000000)
            {
                remainder = (remainder << 1) ^ POLY;
            }
            else
            {
                remainder = remainder << 1;
            }
        }
        /* Save the result in the table. */
        crcTable[dividend] = remainder;
    }
}
void main()
{
	InitialUART0_Timer1(9600);
	crcInit();
	while(1)
	{
		c1 = crc32Compute(data1, 9);	
		printf("%04x%04x\n", c1,c1<<16);
		Timer0_Delay1ms(1000);
	}	
}
