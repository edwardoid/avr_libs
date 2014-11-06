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

#include "uart.h"

#ifdef F_UART

#include <avr/io.h>
#include <stdio.h>

void uart_init (int BAUDRATE_H, int BAUDRATE_L)
{
	UBRRH = BAUDRATE_H;
	UBRRL = BAUDRATE_L;

	UCSRB|= (1<<TXEN)|(1<<RXEN);
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

char	uart_read_byte()
{
	while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }

   //Now USART has got data from host
   //and is available is buffer

   return UDR;
}
	
void	uart_write_byte(char data)
{
	while(!(UCSRA & (1<<UDRE)));

   //Now write the data to USART buffer

   UDR=data;
}

void	uart_write_string(const char* s)
{
	while (*s)
		uart_write_byte(*s++);
}

void	uart_write_num(int num)
{
	char str[32] = {};
	sprintf(str, "%d\n", num);
	uart_write_string(str);
}

void	uart_write_u32num(uint32_t num)
{
	char str[32] = {};
	sprintf(str, "%lu", num);
	uart_write_string(str);
}


void	uart_write_num_hex(int num)
{
	char str[32] = {};
	sprintf(str, "%2X", num);
	uart_write_string(str);
}

void	uart_write_string_line(const char* s)
{
	while (*s)
		uart_write_byte(*s++);
	uart_write_byte('\n');
	uart_write_byte('\0');
}

#endif // F_UART