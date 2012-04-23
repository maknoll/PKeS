/*
 * main.c
 *
 *  Created on: Apr 16, 2012
 *      Author: Sebastian Zug
 *              Eclipse AVR Project
 */

#include <avr/io.h>

/*! Makros for led control*/
#define ledOn(x)  (PORTA |= (1<<x))
#define ledOff(x) (PORTA &=~(1<<x))
#define button(x) ((PINA & (16<<x))==0)

/*! Robby Board Specification*/
//-------------------------------- Port A -------------------------------
#define LED_0		0
#define LED_1		1
#define LED_2		2
#define LED_3		3
#define BUTTON_0	4
#define BUTTON_1	5
#define BUTTON_2	6
#define BUTTON_3	7

void init(){
	//-------------------------------- Port A -------------------------------
	// set leds LED_X as output
	// alternative
	//     - intermediate
	//       DDRA=1+2+4+8;
	//     - using processor makros
	//       DDRA=((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));
	//     - using hardware specific makros
	DDRA |= ((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));
	// disable leds
	PORTA &= ~((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));
}

int main(void)
{
	init();
	//loop
	//alternative: loop_until_bit_is_clear(WARTEPIN, WARTEBIT);
    while (1)
    {
        if button(0)
		ledOn(0);
        if button(1)
        	ledOff(0);
    }
    return (1);	// sollte nie erreicht werden
}
