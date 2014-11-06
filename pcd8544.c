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

#include "pcd8544.h"
#include "time_utils_delays.h"
#include "spi.h"
#include "bitman.h"
#include "debug.h"
#include <string.h>
#include "pcd8544font.h"


#ifdef PCD8544_USE_BUFFER

uint8_t pcd8544_buffer[PCD8544_WIDTH * PCD8544_HEIGHT / 8];

static int16_t	invalid_rc_t	= 0;
static int16_t	invalid_rc_l	= 0;
static int16_t invalid_rc_b = PCD8544_HEIGHT;
static int16_t invalid_rc_r = PCD8544_WIDTH;

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void pcd8544_invalidate(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	invalid_rc_t = min(invalid_rc_t, y);
	invalid_rc_l = min(invalid_rc_l, x);
	invalid_rc_b = max(invalid_rc_b, y + h);
	invalid_rc_r = max(invalid_rc_r, x + w);
}
	
	
#define PCD8544_ADDR(x, y) (PCD8544_WIDTH * ((y) / 8)  + (x))
#endif // PCD8544_USE_BUFFER(y  /  5)

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK		0x0
#define PCD8544_DISPLAYNORMAL		0x4
#define PCD8544_DISPLAYALLON		0x1
#define PCD8544_DISPLAYINVERTED		0x5
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80

static port_ptr_t	pcd8544_ss_port = 0;
static uint8_t		pcd8544_ss_pin = 0;
static port_ptr_t	pcd8544_dc_port = 0;
static uint8_t		pcd8544_dc_pin = 0;

void pcd8544_send(uint8_t dc, uint8_t data)
{
	if(dc)
		set_high(*pcd8544_dc_port, pcd8544_dc_pin);
	else
		set_low(*pcd8544_dc_port, pcd8544_dc_pin);
	
	spi_write_byte_ss(data, pcd8544_ss_pin, pcd8544_ss_port);
}

#define  pcd8544_send_command(command) pcd8544_send(0, command)
#define  pcd8544_send_data(data) pcd8544_send(1, data)

void pcd8544_set_pixel(int16_t x, int16_t y, uint16_t color)
{
#ifdef PCD8544_USE_BUFFER
	pcd8544_invalidate(x, y, 1, 1);
	if(color)
		set_bit(pcd8544_buffer[PCD8544_ADDR(x, y)], y % 8);
	else
		clear_bit(pcd8544_buffer[PCD8544_ADDR(x, y)], y % 8);
#else
	uint16_t addr = x + (y >> 3) * PCD8544_WIDTH;
	pcd8544_send_command(PCD8544_SETXADDR | (addr % PCD8544_WIDTH));
	pcd8544_send_command(PCD8544_SETYADDR | (addr / PCD8544_WIDTH));
	
	if (color)
		pcd8544_send_data( 1 << (y % 8)); //"black pixel"
	else
		pcd8544_send_data(0); //"remove black pixel"
#endif // PCD8455_USE_BUFFER
}

void pcd8544_init(pcd8544_cfg_t* cfg, uint8_t bias, uint8_t contrast)
{
	pcd8544_ss_port = cfg->ss.port;
	pcd8544_ss_pin = cfg->ss.pin;
	
	pcd8544_dc_port = cfg->dc.port;
	pcd8544_dc_pin = cfg->dc.pin;
		
	if(!spi_is_master())
		spi_init_as_master_ex(& (cfg->ss.pin), 1, cfg->ss.ddr, SPI_DIV_CLK_4, SPI_MODE_0);
	else
		set_as_output(*(cfg->ss.ddr), cfg->ss.pin);
	
	//spi_set_master_bit_first(1);
		
	set_as_output(*(cfg->dc.ddr), pcd8544_dc_pin);
	set_as_output(*(cfg->rst.ddr), cfg->rst.pin);
	
	tu_delay_ms(10);
	set_low(*(cfg->rst.port), cfg->rst.pin);
	tu_delay_ms(110);
	set_high(*(cfg->rst.port), cfg->rst.pin);
	tu_delay_ms(64);	
	
	pcd8544_send_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	pcd8544_send_command(PCD8544_SETBIAS | bias);

	if (contrast > 0x7f)
		contrast = 0x7f;
		
	pcd8544_send_command(PCD8544_SETVOP | contrast);
	pcd8544_send_command(PCD8544_FUNCTIONSET);
	pcd8544_send_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
	
#ifdef PCD8544_USE_BUFFER
	pcd8544_render();
#endif // PCD8544_USE_BUFFER
	pcd8544_fill(0);
}

