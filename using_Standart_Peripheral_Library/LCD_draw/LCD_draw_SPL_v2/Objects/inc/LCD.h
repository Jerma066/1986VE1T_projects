
#ifndef __LCD_H
#define __LCD_H

/*
	Тактовая частота контроллера не должна превышать 16 МГц,
	так как иначе не будут выполняться временные требованиня
	LCD-дисплея МЭЛТ.
	Либо при более высоких частотах контроллера	необходимо
	переписать функции работы с LCD используя порты ввода/вывода,
	а не внешнюю системную шину.
*/

#include <stdint.h>

// Функции низкого уровня для LCD-модуля МЭЛТ МТ-12864А v5

typedef enum _dspl {ON = 0x3F, OFF = 0x3E} _display;
typedef enum _crstl {First = 1, Second = 2} _crystal;

#define LCD_CMD1	0x68000000
#define LCD_CMD2	0x70000000

#define LCD_DATA1	0x6C000000
#define LCD_DATA2	0x74000000

void __SysTickInit(void);				//Функция инициализации системного таймера
void Pause8us(void);					//Функция отсчета паузы 8 мкс
uint8_t ReadStatus(_crystal);			//Функция чтения статуса с LCD
void PortsForLCD(void);					//Функция конфигурирования портов ввода/вывода для работы модуля
void SystemBusInit(void);				//Функция конфигурирования внешней системной шины
void ResetLCD(void);					//Функция сброса LCD
void SetStartLine(uint32_t);			//Функция установки первой строаки LCD
void DisplayOnOff(_display);			//Функция включения/выключения LCD дисплея
void SetPage(uint32_t, _crystal);		//Функция устаноовки страницы ОЗУ LCD (строка шириной 8 бит)
void SetAddress(uint32_t);				//Функция установки адреса ОЗУ для вывода изображения (столбец)
uint8_t ReadStatus(_crystal);			//Функция чтения статуса LCD
void WriteByte(uint8_t, uint8_t);		//Функция записи байта в память ОЗУ LCD
uint8_t ReadByte(uint32_t, uint32_t);	//Функция чтения байта из памяти ОЗУ LCD
void ClearLCD(void);					//Функция очистки дисплея LCD
void ClearPage(uint8_t, _crystal);		//Функция для очистки одной страницы дисплея LCD
void ClearPageAtTo(uint32_t, uint32_t, uint32_t);	//Функция очистки LCD внутри строки в пределах одной страницы
void LCDInit(void);						//Функция инициализации LCD-модуля
__inline void EnableSysTick(void);		//Функция для разрешения работы системного таймера и прерываний от него
__inline void DisableSysTick(void);		//Функция для запрета работы системного таймера и прерываний от него
void __SysTickInit(void);				//Функция инициализации системного таймера
void SysTick_Handler(void);				//Обработчик прерываний от системного таймера
void Pause8us(void);					//Функция отсчета паузы 8 мкс

#endif	//__LCD_H
