/************************************************************************/
/* Header File for 1.8TFT SPI 128*160 v1.1                              */
/* Modfied by Jonas Sorgenfrei										    */
/************************************************************************/

/************************************************************************/
/* Global Defines                                                       */
/************************************************************************/
//#define F_CPU 16000000L // run CPU at 16 MHz
#define LED 7 // Boarduino LED on PB5
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include "colors.h"


//Typedefs
typedef uint8_t byte;
typedef int8_t sbyte;

//Global Variables
const byte FONT_CHARS[96][5] PROGMEM = {
	{
		0x00,
		0x00,
		0x00,
		0x00,
		0x00
	},
	// (space)
	{
		0x00,
		0x00,
		0x5F,
		0x00,
		0x00
	},
	// !
	{
		0x00,
		0x07,
		0x00,
		0x07,
		0x00
	},
	// "
	{
		0x14,
		0x7F,
		0x14,
		0x7F,
		0x14
	},
	// #
	{
		0x24,
		0x2A,
		0x7F,
		0x2A,
		0x12
	},
	// $
	{
		0x23,
		0x13,
		0x08,
		0x64,
		0x62
	},
	// %
	{
		0x36,
		0x49,
		0x55,
		0x22,
		0x50
	},
	// &
	{
		0x00
		,
		0x05,
		0x03,
		0x00,
		0x00
	},
	// '
	{
		0x00,
		0x1C,
		0x22,
		0x41,
		0x00
	},
	// (
	{
		0x00,
		0x41,
		0x22,
		0x1C,
		0x00
	},
	// )
	{
		0x08,
		0x2A,
		0x1C,
		0x2A,
		0x08
	},
	// *
	{
		0x08,
		0x08,
		0x3E,
		0x08,
		0x08
	},
	// +
	{
		0x00,
		0x50,
		0x30,
		0x00,
		0x00
	},
	// ,
	{
		0x08,
		0x08,
		0x08,
		0x08,
		0x08
	},
	//	-
	{
		0x00,
		0x60,
		0x60,
		0x00,
		0x00
	},
	// .
	{
		0x20,
		0x10,
		0x08,
		0x04,
		0x02
	},
	// /
	{
		0x3E,
		0x51,
		0x49,
		0x45,
		0x3E
	},
	// 0
	{
		0x00,
		0x42,
		0x7F,
		0x40,
		0x00
	},
	// 1
	{
		0x42,
		0x61,
		0x51,
		0x49,
		0x46
	},
	// 2
	{
		0x21,
		0x41,
		0x45,
		0x4B,
		0x31
	},
	// 3
	{
		0x18,
		0x14,
		0x12,
		0x7F,
		0x10
	},
	// 4
	{
		0x27,
		0x45,
		0x45,
		0x45,
		0x39
	},
	// 5
	{
		0x3C,
		0x4A,
		0x49,
		0x49,
		0x30
	},
	// 6
	{
		0x01,
		0x71,
		0x09,
		0x05,
		0x03
	},
	// 7
	{
		0x36,
		0x49,
		0x49,
		0x49,
		0x36
	},
	// 8
	{
		0x06,
		0x49,
		0x49,
		0x29,
		0x1E
	},
	// 9
	{
		0x00,
		0x36,
		0x36,
		0x00,
		0x00
	},
	// :
	{
		0x00,
		0x56,
		0x36,
		0x00,
		0x00
	},
	// ;
	{
		0x00,
		0x08,
		0x14,
		0x22,
		0x41
	},
	// <
	{
		0x14,
		0x14,
		0x14,
		0x14,
		0x14
	},
	// =
	{
		0x41,
		0x22,
		0x14,
		0x08,
		0x00
	},
	// 	>
	{
		0x02,
		0x01,
		0x51,
		0x09,
		0x06
	},
	// ?
	{
		0x32,
		0x49,
		0x79,
		0x41,
		0x3E
	},
	// @
	{
		0x7E,
		0x11,
		0x11,
		0x11,
		0x7E
	},
	// A
	{
		0x7F,
		0x49,
		0x49,
		0x49,
		0x36
	},
	// B
	{
		0x3E,
		0x41,
		0x41,
		0x41,
		0x22
	},
	// C
	{
		0x7F,
		0x41,
		0x41,
		0x22,
		0x1C
	},
	// D
	{
		0x7F,
		0x49,
		0x49,
		0x49,
		0x41
	},
	// E
	{
		0x7F,
		0x09,
		0x09,
		0x01,
		0x01
	},
	// F
	{
		0x3E,
		0x41,
		0x41,
		0x51,
		0x32
	},
	// G
	{
		0x7F,
		0x08,
		0x08,
		0x08,
		0x7F
	},
	// H
	{
		0x00,
		0x41,
		0x7F,
		0x41,
		0x00
	},
	// I
	{
		0x20,
		0x40,
		0x41,
		0x3F,
		0x01
	},
	// J
	{
		0x7F,
		0x08,
		0x14,
		0x22,
		0x41
	},
	// K
	{
		0x7F,
		0x40,
		0x40,
		0x40,
		0x40
	},
	// L
	{
		0x7F,
		0x02,
		0x04,
		0x02,
		0x7F
	},
	// M
	{
		0x7F,
		0x04,
		0x08,
		0x10,
		0x7F
	},
	// N
	{
		0x3E,
		0x41,
		0x41,
		0x41,
		0x3E
	},
	// O
	{
		0x7F,
		0x09,
		0x09,
		0x09,
		0x06
	},
	// P
	{
		0x3E,
		0x41,
		0x51,
		0x21,
		0x5E
	},
	// Q
	{
		0x7F,
		0x09,
		0x19,
		0x29,
		0x46
	},
	// R
	{
		0x46,
		0x49,
		0x49,
		0x49,
		0x31
	},
	// S
	{
		0x01,
		0x01,
		0x7F,
		0x01,
		0x01
	},
	// T
	{
		0x3F,
		0x40,
		0x40,
		0x40,
		0x3F
	},
	// U
	{
		0x1F,
		0x20,
		0x40,
		0x20,
		0x1F
	},
	// V
	{
		0x7F,
		0x20,
		0x18,
		0x20,
		0x7F
	},
	// W
	{
		0x63,
		0x14,
		0x08,
		0x14,
		0x63
	},
	// X
	{
		0x03,
		0x04,
		0x78,
		0x04,
		0x03
	},
	// Y
	{
		0x61,
		0x51,
		0x49,
		0x45,
		0x43
	},
	// Z
	{
		0x00,
		0x00,
		0x7F,
		0x41,
		0x41
	},
	// [
	{
		0x02,
		0x04,
		0x08,
		0x10,
		0x20
	},
	// "\"
	{
		0x41,
		0x41,
		0x7F,
		0x00,
		0x00
	},
	// ]
	{
		0x04,
		0x02,
		0x01,
		0x02,
		0x04
	},
	// ^
	{
		0x40,
		0x40,
		0x40,
		0x40,
		0x40
	},
	// _
	{
		0x00,
		0x01,
		0x02,
		0x04,
		0x00
	},
	// `
	{
		0x20,
		0x54,
		0x54,
		0x54,
		0x78
	},
	// a
	{
		0x7F,
		0x48,
		0x44,
		0x44,
		0x38
	},
	// b
	{
		0x38,
		0x44,
		0x44,
		0x44,
		0x20
	},
	// c
	{
		0x38,
		0x44,
		0x44,
		0x48,
		0x7F
	},
	// d
	{
		0x38,
		0x54,
		0x54,
		0x54,
		0x18
	},
	// e
	{
		0x08,
		0x7E,
		0x09,
		0x01,
		0x02
	},
	// f
	{
		0x08,
		0x14,
		0x54,
		0x54,
		0x3C
	},
	// g
	{
		0x7F
		,
		0x08,
		0x04,
		0x04,
		0x78
	},
	// h
	{
		0x00,
		0x44,
		0x7D,
		0x40,
		0x00
	},
	// i
	{
		0x20,
		0x40,
		0x44,
		0x3D,
		0x00
	},
	// j
	{
		0x00,
		0x7F,
		0x10,
		0x28,
		0x44
	},
	// k
	{
		0x00,
		0x41,
		0x7F,
		0x40,
		0x00
	},
	// l
	{
		0x7C,
		0x04,
		0x18,
		0x04,
		0x78
	},
	// m
	{
		0x7C,
		0x08,
		0x04,
		0x04,
		0x78
	},
	// n
	{
		0x38,
		0x44,
		0x44,
		0x44,
		0x38
	},
	// o
	{
		0x7C,
		0x14,
		0x14,
		0x14,
		0x08
	},
	// p
	{
		0x08,
		0x14,
		0x14,
		0x18,
		0x7C
	},
	// q
	{
		0x7C,
		0x08,
		0x04,
		0x04,
		0x08
	},
	// r
	{
		0x48,
		0x54,
		0x54,
		0x54,
		0x20
	},
	// s
	{
		0x04,
		0x3F,
		0x44,
		0x40,
		0x20
	},
	// t
	{
		0x3C,
		0x40,
		0x40,
		0x20,
		0x7C
	},
	// u
	{
		0x1C,
		0x20,
		0x40,
		0x20,
		0x1C
	},
	// v
	{
		0x3C,
		0x40,
		0x30,
		0x40,
		0x3C
	},
	// w
	{
		0x44,
		0x28,
		0x10,
		0x28,
		0x44
	},
	// x
	{
		0x0C,
		0x50,
		0x50,
		0x50,
		0x3C
	},
	// y
	{
		0x44,
		0x64,
		0x54,
		0x4C,
		0x44
	},
	// z
	{
		0x00,
		0x08,
		0x36,
		0x41,
		0x00
	},
	// {
	{
		0x00,
		0x00,
		0x7F,
		0x00,
		0x00
	},
	// |
	{
		0x00,
		0x41,
		0x36,
		0x08,
		0x00
	},
	// }
	{
		0x08,
		0x08,
		0x2A,
		0x1C,
		0x08
	},
	// ->
	{
		0x08,
		0x1C,
		0x2A,
		0x08,
		0x08
	},
	//	<-
};

