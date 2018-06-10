/*
 * jsorg002_finalProject_secondMC.c
 *
 * Created: 05.06.2018 13:59:32
 * Author : Jonas
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "timer.h"
#include "pwm.h"

unsigned char redValue = 0x00;


int main(void)
{
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
    /* Replace with your application code */
	Init_PWM();
	//pwm_enable(2);
	//pwm_dutycycle(2, 100);
	
    while (1) 
    {
		if(PINC != redValue){
			redValue = PINC;
			if(redValue >0){
				pwm_enable(2);
				pwm_dutycycle(2, redValue);
			} else {
				pwm_disable(2);
			}	
		}
		_delay_ms(1);
    }
}



