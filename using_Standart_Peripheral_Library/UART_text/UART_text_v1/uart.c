
// ����������� ������������� �����
#include "string.h"
#include "stdlib.h"
#include "link.h"


// ��������������� ����������� UART
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx)
{
	//����� ���� ������, ����� ������ ��������� ������ � ��������� � �����������
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
	// ������������ ����� PC3 (TX) � PC4 (RX)
	MDR_PORTC->FUNC   	= 0x00000140;							     // �������� ������� 3 � 4 ����� ����� C
	MDR_PORTC->RXTX   	= 0x0000;											 // ����� ������ �����
	MDR_PORTC->OE		   &= ~((1<<4)); 	 				         // ����������� �������� ������ (PC4 - ����)	
	MDR_PORTC->OE		   |=  ((1<<3));					         // ����������� �������� ������ (PC3 - �����)
	MDR_PORTC->ANALOG  |=  ((1 << 3)  | (1 << 4));     // ����� ������ ����� (��������)
	MDR_PORTC->PWR     	=  0x000003C0;								 // ����������� ������� ����� ��������
}

void UART_Init(void)
{
	// ��������� ������������ ����� C
  MDR_RST_CLK->PER_CLOCK |= (1 << 23);
	
	// ������������ ����� ��� ������ ����������� UART
  UART_PortCfg();
	
	// ��������� ������������ ����������� UART
  MDR_RST_CLK->PER_CLOCK  |= (1 << 6);				//clock of UART1 On
  MDR_RST_CLK->UART_CLOCK |= (1 << 24);       //enable CLK of UART1
	
	// ��������������� ����������� UART
  UART_Reset(MDR_UART1);
	
	// ������� �������� �������� ������
  // ���������� ���������� �������� �������� �������� �������
	uint32_t divider  = SystemCoreClock / (UART_SPEED >> 2);
	
	// ������ � ������� ����� ����� ��������
  MDR_UART1->IBRD = divider >> 6;
	
	// ������ � ������� ������� ����� ��������
  MDR_UART1->FBRD = divider & 0x3F;
	
	// ������������ ������� ������
  MDR_UART1->LCR_H = (1 << 1)   // ������������� ������������ ���� (��������)
									 | (0 << 2)   // �������� �� ���������� ��� ���������� (��������)
									 | (0 << 3)   // ���������� STOP-����� (����)
									 | (0 << 4)   // ������������� ������ FIFO (�� ������������)
									 | (3 << 5)   // ���������� ����� ������ (8)
									 | (0 << 7);  // �������� �������� ������������ ���� (���������)
							 
	// ������������ ����������� UART
  MDR_UART1->CR = (1 << 0)    // ������ ����������� UART (�������)
								| (0 << 1)    // ������ ��-������ (��������)
								| (0 << 2)    // ������������ ��������� ��� ��-�������� (�� ������������)
								| (0 << 7)    // ����� ���������������� (��������)
								| (1 << 8)    // ���������� �������� (���������)
								| (1 << 9)    // ���������� ������ (��������)
								| (0 << 10)   // �������� ������� �� ����� DTR (�� ������������)
								| (0 << 11)   // �������� ������� �� ����� RTS (�� ������������)
								| (0 << 12)   // �������� ������� �� ����� OUT1 (�� ������������)
								| (0 << 13)   // �������� ������� �� ����� OUT2 (�� ������������)
								| (0 << 14)   // ������������� ����� RTS (�� ������������)
								| (0 << 15);  // ������������� ����� CTS (�� ������������)
						
	// ��������� �������� �� ��������� ���������� �� ����������� UART
  MDR_UART1->IMSC = (1 << 4)   // ���������� �� ���������
									| (1 << 5);  // ���������� �� �����������
							
	// ���������� ���������� ���������� �� ����������� UART
	NVIC_SetPriority(UART1_IRQn, 1);
	
	// ���������� ��������� ���������� �� ����������� UART
	NVIC_EnableIRQ(UART1_IRQn);
}

// ����������� �������� � ����������� ���������
void LCD_PutSymbolModified(uint8_t symbol)
{
  static uint8_t x, y;  // ���������� �������

  // ��������� ������
  switch(symbol)
  {
    // ��������� �������� �������� (Backspace)
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

    // ��������� �������� �������� �� ����� ������ (Enter)
    case 0x0D:
       LCD_PutSymbol('_', x, y);
      x = 0;
      y++;
      if (y > 7)
        y = 0;
      LCD_PutSymbol('_', x, y);
      break;

    // ��������� ��������
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

// ����� �������� �������� � ���������� �� �� ������� ���������� �����
void Thread_Typewriter(void *argument)
{
  // ���������� ��� �������� ���������� ������
  uint8_t data;
	char* string = "Ready to recieve!";

	
  // ����������� ��������� �� �������	
  LCD_PutSymbol('_', 0, 0);
	

	PrintConstText (string, 7, 25);	
	
	
	
  // �������� ����
  while(1)
  {

		
    // �������� ������ ������
    osEventFlagsWait(EventId_UART, EVENT_DATA_RECEIVED, osFlagsWaitAny, osWaitForever);
    // ���������� �������� ������
    data = MDR_UART1->DR;
		
    // ����������� ��������� ������� �� �������
    LCD_PutSymbolModified(data);

    // �������� ������ � �������� �����������
    MDR_UART1->DR = data;

    // �������� ���������� �������� ������
    osEventFlagsWait(EventId_UART, EVENT_DATA_TRANSMITTED, osFlagsWaitAny, osWaitForever);
		
  }
}

// ��������� ���������� ���������� ������ UART
void UART1_IRQHandler(void)
{
  // ���� ��������� ���������� �� ���������...
  if (MDR_UART1->RIS & 0x10)
  {
    // ��������� ����� ���������� ������ ������
    osEventFlagsSet(EventId_UART, EVENT_DATA_RECEIVED);

    // ����� ������� �� ��������� ����������
    MDR_UART1->ICR |= (1 << 4);
  }

  // ���� ��������� ���������� �� �����������...
  if (MDR_UART1->RIS & 0x20)
  {
    // ��������� ����� ���������� �������� ������
    osEventFlagsSet(EventId_UART, EVENT_DATA_TRANSMITTED);

    // ����� ������� �� ��������� ����������
    MDR_UART1->ICR |= (1 << 5);
  }
}
