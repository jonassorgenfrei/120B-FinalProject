#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "io.h"

/*-------------------------------------------------------------------------*/

#define CONTROL_BUS PORTC	// port connected to pins 4 and 6 of LCD disp.
#define RS 2			// pin number of uC connected to pin 4 of LCD disp.
#define E 3			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {
	LCD_WriteCommand(0x02);
	LCD_WriteCommand(0x28);
	LCD_WriteCommand(0x0c);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x01);				 
}

void LCD_SetUpPorts(){
	DDRC = 0xFF; 
	PORTC = 0x00;
}

void LCD_WriteCommand (unsigned char Command) {
	CONTROL_BUS &= ~(1<<RS);// To write Commands
	char z1 = Command << 4;// To shift 4 bits
	Command &= 0b11110000; //masking lower 4 bits
	CONTROL_BUS = (CONTROL_BUS & 0x0F)|Command; // sending data to LCD
	
	CONTROL_BUS |= (1<<E);//send falling edge to send data to LCD
	_delay_ms(1);
	CONTROL_BUS &= ~(1<<3);
	_delay_ms(1);
	z1 &= 0b11110000;//Again masking lower 4 bits
	CONTROL_BUS = (CONTROL_BUS & 0x0F)|z1;//Sending Remaining Data to LCD
	
	CONTROL_BUS |= (1<<E);//send falling edge to send data to LCD
	delay_ms(1);
	CONTROL_BUS &= ~(1<<E);
	delay_ms(1);
	
}

void LCD_WriteData(unsigned char Data) {
	CONTROL_BUS |= (1 << RS);// To write Data
	char t1 = Data << 4;// To shift 4 bits
	Data &= 0b11110000;// for masking the lower 4 bits
	CONTROL_BUS = (CONTROL_BUS & 0x0F)|Data;//sending data

	CONTROL_BUS |= (1<<E); //send falling edge to send data to LCD
	delay_ms(1);
	CONTROL_BUS &= ~(1<<E);
	delay_ms(1);
	
	t1 &= 0b11110000;//Again masking lower 4 bits
	CONTROL_BUS = (CONTROL_BUS & 0x0F)|t1;// Sending remaining data
	
	CONTROL_BUS |= (1<<E);//send falling edge to send data to LCD
	delay_ms(1);
	CONTROL_BUS &= ~(1<<E);
	delay_ms(1);
	
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

static unsigned char LCD_APPEND_DisplayString( unsigned char column, const unsigned char* string) {
	   unsigned char c = column;
	   while(*string) {
		   LCD_Cursor(c++);
		   LCD_WriteData(*string++);
	   }
	   return c;
}

void LCD_printfRGB(char r, char g, char b){
	//LCD_ClearScreen();
	unsigned char pos = 1;
	unsigned char str1[4];
	sprintf(str1, "%03d",r);
	pos = LCD_APPEND_DisplayString(pos, "Color:          R");
	pos = LCD_APPEND_DisplayString(pos, str1);
	pos = LCD_APPEND_DisplayString(pos, "|G");
	unsigned char str2[4];
	sprintf(str2, "%03d",g);
	pos = LCD_APPEND_DisplayString(pos, str2);
	pos = LCD_APPEND_DisplayString(pos, "|B");
	unsigned char str3[4];
	sprintf(str3, "%03d",b);
	pos = LCD_APPEND_DisplayString(pos, str3);
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal
{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}
