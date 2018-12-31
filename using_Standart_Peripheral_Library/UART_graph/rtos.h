
#ifndef RTOS_H
#define RTOS_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ���������� ����� ���������� �������-��������� ��������������
#define EVENT_ADC_EOC 0x00000001 

// ������������� ������� �������
extern osEventFlagsId_t EventId_UART;

// ��������� �������
void RTOS_Init(void);  // ������������� ������������ ������� ��������� �������

#endif
