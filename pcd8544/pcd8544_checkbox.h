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

#ifndef PCD8544_CHECKBOX_H
#define PCD8544_CHECKBOX_H

#include "pcd8544.h"

#ifdef F_PCD8544

#include <inttypes.h>

void pcd8544_draw_checkbox(uint8_t x, uint8_t y, const char* string,  uint8_t checked);

#endif // F_PCD8544

#endif /* PCD8544_CHECKBOX_H */