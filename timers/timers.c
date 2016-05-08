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

#include "timers.h"

#if defined(F_TIMERS)

#include <bitman.h>
#include <avr/io.h>
#include <my_stdlib.h>
#include <avr/interrupt.h>
#include <usart/usart.h>

#define DISABLE_POWER_CONSUPTION_MINIMISING set_bit(PRR, PRTIM0)

static callback_t cb_0 = NULL;
static callback_t cb_1 = NULL;

FORCE void start_0(uint8_t cycle, uint8_t mode, uint16_t prescale)
{
	clear_mask(TCCR0A, _BV(WGM01) | _BV(WGM00)); // Normal
	clear_bit(TCCR0B, WGM02); // Normal


	switch(mode)
	{
		case TIMER0_CTC_MODE:
		{
			set_bit(TCCR0A, WGM01);
			break;
		}
		case TIMER_NORMAL_MODE:
		{
			clear_mask(TCCR0A, _BV(WGM01) | _BV(WGM00)); // Normal
			clear_bit(TCCR0B, _BV(WGM02)); // Normal
			break;
		}
		default:
		{}
	}

	clear_mask(TCCR0B, _BV(CS02) | _BV(CS01) | _BV(CS00));
			
	switch(prescale)
	{
		case TIMER_PRESCALE_0:
		{
			set_mask(TCCR0B, _BV(CS00));
			break;
		}

		case TIMER_PRESCALE_8:
		{
			set_mask(TCCR0B, _BV(CS01));
			break;
		}

		case TIMER_PRESCALE_64:
		{
			set_mask(TCCR0B, _BV(CS01) | _BV(CS00));
			break;
		}

		case TIMER_PRESCALE_256:
		{
			set_mask(TCCR0B, _BV(CS02));
			break;
		}

		case TIMER_PRESCALE_1024:
		{
			set_mask(TCCR0B, _BV(CS02) | _BV(CS00));
			break;
		}
	}
	
	set_bit(TCCR0A, WGM01);


	OCR0A = cycle;
}

FORCE void start_1(uint8_t cycle, uint8_t mode, uint16_t prescale)
{/*
	clear_mask(TCCR0A, WGM01 | WGM00); // Normal
	clear_bit(TCCR0B, WGM02); // Normal
	switch(mode)
	{
		case TIMER1_CTC_MODE:
		{
			set_bit(TCCR0A, WGM01);
			break;
		}
		case TIMER1_CTC_MODE:
		{
			set_bit(TCCR0A, WGM01);
			break;
		}
		case TIMER_NORMAL_MODE:
		{
			clear_mask(TCCR0A, WGM01 | WGM00); // Normal
			clear_bit(TCCR0B, WGM02); // Normal
			break;
		}
		default:
		{}
	}
	set_bit(TCCR0A, WGM01);
	OCR0A = cycle;*/
}

#if defined(F_TIMERS_ENABLE_CALLBACKS)

void timers_start_0(uint8_t cycle, uint8_t mode, uint16_t prescale, callback_t cb)
{
	cb_0 = cb;
	start_0(cycle, mode, prescale);

#if defined(F_TIMERS_ENABLE_CALLBACKS)
	set_bit(TIMSK0, OCIE0A);
#endif
	sei();
}

void timers_start_1(uint8_t cycle, uint8_t mode, uint16_t prescale, callback_t cb)
{
	cb_1 = cb;
	start_1(cycle, mode, prescale);
	sei();
}

#else

void timers_start_0(uint8_t cycle, uint8_t mode, uint16_t prescale)
{
	start_0(cycle, mode, prescale);
}

void timers_start_1(uint8_t cycle, uint8_t mode, uint16_t prescale)
{
	start_1(cycle, mode, prescale);
}
#endif

#if defined(F_TIMERS_ENABLE_CALLBACKS)

ISR (TIMER0_COMPA_vect)
{
	if(cb_0 != NULL)
	{
		(*cb_0)(&TIMER0_COMPA_vect);
	}
}

#endif // F_TIMERS_ENABLE_CALLBACKS

#endif // F_TIMERS