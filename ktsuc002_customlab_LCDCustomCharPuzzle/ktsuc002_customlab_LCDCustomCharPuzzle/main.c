/*
 * ktsuc002_customlab_LCDCustomCharPuzzle.c
 *
 * Created: 5/23/2019 7:56:31 PM
 * Author : Kris Tsuchiyama
 */ 

#include <avr/io.h>
#include "bit.h"
#include "timer.h"
#include "io.c"
#include "io.h"

int main()
{
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x03; PORTA = 0xFC;
	LCD_init();
	unsigned char Character1[8] = {0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00};
	unsigned char Character2[8] = {0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x00};
	LCD_BuildChar(0,Character1);
	LCD_BuildChar(1,Character2);
	LCD_DisplayString(1,"CUSTOM CHAR TEST");
	LCD_Cursor(17);
	LCD_WriteData(0);
	LCD_Cursor(18);
	LCD_WriteData(1);
}

