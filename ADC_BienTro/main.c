#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
 char chu_so[]={0xEB,0x88,0xB3,0xBA,0xD8,0x7A,0x7B,0xA8,0xFB,0xFA};
#define DATA P03			//Du lieu 
#define CLK P15				//Xung 
#define LAT P17				//Chot du lieu de day ra 
#define LED1 P13
#define LED2 P12
#define LED3 P11
#define LED4 P10
#define bat  0
#define tat  1
int cnt = 0;unsigned int adc_value;
void HC595_Output(unsigned char a)
{
	unsigned char i;
	for (i=0; i<8;i++)
	{
		DATA = a & (0x80 >> i);		//Dich bit de day du lieu ra
		CLK=0;
		CLK=1;
	}
	LAT=0;
	LAT=1;
}
void setup(void)
{
	P05_Quasi_Mode;					
	P10_Quasi_Mode;				//LED4 Output
	P11_Quasi_Mode;				//LED3 Output
	P12_Quasi_Mode;				//LED2 Output
	P13_Quasi_Mode;				//LED1 Output
	P15_Quasi_Mode;				//CLK  Output
	P17_Quasi_Mode;				//LAT  Output
	P03_Quasi_Mode;				//DATA Output
	// Khoi tao ngat Timer0
	TMOD=0x01;						//Khoi tao Timer Mode 1
	ET0=1;								//Cho phep ngat Timer
	EA=1;									//Cho phep ngat toan cuc
	TR0=1;								//Khoi tao Timer0
	// Khoi tao ADC
	Enable_ADC_AIN5; 			//Khoi tao ADC pin 04
}

void Timer0_Interrupt() interrupt 1
{
	TH0=0xFA;				//Nap lai gia tri 
	TL0=0xCB;
	cnt++;
	if(cnt==5) cnt=1;
	if(cnt==1)
	{
		LED4=tat;
		LED1=tat;
		HC595_Output(chu_so[adc_value/1000]);
		LED4=bat;
	}
 if(cnt==2)
	{
		LED3=tat;
		LED4=tat;
		HC595_Output(chu_so[(adc_value%1000)/100]);
		LED3=bat;
	}
 if(cnt==3)
	{
		LED2=tat;
		LED3=tat;
		HC595_Output(chu_so[(adc_value%100)/10]);
		LED2=bat;
	}
 if(cnt==4)
	{
		LED1=tat;
		LED2=tat;
		HC595_Output(chu_so[adc_value%10]);
		LED1=bat;
	}
}
void main(void)
{
	setup();
	while(1)
	{
		P05=1;										
		clr_ADCF;									// Xoa co, neu ADCF=1 thi ko the chuyen doi 
		set_ADCS;									// ADCS=1 trong suot qua trinh chuyen doi, 
    while(ADCF == 0);					// ADCF = 0, chuyen doi hoan tat
		adc_value = ADCRH;
    adc_value <<= 4;
    adc_value |= ADCRL;					// Doc gia tri ADC
		Timer0_Delay1ms(100);
	}
}