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

#ifndef timer_H
#define timer_H

#include <lib_ex_config.h>
#include "my_types.h"

#if defined(F_timer)

#define TIMER0_COMPARATOR_A	   OCR0A
#define TIMER0_COMPARATOR_B	   OCR0B
#define TIMER0_PRESCALE_0      (_BV(CS00))
#define TIMER0_PRESCALE_8      (_BV(CS01)
#define TIMER0_PRESCALE_64     (_BV(CS01) | _BV(CS00))
#define TIMER0_PRESCALE_256    (_BV(CS02))
#define TIMER0_PRESCALE_1024   (_BV(CS02) | _BV(CS00))

#define TIMER1_PRESCALE_0      (_BV(CS10))
#define TIMER1_PRESCALE_8      (_BV(CS11))
#define TIMER1_PRESCALE_64     (_BV(CS11) | _BV(CS10))
#define TIMER1_PRESCALE_256    (_BV(CS10))
#define TIMER1_PRESCALE_1024   (_BV(CS12) | _BV(CS10) 

#include "timer_modes.h"

// Timer/Counter0 [
void timer0_start_normal(uint16_t prescale);
/**
	@warning Do not forget to set cycle value by calling timer0_set_cycle
*/
void timer0_start_ctc(uint16_t prescale);

void timer0_set_prescaler(uint16_t prescale);
uint16_t timer0_get_prescaler();

void timer0_set_enable_OCIE0x_interrupt(uint8_t OCIE0x, bool_t enable);

void timer0_set_cycle(byte comparator /* PB5 or PB6 */, uint8_t cycle);
void timer0_set_timeout_ms(uint32_t ms);

// ] Timer/Counter0

void timer1_set_prescaler(uint16_t prescale);
void timer2_set_prescaler(uint16_t prescale);

uint16_t timer1_get_prescaler();
uint16_t timer2_get_prescaler();

void timer1_set_cycle(uint16_t cycle);
void timer2_set_cycle(uint8_t cycle);

void timer1_set_timeout_ms(uint32_t ms);
void timer2_set_timeout_ms(uint32_t ms);

#endif // F_timer

#endif // timer_H