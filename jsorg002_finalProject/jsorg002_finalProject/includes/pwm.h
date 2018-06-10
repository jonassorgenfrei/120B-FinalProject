#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <util/delay.h>

void Init_PWM(void)
{
	// Set mode of operation to FastPWM
	TCCR2A |= (1<<WGM20 | 1<<WGM21);
	
	// Set clock source (prescaler)
	TCCR2B |= (1<<CS21);
	
	// Set to 50% duty cycle
	OCR2A = 0x00;
	OCR2B = 0x00;
}

void pwm_enable(unsigned char channel) {
	if (channel == 1) TCCR2A |= 1<<COM2A1;
	if (channel == 2) TCCR2A |= 1<<COM2B1;
}

void pwm_disable(unsigned char channel) {
	if (channel == 1) TCCR2A &= ~(1<<COM2A1);
	if (channel == 2) TCCR2A &= ~(1<<COM2B1);
}

void pwm_dutycycle(unsigned char channel, uint8_t dutycycle) {
	if (channel == 1) OCR2A = dutycycle;
	if (channel == 2) OCR2B = dutycycle;
}


#endif