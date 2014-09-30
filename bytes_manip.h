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

#ifndef BYTES_MANIP_H
#define BYTES_MANIP_H

#include <avr/sfr_defs.h>

#define concat_bytes(high, low) (((high) << 8) | (low))
#define high_byte(dword) (((dword) >> 8) & 0xFF)
#define low_byte(dword) ((dword) & 0xFF)

#endif // BYTES_MANIP_H