/************************************************************************/
/* MISC ROUNTINES                                                       */
/************************************************************************/
//MISC Routines
void TFT_SetupPorts(void)
{
	DDRB = 0xFF;
	SetBit(PORTB,0);
	/* Set MOSI and SCK output, all others input */
	//DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
}
void TFT_msDelay(int delay)
{
	for(int i=0;i<delay;i++)
	_delay_ms(1);
}
unsigned long intsqrt(unsigned long val){
	unsigned long mulMask = 0x0008000;
	unsigned long retVal = 0;
	
	if(val > 0){
		while(mulMask != 0)
		{
			retVal |= mulMask;
			if((retVal*retVal) > val)
			retVal &= ~mulMask;
			mulMask >>= 1;
		}
	}
	return retVal;
}

/************************************************************************/
/* SPI ROUTINES                                                         */
/************************************************************************/
void TFT_OpenSPI(void)
{
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	//SPCR = 0x50;		//SPI enabled as Master Mode0 at 4 MHz
	//SetBit(SPSR,SPI2X);	//double the SPI rate: 4-->8 MHz
}
void TFT_CloseSPI(void)
{
	SPCR = 0x00;		//clear SPI enable bit
}

byte TFT_Xfer(byte data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

/************************************************************************/
/* ST7735 Routines                                                      */
/************************************************************************/
#define SWRESET 0x01 // software reset
#define SLPOUT 0x11 // sleep out
#define DISPOFF 0x28 // display off
#define DISPON 0x29 // display on
#define CASET 0x2A // column address set
#define RASET 0x2B // row address set
#define RAMWR 0x2C // RAM write
#define MADCTL 0x36 // axis control
#define COLMOD 0x3A // color mode

// 1.8" TFT display constants
#define XSIZE 128
#define YSIZE 160
#define XMAX XSIZE-1
#define YMAX YSIZE-1

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x0400
#define LIME 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void TFT_WriteCmd(byte cmd)
{
	ClearBit(PORTB, 3);
	TFT_Xfer(cmd);
	SetBit(PORTB,3);
}

void TFT_WriteByte(byte b)
{
	TFT_Xfer(b);
}

void TFT_WriteWord(int w)
{
	TFT_Xfer(w>>8);
	TFT_Xfer(w & 0xFF);
}

void TFT_Write888 (long data, int count)
{
	byte red = data>>16;
	byte green = (data>>8) & 0xFF;
	byte blue = data & 0xFF;
	for(;count>0;count--)
	{
		TFT_WriteByte(red);
		TFT_WriteByte(green);
		TFT_WriteByte(blue);
	}
}

void TFT_Write565(int data, unsigned int count){
	//send 16-bit pixel data to the controller
	// note: inlined spi xfer for optimization
	TFT_WriteCmd(RAMWR);
	for(;count>0;count--)
	{
		SPDR = (data>>8);	//write hi byte
		while(!(SPSR & (1<<SPIF))); //wait for transfer to complete
		SPDR = (data & 0xFF); //write lo byte
		while(!(SPSR & (1<<SPIF))); //wait for transfert to complete
	}
}

void TFT_HardwareReset(void)
{
	ClearBit(PORTB, 2);
	TFT_msDelay(1);
	SetBit(PORTB,2);
	TFT_msDelay(150);
}

void TFT_InitDisplay(void)
{
	TFT_HardwareReset();
	TFT_WriteCmd(SLPOUT);
	TFT_msDelay(150);
	TFT_WriteCmd(COLMOD);
	TFT_WriteByte(0x05);
	TFT_WriteCmd(DISPON);
}

void TFT_SetAddrWindow(byte x0, byte y0, byte x1, byte y1){
	TFT_WriteCmd(CASET);
	TFT_WriteWord(x0);
	TFT_WriteWord(x1);
	TFT_WriteCmd(RASET);
	TFT_WriteWord(y0);
	TFT_WriteWord(y1);
}

void TFT_ClearScreen(void){
	TFT_SetAddrWindow(0,0,XMAX,YMAX);
	TFT_WriteCmd(RAMWR);
	for(unsigned int i = 40960; i>0;--i)
	{
		SPDR = 0;
		while(!(SPSR & (1<<SPIF)));
	}
}

/************************************************************************/
/* Graphic Routines                                                     */
/************************************************************************/
int TFT_ColorToInt(color col){
	int color = 0x0000;
	color |= 0x001F * col.B / 255;
	color |= (0x003F * col.G / 255)<<5;
	color |= (0x001F * col.R / 255)<<11;
	return color;
};

void TFT_DrawPixel (byte x, byte y, int color)
{
	TFT_SetAddrWindow(x,y,x,y);	//set active region = 1 pixel
	TFT_Write565(color,1);		//send color for this pixel
}

void TFT_HLine (byte x0, byte x1, byte y, int color){
	byte width = x1-x0+1;
	TFT_SetAddrWindow(x0,y,x1,y);
	TFT_Write565(color,width);
}

void TFT_VLine(byte x, byte y0, byte y1, int color){
	byte height = y1-y0+1;
	TFT_SetAddrWindow(x,y0,x,y1);
	TFT_Write565(color,height);
}

void TFT_Line (int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;
	for(;;)
	{
		TFT_DrawPixel(x0,y0,color);
		if(x0==x1 && y0==y1)
		break;
		e2 = err;
		if(e2 >-dx) {err -= dy; x0 += sx;}
		if(e2 < dy) {err += dx; y0 += sy;}
	}
}

void TFT_DrawRect(byte x0, byte y0, byte x1, byte y1, int color)
{
	TFT_HLine(x0, x1, y0, color);
	TFT_HLine(x0, x1, y1, color);
	TFT_VLine(x0, y0, y1, color);
	TFT_VLine(x1, y0, y1, color);
}

void TFT_FillRect(byte x0, byte y0, byte x1, byte y1, int color){
	byte width = x1-x0+1;
	byte height = y1-y0+1;
	TFT_SetAddrWindow(x0,y0,x1,y1);
	TFT_Write565(color,width*height);
}

void TFT_CircleQuadrant(byte xPos, byte yPos, byte radius, byte quad, int color)
{
	int x, xEnd = (707*radius)/1000 +1;
	for(x=0; x<xEnd; x++){
		byte y = intsqrt(radius*radius - x*x);
		if(quad & 0x01)
		{
			TFT_DrawPixel(xPos+x,yPos+y,color);	//lower right
			TFT_DrawPixel(xPos+y,yPos+x,color);
		}
		if(quad & 0x02)
		{
			TFT_DrawPixel(xPos+x,yPos-y,color);
			TFT_DrawPixel(xPos+y,yPos-x,color);
		}
		if(quad & 0x04)
		{
			TFT_DrawPixel(xPos-x,yPos+y,color);
			TFT_DrawPixel(xPos-y,yPos+x,color);
		}
		if(quad & 0x08){
			TFT_DrawPixel(xPos-x, yPos-y,color);
			TFT_DrawPixel(xPos-y, yPos-y,color);
		}
	}
}

void TFT_Circle (byte xPos, byte yPos, byte radius, int color)
{
	TFT_CircleQuadrant(xPos, yPos, radius, 0x0F, color); //do all 4 quadrants
}

void TFT_RoundRect(byte x0, byte y0, byte x1, byte y1, byte r, int color)
{
	TFT_HLine(x0+r,x1-r,y0,color);
	TFT_HLine(x0+r,x1-r,y1,color);
	TFT_VLine(x0,y0+r,y1-r,color);
	TFT_VLine(x1,y0+r,y1-r,color);
	TFT_CircleQuadrant(x0+r,y0+r,r,8,color);
	TFT_CircleQuadrant(x1-r,y0+r,r,2,color);
	TFT_CircleQuadrant(x0+r,y1-r,r,4,color);
	TFT_CircleQuadrant(x1-r,y1-r,r,1,color);
}

void TFT_FillCircle(byte xPos, byte yPos, byte radius, int color)
{
	long r2 = radius * radius;
	for(int x=0; x<=radius; x++){
		byte y = intsqrt(r2-x*x);
		byte y0 = yPos-y;
		byte y1 = yPos+y;
		TFT_VLine(xPos+x,y0,y1,color);
		TFT_VLine(xPos-x,y0,y1,color);
	}
}

void TFT_Ellipse(int x0, int y0, int width, int height, int color)
{
	int a = width/2, b=height/2;
	int x = 0, y = b;
	long a2 = (long) a*a*2;
	long b2 = (long) b*b*2;
	long error = (long) a*a*b;
	long stopY=0, stopX=a2*b;
	while(stopY <= stopX)
	{
		TFT_DrawPixel(x0+x,y0+y,color);
		TFT_DrawPixel(x0+x,y0-y,color);
		TFT_DrawPixel(x0-x,y0+y,color);
		TFT_DrawPixel(x0-x,y0-y,color);
		x++;
		error -= b2 *(x-1);
		stopY += b2;
		if(error < 0){
			error += a2 * (y-1);
			y--;
			stopX -= a2;
		}
	}
	x=a; y=0; error = b*b*a;
	stopY = a*b2; stopX=0;
	while(stopY >= stopX)
	{
		TFT_DrawPixel(x0+x,y0+y,color);
		TFT_DrawPixel(x0+x,y0-y,color);
		TFT_DrawPixel(x0-x,y0+y,color);
		TFT_DrawPixel(x0-x,y0-y,color);
		y++;
		error -= a2*(y-1);
		stopX += a2;
		if(error < 0)
		{
			error += b2*(x-1);
			x--;
			stopY -= b2;
		}
	}
}

void TFT_FillEllipse(int xPos, int yPos, int width, int height, int color)
{
	int a=width/2, b=height/2;		// get x & y radii
	int x1, x0 = a, y = 1, dx = 0;
	long a2 = a*a, b2 = b*b;		// need longs: big numbers!
	long a2b2 = a2 * b2;
	TFT_HLine(xPos-a,xPos+a,yPos, color); // draw centerline
	while(y<=b){
		for(x1=x0-(dx-1); x1>0; x1--)
		if(b2*x1*x1 + a2*y*y <= a2b2)
		break;
		dx = x0-x1;
		x0 = x1;
		TFT_HLine(xPos-x0, xPos+x0,yPos+y,color);
		TFT_HLine(xPos-x0, xPos+x0,yPos-y,color);
		y += 1;
	}
}

/************************************************************************/
/* Text Routines                                                        */
/************************************************************************/
byte curX, curY;			//current x & y cursor Position
void TFT_GotoXY(byte x, byte y)
{
	curX = x;
	curY = y;
}

void TFT_GotoLine(byte y)
{
	curX = 0;
	curY = y;
}

void TFT_AdvanceCursor(void)
{
	curX++;
	if(curX>20)
	{
		curY++;
		curX = 0;
	}
	if(curY>19)
	curY = 0;
}

void TFT_setOrientation(int degrees){
	byte arg;
	switch(degrees)
	{
		case 90: arg = 0x60; break;
		case 180: arg = 0xC0; break;
		case 270: arg = 0xA0; break;
		default: arg = 0x00; break;
	}
	TFT_WriteCmd(MADCTL);
	TFT_WriteByte(arg);
}

void TFT_PutCh(char ch, byte x, byte y, int color){
	int pixel;
	byte row, col, bit, data, mask = 0x01;
	TFT_SetAddrWindow(x,y,x+4,y+6);
	TFT_WriteCmd(RAMWR);
	for(row=0; row<7;row++){
		for(col=0; col<5;col++){
			data = pgm_read_byte(&(FONT_CHARS[ch-32][col]));
			bit = data & mask;
			if(bit == 0) pixel = BLACK;
			else		pixel = color;
			TFT_WriteWord(pixel);
		}
		mask <<=1;
	}
}

void TFT_WriteChar(char ch, int color)
{
	TFT_PutCh(ch,curX*6, curY*8, color);
	TFT_AdvanceCursor();
}

void TFT_WriteString(char *text, int color)
{
	for(;*text;text++){
		TFT_WriteChar(*text, color);
	}
}

void TFT_WriteInt(int i)
{
	char str[8];
	itoa(i,str,10);
	TFT_WriteString(str,WHITE);
}

void TFT_WriteHex(int i)
{
	char str[8];
	itoa(i,str,16);
	TFT_WriteString(str, WHITE);
}

