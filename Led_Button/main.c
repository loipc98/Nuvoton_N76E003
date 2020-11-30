#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

char so[]={0xEB, 0x88, 0xB3, 0xBA, 0xD8, 0x7A, 0x7B, 0xA8, 0xFB, 0xFA};

#define DATA P03			//Du lieu 
#define CLK P15				//Xung 
#define LAT P17				//Chot du lieu de day ra 
#define LED1 P13
#define LED2 P12
#define LED3 P11
#define LED4 P10
#define bat  0;
#define tat  1; 
void HC595_Output(unsigned char a)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		DATA=a & (0x80 >> i);
		CLK=0;
		CLK=1;
	}
	LAT=0;
	LAT=1;
}
void setup(void)
{
	P15_Quasi_Mode;				//CLK  Output
	P17_Quasi_Mode;				//LAT  Output
	P03_Quasi_Mode;				//DATA Output
	P10_Quasi_Mode;				//LED4 Output
	P11_Quasi_Mode;				//LED3 Output
	P12_Quasi_Mode;				//LED2 Output
	P13_Quasi_Mode;				//LED1 Output

}
void main(void)
{
	setup();
	while(1)
	{
		LED1=bat;
		HC595_Output(so[0]);
		Timer0_Delay1ms(5);
		LED1=tat;
		
		LED2=bat;
		HC595_Output(so[1]);
		Timer0_Delay1ms(5);
		LED2=tat;
		
		LED3=bat;
		HC595_Output(so[2]);
		Timer0_Delay1ms(5);
		LED3=tat;
		
		LED4=bat;
		HC595_Output(so[4]);
		Timer0_Delay1ms(5);
		LED4=tat;
	}
}