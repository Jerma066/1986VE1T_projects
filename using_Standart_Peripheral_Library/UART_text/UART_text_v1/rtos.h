
#ifndef RTOS_H
#define RTOS_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Псеводоним флага завершения аналого-цифрового преобразования
#define EVENT_ADC_EOC 0x00000001 

// Идентификатор объекта событий
extern osEventFlagsId_t EventId_UART;

// Прототипы функций
void RTOS_Init(void);  // Инициализация операционной системы реального времени

#endif
