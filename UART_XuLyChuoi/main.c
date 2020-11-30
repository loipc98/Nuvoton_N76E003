#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include <string.h>

char rx_data[30];
unsigned char led[]={0x00,0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char a= 0x00;
char *Data;
int cnt=0,len;
int check=0;
int i;

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
//Ham gui ky tu
void Send(char arr[])
{
	for(i=0;i<strlen(arr);i++)
			{
				Send_Data_To_UART0(arr[i]);
			}
}

//Ham digitalWrite
void digitalWrite(char x, char y){
	if(y==1){
		a= a|led[x];
	}
	if(y==0){
		a&=~ led[x];
	}
}

//Ngat UART
void UART_ISR(void) interrupt 4
{
	if(RI == 1)
	{
			rx_data[cnt] = Receive_Data_From_UART0();	
			cnt++;
			check = 1;
		  clr_RI;
			
	}
	if(TI==1)
	{
		TI = 0;

	}
}

//Ham tach chuoi
void tachChuoi(){
	unsigned int num,trangthai=1;
	for(i=0;i<len;i++){
		if(Data[i]>='0'&&Data[i]<='9'){
			num*=10;
			num+= Data[i]-'0';
		}
		if(Data[i]==';') trangthai=0;
		else{
			if(num!=0) digitalWrite(num,trangthai);
			num=0;
		}
	}
}

void main(){
	P15_Quasi_Mode;
	P17_Quasi_Mode;
	P03_Quasi_Mode;
	P13_Quasi_Mode;
	P05_Quasi_Mode;
	InitialUART0_Timer1(9600);
	set_TI;								// Set co TI 
	set_EA;								// Cho phep ngat toan cuc
	set_ES;
	led1= sang;
	while(1)
	{
//				if(check == 0)
//		{
//			Send("Hi");
//			Send("\n");
//			Timer0_Delay1ms(500);
//		}
		
		if(check == 1)
		{
			Data= strtok(rx_data,"!");
			len=strlen(Data);
			Send(Data);
			if(rx_data[len]=='!')
			{
			tachChuoi();
			Send("Gui du lieu thanh cong");
			}
			if(rx_data[len]!='!')
			{
			Send("Du lieu sai");
			}
			cnt=0;
			check=0;
		}
		IE74595_Out_Byte(a);
	}
}
