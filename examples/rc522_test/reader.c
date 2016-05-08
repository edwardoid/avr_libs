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

void timer_0_cb(void* vect)
{
	usart_write_string_line("Tick!");
}

int main()
{
	
	usart_init(9600UL);
	
	usart_write_string("Starting....\n\0");
	timers_start_0(200, TIMER0_CTC_MODE, TIMER_PRESCALE_1024, &timer_0_cb);


	while(1) {
		char buff[256];
		memset(buff, '\0', 255);
		buff[255] = '\0';
		usart_read_str(buff, 255);
		usart_write_string(buff);
	}
}