/*
 * jsorg002_finalProject.c
 *
 * Created: 22.05.2018 09:40:43
 * Author : Jonas
 */ 

//Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include "task.h"
#include "bit.h"
#include "timer.h"
#include "io.h"
#include "tft.h"
#include "keypad.h"
#include "pwm.h"
#include "colors.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#define DEGTORAD(x) ((x)*M_PI/180)

#define DISTX 4
#define DISTY 20

/************************************************************************/
/* GLOBAL VARIABLES                                                     */
/************************************************************************/
color mainColor = {255,255,255};
char newInput = 0;
char valueChanged = 0;
typedef struct Point {
	unsigned char x;
	unsigned char y;	
} Point;
Point posOnScreen = {64,80};

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}

//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
    /*Tasks should have members that include: state, period,
        a measurement of elapsed time, and a function pointer.*/
    signed char state; //Task's current state
    unsigned long int period; //Task period
    unsigned long int elapsedTime; //Time elapsed since last task tick
    int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

/************************************************************************/
/* Helper Functions                                                     */
/************************************************************************/

color getColor(int x, int y, int radius, double r){
	
	double phi = atan2(y,x);
	unsigned long hue = (unsigned long)((phi + M_PI) / (2.0 * M_PI) * 360.0);
	unsigned char saturation = (unsigned char)(r / (double)radius * 255.0);
	unsigned char value = 255;
	printf("%c", saturation);
	printf("%c", hue);
	return hsvtorgb(hue, saturation, value);
}

Point getPosition(hsv Color, int radius){
	double saturation = (double)Color.saturation/255.0;
	Point point = {0,0};
		
	if(Color.hue >= 0 && Color.hue <= 90){
		point.x = radius - cos(DEGTORAD(Color.hue))*(saturation*radius);
		point.y = radius - sin(DEGTORAD(Color.hue))*(saturation*radius);
	} else if(Color.hue > 90 && Color.hue <= 180){
		point.x = radius + cos(DEGTORAD(180-Color.hue))*saturation*radius;
		point.y = radius - sin(DEGTORAD(180-Color.hue))*saturation*radius;
	} else if(Color.hue > 180 && Color.hue <= 270) {
		point.x = radius + sin(DEGTORAD(270-Color.hue))*saturation*radius;
		point.y = radius + cos(DEGTORAD(270-Color.hue))*saturation*radius;
	} else {
		point.x = radius - cos(DEGTORAD(360-Color.hue))*saturation*radius;
		point.y = radius + sin(DEGTORAD(360-Color.hue))*saturation*radius;
	}
	point.x += DISTX;
	point.y += DISTY;
	return point;
}

void drawCircle(void) {
	int radius = 60;
	for(int x = -radius; x < radius; x++){
		for(int y = -radius; y < radius; y++){
			double r = sqrt(x*x + y*y);
			if(r < radius){
				byte adjustedX = x + radius; // convert x from [-50, 50] to [0, 100] (the coordinates of the image data array)
				byte adjustedY = y + radius; // convert y from [-50, 50] to [0, 100] (the coordinates of the image data array)
				TFT_DrawPixel(adjustedX+DISTX,adjustedY+DISTY,TFT_ColorToInt(getColor(x,y,radius, r)));
			}
		}
	}
}

/************************************************************************/
/* State Machines                                                       */
/************************************************************************/
enum SM1_STATES {SM1_INIT, SM1_STATE1};

int SMTICK1(int state)
{
	switch(state){
		case SM1_INIT:
			state = SM1_STATE1;
			LCD_DisplayString(1, "SM1 Started");
			break;
		case SM1_STATE1:
			state = SM1_STATE1;
			break;
		default:
			state = SM1_INIT;
			break;
	}
	switch(state){
		case SM1_STATE1:
			if(!newInput){
				LCD_printfRGB(mainColor.R, mainColor.G, mainColor.B);
			}
			break;
	}
	
	return state;
};

