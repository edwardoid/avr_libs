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

#ifndef AVR_LIBS_CONFIG_H
#define AVR_LIBS_CONFIG_H
#include "debug.h"
#define USE_STD_LIB /* enable using <stdlib.h> */


#ifndef F_CPU // change line below according to MCU fuses.
//#define F_CPU 20000000UL
#define F_CPU 16000000UL
#endif // F_CPU

#define F_TIMERS
#define F_TIMERS_ENABLE_CALLBACKS
//#define F_1WIRE
#define F_ADC
#define F_PWM
//#define F_PCD8544
//#define F_SD
#define F_SPI
#define F_USART
#define F_RC522
#define F_NRF24L01

#ifdef F_ENABLE_ALL
//#define F_EXAMPLES

//#define F_1WIRE
//#define F_ADC
#define F_PWM
//#define F_SD
#define F_SPI
#define F_USART
//#define F_PCD8544
#define F_RC522
#endif // F_ENABLE_ALL

#ifndef F_EXAMPLES
#define F_EXAMPLES
#endif

#endif // AVR_LIBS_CONFIG_H