
#ifndef UART_H
#define UART_H

#include "link.h"

// ����������� ������������ ������ ������� �������

#define EVENT_DATA_RECEIVED    0x00000001
#define EVENT_DATA_TRANSMITTED 0x00000002


// ��������� �������� �������� ������
#define UART_SPEED 9600

// ��������� �������
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx);          // ��������������� ����������� UART
void UART_PortCfg(void);                       				 // ������������ ����� ��� ������ � ������������ UART (UART2)
void UART_Init(void);                          				 // ������������� ������ UART (UART2)
void Thread_Typewriter(void *argument);                // ����� �������� �������� � ���������� �� �� ������� ���������� �����

#endif
