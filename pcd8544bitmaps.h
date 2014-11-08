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

#ifndef PCD8544_BITMAPS
#define PCD8544_BITMAPS

#include <avr/pgmspace.h>

// width 2, height
static const byte thermometer[2 * 7] PROGMEM = {
	0x00, 0x00, 0xFF, 0x01, 0xFF, 0x00, 0x00,
	0x30, 0x78, 0xFF, 0xF8, 0xFF, 0x78, 0x30
};	

static const byte celsius[8] PROGMEM = {
	0x02, 0x05, 0x02, 0x00, 0x3E, 0x41, 0x41, 0x22
};

static const byte on_off[2 * 9] PROGMEM = {
	0x80, 0x60, 0x20, 0x00, 0xF8, 0x00, 0x20, 0x60, 0x80, /* 0xF8 is on/off sign */
	0x03, 0x0C, 0x08, 0x10, 0x10, 0x10, 0x08, 0x0C, 0x03
};

static const byte thermo [] PROGMEM = {
	0x00, 0x00, 0xFE, 0xFF, 0xFE, 0x00, 0x00, 0x38, 0x7C, 0xFF, 0xFF, 0xFF, 0x7C, 0x38, 
};


#endif /* PCD8544_BITMAPS */