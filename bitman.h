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

#ifndef _BV
#define _BV(bit) (1 << bit)
#endif // _BV

#define set_bit(dst, bit) ((dst) |= _BV(bit))
#define set_high(dst, bit) set_bit((dst), bit)
#define clear_bit(dst, bit) ((dst) &= ~ _BV((bit)))
#define set_low(dst, bit) clear_bit((dst), (bit))

#define check_bit(dst, bit) ((dst) & _BV((bit)))
#define toggle_bit(dst, bit) ((dst) ^= _BV((bit)))
#define concat_bytes(high, low) (((high) << 8) | (low))
#define test_bit(src, num) ((src) & _BV(num))

#define set_as_input(ddr, pin) clear_bit(ddr, pin)
#define set_as_output(ddr, pin) set_bit(ddr, pin)