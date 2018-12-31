
#ifndef UART_H
#define UART_H

#include "link.h"

// Подключение заголовочных файлов модулей проекта

#define EVENT_DATA_RECEIVED    0x00000001
#define EVENT_DATA_TRANSMITTED 0x00000002


// Требуемая скорость передачи данных
#define UART_SPEED 9600

// Прототипы функций
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx);          // Деинициализация контроллера UART
void UART_PortCfg(void);                       				 // Конфигурация порта для работы с контроллером UART (UART2)
void UART_Init(void);                          				 // Инициализация модуля UART (UART2)
void Thread_Typewriter(void *argument);                // Поток передачи символов с клавиатуры ПК на дисплей отладочной платы

#endif
