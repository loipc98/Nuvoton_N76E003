#include "N76E003.h"
#include "Common.h"
#include "Function_define.h"
#include "Delay.h"
#include "SFR_Macro.h"

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

unsigned int i=0;
unsigned int a=0;
unsigned char j=0;

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
void ngatTimer() interrupt 1{
	TH0= 0xFA;
	TL0= 0xCB;
	TR0= 1;
	j++;
	 if(j==5) j=1;
	 if(j==1){
		 led4= tat;
		 led1=tat;
	  IE74595_Out_Byte(so[a/1000]);
		led4 = sang;
	 }
	 if(j==2){
		 led3=tat;
		 led4=tat;
		IE74595_Out_Byte(so[(a%1000)/100]);
	  led3=sang;
	 }
	 if(j==3){
		 led2= tat;
		 led3=tat;
	  IE74595_Out_Byte(so[(a%100)/10]);
		led2 = sang;
	 }
	 if(j==4){
		 led1= tat;
		 led2= tat;
	  IE74595_Out_Byte(so[a%10]);
		led1= sang;
	 }
	}
void main(){
	P15_Quasi_Mode;
	P17_Quasi_Mode;
	P03_Quasi_Mode;
	P10_Quasi_Mode;
	P11_Quasi_Mode;
	P12_Quasi_Mode;
	P13_Quasi_Mode;
	P05_Quasi_Mode;
	TMOD = 0x01; //timer 0, che do 1
	ET0=1;
	TR0=1;
	EA=1;
	a=0;
	led1=led2=led3=led4=tat;
	while(1){
		P05=~P05;
		a++;
		Timer0_Delay1ms(1000);
		if(a>9999) a=0;
	}
}