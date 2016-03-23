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

#ifndef PCD8544_CALCFONT_H
#define PCD8544_CALCFONT_H

#include "config.h"
#include "my_types.h"
#include <avr/pgmspace.h>

#ifdef F_PCD8544
/*
			* *** *** * * *** *** *** *** *** ***
			*   *   * * * *   *     * * * * * * *
			* *** *** *** *** ***   * *** *** * *
			* *     *   *   * * *   * * *   * * *
			* *** ***   * *** ***   * *** *** *** *
*/
// order: 1 2 3 4 5 6 7 8 9 '.'. Symbols height is 5px


static const byte calc_font [35] PROGMEM = {	0x1F,				//	1	Offset: 0
												0x1D, 0x15, 0x17,	//	2	Offset:	1
												0x15, 0x15, 0x1F,	//	3	Offset:	4
												0x07, 0x04, 0x1F,	//	4	Offset:	7
												0x17, 0x15, 0x1D,	//	5	Offset:	10
												0x1F, 0x15, 0x1D,	//	6	Offset:	13
												0x01, 0x01, 0x1F,	//	7	Offset:	16
												0x1F, 0x15, 0x1F,	//	8	Offset:	19
												0x17, 0x15, 0x1F,	//	9	Offset:	22
												0x1F, 0x11, 0x1F,	//	0	Offset:	25
												0x10,				//	.	Offset: 28
												0x04, 0x0E, 0x04,	//	+	Offset:	29
												0x04, 0x04, 0x04 };	//	-	Offset:	32


#define CALC_FONT_DOT	(0xFFF	- 2)
#define CALC_FONT_PLUS	(0xFFF - 1)
#define CALC_FONT_MINUS	(0xFFF	- 0)

// return x-pos last number. CALC_FONT_DOT is reserved for '.' symbol.
uint8_t pcd8577_print_calc_style_num(uint8_t x, uint8_t y, int16_t num, uint8_t color);

uint8_t pcd8577_print_calc_style_fnum(uint8_t x, uint8_t y, float num, uint8_t color);

uint8_t pcd8577_print_calc_style_num_length_in_px(int16_t num);

#endif // F_PCD8544


#endif /* PCD8544_CALCFONT_H */