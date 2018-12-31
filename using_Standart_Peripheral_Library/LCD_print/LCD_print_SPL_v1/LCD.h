#ifndef __LCD_H
#define	__LCD_H

#include "generic.h"
#include "font.h"

typedef enum _dspl {ON = 0x3F, OFF = 0x3E} _display;
typedef enum _crstl {First = 1, Second = 2} _crystal;

#define LCD_CMD1	0x68000000
#define LCD_CMD2	0x70000000

#define LCD_DATA1	0x6C000000
#define LCD_DATA2	0x74000000
	
	void PortsForLCD(void);															//������� ���������������� ������ �����/������ ��� ������ ������
	void SystemBusInit(void);														//������� ���������������� ������� ��������� ����
	void ResetLCD(void);																//������� ������ LCD
	void SetStartLine(uint32_t);												//������� ��������� ������ ������� LCD
	void SetPage(uint32_t, _crystal);										//������� ���������� �������� ��� LCD (������ ������� 8 ���)
	void SetAddress(uint32_t);													//������� ��������� ������ ��� ��� ������ ����������� (�������)
  void DisplayOnOff(_display);												//������� ���������/���������� LCD �������
	void __SysTickInit(void);														//������� ������������� ���������� �������	
	void Pause8us(void);																//������� ������� ����� 8 ���
	uint8_t ReadStatus(_crystal);												//������� ������ ������� � LCD
	void SystemBusInit(void);														//������� ���������������� ������� ��������� ����
	uint8_t ReadStatus(_crystal);												//������� ������ ������� LCD
	void WriteByte(uint8_t, uint8_t);										//������� ������ ����� � ������ ��� LCD
	uint8_t ReadByte(uint32_t, uint32_t);								//������� ������ ����� �� ������ ��� LCD
	void ClearLCD(void);																//������� ������� ������� LCD
	void ClearPage(uint8_t, _crystal);									//������� ��� ������� ����� �������� ������� LCD
	void ClearPageAtTo(uint32_t, uint32_t, uint32_t);		//������� ������� LCD ������ ������ � �������� ����� ��������
	void LCDInit(void);																	//������� ������������� LCD-������
  void EnableSysTick(void);														//������� ��� ���������� ������ ���������� ������� � ���������� �� ����
	void DisableSysTick(void);													//������� ��� ������� ������ ���������� ������� � ���������� �� ����
	void __SysTickInit(void);														//������� ������������� ���������� �������
	void SysTick_Handler(void);													//���������� ���������� �� ���������� �������
	void Pause8us(void);																//������� ������� ����� 8 ���


#endif
