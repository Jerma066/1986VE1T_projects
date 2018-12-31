
#ifndef __LCDGRAPHIC_H
#define __LCDGRAPHIC_H

#include <stdint.h>

void DrawLine(int, int, int, int);
void DrawCircle(int , int , int);
uint8_t BitSet(uint8_t);
uint8_t GetPage(uint8_t);
void PutPixel(uint8_t, uint8_t);
void DrawVerticalLine(uint8_t, uint8_t, uint8_t);
void DrawHorizontalLine(uint8_t, uint8_t, uint8_t);
void DrawRect(uint8_t, uint8_t, uint8_t, uint8_t);
void DrawTriangle(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);

	
#endif	//__LCDGRAPHIC_H
