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

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <stdint.h>

typedef char    byte;
typedef volatile uint8_t* ddr_ptr_t;
typedef volatile uint8_t* port_ptr_t;
typedef volatile uint8_t* pin_ptr_t;
typedef volatile uint8_t* register_ptr_t;

typedef struct
{
	ddr_ptr_t	ddr;
	port_ptr_t	port;
	uint8_t		pin;
} pin_cfg_t;

#endif // MY_TYPES_H
