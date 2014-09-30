/*
	avr_libs
	Copyright (C) 2014  Edward Sargsyan

	avr_libs is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	avr_libs is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with avr_libs.  If not, see <http://www.gnu.org/licenses/>.
*/


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
