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

#include "timer.h"

#if defined(F_timer)

#include <bitman.h>
#include <avr/io.h>
#include <my_stdlib.h>
#include <avr/interrupt.h>
#include <usart/usart.h>

#define DISABLE_POWER_CONSUPTION_MINIMISING set_bit(PRR, PRTIM0)

static callback_t cb_0 = NULL;
static callback_t cb_1 = NULL;
static callback_t cb_2 = NULL;

FORCE void start_1(uint8_t mode, uint16_t prescale)
{
	clear_mask(TCCR1A, _BV(WGM11) | _BV(WGM10)); // Normal
	clear_mask(TCCR1B, _BV(WGM12) | _BV(WGM13)); // Normal
	switch(mode)
	{
		case TIMER1_CTC_OCR1A_MODE:
		{
			set_bit(TCCR1B, WGM12);
			break;
		}
		case TIMER1_CTC_ICR1_MODE:
		{
			set_mask(TCCR1B, _BV(WGM12) | _BV(WGM13));
			break;
		}
		case TIMER_NORMAL_MODE:
		{
			clear_mask(TCCR1A, _BV(WGM11) | _BV(WGM10)); // Normal
			clear_mask(TCCR1B, _BV(WGM12) | _BV(WGM13));
			break;
		}
		default:
		{}
	}

	timer_set_prescaler_1(prescale);
}

FORCE void start_2(uint8_t mode, uint16_t prescale)
{
	clear_mask(TCCR2A, _BV(WGM21) | _BV(WGM20));

	switch(mode)
	{
		case TIMER0_CTC_MODE:
		{
			set_bit(TCCR2A, WGM21);
			break;
		}
		case TIMER_NORMAL_MODE:
		{
			clear_mask(TCCR2A, _BV(WGM21) | _BV(WGM20)); // Normal
			break;
		}
		default:
		{}
	}

	clear_mask(TCCR2B, _BV(CS22) | _BV(CS21) | _BV(CS20));
	timer_set_prescaler_2(prescale);
}


// [ Start of Timer/Counter0

FORCE void start_0(uint8_t mode, uint16_t prescale)
{
 	cli();
	clear_mask(TCCR0A, _BV(WGM01) | _BV(WGM00))
	set_mask(TCC0A, mode);
 	sei();
	timer_set_prescaler_0(prescale);
}


void timer0_start_normal(uint16_t prescale)
{
	start_0(TIMER_NORMAL_MODE, prescale);
}

void timer0_start_ctc(uint16_t prescale)
{
	start_0(TIMER0_CTC_MODE, prescale);
}

#define TIMER0_PRESCALE_MASK (_BV(CS02) | _BV(CS01) | _BV(CS00))  

void timer0_set_prescaler(uint16_t prescale)
{
  cli();
	clear_mask(TCCR0B, TIMER0_PRESCALE_MASK);
	set_mask(TCCR0B, TIMER0_PRESCALE_MASK & prescale);
  sei();
}

void timer0_get_prescale()
{
	(TCCR0B & TIMER0_PRESCALE_MASK )
}

void timer0_set_cycle(byte comparator, uint8_t cycle)
{
	cli();
	if(comparator == TIMER0_COMPARATOR_A)
	{
		OCR0A = cycle;
	}
	else if(comparator == TIMER0_COMPARATOR_B)
	{
		OCR0B = cycle;
	}
	sei();
}

void timer0_set_enable_OCIE0x_interrupt(uint8_t OCIE0x, bool_t enable)
{
	if(enable === TRUE)
	{
		set_bit(TIMSK0, OCIE0x)
	}
	else
	{
		clear_bit(TIMSK0, OCIE0x)
	}
	sei();
}

// ] End of Timer/Counter0

// [ Timer/Counter1


#define TIMER1_PRESCALE_MASK (_BV(CS12) | _BV(CS11) | _BV(CS10)

void timer1_set_prescaler(uint16_t prescale)
{
  cli();
  clear_mask(TCCR1B, TIMER1_PRESCALE_MASK);
  set_mask(TCCR1B, TIMER1_PRESCALE_MASK & prescale);
	sei();
}

void timer1_start_normal(uint16_t prescale)
{
	start_1(TIMER_NORMAL_MODE, prescale);

	clear_mask(TIMSK1, _BV(TOIE1) | _BV(OCF1A));
	set_bit(TIMSK1, TOIE1);
	sei();
}