void pcd8544_set_contrast(uint8_t contrast)
{
	if (contrast > 0x7f)
		contrast = 0x7f;
	pcd8544_send_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
	pcd8544_send_command(PCD8544_SETVOP | contrast);
	pcd8544_send_command(PCD8544_FUNCTIONSET);
}

void pcd8544_fill(uint8_t c)
{
#ifdef PCD8544_USE_BUFFER
	memset(pcd8544_buffer, c, PCD8544_WIDTH * PCD8544_HEIGHT / 8);
	pcd8544_invalidate(0, 0, PCD8544_WIDTH, PCD8544_HEIGHT);
#else
	int8_t y = 0;
	for(y = 0; y < PCD8544_WIDTH; ++y)
	{
		pcd8544_send_data(c);
		pcd8544_send_data(c);
		pcd8544_send_data(c);
		pcd8544_send_data(c);
		pcd8544_send_data(c);
		pcd8544_send_data(c);		
	}
#endif // PCD8544_USE_BUFFER
}

#define fast_swap(x, y)	(x) ^=	(y);		\
						(y)	^=	(x);		\
						(x) ^=	(y);

#define my_abs(v) ((v > 0) ? (v) : -(v))
						
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

void pcd8544_draw_text(int16_t x, int16_t y, const char* str)
{
	if(str == NULL)
		return;
		
#ifdef PCD8544_USE_BUFFER
	uint8_t x_pos = x;
#else
	pcd8544_send_command(PCD8544_SETXADDR | x);
	pcd8544_send_command(PCD8544_SETYADDR | y);
#endif // PCD8544_USE_BUFFER

	while (*str != '\0')
	{
#ifdef PCD8544_USE_BUFFER
		uint8_t* cursor = pcd8544_buffer + PCD8544_ADDR(x_pos, y);
		*cursor	|= pgm_read_byte(&pcd8544_font[(*str) - 0x20][0]);
		++cursor;
		*cursor	|= pgm_read_byte(&pcd8544_font[(*str) - 0x20][1]);
		++cursor;
		*cursor	|= pgm_read_byte(&pcd8544_font[(*str) - 0x20][2]);
		++cursor;
		*cursor	|= pgm_read_byte(&pcd8544_font[(*str) - 0x20][3]);
		++cursor;
		*cursor	|= pgm_read_byte(&pcd8544_font[(*str) - 0x20][4]);
		x_pos += 6;
#else // PCD8544_USE_BUFFER
		pcd8544_send_data(pgm_read_byte(&pcd8544_font[(*str) - 0x20][0]));
		pcd8544_send_data(pgm_read_byte(&pcd8544_font[(*str) - 0x20][1]));
		pcd8544_send_data(pgm_read_byte(&pcd8544_font[(*str) - 0x20][2]));
		pcd8544_send_data(pgm_read_byte(&pcd8544_font[(*str) - 0x20][3]));
		pcd8544_send_data(pgm_read_byte(&pcd8544_font[(*str) - 0x20][4]));
		pcd8544_send_data(0x00);
#endif // PCD8544
		++str;
	}
	
#ifdef PCD8544_USE_BUFFER
	pcd8544_invalidate(x, y, x_pos - x , 8 );
#endif // PCD8544_USE_BUFFER
}

#ifdef PCD8544_USE_BUFFER
void pcd8544_render()
{
	if(invalid_rc_t >= invalid_rc_b || invalid_rc_l >= invalid_rc_r)
		return;
		
	int16_t min_column = invalid_rc_t >> 3;
	int16_t max_column = invalid_rc_b >> 3;
	++max_column;
	if(max_column == 5)
		max_column = 4;
	for(; min_column <= max_column; ++min_column)
	{
		pcd8544_send_command(PCD8544_SETYADDR | min_column);
		uint16_t i = invalid_rc_l;
		pcd8544_send_command(PCD8544_SETXADDR | (invalid_rc_l));
		for(; i < invalid_rc_r; ++i)
		{
			pcd8544_send_data(pcd8544_buffer[PCD8544_ADDR(i, min_column * 8)]);
		}
	}
	invalid_rc_t = PCD8544_HEIGHT;
	invalid_rc_l = PCD8544_WIDTH;
	invalid_rc_r = 0;
	invalid_rc_b = 0;
}

#endif //	 PCD8544_USE_BUFFER