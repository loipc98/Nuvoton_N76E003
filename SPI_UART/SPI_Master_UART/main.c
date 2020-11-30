#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

// Khoi tao SPI
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
	SPI_Initial();
	InitialUART0_Timer1(9600);
	TI = 1;
}
UINT8 SPI_transfer(UINT8 send_byte)
{
	SPDR = send_byte;
	while(!(SPSR & SET_BIT7));		
	clr_SPIF;
	return SPDR;
}
unsigned int Received_Data_from_Slave()
{
	unsigned int adc_value;
	SS = 0;

	if(	SPI_transfer(0x01) == 0x02) 
	{
		adc_value = SPI_transfer(0x00);
		adc_value <<= 8; 
		adc_value |= SPI_transfer(0x00);
	}
	
	SS = 1;
	return adc_value;
}
void main()
{	
	setup();
	printf("Start\n");
	while(1)
	{
		unsigned int adc_value = Received_Data_from_Slave();
		printf("%d\n",adc_value);
		Timer0_Delay1ms(200);
	}	
}