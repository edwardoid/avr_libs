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

#ifndef PCD8544_H
#define PCD8544_H

#define PCD8544_WIDTH	84
#define PCD8544_HEIGHT	48

#define PCD8544_BLACK 1
#define PCD8544_WHITE 0


#define PCD8544_ADDR(x, y) (PCD8544_WIDTH * ((y) / 8)  + (x))

#include "my_types.h"

/* Enabling buffering. This feature will allow XOR-ing display data. */
#define PCD8544_USE_BUFFER 1

typedef struct
{
	pin_cfg_t	ss;
	pin_cfg_t	dc;
	pin_cfg_t	rst;
} pcd8544_cfg_t;

void pcd8544_init(	pcd8544_cfg_t* cfg, uint8_t bias, uint8_t contrast);
void pcd8544_set_contrast(uint8_t contrast);
void pcd8544_fill(uint8_t c);
#define pcd8544_clear() pcd8544_fill(0)
void pcd8544_set_pixel(int16_t x, int16_t y, uint16_t color);

#ifdef PCD8544_USE_BUFFER

uint8_t pcd8544_buffer[PCD8544_WIDTH * PCD8544_HEIGHT / 8];

void	pcd8544_render();
void	pcd8544_invalidate(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
uint8_t	pcd8544_get_pixel(int16_t x, int16_t y);
#else
#define pcd8544_render()
void	pcd8544_invalidate(x, y, w, h)
#endif // PCD8544_USE_BUFFER

#endif /* PCD8544_H */