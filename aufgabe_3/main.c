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

uint8_t sensor = 0;
uint16_t sensor_data[] = {0x00, 0x00, 0x00, 0x00};

void adc_init()
{
	/* internal reference voltage */
	ADMUX = (1<<REFS1) | (1<<REFS0);

	/* prescaler 128 */
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);

	/* enable adc */
	ADCSRA |= (1<<ADEN);

	/* start dummy conversion */
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	uint16_t dummy = ADCW;

	/* enable adc interrupt */
	ADCSRA |= (1<<ADIE);
}

void adc_read(uint8_t channel)
{
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);
}

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

	TCCR0A = (1<<CS00) | (1<<CS02);
	TIMSK0 |= (1<<TOIE0);
}

int main()
{
	init();
	adc_init();
	pwm_init();
	motor_init();
	sei();

	adc_read(0);

	while(1);

	return 0;
}

ISR(TIMER0_OVF_vect)
{
	if(sensor_data[2] > 300 && sensor_data[3] < 300)
		motor(MOTOR_RIGHT);
	else if(sensor_data[2] < 300 && sensor_data[3] > 300)
		motor(MOTOR_LEFT);
	else if(sensor_data[2] < 300 && sensor_data[3] < 300)
		motor(MOTOR_RIGHT);
	else
		motor(MOTOR_FORWARD);
}

ISR(ADC_vect)
{
	sensor_data[sensor] = ADCW;
	sensor = (sensor + 1) & 0x03;
	adc_read(sensor);
}
