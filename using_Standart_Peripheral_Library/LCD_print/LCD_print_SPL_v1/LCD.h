#ifndef __LCD_H
#define	__LCD_H

#include "generic.h"
#include "font.h"

typedef enum _dspl {ON = 0x3F, OFF = 0x3E} _display;
typedef enum _crstl {First = 1, Second = 2} _crystal;

#define LCD_CMD1	0x68000000
#define LCD_CMD2	0x70000000

#define LCD_DATA1	0x6C000000
#define LCD_DATA2	0x74000000
	
	void PortsForLCD(void);															//Функция конфигурирования портов ввода/вывода для работы модуля
	void SystemBusInit(void);														//Функция конфигурирования внешней системной шины
	void ResetLCD(void);																//Функция сброса LCD
	void SetStartLine(uint32_t);												//Функция установки первой строаки LCD
	void SetPage(uint32_t, _crystal);										//Функция устаноовки страницы ОЗУ LCD (строка шириной 8 бит)
	void SetAddress(uint32_t);													//Функция установки адреса ОЗУ для вывода изображения (столбец)
  void DisplayOnOff(_display);												//Функция включения/выключения LCD дисплея
	void __SysTickInit(void);														//Функция инициализации системного таймера	
	void Pause8us(void);																//Функция отсчета паузы 8 мкс
	uint8_t ReadStatus(_crystal);												//Функция чтения статуса с LCD
	void SystemBusInit(void);														//Функция конфигурирования внешней системной шины
	uint8_t ReadStatus(_crystal);												//Функция чтения статуса LCD
	void WriteByte(uint8_t, uint8_t);										//Функция записи байта в память ОЗУ LCD
	uint8_t ReadByte(uint32_t, uint32_t);								//Функция чтения байта из памяти ОЗУ LCD
	void ClearLCD(void);																//Функция очистки дисплея LCD
	void ClearPage(uint8_t, _crystal);									//Функция для очистки одной страницы дисплея LCD
	void ClearPageAtTo(uint32_t, uint32_t, uint32_t);		//Функция очистки LCD внутри строки в пределах одной страницы
	void LCDInit(void);																	//Функция инициализации LCD-модуля
  void EnableSysTick(void);														//Функция для разрешения работы системного таймера и прерываний от него
	void DisableSysTick(void);													//Функция для запрета работы системного таймера и прерываний от него
	void __SysTickInit(void);														//Функция инициализации системного таймера
	void SysTick_Handler(void);													//Обработчик прерываний от системного таймера
	void Pause8us(void);																//Функция отсчета паузы 8 мкс


#endif
