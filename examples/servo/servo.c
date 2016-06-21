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

#include <usart/usart.h>
#include <timers/timers.h>
#include <time_utils_delays.h>
#include <pwm/pwm.h>
#include <adc/adc.h>
#include <debug.h>

#define abs(x) ((x > 0) ? (x) : (-(x)))

int main()
{
	usart_init(9600UL);
	usart_write_string_line("Servo example.");

	DDRD |= (1 << DDD6);
	DDRB |= (1 << DDB1)|(1 << DDB2);

	//pwm_t0_set_invert_mode(PWM_NON_INVERTED);
    //TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    //pwm_t0_start(PWM_T0_FAST, 230, TIMER_PRESCALE_64);
    pwm_t1_start(PWM_T1_FAST_OCR1A, 60, TIMER1_PRESCALE_8);
    pwm_t1_set_invert_mode(PWM_T1_NON_INVERTED_OC1A);
    //OCR0A = 60;
    OCR1A = 1024;
	adc_select_reference(ADC_SRC_AVCC);
	adc_divide_by(ADC_DIV_128);

	uint32_t prev = 0;
	char buff[10];
	
	do
	{
		memset(&buff, '\0', 10);
		usart_read_str(&buff, 10);
		usart_write_string_line(buff);
		/*double p1 = (double)usart_write_num;
		tu_delay_ms(20);
		
		double p2 = (double)adc_read(ADC0) / 3.;
		tu_delay_ms(20);

		double p3 = (double)adc_read(ADC0) / 3;
		
		int val = ((double)(p1 + p2 + p3) / (double)4.);
		//DBG_VALUE("", p1);*/

		int val = usart_read_num(); 

		if(abs(prev - val) < 5 || val == 0)
		{
			continue;
		}
		prev = val;
		usart_write_num(val);
		usart_write_byte('\n');

		OCR1A = val;
		tu_delay_ms(200);

	} 
	while(1);
}

#endif