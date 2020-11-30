#include "N76E003.h"
#include "Common.h"
#include "Function_define.h"
#include "SFR_Macro.h"
#include "Delay.h"

unsigned char temp _at_ 0x08;
unsigned char idata itemp _at_ 0x80;
unsigned char xdata xtemp _at_ 0x80;

int cnt = 0;
unsigned long adc_value;
void PWM_set_period(unsigned int period_value)
{
	PWMPL = period_value & 0x00FF;
	PWMPH = (period_value & 0xFF00)>>8;
}

void PWM_set_on(unsigned int on_value)
{
	PWM2L = (on_value & 0x00FF);
  PWM2H = ((on_value & 0xFF00) >> 8);
  set_LOAD;
}
void main(){
	P30_Input_Mode;
	P05_Quasi_Mode;					
	PWM2_P05_OUTPUT_ENABLE;
	PWM_IMDEPENDENT_MODE;
	PWM_CLOCK_DIV_8;
	PWM_set_period(2000);
	
	// Khoi tao ADC
	Enable_ADC_AIN5; 			//Khoi tao ADC pin 04
	PWM2H = 0x00;
  PWM2L = 0x00;
  set_LOAD;
  set_PWMRUN;
	InitialUART0_Timer3(9600);
	TI = 1;			//Khoi dong ham printf
	while(1){
		P05=1;										
		clr_ADCF;									// Xoa co, neu ADCF=1 thi ko the chuyen doi 
		set_ADCS;									// ADCS=1 trong suot qua trinh chuyen doi, 
    while(ADCF == 0);					// ADCF = 0, chuyen doi hoan tat
    adc_value = ADCRH;
    adc_value <<= 4;
    adc_value |= ADCRL;		// Doc gia tri ADC
		PWM_set_on(adc_value*2000/4096);
//		printf("\n Gia tri bien tro la: %d",adc_value);
		Send_Data_To_UART0(adc_value);
		Timer0_Delay1ms(100);
	}
}