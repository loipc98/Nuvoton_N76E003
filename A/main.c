#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include <string.h>

UINT8 rx_data[50];
unsigned char led[]={0x00,0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char a = 0x00;
int i;
int cnt=0;
int check=0;
int rx_len;
int num=0;
int device_stat;
#define led1 P13
#define DATA P03
#define LATCH_CLK P17
#define SH_CLK P15
#define sang 0
#define tat 1

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

//Ham digitalWrite
void digitalWrite(char x, char y){
	if(y==1){
	a |= led[x];
	}
	if(y==0){
	a &=~led[x];
	}
}

// Ham gui ki tu
void Send(char arr[])
{
	for(i=0;i<strlen(arr);i++)
			{
				Send_Data_To_UART0(arr[i]);
			}
}

// Xu ly chuoi
void Xu_ly_chuoi()
{
	for(i=0;i<=rx_len;i++)
	{
		if(rx_data[i] == 'o')
		{
			if(rx_data[i+1] == 'n') 
			{				
				device_stat = 1;
			}
			else if(rx_data[i+1] == 'f') 
			{
				device_stat =0;
			}
		}
		if(rx_data[i] >= '0' && rx_data[i] <= '9')
		{
			num *= 10;
			num += rx_data[i] - '0';
		}
		else
		{
			digitalWrite(num,device_stat);
			num=0;
		}
	}
}

void main(void)
{
	P15_Quasi_Mode;
	P17_Quasi_Mode;
	P03_Quasi_Mode;
	P13_Quasi_Mode;
	P05_Quasi_Mode;
	InitialUART0_Timer1(9600);
	set_EA;								// Cho phep ngat toan cuc
	set_ES;								// Cho phep ngat UART
	led1=sang;
	while(1)
	{

		if(check == 1)
		{		
				Send(rx_data);
				rx_len = cnt;
				Xu_ly_chuoi();
				cnt = 0;
				check = 0;
		}
		IE74595_Out_Byte(a);	
	}
}

// Ngat UART ngoai
void SerialPort0_ISR(void) interrupt 4
{
	if(RI == 1)
	{
		  clr_RI;
			rx_data[cnt] = SBUF;
			if(rx_data[cnt] == '&') cnt = 0;
		  else if(rx_data[cnt] == '!') check = 1;
			else cnt++;
		if(cnt>50) cnt=0;
	}
	if(TI == 1)
	{
		clr_TI;
	}
}