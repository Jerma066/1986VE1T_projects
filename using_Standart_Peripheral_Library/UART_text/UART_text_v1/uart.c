
// Подключение заголовочного файла
#include "string.h"
#include "stdlib.h"
#include "link.h"


// Деинициализация контроллера UART
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx)
{
	//Сброс всех флагов, кроме флагов отсутсвия данных в приемнике и передатчике
	MDR_UARTx->DR      = 0;
	MDR_UARTx->RSR_ECR = 0;
	MDR_UARTx->FR			 = (1<<4) | (1 << 7);
	MDR_UARTx->ILPR    = 0;
  MDR_UARTx->IBRD    = 0;
  MDR_UARTx->FBRD    = 0;
  MDR_UARTx->LCR_H   = 0;
	MDR_UARTx->CR      = 0;
	MDR_UARTx->IFLS    = 0;
  MDR_UARTx->IMSC    = 0;
	MDR_UARTx->RIS 		 = 0;
	MDR_UARTx->MIS 		 = 0;
	MDR_UARTx->DMACR   = 0;
	MDR_UARTx->ICR 		 = 0;
}

void UART_PortCfg(void)
{
	// Конфигурации линий PC3 (TX) и PC4 (RX)
	MDR_PORTC->FUNC   	= 0x00000140;							     // Основная функции 3 и 4 пинов порта C
	MDR_PORTC->RXTX   	= 0x0000;											 // Сброс данных порта
	MDR_PORTC->OE		   &= ~((1<<4)); 	 				         // Напрваление передачи данных (PC4 - ввод)	
	MDR_PORTC->OE		   |=  ((1<<3));					         // Направление передачи данных (PC3 - вывод)
	MDR_PORTC->ANALOG  |=  ((1 << 3)  | (1 << 4));     // Режим работы линий (цифровой)
	MDR_PORTC->PWR     	=  0x000003C0;								 // Максимально быстрый фронт передачи
}

void UART_Init(void)
{
	// Включение тактирования порта C
  MDR_RST_CLK->PER_CLOCK |= (1 << 23);
	
	// Конфигурация порта для работы контроллера UART
  UART_PortCfg();
	
	// Включение тактирования контроллера UART
  MDR_RST_CLK->PER_CLOCK  |= (1 << 6);				//clock of UART1 On
  MDR_RST_CLK->UART_CLOCK |= (1 << 24);       //enable CLK of UART1
	
	// Деинициализация контроллера UART
  UART_Reset(MDR_UART1);
	
	// Задание скорости передачи данных
  // Вычисление требуемого значения делителя тактовой частоты
	uint32_t divider  = SystemCoreClock / (UART_SPEED >> 2);
	
	// Запись в регистр целой части делителя
  MDR_UART1->IBRD = divider >> 6;
	
	// Запись в регистр дробной части делителя
  MDR_UART1->FBRD = divider & 0x3F;
	
	// Конфигурация формата данных
  MDR_UART1->LCR_H = (1 << 1)   // Использование контрольного бита (включено)
									 | (0 << 2)   // Проверка на нечетность или нечетность (четность)
									 | (0 << 3)   // Количество STOP-битов (один)
									 | (0 << 4)   // Использование буфера FIFO (не используется)
									 | (3 << 5)   // Количество битов данных (8)
									 | (0 << 7);  // Фиксация значения контрольного бита (запрещена)
							 
	// Конфигурация контроллера UART
  MDR_UART1->CR = (1 << 0)    // Работа контроллера UART (включен)
								| (0 << 1)    // Работа ИК-кодека (отключен)
								| (0 << 2)    // Длительность импульсов при ИК-передаче (не используется)
								| (0 << 7)    // Режим самотестирования (отключен)
								| (1 << 8)    // Разрешение передачи (разрешена)
								| (1 << 9)    // Разрешение приема (разрешен)
								| (0 << 10)   // Инверсия сигнала на линии DTR (не используется)
								| (0 << 11)   // Инверсия сигнала на линии RTS (не используется)
								| (0 << 12)   // Инверсия сигнала на линии OUT1 (не используется)
								| (0 << 13)   // Инверсия сигнала на линии OUT2 (не используется)
								| (0 << 14)   // Использование линии RTS (не используется)
								| (0 << 15);  // Использование линии CTS (не используется)
						
	// Настройка запросов на обработку прерываний от контроллера UART
  MDR_UART1->IMSC = (1 << 4)   // Прерывание от приемника
									| (1 << 5);  // Прерывание от передатчика
							
	// Назначение приоритета прерывания от контроллера UART
	NVIC_SetPriority(UART1_IRQn, 1);
	
	// Разрешение обработки прерывания от контроллера UART
	NVIC_EnableIRQ(UART1_IRQn);
}

// Отображение символов с инкрементом координат
void LCD_PutSymbolModified(uint8_t symbol)
{
  static uint8_t x, y;  // Координаты символа

  // Обработка команд
  switch(symbol)
  {
    // Обработка операции стирания (Backspace)
    case 0x7F:
      LCD_PutSymbol(' ', x, y);
      x--;
      if (x & 0xF0)
      {
        x = 127;
        y--;
        if (y & 0xF8)
        {
          x = 0;
          y = 0;
        }
      }
      LCD_PutSymbol(' ', x, y);
      break;

    // Обработка операции перехода на новую строку (Enter)
    case 0x0D:
       LCD_PutSymbol('_', x, y);
      x = 0;
      y++;
      if (y > 7)
        y = 0;
      LCD_PutSymbol('_', x, y);
      break;

    // Обработка символов
    default:
    LCD_PutSymbol(symbol, x, y);
    x+=6;
    if (x > 127)
    {
      x = 0;
      y++;
      if (y > 7)
        y = 0;
    }
    LCD_PutSymbol('_', x, y);
    break;
  }
}

// Поток передачи символов с клавиатуры ПК на дисплей отладочной платы
void Thread_Typewriter(void *argument)
{
  // Переменная для хранения полученных данных
  uint8_t data;
	char* string = "Ready to recieve!";

	
  // Отображение указателя на дисплей	
  LCD_PutSymbol('_', 0, 0);
	

	PrintConstText (string, 7, 25);	
	
	
	
  // Основной цикл
  while(1)
  {

		
    // Ожидание приема данных
    osEventFlagsWait(EventId_UART, EVENT_DATA_RECEIVED, osFlagsWaitAny, osWaitForever);
    // Считывание принятых данных
    data = MDR_UART1->DR;
		
    // Отображение принятого символа на дисплей
    LCD_PutSymbolModified(data);

    // Передача данных в обратном направлении
    MDR_UART1->DR = data;

    // Ожидание завершения передачи данных
    osEventFlagsWait(EventId_UART, EVENT_DATA_TRANSMITTED, osFlagsWaitAny, osWaitForever);
		
  }
}

// Обработка аппаратных прерываний модуля UART
void UART1_IRQHandler(void)
{
  // Если произошло прерывание от приемника...
  if (MDR_UART1->RIS & 0x10)
  {
    // Установка флага завершения приема данных
    osEventFlagsSet(EventId_UART, EVENT_DATA_RECEIVED);

    // Сброс запроса на обработку прерывания
    MDR_UART1->ICR |= (1 << 4);
  }

  // Если произошло прерывание от передатчика...
  if (MDR_UART1->RIS & 0x20)
  {
    // Установка флага завершения передачи данных
    osEventFlagsSet(EventId_UART, EVENT_DATA_TRANSMITTED);

    // Сброс запроса на обработку прерывания
    MDR_UART1->ICR |= (1 << 5);
  }
}
