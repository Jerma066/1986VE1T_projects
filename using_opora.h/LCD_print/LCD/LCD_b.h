
#ifndef LCDB_H
#define LCDB_H

#include "LCD.h"

// Стандартные библиотеки языка Си
#include <stdio.h>
#include <string.h>

typedef enum _notation_ {hex,dec,oct,bin} notation;

void ClearPage(uint8_t, _crystal);
void PrintSymbol(uint8_t, uint8_t);
void PrintConstText(char*, uint8_t, uint8_t);
void DigitalToChar(uint8_t*, uint32_t, uint32_t*);
void PrintText(char*, uint32_t,uint8_t, uint8_t);
uint32_t StrLen(char*);
uint32_t FindPosition (char*, uint32_t);

#endif	
