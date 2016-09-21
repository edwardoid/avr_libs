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



#include "usart.h"

#ifdef F_USART

#include <avr/io.h>
#include <stdio.h>

#include "usart_regs.h"
#include "my_stdlib.h"

void	usart_init (uint32_t baud_rate)
{
	USART_BR_HI_REG = BAUD_RATE_H(baud_rate);
	USART_BR_LO_REG = BAUD_RATE_L(baud_rate);

	USART_CTL_B |= _BV(TX_ENABLE_BIT) | _BV(RX_ENABLE_BIT);
    USART_CTL_C |= _BV(USART_BR_SEL) | _BV(USART_FRAME_BIT0) | _BV(USART_FRAME_BIT1);
}

char	usart_read_byte()
{
	while(!(USART_CTL_A & _BV(USART_GOT_DATA_BIT)));
	
	//Now USART has got data from host
	//and is available is buffer

	return USART_DATA_REG;
}

uint16_t	usart_read_str(char* buff, uint16_t sz)
{
	uint16_t count = 0;
	do
	{
		*buff = usart_read_byte();
		++count;
		if(*buff == '\0')
		{
			return count;
		}
		++buff;
		--sz;
	}
	while(sz);
	return count;
}
	
void	usart_write_byte(char data)
{
	while(!(USART_CTL_A & _BV(USART_DATA_READY_BIT)));
	
	USART_DATA_REG = data;
}

void	usart_write_string(const char* s)
{
	while (*s)
		usart_write_byte(*s++);

	usart_write_byte('\0');
}

void	usart_write_num(int num)
{
	char str[10];
	memset(str, '\0', 10);
	sprintf(str, "%d", num);
	usart_write_string(str);
}


int	usart_read_num()
{
	char str[10];
	memset(str, '\0', 10);
	if(0 == usart_read_str(str, 10))
	{
		return 0;
	}
	int res = 0;
	sscanf(str, "%d", &res);
	return res;
}

void	usart_write_string_line(const char* s)
{
	while (*s)
		usart_write_byte(*s++);
	usart_write_byte('\n');
	usart_write_byte('\0');
}

/*
	%d -- int
	
	%8d -- int8_t
	%8u -- uint8_t
	
	%16d -- int16_t
	%16u -- uint16_t

	%32d -- int32
	%32u -- uint32_t

	%l -- long
	%lu -- unsigned long

	%x -- hex int
	%8x -- hex uint8_t
	%16x -- hex uint16_t
	%32x -- hex uint32_t

	%f -- float
*/

#define SZ_INT sizeof(int)
#define SZ_LONG sizeof(long)
#define SZ_FLOAT sizeof(float)

#include <stdarg.h>

void usart_printf_d(va_list ap, uint8_t len, int base)
{
	int v = 0;
	switch(len)
	{
		case 0:
		case 8: { v = va_arg(ap, int); break; }
		case 16: { v = va_arg(ap, int16_t); break; }
		case 32: { v = va_arg(ap, int32_t); break; }
		default: { return; }
	}

	if(base == 16) { usart_write_byte('0'); usart_write_byte('x'); }
	
	if (v == 0) { usart_write_byte('0'); }
	else
	{
		char b[32] = {	0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0 };
		char* pos = &b[31];
		
		if(v < 0) { usart_write_byte('-'); }
		while(v != 0)
		{
			uint8_t n = v % base;
			if(n < 10) { *pos = '0' + n; }
			else { *pos = 'A' - (n - 10); }
			--pos;
			v /= base;
		}

		while(pos <= &b[31] )
		{
			usart_write_byte(*pos ++);
		}
	}
}

void usart_printf_u(va_list ap, uint8_t len, int base)
{
	unsigned int v = 0;
	switch(len)
	{
		case 0:
		case 8: { v = va_arg(ap, unsigned int); break; }
		case 16: { v = va_arg(ap, uint16_t); break; }
		case 32: { v = va_arg(ap, int32_t); break; }
		default: { return; }
	}

	if(base == 16) { usart_write_byte('0'); usart_write_byte('x'); }
	
	if (v == 0) { usart_write_byte('0'); }
	else
	{
		char b[32] = {	0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0 };
		char* pos = &b[31];
		
		while(v != 0)
		{
			uint8_t n = v % base;
			if(n < 10) { *pos = '0' + n; }
			else { *pos = 'A' - (n - 10); }
			--pos;
			v /= base;
		}

		while(pos <= &b[31] )
		{
			usart_write_byte(*pos ++);
		}
	}
}

#endif // F_USART