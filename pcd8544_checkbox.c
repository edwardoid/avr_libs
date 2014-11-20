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

#include "pcd8544_checkbox.h"
#include "pcd8544drawing.h"
#include "pcd8544_tiny_font.h"
#include "pcd8544_default_font.h"

#ifdef F_PCD8544

void pcd8544_draw_checkbox(uint8_t x, uint8_t y, const char* string,  uint8_t checked)
{
	pcd8544_draw_rect(y + 1, x + 1, y + 8, x + 8, 1);
	if(checked)
		pcd8544_fill_rect(y + 3, x + 3, y + 7, x + 7, 1);
		
	pcd8544_print_str(x + 11, y + 1, string, &default_font, 1, ALIGN_LEFT);
}

#endif // F_PCD8544