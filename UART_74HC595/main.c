#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "Function_define.h"
#include "SFR_Macro.h"

unsigned char led[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char a= 0x00,b;
#define led1 P13
#define DATA P03
#define LATCH_CLK P17
#define SH_CLK P15
#define sang 0
#define tat 1
unsigned int i=0;
//Ham dieu khien 74HC595
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
//Ham bat/tat led
void digitalWrite(char x, char y){
	if(y==1){
		a= a|led[x-1];
	}
	if(y==0){
		
		b= a^led[x-1];
		if(a>b) a=b;
	}
}
//Ham ngat Timer Sang led
void ngatTimer() interrupt 1{
	TH0= 0xFA;
	TL0= 0xCB;
	TR0= 1;

		 led1= tat;
	  IE74595_Out_Byte(a);
		led1 = sang;
	 }

void main()
{
	P15_Quasi_Mode;
	P17_Quasi_Mode;
	P03_Quasi_Mode;
	P13_Quasi_Mode;
	P05_Quasi_Mode;
	TMOD = 0x01; //timer 0, che do 1
	ET0=1;
	TR0=1;
	EA=1;
	while(1)
	{
		P05=~P05;
    digitalWrite(1,0);
		digitalWrite(2,0);
		digitalWrite(3,0);
		digitalWrite(4,0);
		digitalWrite(5,1);
		digitalWrite(6,1);
		digitalWrite(7,1);
		digitalWrite(8,1);
  	Timer0_Delay1ms(1000);
	}
}