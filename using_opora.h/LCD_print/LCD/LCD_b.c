
#ifndef LCDB_C
#define LCDB_C

/************************************************
*** ������� ��� ������ � �������� ���� ��-12864�
***	������� �������� ������
************************************************/

#include "font.h"
#include "LCD.h"
#include "LCD_b.h"

//--------------------------------------------------------------------------------------
//������� ��� ���������� ����� ������ ��������
//			 				���������:	Str - ��������� �� ������, ������ ������� ���������� ������
//������������ �������: ���-�� ����, ���������� �������
//--------------------------------------------------------------------------------------
uint32_t StrLen(char* Str)
{
	char* ptr;
	uint32_t Len;

	ptr = Str;
	Len = 0;
	while(*(ptr++)!=0x00)	Len++;

	return Len;
}


//--------------------------------------------------------------------------------------
//������� ��� ���������� ������� �� ������� ����� ���������� �����
//			 				  ���������:	Str - ��������� �� ������, � �������  
//							  ��������� % - ����������� ������� ����� � ������ 
//������������ �������: ���-�� ����, ���������� �������
//--------------------------------------------------------------------------------------
uint32_t FindPosition(char* Str, uint32_t Length)
{
	uint32_t LenWord=0;
	while(LenWord++ < Length)
		if(Str[LenWord]=='%'){return LenWord;};
	
} 


//--------------------------------------------------------------------------------------
//������� ��� �������� ����� Val � ������ ��������, �� ������� ��������� MyStr
//���������:	MyStr - ��������� �� ������, � ������� ����� ������� ���������
//	 				  (������ ������ ��������� 32 �������)
//     				Value - ��������, ������� ���������� ���������� �� ������
//	   				Len - ���������, �� �������� ����� ���������� ���-�� ��������� �
//            ������� (�� ������� ������������ 0)
//������������ �������� ���
//--------------------------------------------------------------------------------------
void DigitalToChar(uint8_t* MyStr, uint32_t Val, uint32_t* Len)
{
	uint32_t f,i,j,non=0;
	uint8_t Arr[33];

	
	for(i=0; i<10; i++)
	{
		
		f = Val % 10;
		Arr[i]=0x30+f;
		Val=Val / 10;
		
		if(Val==0)	//����������� ������ ����
		{
			non=i;
			break;
		}
	}

//�������� ����������� �������������� ������ ��������
		if(non==0)
		{
			MyStr[0] = Arr[0];
			MyStr[1] = 0;
			*Len = 1;
			return;
		}
		else
		{
			*Len = 0;
			j = non;
			for(i=0; i<=non; i++)
			{
				MyStr[i] = Arr[j--];
				(*Len)++;
			}
			MyStr[i] = 0;
			return;
		}
	}

	
//-------------------------------------------------------------------------------------------------------
//������� ��� ����������� ������ ������� �� ������ �������� 6�8 �������� (6 - ������, 8 - ������)
//���������:	Symbol - ������, ������� ���� ����������
//						Crystal - �������� LCD, �� ������� ���� ���������� ������
// 						Adress - ������� �� ����������� 
//-------------------------------------------------------------------------------------------------------
void PrintSymbol(uint8_t Symbol, uint8_t Address)
{
	uint32_t Offset= (Symbol - FONT_MIN_CODE)* FONT_WIDTH;
	
		for(int i = Offset; i< (Offset+6); i++) 
		 {
				 Address ++;
			   WriteByte(Font_6x8_Data[i],Address);
		 }		

}


//-------------------------------------------------------------------------------------------------------
//������� ��� ����������� ����������� ������ �� ������
//���������:	String - ��������� �� ����������� ������, ������� ���� �������
//						Page - �������� ��� LCD, �� ������� ��������� ���������� ����� (����� ������)
//						Address - �����, � �������� ���������� �������� ����� �� LCD (������� �� ����������� )
// 						������������ �������� ���
//� ������ ��������� ���� ������ ���� 0�00!
//-------------------------------------------------------------------------------------------------------
void PrintConstText(char* String, uint8_t Page, uint8_t Address)
{
//������������ ���������� ������� ������!
	int LenWord, Length=0;

	ClearPage(Page,First);
	ClearPage(Page,Second);
	
	SetPage(Page,First);
	SetPage(Page,Second);


	Length = StrLen(String);
	for(LenWord=0;LenWord<Length;LenWord++)
	{	
		Address+=6;
		PrintSymbol(String[LenWord],(Address - 6));
  }
}

//--------------------------------------------------------------------------------------------------
//������� ��� ����������� ������ � ������� ����� �� ������ (��������� ����� ����������� �����)
//���������:	Text - ��������� �� ������, ������� ���� �������
//						Value - ��������, ������� ���������� ���������� �� ������
//					  Page - �������� ��� LCD, �� ������� ��������� ���������� �����
//					  Address - �����, � �������� ���������� �������� ����� �� LCD 
//						Method - ������ ����������� ������ �� ������ (�� ����� ���� ��� �� ������ ����)
//						ntn - ������� ���������, � ������� �������� �������� ����������
//������������ �������� ���
//� ������ ��������� ���� ������ ���� 0�00!
//--------------------------------------------------------------------------------------------------
void PrintText(char* String, uint32_t Value, uint8_t Page, uint8_t Address)
{
//������������ ���������� ������� ������!
	uint32_t LenWord=0, Length=0, PositionForInsert=0, Temp, ValNumOfChar, ptr;
	uint8_t  CharVal[33];
	char* ms, NewStr[21];
	
  ms = String;
	
//��������� ����� ������
	Length = StrLen(String);
	if(Length>21) {Length = 21;} 

//��������� ����� ������� �������� Value	
	PositionForInsert = FindPosition(String, Length);
	
//��������� �������� Value � ������ ��������
	DigitalToChar(&CharVal[0],Value,&ValNumOfChar);
	
//��������� ����� ������ �� �������
	for(Temp=0; Temp<PositionForInsert; Temp++)	
					NewStr[Temp] = ms[Temp];
	
  if((ValNumOfChar+Temp)>21)
  {
	  do
			 {
				 ValNumOfChar--;
			 } while ((ValNumOfChar+Temp > 21)&&(LenWord!=0));
  }	

 ValNumOfChar+=Temp;
 for(LenWord=0;Temp<ValNumOfChar;Temp++)	NewStr[Temp] = CharVal[LenWord++];

//���������� ��, ��� �������� ����� �������
	LenWord = Length - (PositionForInsert + 1);
	PositionForInsert++;
	if((Temp < 21)&&(LenWord!=0))
		do
			{
				Temp++;
				LenWord--;
				NewStr[Temp-1] = ms[PositionForInsert++];
			} while ((Temp < 21)&&(LenWord!=0));
			
	PrintConstText(NewStr, Page, Address);
}


#endif
