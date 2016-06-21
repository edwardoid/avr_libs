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

#include <avr/interrupt.h>
#include <usart/usart.h>
#include <timers/timers.h>
#include <pwm/pwm.h>
#include <time_utils_delays.h>
#include <adc/adc.h>

void timer_0_cb(void* vect)
{
	usart_write_string_line("Tick0!");
}

void timer_1_cb(void* vect)
{
	usart_write_string_line("Tick1!");
}


void timer_2_cb(void* vect)
{
	usart_write_string_line("Tick2!");
}

int main()
{
	DDRD |= (1 << DDD6);
	usart_init(9600UL);
	usart_write_string_line("Starting...");
	
    OCR0A = 1;
    // set PWM for 50% duty cycle


    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= (1 << CS00 ) | (1 << CS01);
    // set prescaler to 8 and starts PWM


    uint8_t duty = 1;
    usart_write_num(duty);
	usart_write_byte('\n');
	adc_select_reference(ADC_SRC_INT);
	adc_divide_by(ADC_DIV_128);
	uint16_t prev = 0;
	usart_write_num(prev);
    while (1)
    {
    	uint16_t v = adc_read(ADC0);

    	if(v == prev)
    	{
    		continue;
    	}
    	usart_write_num(v);
    	usart_write_byte('\n');
    	prev = v;
    	OCR0A = 5;
        // we have a working Fast PWM
    }
}