
#include "opora.h"
#include "config.h"
#include "LCD.h"
#include "LCDGraphic.h"

void DrawWindow(uint8_t, uint8_t);
void DrawSnow(int, int);

int main()
{
	uint32_t i=0, j=0;
	ClkConfig();
	LCDInit();
	ClearLCD();

	DrawSnow(10,30);
	DrawSnow(20,40);
	DrawSnow(30,50);
	DrawSnow(40,62);	

	DrawSnow(10,50);
	DrawSnow(20,62);

	DrawSnow(10,10);
	DrawSnow(20,20);
	DrawSnow(30,30);
	DrawSnow(40,40);
	DrawSnow(50,50);
	DrawSnow(60,62);

	DrawSnow(30,10);
	DrawSnow(40,20);
	DrawSnow(50,30);
	DrawSnow(60,40);
	DrawSnow(70,50);
	DrawSnow(80,62);

	DrawSnow(50,10);
	DrawSnow(60,20);
	DrawSnow(70,30);
	DrawSnow(80,40);
	DrawSnow(90,50);
	DrawSnow(100,62);

	DrawSnow(70,10);
	DrawSnow(80,20);
	DrawSnow(90,30);
	DrawSnow(100,40);
	DrawSnow(110,50);
	DrawSnow(120,62);

	DrawSnow(90,10);
	DrawSnow(100,20);
	DrawSnow(110,30);
	DrawSnow(120,40);
	
	DrawSnow(110,10);
	DrawSnow(120,20);

	while(1)
	{
		for(i=0;i<1000000;i++);
		SetStartLine(j);
		j--;
	}
}

void DrawSnow(int X, int Y)
{
	DrawHorizontalLine(X-5,X+5,Y);
	DrawVerticalLine(X,Y-5,Y+5);

	DrawLine(X-4,Y-4,X+4,Y+4);
	DrawLine(X+4,Y-4,X-4,Y+4);
	
	DrawLine(X-2,Y-5,X,Y-3);
	DrawLine(X+2,Y-5,X,Y-3);

	DrawLine(X-2,Y+5,X,Y+3);
	DrawLine(X+2,Y+5,X,Y+3);

	DrawLine(X-5,Y-2,X-3,Y);
	DrawLine(X-5,Y+2,X-3,Y);

	DrawLine(X+3,Y,X+5,Y-2);
	DrawLine(X+3,Y,X+5,Y+2);
}
