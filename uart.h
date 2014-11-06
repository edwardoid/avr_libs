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

#ifndef UART_H
#define UART_H

#include "config.h"
#ifdef F_UART

#include "bitman.h"
#include "my_types.h"
#include <stdint.h>


#define _4800_UBBRH 0x00
#define _4800_UBBRL 0xCF
#define _4800_UBBR concat_bytes(_4800_UBBRH, _4800_UBBRL) 

#define _9600_UBBRH 0x00
#define _9600_UBBRL 0x67
#define _9600_UBBR concat_bytes(_9600_UBBRH, _9600_UBBRL) 

#define _19200_UBBRH 0x00
#define _19200_UBBRL 0x33
#define _19200_UBBR concat_bytes(_19200_UBBRH, _19200_UBBRL)

#define _38400_UBBRH 0x00
#define _38400_UBBRL 0x19
#define _38400_UBBR concat_bytes(_38400_UBBRH, _38400_UBBRL) 

#define _57600_UBBRH 0x00
#define _57600_UBBRL 0x10
#define _57600_UBBR concat_bytes(_57600_UBBRH, _57600_UBBRL) 

#define _115200_UBBRH 0x00
#define _115200_UBBRL 0x08
#define _115200_UBBR concat_bytes(_115200_UBBRH, _115200_UBBRL) 

void	uart_init (int BAUDRATE_H, int BAUDRATE_L);

char	uart_read_byte();
	
void	uart_write_byte(char data);

void	uart_write_string(const char* s);

void	uart_write_num(int num);

void	uart_write_u32num(uint32_t num);

void	uart_write_string_line(const char* s);

#endif // F_UART

#endif // UART_H
