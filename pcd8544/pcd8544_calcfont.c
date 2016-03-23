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

#include "pcd8544_calcfont.h"
#include "pcd8544drawing.h"

#include "config.h"
#ifdef F_PCD8544


uint8_t pcd8577_print_calc_style_num_length_in_px(int16_t num)
{
	uint8_t lenght = 0;
	if(num == 0)
		lenght += 3;
	if(num == CALC_FONT_DOT)
	{
		return 2;
	}
	if(num == CALC_FONT_PLUS)
	{
		return 4;
	}
	if(num == CALC_FONT_MINUS)
	{
		return 4;
	}
	
	if(num < 0)
	{
		lenght += 4;
		num = -num;
	}
	
	while(num != 0)
	{
		if(num  % 10 == 1)
			lenght += 2;
		else
			lenght += 4;
		
		num /= 10;
	}
	return lenght;
}

uint8_t pcd8577_print_calc_style_num(uint8_t x, uint8_t y, int16_t num, uint8_t color)
{
	uint8_t lenght = pcd8577_print_calc_style_num_length_in_px(num);
	uint8_t pos = lenght;
	lenght += x;
	if(num == 0)
	{
		pcd8544_draw_bitmap(x, y, calc_font + 25, 3, 5, color);
		return lenght;
	}
	else if(num == CALC_FONT_DOT)
	{
		pcd8544_draw_bitmap(x, y, calc_font + 28, 1, 5, color);
		return lenght;
	}
	else if(num == CALC_FONT_PLUS)
	{
		pcd8544_draw_bitmap(x, y, calc_font + 29, 3, 5, color);
		return lenght;
	}
	else if(num == CALC_FONT_MINUS)
	{
		pcd8544_draw_bitmap(x, y, calc_font + 32, 3, 5, color);
		return lenght;
	}
	
	if(num < 0)
	{
		pcd8544_draw_bitmap(x, y, calc_font + 32, 3, 5, color);
		num = -num;
	}
	
	while(num != 0)
	{
		uint8_t	digit = num % 10;
		num /= 10;
		if(digit == 1)
		{
			pos -= 2;
			pcd8544_draw_bitmap(x + pos, y, calc_font, 1, 5, color);
		}	
		else
		{
			pos -= 4;
			if(digit != 0)
				pcd8544_draw_bitmap( x + pos, y, calc_font + (digit - 2) * 3 + 1, 3, 5, color );
			else
				pcd8544_draw_bitmap(x + pos, y, calc_font + 25, 3, 5, color );
		}
	}
	
	return lenght;
}

uint8_t pcd8577_print_calc_style_fnum(uint8_t x, uint8_t y, float num, uint8_t color)
{
	int8_t num_after_dot = (int16_t)(num * 10) % 10;
	if(num_after_dot < 0)
		num_after_dot = -num_after_dot;
	x = pcd8577_print_calc_style_num(x , y, num, color);
	x = pcd8577_print_calc_style_num(x , y, CALC_FONT_DOT, color);
	x = pcd8577_print_calc_style_num(x , y, num_after_dot, color);

	return x + 3;
}

#endif // F_PCD8544