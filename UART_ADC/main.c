#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include <string.h>
unsigned int adc_value;
char buffer[5];

// Ham gui ki tu
void Send(char arr[])
{
	int i;
	for(i=0;i<strlen(arr);i++)
			{
				Send_Data_To_UART0(arr[i]);
			}
}

void main(void)
{
		// Khoi tao ADC
	Enable_ADC_AIN5; 			//Khoi tao ADC pin 04
  Set_All_GPIO_Quasi_Mode;
	InitialUART0_Timer1(9600);
	
	P05_Quasi_Mode;
	TI=1;
	while(1)
	{

		clr_ADCF;									// Xoa co, neu ADCF=1 thi ko the chuyen doi 
		set_ADCS;									// ADCS=1 trong suot qua trinh chuyen doi, 
    while(ADCF == 0);					// ADCF = 0, chuyen doi hoan tat
		adc_value = ADCRH;
    adc_value <<= 4;
    adc_value |= ADCRL;					// Doc gia tri ADC
		Send_Data_To_UART0(adc_value/1000+'0');
		Send_Data_To_UART0((adc_value%1000)/100+'0');
		Send_Data_To_UART0((adc_value%100)/10+'0');
		Send_Data_To_UART0(adc_value%10+'0');
		Send("\n");
		Timer0_Delay1ms(500);
	}
}

	
