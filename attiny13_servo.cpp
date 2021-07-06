ï»¿#include "Arduino.h"
#define F_CPU 9600000
#define LED PB1
#include <avr/io.h>
int servo=PB0;
void servoPulse (int servo, int a)
{
	a = (a * 4) + 1000;
	digitalWrite(servo, HIGH);
	delayMicroseconds(a);
	digitalWrite(servo, LOW);
	delayMicroseconds(50000-a);
}
void adc_setup (void)
{
	// Set the ADC input to PB2/ADC1
	ADMUX |= (1 << MUX0);
	ADMUX |= (1 << ADLAR);
	// PRESCALER I saat/128 olarak ayarlayÄ±n ve ADC'yi etkinleÅŸtir
	// 9.6 MHz'de  75 kHz'dir.
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}
int adc_read (void)
{
	// dÃ¶nÃ¼ÅŸtÃ¼rmeyi baÅŸlat
	ADCSRA |= (1 << ADSC);
	//bitmesini bekle
	while (ADCSRA & (1 << ADSC));
	return ADCH; // deÄŸeri dÃ¶ndÃ¼r
}
void pwm_setup (void)
{
	// timer i 0 prescalerÄ± clock/8 olarak ayarlayÄ±n.
	//  9.6 MHz de  1.2 MHz.
	// ATtiny13 datasheet, Tablo 11.9.
	TCCR0B |= (1 << CS01);
	
	//  'Fast PWM' modu
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	
	//  OC0B Ã§Ä±kÄ±ÅŸÄ±nÄ± temizle, artarak saymayÄ± baÅŸalt
	TCCR0A |= (1 << COM0B1);
}
void pwm_write (int val)
{
	OCR0B = val;
}
void setup()
{
	DDRB |= (1 << LED);
	pinMode(servo, OUTPUT);
}
void loop ()
{
	long a;
	pwm_setup();
	adc_setup();
	a = adc_read();
	servoPulse(servo, a);
	pwm_write(a);
}
