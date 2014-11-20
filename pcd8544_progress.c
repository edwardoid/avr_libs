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

#include "pcd8544_progress.h"
#include "pcd8544drawing.h"
#include "pcd8544_calcfont.h"

#ifdef F_PCD8544

void pcd8544_draw_progress(uint8_t x, uint8_t y, uint8_t width, uint8_t value, uint8_t draw_value, uint8_t color)
{
	if(width < 5)
		return;
		
	pcd8544_draw_bitmap(x, y, pcd8544_progress, 2, 8, color);
	width -= 2;
	pcd8544_draw_bitmap(x + width, y, pcd8544_progress + 4, 2, 8, color);
	
	uint8_t val_pos = (value * width) / 100;
	
	uint8_t i = 0;
	for (i = 0; i < width - 2; ++i )
	{
		if(i == val_pos)
			pcd8544_draw_bitmap(x + 2 + i, y, pcd8544_progress + 3, 1, 8, color);
		else
			pcd8544_draw_bitmap(x + 2 + i, y, pcd8544_progress + 2, 1, 8, color);
	}
	
	if(draw_value)
	{
		uint8_t txt = pcd8577_print_calc_style_num_length_in_px(value);
		txt = (width - txt + 4 ) / 2;
		pcd8577_print_calc_style_num(txt, y + 8, value, color);
	}
}

#endif // F_PCD8544
