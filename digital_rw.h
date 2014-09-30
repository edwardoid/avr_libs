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

#ifndef READ_DIGITAL_VALUE_H
#define READ_DIGITAL_VALUE_H

#include <avr/io.h>

#define read_digital(port, pin) (bit_is_set(port, pin))
#define rd(port, pin) read_digital(port, pin)

#define write_digital_high(port, pin) sbi(port, pin)
#define wdh(port, pin) write_digital_high(port, pin)

#define write_digital_low(port, pin) cbi(port, pin)
#define wdl(port, pin) write_digital_low(port, pin)

#define _HIGH 1
#define _LOW 0

#define digital_toggle(port, pin) (port ^= _BV(pin)
#define write_digital(port, pin, val) (val != _LOW ? wdh(port, pin) : wdl(port, bin)

#endif // READ_DIGITAL_VALUE