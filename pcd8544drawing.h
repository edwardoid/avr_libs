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

#include "config.h"
#include "pcd8544.h"

#ifndef PCD8544DRAWING_H
#define PCD8544DRAWING_H


#ifdef F_PCD8544

void pcd8544_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
void pcd8544_draw_horisontal_line(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color); // very fast
void pcd8544_draw_vertical_line(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color);

#define pcd8544_draw_rect(top, left, bottom, right, color)				\
	{																	\
		pcd8544_draw_horisontal_line(left, right, top, color);			\
		pcd8544_draw_horisontal_line(left, right + 1, bottom, color);	\
		pcd8544_draw_vertical_line(left, top, bottom, color);			\
		pcd8544_draw_vertical_line(right, top, bottom, color);			\
	}

void pcd8544_fill_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right, uint8_t color);

void pcd8544_draw_circle(int16_t x0, int16_t y0, int16_t r,  uint16_t color);

void pcd8544_fill_circle(int16_t x0, int16_t y0, int16_t r,  uint16_t color);

void pcd8544_draw_rounded_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right, uint8_t radius, uint8_t color);

void pcd8544_fill_rounded_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right, uint8_t radius, uint8_t color);

void pcd8544_draw_text(int16_t x, int16_t y, const char* str);

void pcd8544_draw_bitmap(uint8_t x, uint8_t y, const byte* bitmap, uint8_t width, uint8_t height, uint8_t color);

#endif // F_PCD8544

#endif /* PCD8544DRAWING_H */