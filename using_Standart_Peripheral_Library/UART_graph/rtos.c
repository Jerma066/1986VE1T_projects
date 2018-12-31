
// ����������� ������������� �����
#include "rtos.h"

// ������������� ������� �������
osEventFlagsId_t EventId_UART;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
	// �������� ������ �������������� ������
  static const osThreadAttr_t ThreadAttr_Typewriter =
  {
    .stack_size = 600  // ������ ����� (� ������)
  };
	
	// ������������� ���� ����
  osKernelInitialize();

	// �������� ������ �������� �������� � ���������� �� �� ������� ���������� �����
  osThreadNew(Thread_Typewriter, NULL, &ThreadAttr_Typewriter);
	
  // �������� ������� �������
  EventId_UART = osEventFlagsNew(NULL);	
	
  // ������ ���� ����
  osKernelStart();	
}
