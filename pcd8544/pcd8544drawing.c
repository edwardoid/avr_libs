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

#include "pcd8544drawing.h"
#include "my_stdlib.h"
//#include "utils.h"
#include "bitman.h"
#include <avr/pgmspace.h>

#ifdef F_PCD8544
						
void pcd8544_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
	int8_t dx =	my_abs(x2 - x1);
	int8_t step_x =	x1 < x2 ? 1 : -1;
	int8_t dy = -my_abs(y2 - y1);
	int8_t step_y =	y1 < y2 ? 1 : -1;
	
	int16_t err = dx + dy;
	int16_t err_new = 0;
	
	while(1)
	{
		pcd8544_set_pixel(x1, y1, color);
		if (x1 == x2 && y1 == y2)
		break;
		err_new = err << 1;
		if (err_new >= dy)
		{
			err += dy;
			x1 += step_x;
		}
		if (err_new <= dx)
		{
			err += dx;
			y1 += step_y;
		}
	}
}

void pcd8544_draw_horisontal_line(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color)
{
	if(x1 > x2)
		fast_swap(x1, x2);

#ifdef PCD8544_USE_BUFFER
	uint8_t* pos = pcd8544_buffer + PCD8544_ADDR(x1, y);
#endif // PCD8544_USE_BUFFER
	for(; x1 < x2; ++x1)
	{
		#ifdef PCD8544_USE_BUFFER
		if(color)
			set_bit(*pos, y % 8);
		else
			clear_bit(*pos, y % 8);
		++pos;
		#else
			pcd8544_set_pixel(x1, y, color);
		#endif
	}
	
	#ifdef PCD8544_USE_BUFFER
		pcd8544_invalidate(x1, y, x2 - x1, 1);
	#endif // PCD8544_USE_BUFFER
}

void pcd8544_draw_vertical_line(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color)
{
	if(y1 > y2)
		fast_swap(y1, y2);
	for(; y1 < y2; ++y1)
	{
		pcd8544_set_pixel(x, y1, color);
	}
}

void pcd8544_fill_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right, uint8_t color)
{
	if(top > bottom)
		fast_swap(top, bottom);
	
	for(; top < bottom; ++top)
		pcd8544_draw_horisontal_line(left, right, top, color);
}

#ifdef PCD8544_USE_BUFFER

void pcd8544_invert_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right)
{
	if(top > bottom)
		fast_swap(top, bottom);
		
	if(left > right)
		fast_swap(left, right);

	uint8_t l = left;
	for(; top < bottom; ++top)
	{
		for(l = left; l < right; ++l)
		{
			pcd8544_set_pixel(l, top, !pcd8544_get_pixel(l, top));
		}
	}
}

#endif // PCD8544_USE_BUFFER

void pcd8544_draw_circle_helper(int16_t x0, int16_t y0, int16_t r,  uint16_t color, uint8_t quadrant)
{
	int16_t x = -r;
	int16_t y = 0;
	int16_t error = 2 - 2 * r;
	
	if(quadrant & _BV(5))
	{
		pcd8544_draw_horisontal_line(x0 - r, x0 + r, y0, color);
	}
		
	do
	{
		if(quadrant & _BV(4))
		{
			pcd8544_set_pixel(x0 - x,	y0 + y, color);
		}
		
		if(quadrant & _BV(3))
		{
			pcd8544_set_pixel(x0 - y,	y0 - x, color);
		}
		
		if(quadrant & _BV(2))
		{
			pcd8544_set_pixel(x0 + x,	y0 - y, color);
		}
		if(quadrant & _BV(1))
		{
			pcd8544_set_pixel(x0 + y,	y0 + x, color);
		}
		
		if(quadrant & _BV(5))
		{
			pcd8544_draw_horisontal_line(x0 - y, x0 + y, y0 + x, color);
			pcd8544_draw_horisontal_line(x0 - y, x0 + y, y0 - x, color);
		}
		
		r = error;
		if(r <= y)
		{
			error += ++y * 2 + 1;
		}
		if(r > x || error > y)
		{
			error += ++x * 2 + 1;
		}
		
	} while (x < 0);
}

void pcd8544_draw_circle(int16_t x0, int16_t y0, int16_t r,  uint16_t color)
{
	pcd8544_draw_circle_helper(x0, y0, r, color, _BV(1) | _BV(2) | _BV(3) | _BV(4));
}

void pcd8544_fill_circle(int16_t x0, int16_t y0, int16_t r,  uint16_t color)
{
	pcd8544_draw_circle_helper(x0, y0, r, color, _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5));
}

void pcd8544_draw_rounded_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right, uint8_t radius, uint8_t color)
{
	if(bottom < top)
		fast_swap(top, bottom);
	if(left > right)
		fast_swap(left, right);
		
	uint8_t h = bottom - top;
	uint8_t w = right - left;
	radius %= ((w > h) ? (h / 2) : (w / 2));
	
	pcd8544_draw_horisontal_line(left + radius, right - radius, top, color);
	pcd8544_draw_horisontal_line(left + radius, right - radius + 1, bottom, color);
	pcd8544_draw_vertical_line(left, top + radius, bottom - radius + 1, color);
	pcd8544_draw_vertical_line(right, top + radius, bottom - radius, color);
	
	pcd8544_draw_circle_helper(right - radius, top + radius, radius, color, _BV(1));
	pcd8544_draw_circle_helper(left + radius, top + radius, radius, color, _BV(2));
	
	pcd8544_draw_circle_helper(left + radius, bottom - radius, radius, color, _BV(3));
	pcd8544_draw_circle_helper(right - radius, bottom - radius, radius, color, _BV(4));
}

void pcd8544_fill_rounded_rect(uint8_t top, uint8_t left, uint8_t bottom, uint8_t right, uint8_t radius, uint8_t color)
{
	if(bottom < top)
	fast_swap(top, bottom);
	if(left > right)
	fast_swap(left, right);
	
	uint8_t h = bottom - top;
	uint8_t w = right - left;
	radius %= ((w > h) ? (h / 2) : (w / 2));
	
	pcd8544_fill_rect(top + radius, left + radius, bottom - radius + 1, right - radius, color);
	
	pcd8544_fill_rect(top, left + radius, top + radius, right - radius, color);
	pcd8544_fill_rect(bottom - radius, left + radius, bottom + 1, right - radius, color);

	pcd8544_fill_rect(top + radius, left, bottom - radius, left + radius, color);
	pcd8544_fill_rect(top + radius, right - radius, bottom - radius, right, color);
	
	if(radius > 1)
	{
		pcd8544_draw_circle_helper(left + radius, top + radius, radius, color, _BV(5));
		pcd8544_draw_circle_helper(right - radius, top + radius, radius, color, _BV(5));
		pcd8544_draw_circle_helper(left + radius, bottom - radius, radius, color, _BV(5));
		pcd8544_draw_circle_helper(right - radius, bottom - radius, radius, color, _BV(5));
	}
}


void pcd8544_draw_bitmap(uint8_t x, uint8_t y, const byte* bitmap, uint8_t width, uint8_t height, uint8_t color)
{
	uint8_t x_pos = 0;
	uint8_t y_pos = 0;
	
	for(y_pos = 0; y_pos < height; ++y_pos)
	{
		for(x_pos = 0; x_pos < width; ++x_pos )
		{
			if(test_bit(pgm_read_byte(bitmap + x_pos + (y_pos >> 3 ) * width), (y_pos % 8)))
			{
				pcd8544_set_pixel(x + x_pos, y + y_pos , color);
			}
		}
	}	
}

#endif // F_PCD8544