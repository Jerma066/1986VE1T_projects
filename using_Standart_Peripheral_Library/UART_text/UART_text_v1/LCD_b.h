
#ifndef LCDB_H
#define LCDB_H

#include "LCD.h"

typedef enum _notation_ {hex,dec,oct,bin} notation;

void ClearPage(uint8_t, _crystal);
void PrintSymbol(uint8_t, uint8_t);
void PrintConstText(char*, uint8_t, uint8_t);
void DigitalToChar(uint8_t*, uint32_t, uint32_t*);
void PrintText(char*, uint32_t,uint8_t, uint8_t);
void LCD_PutSymbol (uint8_t, uint8_t, int8_t);
uint32_t StrLen(char*);
uint32_t FindPosition (char*, uint32_t);

#endif	
