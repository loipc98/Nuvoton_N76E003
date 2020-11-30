#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

/***********************************************************************
	WDT CONFIG enable 
	warning : this macro is only when ICP not enable CONFIG WDT function
	copy this marco code to you code to enable WDT reset.
************************************************************************/
void Check_WDT_Reset_Config(void)
{
		set_IAPEN;              //Enable function IAP
		IAPAL = 0x04;
		IAPAH = 0x00;	          //Address CONFIG 4 0004H
		IAPFD = 0xFF;    
	  IAPCN = 0xC0;						//Read config command
		set_IAPGO;              //Storage CONFIG4 data
		if ((IAPFD&0xF0)==0xF0)  
		{
				IAPFD = 0x0F;
				IAPCN = 0xE1;       //Write CONFIG
        set_IAPGO;                         						//trigger IAP
				while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
				clr_CFUEN;          // Inhibit erasing or programming CONFIG bytes by IAP
				clr_IAPEN;          //IAP function disable
				EA = 0;
				TA = 0xAA;
				TA = 0x55;
				CHPCON &= 0xFD;     //BS=0
				TA = 0xAA;
				TA = 0x55;
				CHPCON |= 0x80;     //Lenh cuoi cung duoc thuc hien truoc khi thiet lap lai thiet bi
		}
		clr_IAPEN;
}


/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{
   P05_Quasi_Mode;

		Check_WDT_Reset_Config();
	
	  TA=0xAA;TA=0x55;WDCON|=0x07;						//Setting WDT prescale 1/256 1,638s
		set_WDCLR;														//Clear WDT timer 
		while((WDCON|~SET_BIT6)==0xFF);				//confirm WDT clear is ok before into power down mode
		EA = 1;
		set_WDTR;															//WDT run
		
    while(1)
		{
			P05= 0x00;
			Timer0_Delay1ms(500);
			P05=0xff;
			Timer0_Delay1ms(500);
		  while(1);
			
		}
}

