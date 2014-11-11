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

#ifndef PCD8544_FONT_H
#define PCD8544_FONT_H

#include "config.h"

#ifdef F_PCD8544

#define ALIGN_LEFT		0
#define ALIGN_CENTER	1
#define ALIGN_RIGHT		2

#include "my_types.h"

struct font_t
{
	uint8_t			first;
	uint8_t			count;
	const byte*		font;
	const uint8_t*  widths;	// if all symbols are fixed width this pointer should point to a array with following format [255, w]
	uint8_t			height;
};


uint16_t	pcd8544_get_symbol_offset(const struct font_t* font, char symbol);
uint8_t	pcd8544_get_symbol_width(const struct font_t* font, char symbol);
uint8_t pcd8544_get_str_length_in_px(const char* str, const struct font_t* font);

uint8_t pcd8544_print_str(uint8_t x, uint8_t y, const char* str, const struct font_t* font, uint8_t color, uint8_t align);

#endif // F_PCD8544

#endif // PCD8544_FONT_H