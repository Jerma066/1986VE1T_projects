
// ����������� ������������� �����
#include "clk.h"

// ������������� ������� ������������ ����������������
void CPU_Init(void)
{
  // ����� �������� ������� ������������
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
  
	
  // ���������� ������������ ���� ������,
  // ����� ������� ������������, ����������� ������ � ����� B
	MDR_RST_CLK->PER_CLOCK = (1 << 4) | (1 << 27) | (1 << 22);
	
	
	
  // ��������� ���������� HSE
  MDR_RST_CLK->HS_CONTROL = (1 << 0); 

  // �������� ����� ���������� HSE � ������� �����
  while ((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY) == 0);

  // ������������� ����� PLL
  MDR_RST_CLK->PLL_CONTROL = (1 << 2)    // ��������� ����� PLL_�PU
													 | (1 << 8);   // ������� ������������ ��������� (1 + 1 = 2)

  // �������� ����� ����� PLL � ������� �����
  while ((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) == 0);

  // ��������� ������ ������������ �������� �������
	MDR_RST_CLK->CPU_CLOCK = 0x00000106;

  // ���������� ����������� HSI � LSI
  MDR_BKP->REG_0F &= ~((1 << 15) | (1 << 22));

  // ���������� �������� �������� �������
  SystemCoreClockUpdate();
}
