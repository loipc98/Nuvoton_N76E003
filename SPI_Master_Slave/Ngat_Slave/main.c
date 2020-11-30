#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

int x = 0;
int y = 0;
// Khoi tao SPI
void SPI_Initial(void)
{ 
		P15_Quasi_Mode;						// P15 (SS) Quasi mode
		P10_Quasi_Mode;						// P10(SPCLK) Quasi mode
		P00_Quasi_Mode;						// P00 (MOSI) Quasi mode
    P01_Quasi_Mode;						// P22 (MISO) Quasi mode
            
    set_P0S_5;                //Schmitt triggered input select.
    
    clr_MSTR;                 // che do Slave
    clr_LSBFE;                // Gui byte cao truoc

    clr_CPOL;                 // SPCLK o muc thap khi trang thai nghi
    set_CPHA;                 // Doc du lieu o canh 2    
    
		set_EA;											// Cho phep ngat toan cuc
		set_ESPI;										// Cho phep ngat SPI
	
    set_SPIEN;                // Khoi tao SPI
    clr_SPIF; 								// Xoa co ngat
}

void setup(void)
{
	Set_All_GPIO_Quasi_Mode;
	SPI_Initial();
	InitialUART0_Timer1(9600);
	TI = 1;
}

void main()
{
	setup();
	while(1)
	{
	}
}

void SPI_ISR(void) interrupt 9
{  
	clr_SPIF;
	y = SPDR;
	printf("%d\n",y);
	SPDR = '1';
}