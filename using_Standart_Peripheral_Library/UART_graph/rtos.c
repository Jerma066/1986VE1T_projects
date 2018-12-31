
// Подключение заголовочного файла
#include "rtos.h"

// Идентификатор объекта событий
osEventFlagsId_t EventId_UART;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
	// Атрибуты потока приемопередачи данных
  static const osThreadAttr_t ThreadAttr_Typewriter =
  {
    .stack_size = 600  // Размер стека (в байтах)
  };
	
	// Инициализация ядра ОСРВ
  osKernelInitialize();

	// Создание потока передачи символов с клавиатуры ПК на дисплей отладочной платы
  osThreadNew(Thread_Typewriter, NULL, &ThreadAttr_Typewriter);
	
  // Создание объекта событий
  EventId_UART = osEventFlagsNew(NULL);	
	
  // Запуск ядра ОСРВ
  osKernelStart();	
}