enum SM2_STATES {SM2_INIT, SM2_LEDS};
int SMTICK2(int state){
	switch(state){
		case SM2_INIT:
			state = SM2_LEDS;
			LCD_DisplayString(1, "SM2 Started");
			break;
		case SM2_LEDS:
			state = SM2_LEDS;
			break;
		default:
			state = SM2_INIT;
			break;
	}	
	switch(state) {
		case SM2_LEDS:
			
			PORTD = (PORTD & 0xC0) | (mainColor.R & 0x3F);
			PORTC = (PORTD & 0xFC) | (mainColor.R >>6);		
			if(mainColor.G > 0){
				pwm_enable(1);
				pwm_dutycycle(1, mainColor.G);
			} else {
				pwm_disable(1);
			}
			if(mainColor.B > 0){
				pwm_enable(2);
				pwm_dutycycle(2, mainColor.B);
			} else {
				pwm_disable(2);
			}
			break;
	}
	
	return state;
};

enum SM3_STATES {SM3_INIT, SM3_WAIT, SM3_UPDATE};
int SMTICK3(int state){
	switch(state){
		case SM3_INIT:
			state = SM3_WAIT;
			LCD_DisplayString(1, "SM3 Started");
			drawCircle();
			TFT_FillCircle(posOnScreen.x,posOnScreen.y,5,BLACK);
			break;
		case SM3_WAIT:
			if(valueChanged){
				state = SM3_UPDATE;
			} else {
				state = SM3_WAIT;
			}
			break;
		case SM3_UPDATE:
			LCD_DisplayString(1, "SAVING");
			drawCircle();
			posOnScreen = getPosition(rgbtohsv(mainColor),60);
			if(mainColor.R != 0 || mainColor.G != 0 || mainColor.B != 0){
				TFT_FillCircle(posOnScreen.x,posOnScreen.y,5,BLACK);
			}
			valueChanged = 0;
			state = SM3_WAIT;
			break;
		default:
			state = SM3_INIT;
			break;
	}
	switch(state) {
		case SM3_WAIT:
			break;
	}
	return state;
};

enum ColorInput {INPUT_RED, INPUT_GREEN, INPUT_BLUE, INPUTDONE};
color tempColor;
unsigned long value = 0;
unsigned char inputs = 0;
unsigned char inputState = INPUT_RED;
unsigned char holdTime = 0;
unsigned char cursorPosition = 18;
unsigned char lastInput = '\0';
enum SM4_STATES {SM4_INIT, SM4_WAIT, SM4_INPUT, SM4_HOLD, SM4_COLOR, SM4_SAVE};
int SMTICK4(int state){
	switch(state){
		case SM4_INIT:
			state = SM4_WAIT;
			break;
		case SM4_WAIT:
			if(GetKeypadKey() != '\0'){
				state = SM4_INPUT;
				lastInput = GetKeypadKey();
			} else {
				state = SM4_WAIT;
			}	
			break;
		case SM4_INPUT:
			if(GetKeypadKey() == '*'){
				state = SM4_HOLD;
			} else {
				state = SM4_WAIT;
			}
			break;
		case SM4_HOLD:
			if(GetKeypadKey() == '*' && holdTime >= 4){
				LCD_WriteCommand(0x0E);
				holdTime = 0;
				state = SM4_COLOR;
				cursorPosition = 18;
				value = 0;
				inputs = 0;
				newInput = 1;
			} else if(GetKeypadKey() == '*' && holdTime < 4) {
				holdTime++;
				state = SM4_HOLD;
			} else {
				holdTime = 0;
				state = SM4_WAIT;
			}
			break;
		case SM4_COLOR:
			if(inputState != INPUTDONE){
				LCD_Cursor(cursorPosition);
				state = SM4_COLOR;
			} else {
				cursorPosition = 18;
				inputState = INPUT_RED;
				newInput = 0;
				mainColor = tempColor;
				valueChanged = 1;
				state = SM4_SAVE;
			}
			break;
		case SM4_SAVE:
			if(valueChanged){
				state = SM4_SAVE;
			} else {
				state = SM4_WAIT;
			}
			break;
		default:
			state = SM4_INIT;
			break;
	}
	switch(state){
		case SM4_COLOR:
			if(GetKeypadKey() != lastInput && GetKeypadKey() != '\0'){
				if(GetKeypadKey() >= '0' && GetKeypadKey() <= '9'){
					LCD_WriteData(GetKeypadKey());
					cursorPosition++;
					switch(inputs){
						case 0:
							value = 100*(GetKeypadKey()-'0');
							inputs++;
							break;
						case 1:
							value += 10*(GetKeypadKey()-'0');
							inputs++;
							break;
						case 2:
							value += (GetKeypadKey()-'0');
							cursorPosition += 2;
							if(value > 255){
								value = 255;
							}
							switch(inputState){
								case INPUT_RED:
									tempColor.R = (unsigned char)value;
									break;
								case INPUT_GREEN:
									tempColor.G = (unsigned char)value;
									break;
								case INPUT_BLUE:
									tempColor.B = (unsigned char)value;
									break;
							}
							inputState++;
							inputs = 0;
							value = 0;
							break;
					}
				}
				lastInput = GetKeypadKey();
			} else if(GetKeypadKey() == '\0'){
				lastInput = '\0';
			}
			break;
		case SM4_SAVE:
			break;
	}
	return state;
};

