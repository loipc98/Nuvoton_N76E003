#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
char str[]="Deviot";
char rx_data;
int i;
void UART_ISR(void) interrupt 4
{
	if(RI==1)
	{
		rx_data=Receive_Data_From_UART0();
		Send_Data_To_UART0(rx_data);
		if(rx_data == '1') P05 = 0xff;
		if(rx_data == '0') P05 = 0x00;
		clr_RI;
		//clr_TI;
	}
	if(TI==1)
	{
		TI=0;
	}
}
void main(void)
{
	InitialUART0_Timer1(9600);
	set_TI;
	set_EA;
	set_ES;
	P05_Quasi_Mode;
	while(1)
	{
		for(i=0;str[i]!=0;i++)
		{
			Send_Data_To_UART0(str[i]);
		}
		Send_Data_To_UART0('\n');
		Timer0_Delay1ms(300);
	}
}