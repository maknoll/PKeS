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

void init()
{
	DDRA |= ((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));
	PORTA &= ~((1<<LED_0) | (1<<LED_1) | (1<<LED_2) | (1<<LED_3));

	ADMUX = (1<<REFS1) | (1<<REFS0);
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); //TF=128
	ADCSRA |= (1<<ADEN);

	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC)) {}

	ADCW;

	ADCSRA |= (1<<ADIE);
}

/* ADC Einzelmessung */
void ADC_Read(uint8_t channel)
{
	// Kanal waehlen, ohne andere Bits zu beeinflußen
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
}

void display(unsigned char number)
{
	if(number > 4)
		number = 4;

	PORTA &= ~(0x0f);
	while(number)
		PORTA |= (1 << (number-- - 1));
}


int main(void)
{
	init();

	TCCR0A = (1<<CS00) | (1<<CS02);
	TIMSK0 |= (1<<TOIE0);
	sei();

	while(1)
	{

	}

	return 0;
}

ISR(TIMER0_OVF_vect)
{
	ADC_Read(0);
}

ISR(ADC_vect)
{
	display((ADCW + 128) / 255);
}
