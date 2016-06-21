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

#include <lib_ex_config.h>

#if defined(F_EXAMPLES)

#include <utils.h>
#include <usart/usart.h>
#include <avr/interrupt.h>
#include <timers/timers.h>
#include <bitman.h>
#include <debug.h>

#define SINGLE_TEST_SEC 10UL

volatile uint64_t counter = 0UL;
volatile uint64_t elapsed = 0UL;
volatile uint64_t top = 0xFFUL;
volatile uint64_t prescale = 1UL;

void reset()
{
	counter = 0UL;
	elapsed = 0UL;
	//top = 0xFFUL;
	prescale = 1UL;
}

void test_timer0_with(uint64_t ps_num, uint8_t mode, uint8_t ps, uint64_t top_val)
{
	timer0_set_enable_OCIE0x_interrupt(OCIE0A, FALSE);
	reset();
	usart_write_string("\n\tPrescale ");
	usart_write_num((int)ps_num);
	usart_write_byte('\n');
	prescale = ps_num;
	//top = top_val;
	timer0_start(mode, ps);
	if(mode != TIMER0_NORMAL)
	{
		timer0_set_cycle(TIMER0_COMPARATOR_A, (uint8_t)top_val);
	}
	timer0_set_enable_OCIE0x_interrupt(OCIE0A, TRUE);
	while(elapsed != SINGLE_TEST_SEC);
	timer0_set_enable_OCIE0x_interrupt(OCIE0A, FALSE);
}


void test_timer0_normal()
{
	timer0_set_enable_OCIE0x_interrupt(OCIE0A, FALSE);
	usart_write_string("Timer/Counter0 normal mode: ");
	test_timer0_with(1UL, TIMER0_NORMAL, TIMER0_PRESCALE_0, 0xFFUL);
	test_timer0_with(8UL, TIMER0_NORMAL, TIMER0_PRESCALE_8, 0xFFUL);
	test_timer0_with(64UL, TIMER0_NORMAL, TIMER0_PRESCALE_64, 0xFFUL);
	test_timer0_with(256UL, TIMER0_NORMAL, TIMER0_PRESCALE_256, 0xFFUL);
	test_timer0_with(1024UL, TIMER0_NORMAL, TIMER0_PRESCALE_1024, 0xFFUL);
	usart_write_string_line("Done");	
}

void test_timer0_ctc()
{
	timer0_set_enable_OCIE0x_interrupt(OCIE0A, FALSE);
	usart_write_string("Timer/Counter0 CTC mode: ");
	test_timer0_with(1UL, TIMER0_CTC, TIMER0_PRESCALE_0, 0x0FUL);
	test_timer0_with(8UL, TIMER0_CTC, TIMER0_PRESCALE_8, 0x0FUL);
	test_timer0_with(64UL, TIMER0_CTC, TIMER0_PRESCALE_64, 0x0FUL);
	test_timer0_with(256UL, TIMER0_CTC, TIMER0_PRESCALE_256, 0x0FUL);
	test_timer0_with(1024UL, TIMER0_CTC, TIMER0_PRESCALE_1024, 0x0FUL);
	usart_write_string_line("Done");
}

void test_timer0()
{
	test_timer0_normal();
}

int main()
{
	usart_init(9600UL);
	usart_write_string_line("Timers example..");

	set_as_output(DDRB, PB0);

	test_timer0();
	forever {

	}
}


ISR (TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
	++counter;
	if(counter == (F_CPU / top))
    {
    	usart_write_byte('.');
    	counter = 0;
    	++elapsed;
    	toggle_bit(PORTB, PB0);
    }
}

#endif