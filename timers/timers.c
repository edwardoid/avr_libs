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

/*
static callback_t cb_0 = NULL;
static callback_t cb_1 = NULL;
static callback_t cb_2 = NULL;
*/

// [ Start of Timer/Counter0

void timer0_start(uint8_t mode, uint8_t prescale)
{
 	cli();
	clear_mask(TCCR0A, (_BV(WGM01) | _BV(WGM00)));
	set_mask(TCCR0A, mode);
 	sei();
	timer0_set_prescaler(prescale);
}


void timer0_start_normal(uint8_t prescale)
{
	timer0_start(TIMER0_NORMAL, prescale);
}

void timer0_start_ctc(uint8_t prescale)
{
	timer0_start(TIMER0_CTC, prescale);
}

#define TIMER0_PRESCALE_MASK (_BV(CS02) | _BV(CS01) | _BV(CS00))  

void timer0_set_prescaler(uint8_t prescale)
{
  cli();
	clear_mask(TCCR0B, TIMER0_PRESCALE_MASK);
	set_mask(TCCR0B, TIMER0_PRESCALE_MASK & prescale);
  sei();
}	

uint8_t timer0_get_prescale()
{
	return (TCCR0B & TIMER0_PRESCALE_MASK);
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

void timer0_set_timeout_us(uint32_t ms)
{
	uint32_t freq = F_CPU / (uint32_t) timer0_get_prescaler();
	uint16_t cycle = ms * freq;
	timer1_set_cycle(cycle);
}

void timer0_set_enable_OCIE0x_interrupt(uint8_t OCIE0x, bool_t enable)
{
	if(enable == TRUE)
	{
		set_bit(TIMSK0, OCIE0x);
	}
	else
	{
		clear_bit(TIMSK0, OCIE0x);
	}
	sei();
}

// ] End of Timer/Counter0

// [ Timer/Counter1



#define TIMER1_MODE_MASK_11_10 (_BV(WGM11) | _BV(WGM10))
#define TIMER1_MODE_MASK_13_12 (_BV(WGM13) | _BV(WGM12))

void timer1_start(uint8_t mode, uint8_t prescale)
{
	cli();
	timer1_set_prescaler(prescale);

	clear_mask(TCCR1A, TIMER1_MODE_MASK_11_10);
	set_mask(TCCR1A, prescale & TIMER1_MODE_MASK_11_10);

	clear_mask(TCCR1B, _BV(WGM13) | _BV(WGM12));
	set_mask(TCCR1B, prescale & TIMER1_MODE_MASK_13_12);

	sei();
}

void timer1_start_normal(uint8_t prescale)
{
	timer1_start(TIMER1_NORMAL, prescale);
}

uint8_t timer1_get_mode()
{
	return (TCCR1A & TIMER1_MODE_MASK_11_10) | (TCCR1B & TIMER1_MODE_MASK_13_12);
}

#define TIMER1_PRESCALE_MASK (_BV(CS12) | _BV(CS11) | _BV(CS10))

void timer1_set_prescaler(uint8_t prescale)
{
	cli();
	clear_mask(TCCR1B, TIMER1_PRESCALE_MASK);
	set_mask(TCCR1B, prescale & TIMER1_PRESCALE_MASK);
	sei();
}

uint8_t timer1_get_prescaler()
{
	return (TCCR1B & TIMER1_PRESCALE_MASK);
}

void timer1_set_cycle(uint16_t cycle)
{
	uint8_t mode = timer1_get_mode();
	if(mode == TIMER1_CTC_OCR1A)
	{
		OCR1A = cycle;
	}
	else if(mode == TIMER1_CTC_ICR1)
	{
		ICR1 = cycle;
	}
}

void timer1_set_timeout_ms(uint32_t ms)
{
	uint32_t freq = F_CPU / (uint32_t) timer1_get_prescaler();
	uint16_t cycle = ms * freq / 1000;
	timer1_set_cycle(cycle);
}

// ] Timer/Counter1



// [ Timer/Counter2

#define TIMER2_MODE_MASK_21_20 	(_BV(WGM21) | _BV(WGM20))
#define TIMER2_MODE_MASK_22		(_BV(WGM22))
#define TIMER1_MODE_MASK (TIMER2_MODE_MASK_22 | TIMER2_MODE_MASK_21_20)

void timer2_start(uint8_t mode, uint8_t prescale)
{
	cli();
	clear_mask(TCCR2A, TIMER2_MODE_MASK_21_20);
	clear_mask(TCCR2B, TIMER2_MODE_MASK_22);
	set_mask(TCCR2A, mode & TIMER2_MODE_MASK_21_20);
	set_mask(TCCR2B, mode & TIMER2_MODE_MASK_22);
	timer2_set_prescaler(prescale);
	sei();
}

void timer2_start_normal(uint8_t prescale)
{
	timer2_start(TIMER2_NORMAL, prescale);
}

void timer2_start_ctc(uint8_t prescale)
{
	timer2_start(TIMER2_CTC, prescale);
}

#define TIMER2_PRESCALE_MASK (_BV(CS22) | _BV(CS21) | _BV(CS20))

void timer2_set_prescaler(uint8_t prescale)
{
	cli();
	clear_mask(TCCR2B, TIMER2_PRESCALE_MASK);
	set_mask(TCCR2B, TIMER2_PRESCALE_MASK & prescale);
	sei();
}

uint8_t timer2_get_prescaler()
{
	return TCCR2B & TIMER2_PRESCALE_MASK;
}

void timer2_set_cycle(uint16_t cycle)
{
	OCR2A = cycle;
}

void timer2_set_timeout_ms(uint32_t ms)
{
	uint32_t freq = F_CPU / (uint32_t) timer2_get_prescaler();
	uint16_t cycle = ms * freq / 1000;
	timer2_set_cycle(cycle);
}

// ] Timer/Counter2
/*

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
}*/

#endif // F_TIMERS

