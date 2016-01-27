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

#ifndef ADC_H
#define ADC_H

#include "config.h"

#ifdef F_ADC

#include <avr/io.h>
#include "bitman.h"
#include <stdint.h>

#define ADC_SRC_AREF	0
#define ADC_SRC_AVCC	(_BV(REFS0))
#define ADC_SRC_INT		(_BV(REFS0) | _BV(REFS1))

#define ADC_DIV_2 0
#define ADC_DIV_2_EX (_BV(ADPS0))

#define ADC_DIV_4 (_BV(ADPS1))
#define ADC_DIV_8 (_BV(ADPS1) | _BV(ADPS0))

#define ADC_DIV_16 (_BV(ADPS2))
#define ADC_DIV_32 (_BV(ADPS2) | _BV(ADPS0))
#define ADC_DIV_64 (_BV(ADPS2) | _BV(ADPS1))
#define ADC_DIV_128 (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0))

#define ADC0 0
#define ADC1 (_BV(MUX0))
#define ADC2 (_BV(MUX1))
#define ADC3 (_BV(MUX1) | _BV(MUX0))
#define ADC4 (_BV(MUX2))
#define ADC5 (_BV(MUX2) | _BV(MUX0))
#define ADC6 (_BV(MUX2) | _BV(MUX1))
#define ADC7 (_BV(MUX2) | _BV(MUX1) | _BV(MUX0))



#define		adc_divide_by(div) (ADCSRA = (ADCSRA & ~0x7) | div)
#define		adc_select_reference(source) ADMUX = (ADMUX & ~(_BV(REFS1) | _BV(REFS0))) | source
#define		adc_select(pin) ADMUX = (ADMUX & ~ADC7) | pin
uint16_t 	adc_read(uint8_t adc);

#endif // F_ADC

#endif // ADC_H
