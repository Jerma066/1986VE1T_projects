#include "opora.h"
#include "config.h"
#include "LCD.h"
#include "LCD_b.h"

int main()
{
	char* string = "Hello Mikhail!";
	int k = 79;
	char message[16];
	
	LCDInit();
	ClearLCD();
	
	//PrintSymbol('H', 0);
	PrintConstText (string, 1, 0);
	//PrintText ("Number n = % parts", k, 2, 0);
	
	//sprintf(message, "Number k = %d parts", k);
	//PrintConstText (message, 3, 0);
	
}
