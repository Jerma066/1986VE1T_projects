
#ifndef __CONFIG_C
#define __CONFIG_C

#include "opora.h"
#include "config.h"

//--- Clock configuration ---
void ClkConfig()
{
	RST_CLK->HS_CONTROL=0x00000001;							//HSE - On; Osc mode On
	while((RST_CLK->CLOCK_STATUS&0x04)!=0x04);	//Wait until HSE not ready

	RST_CLK->PLL_CONTROL=(1<<8)|(1<<2);					//CPU PLL On, PLL MULL 2
	while((RST_CLK->CLOCK_STATUS&0x02)!=0x02);	//Wait until CPU PLL not ready
	
	RST_CLK->PER_CLOCK|=0x08;										//EEPROM_CTRL clock enable
	EEPROM->CMD=0;
	RST_CLK->PER_CLOCK&=(~0x08);								//EEPROM_CTRL clock disable

	RST_CLK->CPU_CLOCK=0x00000106;							//CPU Clock = CPU_PLL (16MHz)
	RST_CLK->PER_CLOCK|=(1<<24); 								//clock of PORTD On	
}

#endif