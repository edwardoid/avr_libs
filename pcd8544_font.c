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

#include "pcd8544_font.h"
#include "my_stdlib.h"
#include <avr/pgmspace.h>
#include "uart.h"
#include "pcd8544drawing.h"

#ifdef F_PCD8544

uint16_t	pcd8544_get_symbol_offset(const struct font_t* font, char symbol)
{
	if(font == NULL || font->font == NULL || font->first > symbol)
		return NULL;
		
	uint8_t offset = 0;
	uint8_t diff = symbol - font->first;
	
	const uint8_t first_byte = pgm_read_byte(font->widths);
	
	if(first_byte == 255)
	{
		return (diff) * pgm_read_byte(font->widths + 1);
	}
	
	const uint8_t* width = font->widths;
	while(diff-- != 0)
	{
		offset += pgm_read_byte(width);
		++width;
	}
	
	return offset;
}

uint8_t	pcd8544_get_symbol_width(const struct font_t* font, char symbol)
{	
	if(font == NULL || font->font == NULL || font->first > symbol)
		return 0;
	
	const uint8_t first_byte = pgm_read_byte(font->widths);
	
	if(first_byte == 255)
		return pgm_read_byte(font->widths + 1);
	
	return pgm_read_byte(font->widths + symbol - font->first);
}

uint8_t pcd8544_get_str_length_in_px(const char* str, const struct font_t* font)
{
	uint8_t len = 0;
	while (*str != '\0')
	{
		len += pcd8544_get_symbol_width(font, *str) + 1;
		++str;
	}
	return len - 1;
}

uint8_t pcd8544_print_str(uint8_t x, uint8_t y, const char* str, const struct font_t* font, uint8_t color, uint8_t align)
{
	const uint8_t len = pcd8544_get_str_length_in_px(str, font);
	if(align == ALIGN_CENTER)
	{
		x = (PCD8544_WIDTH - len) >> 1;
	}
	else if(align == ALIGN_RIGHT)
	{
		x = (PCD8544_WIDTH - len);
	}
	
	x %= PCD8544_WIDTH;
	
	while (*str != '\0')
	{
		uint16_t offset = pcd8544_get_symbol_offset(font, *str);
		uint8_t width = pcd8544_get_symbol_width(font, *str);
		if(x + width > PCD8544_WIDTH)
		{
			x = 0;
			y += font->height + 1;
		}
		
		pcd8544_draw_bitmap(x, y, font->font + offset, width, font->height, color);
		
		x += width + 1;

		if(x > PCD8544_WIDTH)
		{
			x = 0;
			y += font->height + 1;
		}
		++str;
	}
	
	return x - 1;
}

#endif // F_PCD8544