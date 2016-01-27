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


#include "config.h"

#ifdef F_ADC
#include "adc.h"
uint16_t  adc_read(uint8_t adc)
{
	adc_select(adc);
	set_bit(ADCSRA, ADEN);
	clear_bit(ADMUX, ADLAR);
	set_bit(ADCSRA, ADSC);
	
	while(test_bit(ADCSRA, ADIF));

	uint16_t v  = ADC;
	clear_bit(ADCSRA, ADIF);
	return v;
}

#endif // F_ADC