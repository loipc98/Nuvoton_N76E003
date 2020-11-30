#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "string.h"
#include "stdlib.h"
char rx_data;
void SerialPort0_ISR(void) interrupt 4 
{
    if (RI==1) 
    {                                       /* if reception occur */
				rx_data= Receive_Data_From_UART0();
			  if(rx_data == '1') P05 = 0xff;
		    if(rx_data == '0') P05 = 0x00;
		    Send_Data_To_UART0(rx_data);
			  clr_RI;                             /* clear reception flag for next reception */
    }
    if(TI==1)
    {
        clr_TI;                             /* if emission occur */
    }
}

void sendChuoi(char *a){
	int i,n;
	n= strlen(a);
	for(i=0;i<n;i++){
		Send_Data_To_UART0(a[i]);
	}
}
int c=123;
char b[5];
void main(){
	InitialUART0_Timer1(9600);
	P05_Quasi_Mode;
	clr_TI; 
  set_ES;           //enable UART interrupt
  set_EA;           //enable global interrupt
//	itoa(c,b,10);
	while(1){
		sendChuoi(b);
		Timer0_Delay1ms(200);
	}
}