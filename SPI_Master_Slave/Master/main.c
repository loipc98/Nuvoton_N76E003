#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
int adc_value,n = 0;
char adc[2];

void SPI_Initial(void)
{
	P15_Quasi_Mode;					// Chan SS
	P10_Quasi_Mode;					// Chan SPCLK
	P00_Quasi_Mode;					// Chan MOSI
	P01_Quasi_Mode;					// Chan MISO
	
	set_DISMODF;						// SS che do I/O cho Master// DISMODF = 1 && SSOE = 0
	clr_SSOE;								// SS che do I/O cho Master// DISMODF = 1 && SSOE = 0
	
	clr_LSBFE; 							// Gui byte cao truoc
	
	clr_CPOL;								// SPCLK o muc thap khi trang thai nghi
	set_CPHA; 							// Doc du lieu o canh 2
	
	set_MSTR;								// Che do Master 
	SPICLK_DIV16;						// Bo chia 16
	set_SPIEN;							// Khoi tao SPI
	clr_SPIF;								// Xoa co bao ngat
}

void setup(void)
{
	Set_All_GPIO_Quasi_Mode;
	P05=0x00;
	InitialUART0_Timer1(9600);
	TI = 1;
	SPI_Initial();
}
void main()
{	
	setup();
	Send_Data_To_UART0('1');
	while(1)
	{
		SS = 0;
		SPDR = 0xFF;
		while(!(SPSR & SET_BIT7));		
		clr_SPIF;
		y = SPDR;
		SS = 1;
		if(n==1) adc[0]=SPDR;
		if(n==2) adc[1]=SPDR;
		if(n==3) n=0;
		n++;
		adc_value= adc[0];
		adc_value <<=4;
		adc_value |= adc[1];
		printf("%d",adc_value);
		printf("\n");
		Timer0_Delay1ms(300);
	}	
}