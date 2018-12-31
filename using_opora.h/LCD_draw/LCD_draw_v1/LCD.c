#ifndef __LCD_C
#define __LCD_C

/*
	Тактовая частота контроллера не должна превышать 16 МГц,
	так как иначе не будут выполняться временные требованиня
	LCD-дисплея МЭЛТ.
	Либо при более высоких частотах контроллера	необходимо
	переписать функции работы с LCD используя порты ввода/вывода,
	а не внешнюю системную шину.
*/

// Функции низкого уровня для LCD-модуля МЭЛТ МТ-12864А v5

#include "opora.h"
#include "LCD.h"

uint32_t Flag8us, DisableFlag;

//Функция конфигурирования портов ввода/вывода для работы модуля
void PortsForLCD()
{
//порт А - шина данных
	RST_CLK->PER_CLOCK |= 0x15<<21;	//Тактирование портов A, C, E
	
//для получения основной функции порта xn в поле FUNС структуры PORTx записывае 1 на позицию 2n и 0 на позицию (2n+1)	
	PORTA->FUNC |= 0x00005555;	//PORTA[7:0] - основная функция (шина данных)
	PORTA->ANALOG |= 0x00FF;		//PORTA[7:0] - digital
	PORTA->PWR |= 0x0000FFFF;		//используем максимально быстрый фронт для шины данных

	PORTC->FUNC |= 0x00000021;	//PORTC 0 - основная функция PORTC 2 - альтернативная 
	PORTC->ANALOG |= 0x0005; 		//PORTC0 и PORTC1 - digital
	PORTC->PWR |= 0x00000051;		//используем максимально быстрый фронт

	PORTE->FUNC |= 0x2A000000;	//PORTE 12, 13, 14 - альтернативная функция (A26, A27, A28)
	PORTE->ANALOG |= 0x7000;		//digital 
	PORTE->PWR |= 0x3F000000; 	//используем максимально быстрый фронт
	
	PORTF->FUNC = 0;						//PORTF0 - функция порт
	PORTF->RXTX = 0;						//Режим работы вывода порта Данные для выдачи на выводы порта и для чтения. 
	PORTF->OE |= 0x0001;				//Направление порта на выход
	PORTF->ANALOG = 0x0001;		  //digital
	PORTF->PWR |= 0x00000003;		//используем максимально быстрый фронт

} 


//Функция конфигурирования внешней системной шины
void SystemBusInit()
{
	//PCLK[30] - EXT_BUS_CNTRL
	RST_CLK->PER_CLOCK |= 1<<30;							//Тактирование внешней системной шины
	EXT_BUS_CNTRL->EXT_BUS_CONTROL = 0xF002;  
}


//Функция включения/выключения LCD дисплея
//Параметры: Function - включение или выключение LCD
//Принимаемые значения параметра: ON - включение (0x3F), OFF - выключение (0x3E).
void DisplayOnOff(_display Function)
{
	*((uint32_t*)LCD_CMD1) = Function;
	Pause8us();
	
	*((uint32_t*)LCD_CMD2) = Function;
	Pause8us();
}


//Функция установки первой строки LCD
//Параметры: LineNumber - первая строка LCD
void SetStartLine(uint32_t LineNumber)
{
	*((uint32_t*)LCD_CMD1) = 0xC0|(LineNumber&0x3F);
	Pause8us();
	
	*((uint32_t*)LCD_CMD2) = 0xC0|(LineNumber&0x3F);
	Pause8us();
}


//Функция устаноовки страницы ОЗУ LCD (строка шириной 8 бит)
//Параметры: Page - необходимая страница LCD (может принимать значения от 0 до 7)
//			 Crystal - кристалл LCD, на котором устанавливается страница Page.
//			 (может принимать значение First (1) или Second (2))
void SetPage(uint32_t Page, _crystal Crystal)
{
	
		if (Crystal == First) {*((uint32_t*)LCD_CMD1) = (0xB8|(Page&0x07));}
										 else {*((uint32_t*)LCD_CMD2) = (0xB8|(Page&0x07));}
	
		Pause8us();									
}


//Функция установки адреса ОЗУ для вывода изображения (столбец).
//Параметры: Address - адрес, с которого будет выводиться изображение
//			 на текущей странице LCD (может принимать значения от 0 до 127).
void SetAddress(uint32_t Address)
{
	if(Address < 64){*((uint32_t*)LCD_CMD1) = (0x40|(Address&0x3F));}
							else{*((uint32_t*)LCD_CMD2) = (0x40|(Address&0x3F));}
	Pause8us();
}


//Функция чтения статуса LCD
//Параметры: Crystal - кристалл LCD, с которого будет читаться
//			 информация (может принимать значение First (1) или Second (2))
uint8_t ReadStatus(_crystal Crystal)
{
	uint8_t Temp;
	
	if (Crystal==First){Temp = *((uint32_t*)LCD_CMD1);}
	 						  else {Temp = *((uint32_t*)LCD_CMD2);}
		Pause8us();							
		return Temp;
}


//Функция записи байта в память ОЗУ LCD
//Параметры: Byte - байт для записи в ОЗУ
//			 		 Address - адрес, по которому будет произведена запись
//			 		 (может принимать значения от 0 до 127).
void WriteByte(uint8_t Byte, uint8_t Address)
{
	if(Address < 64){SetAddress(Address); *((uint32_t*)LCD_DATA1) = Byte;}
							else{SetAddress(Address); *((uint32_t*)LCD_DATA2) = Byte;}
	Pause8us();
}

//Функция чтения байта из памяти ОЗУ LCD
//Параметры: Page - страница, с которой считывается байт
//			 		 Address - адрес, по которому будет считываться байт
//			 		 (может принимать значения от 0 до 127).
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


//Функция сброса LCD-модуля
void ResetLCD()
{
	PORTF->CLRTX = 1;
	Pause8us();
	PORTF->SETTX = 1;
	Pause8us();
}


//Функция очистки дисплея LCD
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

//Функция для очистки одной страницы дисплея LCD
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


//Функция для разрешения работы системного таймера и прерываний от него
__inline void EnableSysTick()
{
	SysTick->CTRL = 0x00000003;	//Enabel SysTick, Enable Interrupt	
}

//Функция для запрета работы системного таймера и прерываний от него
__inline void DisableSysTick()
{
	SysTick->CTRL = 0x00000000;	//Disable SysTick, Disable Interrupt	
}

//Функция инициализации системного таймера
void __SysTickInit()
{
	if((SysTick->CTRL&1) == 1) DisableFlag = 1;
						 else SysTick->LOAD = 0x00000080;	//Pause 8 us (HCLK = 16MHz)
}

//Обработчик прерываний от системного таймера
void SysTick_Handler()
{
	Flag8us = 1;
}

// 8 мкс
void Pause8us()
{
	EnableSysTick();
	
	while(Flag8us != 1);
	
	DisableSysTick();	
	Flag8us = 0;
}

//Функция инициализации LCD-модуля
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
