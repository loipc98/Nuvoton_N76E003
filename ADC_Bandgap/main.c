/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E885 ADC demo code
//***********************************************************************************************************

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
#if 0
//#define Enable_ADC_BandGap	ADCCON0|=SET_BIT3;ADCCON0&=0xF8;																															//Band-gap 1.22V
#endif

double  Bandgap_Voltage,VDD_Voltage;			//please always use "double" mode for this
unsigned  char xdata ADCdataH[5], ADCdataL[5];
int ADCsumH=0, ADCsumL=0;
unsigned char ADCavgH,ADCavgL;

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
int cnt = 0;
unsigned long VDD;
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
		HC595_Output(chu_so[VDD/1000]);
		LED4=bat;
	}
 if(cnt==2)
	{
		LED3=tat;
		LED4=tat;
		HC595_Output(chu_so[(VDD%1000)/100]);
		LED3=bat;
	}
 if(cnt==3)
	{
		LED2=tat;
		LED3=tat;
		HC595_Output(chu_so[(VDD%100)/10]);
		LED2=bat;
	}
 if(cnt==4)
	{
		LED1=tat;
		LED2=tat;
		HC595_Output(chu_so[VDD%10]);
		LED1=bat;
	}
}

void READ_BANDGAP()
{
		UINT8 BandgapHigh,BandgapLow,BandgapMark;
		double Bandgap_Value,Bandgap_Voltage_Temp;
	
		set_IAPEN;
		IAPCN = READ_UID;
		IAPAL = 0x0d;
    IAPAH = 0x00;
    set_IAPGO;
		BandgapLow = IAPFD;
		BandgapMark = BandgapLow&0xF0;
			
		if (BandgapMark==0x80)
		{
				BandgapLow = BandgapLow&0x0F;
				IAPAL = 0x0C;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage_Temp = Bandgap_Value*3/4;
				Bandgap_Voltage = Bandgap_Voltage_Temp - 33;			//the actually banggap voltage value is similar this value.
		}
		if (BandgapMark==0x00)
		{
				BandgapLow = BandgapLow&0x0F;
				IAPAL = 0x0C;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage= Bandgap_Value*3/4;
		}
		if (BandgapMark==0x90)
		{
				IAPAL = 0x0E;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				IAPAL = 0x0F;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapLow = IAPFD;
				BandgapLow = BandgapLow&0x0F;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage= Bandgap_Value*3/4;
		}
		clr_IAPEN;
//			printf ("\n BG High = %bX",BandgapHigh); 
//			printf ("\n BG Low = %bX",BandgapLow); 
//			printf ("\n BG ROMMAP = %e",Bandgap_Voltage); 
}
		
		
/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/
void main (void) 
{
	double bgvalue;
	unsigned int i;
	P30_Input_Mode;					
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
		InitialUART0_Timer1(115200);
		READ_BANDGAP();
		printf ("\n BG ROMMAP = %e",Bandgap_Voltage); 
	
		while (1)
		{
				Enable_ADC_BandGap;												
				CKDIV = 0x02;															// IMPORTANT!! Modify system clock to 4MHz ,then add the ADC sampling clock base to add the sampling timing.
				for(i=0;i<5;i++)													// All following ADC detect timing is 200uS run under 4MHz.
				{
						clr_ADCF;
						set_ADCS;																
						while(ADCF == 0);
						ADCdataH[i] = ADCRH;
						ADCdataL[i] = ADCRL;
				}		
				CKDIV = 0x00;															// After ADC sampling, modify system clock back to 16MHz to run next code.
				Disable_ADC;
				for(i=2;i<5;i++)													// use the last 3 times data to make average 
				{
					ADCsumH = ADCsumH + ADCdataH[i];
					ADCsumL = ADCsumL + ADCdataL[i];
				}				
				ADCavgH = ADCsumH/3;
				ADCavgL = ADCsumL/3;
				bgvalue = (ADCavgH<<4) + ADCavgL;
				VDD_Voltage = (0x1000/bgvalue)*Bandgap_Voltage;
				VDD = VDD_Voltage;
				Timer0_Delay1ms(500);
				ADCsumH = 0;
				ADCsumL = 0;
		}
}


