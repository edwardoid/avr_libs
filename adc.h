#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include "bitman.h"
#include <stdint.h>

uint16_t  adc_read(uint8_t pin)
{
	ADMUX |= pin;
	set_bit(ADMUX, REFS0);
	clear_bit(ADMUX, ADLAR);
	
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);	
	set_bit(ADCSRA, ADEN);
	set_bit(ADCSRA, ADSC);
	
	while(ADCSRA & _BV(ADSC));

	uint16_t v = ADCL;	
	return (ADCH << 8) + v;;
}

#endif // ADC_H
