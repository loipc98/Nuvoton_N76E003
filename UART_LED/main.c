#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include <string.h>
#include <math.h>
//char str[]="Hello";

char rx_data[30];
unsigned char led[]={0x00,0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char a= 0x00;
char *Data1,*Data2,*Data3,*Data4;
int dkLed=0;dk1=0;dk2=0;
int i;
int cnt=0;
int send=0;

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

//Ham StringToInt
int stringToInt(unsigned char *b)
 {
	 int i;
 	int l1 = strlen(b);
 	int num1 = 0;
 	for(i = l1-1;i>=0;--i){
 		num1+=(int)(b[i]-'0')*pow(10,l1-i-1);
	 }
	 return num1;
 }
 
//Ham digitalWrite
void digitalWrite(char x, char y){
	if(y==1){
		a|= led[x];
	}
	if(y==0){
	  a&=~ led[x];
	}
}
 
//Ham kiem tra Dieu Kien
  void kiemTraDK()
 {
	 if(dkLed==1){
		 if(dk1==1)
		 {
			 digitalWrite(stringToInt(Data3),1);
		 }
		 if(dk1==0)
		 {
			 digitalWrite(stringToInt(Data3),0);
		 }
		 if(dk2==1)
		 {
			 digitalWrite(stringToInt(Data4),1);
		 }
		 if(dk2==0)
		 {
			 digitalWrite(stringToInt(Data4),0);
		 }
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
// Ham tach chuoi
void TachChuoi()				
{
	Data1= strtok(Data1,":");
	if(strcmp(Data1,"led")==0) dkLed=1;
  Data1= strtok(NULL,";");
  Data2= strtok(NULL,";");
  	
  	//Xy ly data1
  Data1= strtok(Data1,":");
  if(strcmp(Data1,"on")==0) dk1=1;
  while(Data1!=NULL){
  	Data1= strtok(NULL,",");
		Data3= Data1;
		kiemTraDK();
	  }
  	//Xu lu data2
  Data2= strtok(Data2,":");
  if(strcmp(Data2,"off")==0) dk1=1;
  while(Data2!=NULL){
  	Data2= strtok(NULL,",");
		Data4= Data2;
		kiemTraDK();
	  }
}


// Ngat UART ngoai
void UART_ISR(void) interrupt 4
{
	if(RI==1)
	{
			rx_data[cnt] = Receive_Data_From_UART0();	
			cnt++;
			send=1;
		  clr_RI;
	}
	if(TI==1)
	{
		TI=0;
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
	set_TI;								// Set co TI 
	set_EA;								// Cho phep ngat toan cuc
	set_ES;								// Cho phep ngat UART
//	IE74595_Out_Byte(a);
	led1=sang;
	while(1)
	{
		if(send == 0)
		{
			Send("Hello");
			Send("\n");
			Timer0_Delay1ms(100);
		}
		if(send == 1)
		{
	    Data1= strtok(rx_data,"!");
			Send(Data1);
			send=0;
			cnt=0;
		}
		TachChuoi();
		IE74595_Out_Byte(a);
	}
}