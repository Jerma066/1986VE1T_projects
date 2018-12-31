
#ifndef __CONFIG_C
#define __CONFIG_C

#include "config.h"
#include "generic.h"

//--- Clock configuration ---
void ClkConfig()
{
	MDR_RST_CLK->HS_CONTROL=0x00000001;							//HSE - On; Osc mode On
	while((MDR_RST_CLK->CLOCK_STATUS&0x04)!=0x04);	//Wait until HSE not ready

	MDR_RST_CLK->PLL_CONTROL=(1<<8)|(1<<2);					//CPU PLL On, PLL MULL 2
	while((MDR_RST_CLK->CLOCK_STATUS&0x02)!=0x02);	//Wait until CPU PLL not ready
	
	MDR_RST_CLK->PER_CLOCK|=0x08;										//EEPROM_CTRL clock enable
	MDR_EEPROM->CMD=0;
	MDR_RST_CLK->PER_CLOCK&=(~0x08);								//EEPROM_CTRL clock disable

	MDR_RST_CLK->CPU_CLOCK=0x00000106;							//CPU Clock = CPU_PLL (16MHz)
	MDR_RST_CLK->PER_CLOCK|=(1<<24); 								//clock of PORTD On	
}

#endif
