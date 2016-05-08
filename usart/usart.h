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

#ifndef USART_H
#define USART_H

#include <lib_ex_config.h>
#if defined(F_USART)

#include "bitman.h"
#include "my_types.h"
#include <stdint.h>

#define CALC_BRR(rate) (((F_CPU >> 4) / rate) - 1)

#define BAUD_RATE_H(rate) (uint8_t)((CALC_BRR(rate) >> 8) & 0xFF)
#define BAUD_RATE_L(rate) (uint8_t)(CALC_BRR(rate) & 0xFF)

void	usart_init (uint32_t baud_rate);

char	usart_read_byte();

uint16_t	usart_read_str(char* buff, uint16_t sz);
	
void	usart_write_byte(char data);

void	usart_write_string(const char* s);

void	usart_write_num(int32_t num);

void	usart_write_string_line(const char* s);

#endif // F_USART

#endif // USART_H
