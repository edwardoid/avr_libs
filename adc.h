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

#ifdef F_ADC

#include <avr/io.h>
#include "bitman.h"
#include <stdint.h>

#define ADC_SRC_AREF	1
#define ADC_SRC_AVCC	2
#define ADC_SRC_INT		3

void		adc_select_source(uint8_t source);
uint16_t 	adc_read(uint8_t pin);

#endif // F_ADC

#endif // ADC_H
