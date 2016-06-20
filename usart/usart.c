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

#endif // F_USART