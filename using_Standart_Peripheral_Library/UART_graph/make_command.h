
#ifndef MAKECOMMAND_H
#define MAKECOMMAND_H

/*******************************************************************
*** Фукнкции для формирования команды из данных полученнных по uart
*******************************************************************/

#include <stdint.h>
#include <string.h>


void DrawCircle (int , int , int );
void DrawLine (int, int, int, int);
void DrawRect (int, int, int, int);
void DrawTriangle(int, int, int, int, int, int);
void PutPixel(int, int);
void DestructStr (char*, int);
int Find_Position (char*, int, char);
void LCD_ExecCommand(char*, int*);
void PutSymbolInCommand(char*, char, int*);
void MakeCommand(char*); 
int MakeNumber (char*, int);

#endif
