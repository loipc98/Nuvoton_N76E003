#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

int adc_value;
bit received_flag = 0;
int cnt = 0;
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
// Dat chu ky PWM
void set_PWM_period(unsigned int period_value)		
{
	PWMPL = period_value & 0x00FF;
	PWMPH = (period_value & 0xFF00)>>8;
}

 // Thiet lap do rong xung PWM
void set_PWM_on(unsigned int on_value)						//set do rong xung
{
	PWM2L = (on_value & 0x00FF);
  PWM2H = ((on_value & 0xFF00) >> 8);
  set_LOAD;
}

// Khoi tao PWM
void PWM_Initial(void)
{
	PWM2_P05_OUTPUT_ENABLE;
	PWM_IMDEPENDENT_MODE;
	PWM_CLOCK_DIV_8;
	set_PWM_period(4095);
	set_PWMRUN;
}
void setup(void)
{
	Set_All_GPIO_Quasi_Mode;
	P05=0x00;
	SPI_Initial();
	PWM_Initial();
	// Khoi tao ADC
	Enable_ADC_AIN5;            // Khoi tao ADC pin 4
	InitialUART0_Timer1(9600);
	TI = 1;
	SPDR = 0x02;
}

void adc_read(void)
{																			
		clr_ADCF;									// Xoa co, neu ADCF=1 thi ko the chuyen doi 
		set_ADCS;									// ADCS=1 trong suot qua trinh chuyen doi, 
    while(ADCF == 0);					// ADCF = 0, chuyen doi hoan tat
		adc_value = ADCRH<<4;			// Doc gia tri ADC
		adc_value |= ADCRL;
		set_PWM_on(adc_value);		// Xuat xung PWM
		Timer0_Delay1ms(20);	
}
void main()
{
	setup();
	while(1)
	{
		adc_read();
		printf("%d\n",adc_value);
		Timer0_Delay1ms(100);
	}
}

void SPI_ISR(void) interrupt 9
{       
		clr_SPIF;
    if(cnt == 0)
    {       
			if(SPDR == 0x01)
			{
				SPDR = adc_value >> 8;
			}
    }
		else if (cnt == 1)
		{
			SPDR = adc_value & 0xFF;
		}
		cnt ++ ;
		if(cnt==3)
		{
			cnt=0;
			SPDR = 0x02;
		}
}