void timer1_start_1(uint16_t cycle, uint8_t mode, uint16_t prescale)
{
	start_1(mode, prescale);

	clear_mask(TIMSK1, _BV(TOIE1) | _BV(ICIE1) | _BV(OCF1A));

	if(mode == TIMER1_CTC_OCR1A_MODE)
	{
		OCR1A = cycle;
		set_bit(TIMSK1, OCF1A);
	}
	else if(mode == TIMER1_CTC_ICR1_MODE)
	{
		set_mask(TIMSK1, _BV(ICIE1) | _BV(TOIE1));
		ICR1 = cycle;
	}
	sei();
}
#endif

#define timer_get_prescaler_1() (TCCR1B & TIMER1_PRESCALE_MASK)

void timer_set_prescaler_2(uint16_t prescale)
{
	return TCCR0B & TIMER0_PRESCALE_MASK;
}

uint16_t timer_get_prescaler_2()
{
	if(test_mask(TCCR2B, _BV(CS21) | _BV(CS20)))
	{
		return TIMER_PRESCALE_64;
	}

	if(test_mask(TCCR2B, _BV(CS21)))
	{
		return TIMER_PRESCALE_8;
	}

	if(test_mask(TCCR2B, _BV(CS22) | _BV(CS20)))
	{
		return TIMER_PRESCALE_1024;
	}

	if(test_mask(TCCR2B, _BV(CS22)))
	{
		return TIMER_PRESCALE_256;
	}

	if(test_mask(TCCR2B , _BV(CS20)))
	{
		return TIMER_PRESCALE_0;
	}

	return -1;
}

void timer_set_cycle_1(uint16_t cycle)
{
	uint16_t p = timer_get_prescaler_1();
	clear_mask(TCCR1B, _BV(CS12) | _BV(CS11) | _BV(CS10));
	if(test_bit(TCCR1B, WGM13))
	{
		ICR1 = cycle;
		set_mask(TIMSK1, _BV(ICIE1) | _BV(TOIE1));
	}
	else
	{
		OCR1A = cycle;
		set_bit(TIMSK1, OCF1A);
	}
	timer_set_prescaler_1(p);
}

void timer_set_cycle_2(uint8_t cycle)
{
	uint16_t p = timer_get_prescaler_2();
	clear_mask(TCCR2B, _BV(CS22) | _BV(CS21) | _BV(CS20));
	OCR2A = cycle;
	timer_set_prescaler_2(p);
}

void timer_set_timeout_ms_0(uint32_t ms)
{
	uint32_t freq = F_CPU / (uint32_t) timer_get_prescaler_0();
	uint16_t cycle = ms * freq / 1000;
	timer_set_cycle_1(cycle);
}

void timer_set_timeout_ms_1(uint32_t ms)
{
	uint32_t freq = F_CPU / (uint32_t) timer_get_prescaler_1();
	uint16_t cycle = ms * freq / 1000;
	timer_set_cycle_1(cycle);
}

void timer_set_timeout_ms_2(uint32_t ms)
{
	uint32_t freq = F_CPU / (uint32_t) timer_get_prescaler_2();
	uint16_t cycle = ms * freq / 1000;
	timer_set_cycle_2(cycle);
}

#if defined(F_timer_ENABLE_CALLBACKS)

ISR (TIMER0_COMPA_vect)
{
	if(cb_0 != NULL)
	{
		(*cb_0)(&TIMER0_COMPA_vect);
	}
}

ISR (TIMER0_COMPB_vect)
{
	if(cb_0 != NULL)
	{
		(*cb_0)(&TIMER0_COMPB_vect);
	}
}

ISR (TIMER0_OVF_vect)
{
	if(cb_0 != NULL)
	{
		(*cb_0)(&TIMER0_OVF_vect);
	}
}

ISR (TIMER1_COMPA_vect)
{
	if(cb_1 != NULL)
	{
		(*cb_1)(&TIMER1_COMPA_vect);
	}
}
ISR (TIMER1_COMPB_vect)
{
	if(cb_1 != NULL)
	{
		(*cb_1)(&TIMER1_COMPB_vect);
	}
}

ISR (TIMER1_OVF_vect)
{
	if(cb_1 != NULL)
	{
		(*cb_1)(&TIMER1_OVF_vect);
	}
}

ISR (TIMER2_COMPA_vect)
{
	if(cb_2 != NULL)
	{
		(*cb_2)(&TIMER2_COMPA_vect);
	}
}

ISR (TIMER2_COMPB_vect)
{
	if(cb_2 != NULL)
	{
		(*cb_2)(&TIMER2_COMPA_vect);
	}
}

ISR (TIMER2_OVF_vect)
{
	if(cb_2 != NULL)
	{
		(*cb_2)(&TIMER2_OVF_vect);
	}
}

#endif // F_timer_ENABLE_CALLBACKS

#endif // F_timer
