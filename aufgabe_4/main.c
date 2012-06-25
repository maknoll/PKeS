#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

/* Makros for led control */
#define ledOn(x) (PORTA |= (1<<x))
#define ledOff(x) (PORTA &= ~(1<<x))
#define button(x) ((PINA & (16<<x)) == 0)

/* Robby Board Specification */
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define BUTTON_0 4
#define BUTTON_1 5
#define BUTTON_2 6
#define BUTTON_3 7

#define MOTOR_STOP 0x00
#define MOTOR_FORWARD 0x42
#define MOTOR_BACK 0x81
#define MOTOR_LEFT 0x82
#define MOTOR_RIGHT 0x41

void motor(uint8_t direction)
{
	PORTE &= ~(1<<PE2) & ~(1<<PE3);
	PORTG &= ~(1<<PG0) & ~(1<<PG1);

	PORTG |= direction & 0x0f;
	PORTE |= direction >> 4;
}

void motor_init()
{
	DDRE = (1<<PE2) | (1<<PE3);
	DDRG = (1<<PG0) | (1<<PG1);

	motor(MOTOR_STOP);
}

void pwm_init()
{
	DDRB |= (1<<PB5) | (1<<PB6);
	TCCR1A = (1<<WGM11) | (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);
	TCCR1B = (1<<CS11) | (1<<CS10);

	OCR1AH = 0x01;
	OCR1AL = 0x00;

	OCR1BH = 0x01;
	OCR1BL = 0x00;
}

void init()
{
	DDRA |= ((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));
	PORTA &= ~((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));

	EIMSK |= (1<<INT2) | (1<<INT3);
}

#define STATE_0 0
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3
volatile int state = STATE_0;

#define DISTANCE_1 100
#define DISTANCE_2 200
#define DISTANCE_3 300

volatile int counter = 0;

int main()
{
	init();
	pwm_init();
	motor_init();
	sei();

	while(1)
	{
		switch(state)
		{
			case STATE_0:
				if(button(1))
				{
					motor(MOTOR_FORWARD);
					state = STATE_1;
				}
				else if(button(2))
				{
					motor(MOTOR_FORWARD);
					state = STATE_2;
				}
				else if(button(3))
				{
					motor(MOTOR_FORWARD);
					state = STATE_3;
				}
				break;

			case STATE_1:
				if(counter >= DISTANCE_1)
				{
					motor(MOTOR_STOP);
					counter = 0;
					state = STATE_0;
				}
				break;

			case STATE_2:
				if(counter >= DISTANCE_2)
				{
					motor(MOTOR_STOP);
					counter = 0;
					state = STATE_0;
				}
				break;

			case STATE_3:
				if(counter >= DISTANCE_3)
				{
					motor(MOTOR_STOP);
					counter = 0;
					state = STATE_0;
				}
				break;
		}
	}

	return 0;
}

ISR(INT2_vect)
{
	counter++;
}

ISR(INT3_vect)
{
	// evtl. für Richtungskorrektur
}
