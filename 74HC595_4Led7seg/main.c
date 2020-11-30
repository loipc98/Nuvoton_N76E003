#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"
#include "Common.h"
#include "Delay.h"
			
char so[]= {0xEB, 0x88, 0xB3, 0xBA, 0xD8, 0x7A, 0x7B, 0xA8, 0xFB, 0xFA};
#define led1 P13
#define led2 P12
#define led3 P11
#define led4 P10
#define sang 0
#define tat 1
#define DATA P03
#define LATCH_CLK P17
#define SH_CLK P15

void IE74595_Out_Byte(unsigned char b)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		DATA = b& (0x80>>i); // lay tung bit cua byte b gui 
		SH_CLK=0;
    SH_CLK=1;
	}
		LATCH_CLK= 0;
	  LATCH_CLK= 1;
}
void main()
{
	P15_Quasi_Mode;
	P17_Quasi_Mode;
	P03_Quasi_Mode;
	P10_Quasi_Mode;
	P11_Quasi_Mode;
	P12_Quasi_Mode;
	P13_Quasi_Mode;
	
	while(1)
	{
		led1 = sang;
	  IE74595_Out_Byte(0xEB);
		Timer0_Delay1ms(5);
		led1 = tat;
		
		led2 = sang;
	  IE74595_Out_Byte(so[2]);
		Timer0_Delay1ms(5);
		led2 = tat;
		
		led3 = sang;
	  IE74595_Out_Byte(so[3]);
		Timer0_Delay1ms(5);
		led3 = tat;
		
		led4 = sang;
	  IE74595_Out_Byte(so[4]);
		Timer0_Delay1ms(5);
		led4 = tat;
		
		
	}
}