/************************************************************************/
/* MAIN FUNCTION                                                        */
/************************************************************************/
int main(void)
{	
	/************************************************************************/
	/* INIT PORTS                                                           */
	/************************************************************************/
	DDRA = 0xF0; PORTA = 0x0F;// PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_SetUpPorts();
	TFT_SetupPorts();
	/************************************************************************/
	/* INIT DEVICES                                                         */
	/************************************************************************/
	LCD_init();
	Init_PWM();
	LCD_DisplayString(2, "LCD INIT");
	TFT_OpenSPI();
	TFT_InitDisplay();
	unsigned char cust[16] = {
		0b11111,
		0b10001,
		0b11101,
		0b11101,
		0b10101,
		0b11011,
		0b11111,
		0b11111,
		
		0b11111,
		0b11111,
		0b10001,
		0b10111,
		0b10001,
		0b11101,
		0b10001,
		0b11111
	};
	LCD_WriteCommand(0x40);
	for(unsigned i = 0; i <= 16; i++){
		LCD_WriteData(cust[i]);
	}
	LCD_WriteCommand(0);
	LCD_WriteCommand(1);
	
	TFT_ClearScreen();
	TFT_FillEllipse(60,75,100,50,BLACK);
	TFT_Ellipse(60,75,100,50,LIME);
	char *str = "UCR CS120B";
	TFT_GotoXY(5,9);
	TFT_WriteString(str, YELLOW);
	LCD_DisplayString(1, "120B            Final Project");
	_delay_ms(1000);
	LCD_DisplayString(1, "Jonas Sorgenfrei");
	LCD_Cursor(18);
	LCD_WriteData(0);
	LCD_Cursor(19);
	LCD_WriteData(1);
	_delay_ms(1000);
		
	/************************************************************************/
	/* INIT Variables                                                       */
	/************************************************************************/
	
	/************************************************************************/
	/* State Machines/Tasks                                                 */
	/************************************************************************/
	//Period for the tasks
	unsigned long int SMTick1_calc = 1000;
	unsigned long int SMTick2_calc = 1;
	unsigned long int SMTick3_calc = 1000;
	unsigned long int SMTick4_calc = 200;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	tmpGCD = findGCD(tmpGCD, SMTick4_calc);
	
	//greates common divisor for all tasks or smallest time unit for tasks
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;
	unsigned long int SMTick4_period = SMTick4_calc/GCD;

	//Declare an array of tasks
	static task task1, task2, task3, task4;
	task * tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1; // Task initial state
	task1.period = SMTick1_period;	//Task Period.
	task1.elapsedTime = SMTick1_period; //Task current elapsed time
	task1.TickFct = &SMTICK1; //Function pointer for the tick.
	
	//Task 2
	task2.state = -1; // Task initial state
	task2.period = SMTick2_period;	//Task Period.
	task2.elapsedTime = SMTick2_period; //Task current elapsed time
	task2.TickFct = &SMTICK2; //Function pointer for the tick.

	//Task 3
	task3.state = -1; // Task initial state
	task3.period = SMTick3_period;	//Task Period.
	task3.elapsedTime = SMTick3_period; //Task current elapsed time
	task3.TickFct = &SMTICK3; //Function pointer for the tick.

	//Task 4
	task4.state = -1; // Task initial state
	task4.period = SMTick4_period;	//Task Period.
	task4.elapsedTime = SMTick4_period; //Task current elapsed time
	task4.TickFct = &SMTICK4; //Function pointer for the tick.
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
		
	/************************************************************************/
	/* MAIN LOOP                                                            */
	/************************************************************************/
	unsigned short i;
	while(1){
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	/************************************************************************/
	/* CLOSING                                                              */
	/************************************************************************/
	TFT_CloseSPI();
	return 0;
}