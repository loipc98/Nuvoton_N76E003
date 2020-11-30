#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

void setup(void)
{
	P14_Input_Mode;					// Button
	P05_Quasi_Mode;					// Led 1
	P06_Quasi_Mode;					// Led 2
	PICON |= 0x01;					// Pin interrupt port 1
	Enable_BIT4_LowLevel_Trig;
	
	set_EPI;								// Cho phep ngat pin
	set_EA;									// Cho phep ngat toan cuc
}

void main()
{
	setup();
	while(1)
	{
		P05 =! P05;
		Timer0_Delay1ms(500);
	}
}

void PinInterrupt_ISR (void) interrupt 7
{
	if(PIF == 0x10)					// Interrupt pin 4
	{
		P06 = 0x00 ;					
	}
}