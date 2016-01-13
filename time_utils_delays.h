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

#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <util/delay.h>
#define LOOP_CYCLES 8 				//Number of cycles that the loop takes

#define tu_delay_us(num) tu_delay_cycles(num / ( 8 * ( 1 / (F_CPU / 1000000.))))
#define tu_delay_ms(num) tu_delay_cycles(num / ( 8 * ( 1 / (F_CPU / 1000.   ))))

#include <inttypes.h>
void tu_delay_cycles(uint32_t cycles);

#endif // TIME_UTILS_H