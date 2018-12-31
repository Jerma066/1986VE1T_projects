#include "config.h"
#include "LCD.h"
#include "LCDGraphic.h"
#include "generic.h"

int main()
{
	
	ClkConfig();
	LCDInit();
	ClearLCD();

	DrawTriangle(30,59, 97, 59, 64, 1);
	DrawVerticalLine(64, 59, 1);
	DrawCircle(64, 39, 20);
			
}
