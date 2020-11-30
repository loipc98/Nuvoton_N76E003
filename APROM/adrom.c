//***********************************************************************************************************
//  File Function: N76E003 APROM program DATAFLASH as EEPROM way 
//***********************************************************************************************************
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

volatile unsigned char xdata page_buffer[128];

//Ham doc byte APROM cho truoc dia chi
UINT8 Read_APROM_BYTE(UINT16 code *u16_addr)
{
	UINT8 rdata;
	rdata = *u16_addr>>8;
	return rdata;
}	
//Ham ghi du lieu flashdata voi du lieu va dia chi
void Write_DATAFLASH_BYTE(unsigned int u16EPAddr,unsigned char u8EPData) //0x3802,0x34
{
	unsigned char looptmp=0; //bien lap
	unsigned int u16_addrl_r;//bien luu dia chi tren XRAM
	unsigned int RAMtmp;     
	
	// Kiem tra dia chi bat dau trang
	u16_addrl_r=(u16EPAddr/128)*128; //0x3800
	//Save APROM data to XRAM0         
	for(looptmp=0;looptmp<0x80;looptmp++)  //128
	{
		RAMtmp = Read_APROM_BYTE((unsigned int code *)(u16_addrl_r+looptmp)); //doc lan luot cac dia chi 
		page_buffer[looptmp]=RAMtmp;     //(0x3800+looptmp)
	}
// Modify customer data in XRAM 
		page_buffer[u16EPAddr&0x7f] = u8EPData;     //luu gia tri u8EPData vao page_buffer trong XRAM (page_buffer[0x3802 & 0x7f] = 0x34)
	
//Erase APROM DATAFLASH page
  	IAPAL = u16_addrl_r&0xff;                  //Lay 8 bit thap 0x3800: 0x00
	  IAPAH = (u16_addrl_r>>8)&0xff;           //Lay 8 bit cao 0x3800: 0x38
		IAPFD = 0xFF;                           
	  set_IAPEN;                              // Enable Function IAP
		set_APUEN;                              //APROM updated enable
    IAPCN = 0x22; 		                      //Mode APROM page-erase
 		set_IAPGO;                              //
		
//Save changed RAM data to APROM DATAFLASH
		set_IAPEN;                             //Enable Function IAP
		set_APUEN;                             //APROM updated enable
	  IAPCN = 0x21;                          // IAPFD: Data in
		for(looptmp=0;looptmp<0x80;looptmp++)
		{
			IAPAL = (u16_addrl_r&0xff)+looptmp;  // (0x3800&0xff)+looptmp= 0x00+ looptmp
      IAPAH = (u16_addrl_r>>8)&0xff;       // 0x38
			//Dia chi 0x3800
			IAPFD = page_buffer[looptmp];
			set_IAPGO;			
		}
		clr_APUEN;
		clr_IAPEN;
}	
	
/******************************************************************************************************************/	


void main (void) 
{
		UINT8 datatemp;
/* -------------------------------------------------------------------------*/
/*  Dataflash use APROM area, please ALWAYS care the address of you code    */
/*	APROM 0x3800~0x38FF demo as dataflash 				      				      			*/
/* 	Please use Memory window key in C:0x3800 to check earse result					*/	      
/* -------------------------------------------------------------------------*/
	  P05_Quasi_Mode;
		InitialUART0_Timer1(9600);
	  TI=1;
//		Write_DATAFLASH_BYTE (0x3805,'a');
	  P05=0;
	  if(Read_APROM_BYTE(0x3802)==0x01)
		{
			P05= Read_APROM_BYTE(0x3805);
		}
		Timer0_Delay1ms(2000);
		Write_DATAFLASH_BYTE (0x3805,0xFF);
		Write_DATAFLASH_BYTE (0x3802,0x01);
    while(1)
		{
    //call read byte
		datatemp = Read_APROM_BYTE(0x3805);
		P05= datatemp;
		Send_Data_To_UART0(datatemp);
		}
}
//-----------------------------------------------------------------------------------------------------------
