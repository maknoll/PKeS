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

typedef struct
{
	int length;
	int state;
	int *leds;
} program_t;

#define PROGRAMS 2

program_t program[PROGRAMS] = {
	{8, 0, (int[8]){0, 1, 3, 11, 15, 11, 3, 1}},
	{6, 0, (int[6]){0, 1, 2, 4, 2, 1}}
};
int active_program = 0;

int counter = 0;

int button_debounce = 0;

void init()
{
	DDRA |= ((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));
	PORTA &= ~((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));
}

void buttonPressed()
{
	active_program++;
	active_program %= PROGRAMS;
}

int main(void)
{
	init();

	TCCR0A = (1<<CS00) | (1<<CS02);
	TIMSK0 |= (1<<TOIE0);
	sei();

	while(1)
	{
		if(button_debounce)
		{
			if(!button(0))
			{
				_delay_ms(50);
				button_debounce = 0;
			}
		}
		else
		{
			if(button(0))
			{
				_delay_ms(50);
				button_debounce = 1;
				buttonPressed();
			}
		}
	}

	return 0;
}

ISR(TIMER0_OVF_vect)
{
	if(counter++ >= 30)
	{
		program_t *active = &program[active_program];
		PORTA = active->leds[active->state];
		active->state = (active->state + 1) % active->length;
		counter = 0;
	}
}
