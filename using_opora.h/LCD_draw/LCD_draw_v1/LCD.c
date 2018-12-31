#ifndef __LCD_C
#define __LCD_C

/*
	�������� ������� ����������� �� ������ ��������� 16 ���,
	��� ��� ����� �� ����� ����������� ��������� �����������
	LCD-������� ����.
	���� ��� ����� ������� �������� �����������	����������
	���������� ������� ������ � LCD ��������� ����� �����/������,
	� �� ������� ��������� ����.
*/

// ������� ������� ������ ��� LCD-������ ���� ��-12864� v5

#include "opora.h"
#include "LCD.h"

uint32_t Flag8us, DisableFlag;

//������� ���������������� ������ �����/������ ��� ������ ������
void PortsForLCD()
{
//���� � - ���� ������
	RST_CLK->PER_CLOCK |= 0x15<<21;	//������������ ������ A, C, E
	
//��� ��������� �������� ������� ����� xn � ���� FUN� ��������� PORTx ��������� 1 �� ������� 2n � 0 �� ������� (2n+1)	
	PORTA->FUNC |= 0x00005555;	//PORTA[7:0] - �������� ������� (���� ������)
	PORTA->ANALOG |= 0x00FF;		//PORTA[7:0] - digital
	PORTA->PWR |= 0x0000FFFF;		//���������� ����������� ������� ����� ��� ���� ������

	PORTC->FUNC |= 0x00000021;	//PORTC 0 - �������� ������� PORTC 2 - �������������� 
	PORTC->ANALOG |= 0x0005; 		//PORTC0 � PORTC1 - digital
	PORTC->PWR |= 0x00000051;		//���������� ����������� ������� �����

	PORTE->FUNC |= 0x2A000000;	//PORTE 12, 13, 14 - �������������� ������� (A26, A27, A28)
	PORTE->ANALOG |= 0x7000;		//digital 
	PORTE->PWR |= 0x3F000000; 	//���������� ����������� ������� �����
	
	PORTF->FUNC = 0;						//PORTF0 - ������� ����
	PORTF->RXTX = 0;						//����� ������ ������ ����� ������ ��� ������ �� ������ ����� � ��� ������. 
	PORTF->OE |= 0x0001;				//����������� ����� �� �����
	PORTF->ANALOG = 0x0001;		  //digital
	PORTF->PWR |= 0x00000003;		//���������� ����������� ������� �����

} 


//������� ���������������� ������� ��������� ����
void SystemBusInit()
{
	//PCLK[30] - EXT_BUS_CNTRL
	RST_CLK->PER_CLOCK |= 1<<30;							//������������ ������� ��������� ����
	EXT_BUS_CNTRL->EXT_BUS_CONTROL = 0xF002;  
}


//������� ���������/���������� LCD �������
//���������: Function - ��������� ��� ���������� LCD
//����������� �������� ���������: ON - ��������� (0x3F), OFF - ���������� (0x3E).
void DisplayOnOff(_display Function)
{
	*((uint32_t*)LCD_CMD1) = Function;
	Pause8us();
	
	*((uint32_t*)LCD_CMD2) = Function;
	Pause8us();
}


//������� ��������� ������ ������ LCD
//���������: LineNumber - ������ ������ LCD
void SetStartLine(uint32_t LineNumber)
{
	*((uint32_t*)LCD_CMD1) = 0xC0|(LineNumber&0x3F);
	Pause8us();
	
	*((uint32_t*)LCD_CMD2) = 0xC0|(LineNumber&0x3F);
	Pause8us();
}


//������� ���������� �������� ��� LCD (������ ������� 8 ���)
//���������: Page - ����������� �������� LCD (����� ��������� �������� �� 0 �� 7)
//			 Crystal - �������� LCD, �� ������� ��������������� �������� Page.
//			 (����� ��������� �������� First (1) ��� Second (2))
void SetPage(uint32_t Page, _crystal Crystal)
{
	
		if (Crystal == First) {*((uint32_t*)LCD_CMD1) = (0xB8|(Page&0x07));}
										 else {*((uint32_t*)LCD_CMD2) = (0xB8|(Page&0x07));}
	
		Pause8us();									
}


//������� ��������� ������ ��� ��� ������ ����������� (�������).
//���������: Address - �����, � �������� ����� ���������� �����������
//			 �� ������� �������� LCD (����� ��������� �������� �� 0 �� 127).
void SetAddress(uint32_t Address)
{
	if(Address < 64){*((uint32_t*)LCD_CMD1) = (0x40|(Address&0x3F));}
							else{*((uint32_t*)LCD_CMD2) = (0x40|(Address&0x3F));}
	Pause8us();
}


