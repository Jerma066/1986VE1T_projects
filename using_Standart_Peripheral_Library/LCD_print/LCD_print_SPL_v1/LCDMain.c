#include "LCD.h"
#include "LCD_b.h"
#include "generic.h"

int main()
{
	char* string = "Hello !";
	int k = 79;
	char message[16];
	
	LCDInit();
	ClearLCD();
	
	PrintSymbol('H', 0);
	PrintConstText (string, 1, 0);
	PrintText ("Number n = % parts", k, 2, 0);
	
	sprintf(message, "Number k = %d parts", k);
	PrintConstText (message, 3, 0);
	
	LCD_PutSymbol('W', 54, 4);
	LCD_PutSymbol('I', 54, 5);
	LCD_PutSymbol('N', 54, 6);
}
