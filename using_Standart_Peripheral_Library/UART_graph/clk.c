
// Подключение заголовочного файла
#include "clk.h"

// Инициализация системы тактирования микроконтроллера
void CPU_Init(void)
{
  // Сброс настроек системы тактирования
	MDR_RST_CLK->PLL_CONTROL   = 0;
  MDR_RST_CLK->HS_CONTROL    = 0;
  MDR_RST_CLK->CPU_CLOCK     = 0;
  MDR_RST_CLK->USB_CLOCK     = 0;
  MDR_RST_CLK->ADC_MCO_CLOCK = 0;
  MDR_RST_CLK->RTC_CLOCK     = 0;
  MDR_RST_CLK->CAN_CLOCK     = 0;
  MDR_RST_CLK->TIM_CLOCK     = 0;
  MDR_RST_CLK->UART_CLOCK    = 0;
  MDR_RST_CLK->SSP_CLOCK     = 0;
  
	
  // Отключение тактирования всех блоков,
  // кроме системы тактирования, батарейного домена и порта B
	MDR_RST_CLK->PER_CLOCK = (1 << 4) | (1 << 27) | (1 << 22);
	
	
	
  // Включение генератора HSE
  MDR_RST_CLK->HS_CONTROL = (1 << 0); 

  // Ожидание входа генератора HSE в рабочий режим
  while ((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY) == 0);

  // Инициализация блока PLL
  MDR_RST_CLK->PLL_CONTROL = (1 << 2)    // Включение блока PLL_СPU
													 | (1 << 8);   // Задание коэффициента умножения (1 + 1 = 2)

  // Ожидание входа блока PLL в рабочий режим
  while ((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) == 0);

  // Настройка тракта формирования тактовой частоты
	MDR_RST_CLK->CPU_CLOCK = 0x00000106;

  // Отключение генераторов HSI и LSI
  MDR_BKP->REG_0F &= ~((1 << 15) | (1 << 22));

  // Обновление значения тактовой частоты
  SystemCoreClockUpdate();
}
