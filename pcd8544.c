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

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

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
	uint16_t addr = x + (y >> 3) * PCD8544_WIDTH;
	pcd8544_send_command(PCD8544_SETXADDR | (addr % PCD8544_WIDTH));
	pcd8544_send_command(PCD8544_SETYADDR | (addr / PCD8544_WIDTH));
	
	if (color)
		pcd8544_send_data( 1<< (y % 8)); //"black pixel"
	else
		pcd8544_send_data(0); //"remove black pixel"
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
}

void pcd8544_set_contrast(uint8_t contrast)
{
	if (contrast > 0x7f)
		contrast = 0x7f;
	pcd8544_send_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
	pcd8544_send_command( PCD8544_SETVOP | contrast);
	pcd8544_send_command(PCD8544_FUNCTIONSET);
}

void pcd8544_fill(uint8_t c)
{
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
	pcd8544_send_command(PCD8544_SETXADDR | x);
	pcd8544_send_command(PCD8544_SETYADDR | y);

	while (*str != '\0')
	{
		pcd8544_send_data(pcd8544_font[(*str) - 0x20][0]);
		pcd8544_send_data(pcd8544_font[(*str) - 0x20][1]);
		pcd8544_send_data(pcd8544_font[(*str) - 0x20][2]);
		pcd8544_send_data(pcd8544_font[(*str) - 0x20][3]);
		pcd8544_send_data(pcd8544_font[(*str) - 0x20][4]);
		pcd8544_send_data(0x00);
		++str;
	}
}