//������� ������ ������� LCD
//���������: Crystal - �������� LCD, � �������� ����� ��������
//			 ���������� (����� ��������� �������� First (1) ��� Second (2))
uint8_t ReadStatus(_crystal Crystal)
{
	uint8_t Temp;
	
	if (Crystal==First){Temp = *((uint32_t*)LCD_CMD1);}
	 						  else {Temp = *((uint32_t*)LCD_CMD2);}
		Pause8us();							
		return Temp;
}


//������� ������ ����� � ������ ��� LCD
//���������: Byte - ���� ��� ������ � ���
//			 		 Address - �����, �� �������� ����� ����������� ������
//			 		 (����� ��������� �������� �� 0 �� 127).
void WriteByte(uint8_t Byte, uint8_t Address)
{
	if(Address < 64){SetAddress(Address); *((uint32_t*)LCD_DATA1) = Byte;}
							else{SetAddress(Address); *((uint32_t*)LCD_DATA2) = Byte;}
	Pause8us();
}

//������� ������ ����� �� ������ ��� LCD
//���������: Page - ��������, � ������� ����������� ����
//			 		 Address - �����, �� �������� ����� ����������� ����
//			 		 (����� ��������� �������� �� 0 �� 127).
uint8_t ReadByte(uint32_t Page, uint32_t Address)
{
	uint8_t Temp;
	if(Address < 64)
	{
		SetPage(Page,First);
		SetAddress(Address);
		Temp = *((uint32_t*)LCD_DATA1);
		Pause8us();
		Temp = *((uint32_t*)LCD_DATA1);
	}
	else
	{
		SetPage(Page,Second);
		SetAddress(Address);
		Temp = *((uint32_t*)LCD_DATA2);
		Pause8us();
		Temp = *((uint32_t*)LCD_DATA2);
	}
	Pause8us();
	return Temp;
}


//������� ������ LCD-������
void ResetLCD()
{
	PORTF->CLRTX = 1;
	Pause8us();
	PORTF->SETTX = 1;
	Pause8us();
}


//������� ������� ������� LCD
void ClearLCD()
{
	uint32_t i, j;
	
	SetAddress(0);
	for(j=0;j<8;j++)
	{
		SetPage(j,First);
		for(i=0;i<64;i++)
		{
			WriteByte(0x00,i);
		}
	}
	
	SetAddress(64);
	for(j=0;j<8;j++)
	{
		SetPage(j,Second);
		for(i=64;i<128;i++)
		{
			WriteByte(0x00,i);
		}
	}
	
	SetAddress(0);
	SetPage(0,First);
	SetAddress(64);
	SetPage(0,Second);
}

//������� ��� ������� ����� �������� ������� LCD
void ClearPage(uint8_t Page, _crystal Crys)
{
	uint8_t i;
	if(Crys == First)
	{
		SetPage(Page,First);
		SetAddress(0);
		for(i=0;i<64;i++)
		{
			*((uint32_t*)LCD_DATA1) = 0x00;
			Pause8us();
		}
	}
	else
	{
		SetPage(Page,Second);
		SetAddress(64);
		for(i=0;i<64;i++)
		{
			*((uint32_t*)LCD_DATA2) = 0x00;
			Pause8us();
		}
	}	
}


//������� ��� ���������� ������ ���������� ������� � ���������� �� ����
__inline void EnableSysTick()
{
	SysTick->CTRL = 0x00000003;	//Enabel SysTick, Enable Interrupt	
}

//������� ��� ������� ������ ���������� ������� � ���������� �� ����
__inline void DisableSysTick()
{
	SysTick->CTRL = 0x00000000;	//Disable SysTick, Disable Interrupt	
}

//������� ������������� ���������� �������
void __SysTickInit()
{
	if((SysTick->CTRL&1) == 1) DisableFlag = 1;
						 else SysTick->LOAD = 0x00000080;	//Pause 8 us (HCLK = 16MHz)
}

//���������� ���������� �� ���������� �������
void SysTick_Handler()
{
	Flag8us = 1;
}

// 8 ���
void Pause8us()
{
	EnableSysTick();
	
	while(Flag8us != 1);
	
	DisableSysTick();	
	Flag8us = 0;
}

//������� ������������� LCD-������
void LCDInit()
{

	PortsForLCD();
	SystemBusInit();
	Flag8us = 0;
	DisableFlag = 0;
	__SysTickInit();
	if(DisableFlag != 1)
	{
		ResetLCD();
		ClearLCD();
		DisplayOnOff(ON);
		SetStartLine(0);
	}
}


#endif	//__LCD_C
