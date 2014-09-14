#include "adc.h"

void adc_select_source(uint8_t source)
{
	switch(source)
	{
		case ADC_SRC_AREF:
		{
			clear_bit(ADMUX, REFS0);
			clear_bit(ADMUX, REFS1);
			break;
		}
		
		case ADC_SRC_AVCC:
		{
			set_bit(ADMUX, REFS0);
			clear_bit(ADMUX, REFS1);
			break;
		}
		
		case ADC_SRC_INT:
		default:
		{
			set_bit(ADMUX, REFS0);
			set_bit(ADMUX, REFS1);
		}
	}
}

uint16_t  adc_read(uint8_t pin)
{
	ADMUX |= pin;
	clear_bit(ADMUX, ADLAR);
	
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);	
	set_bit(ADCSRA, ADEN); // enable adc
	set_bit(ADCSRA, ADSC); // start conversion
	
	while(ADCSRA & _BV(ADSC));

	uint16_t v = ADCL;	
	return (ADCH << 8) + v;;
}
