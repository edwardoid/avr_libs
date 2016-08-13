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
#include <NRF24l01/NRF24l01.h>
#include <debug.h>

#define abs(x) ((x > 0) ? (x) : (-(x)))

int main()
{
	usart_init(9600UL);
	usart_write_string_line("NRF24l01(+) example.");

	forever {
		
	}
}

#endif