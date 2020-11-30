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
#define on 0
#define off 1
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

void hien_Thi(unsigned char gio, unsigned char phut)
{
	unsigned char chuc_gio, chuc_phut, donvi_gio, donvi_phut,i;
	chuc_gio= gio/10; donvi_gio= gio%10;
	chuc_phut= phut/10; donvi_phut= phut%10;
	for(i=0;i<50;i++)
	{
		led1 = on, IE74595_Out_Byte(so[donvi_phut]); Timer0_Delay1ms(5); led1=off;
		led2 = on, IE74595_Out_Byte(so[chuc_phut]); Timer0_Delay1ms(5); led2=off;
		led3 = on, IE74595_Out_Byte(so[donvi_gio]); Timer0_Delay1ms(5); led3=off;
		led4 = on, IE74595_Out_Byte(so[chuc_gio]); Timer0_Delay1ms(5); led4=off;

	}
}
	unsigned char gio= 3, phut = 59 , giay= 0;
void main()
{
	P15_Quasi_Mode;
	P17_Quasi_Mode;
	P03_Quasi_Mode;
	P10_Quasi_Mode;
	P11_Quasi_Mode;
	P12_Quasi_Mode;
	P13_Quasi_Mode;
	P05_Quasi_Mode;	
	while(1)
	{
		giay++;
		P05=~P05;
		hien_Thi(gio, phut);
		if(giay >59) phut++;
		if(phut > 59) gio++;
		if(giay >59)giay=0;
		if(phut >59) phut=0;
		if(gio >23) gio=0;			
	}
}