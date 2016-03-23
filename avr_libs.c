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


#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "config.h"
#include <usart/usart.h>
#include "bitman.h"
#include <1wire/1wire.h>
#include "time_utils_delays.h"
#include <pwm/pwm.h>
#include <spi/spi.h>
#include <sd/sd.h>
#include "time_utils_counters.h"
#include <stdint.h>
#include <pcd8544/pcd8544.h>
#include <pcd8544/pcd8544drawing.h>
#include <pcd8544/pcd8544_calcfont.h>
#include <pcd8544/pcd8544_progress.h>
#include <pcd8544/pcd8544_font.h>
#include <pcd8544/pcd8544_tiny_font.h>

#define WORKING		0x00
#define SETIING_MAX	0x01
#define SETTING_MIN 0x02

const char tst_bmp[] PROGMEM = { 1, 2, 4, 8 };

#ifdef F_1WIRE

double get_temp_sensor(ow_conf_t* cfg, uint8_t num)
{
	if(!ow_reset(cfg))
	{
		usart_write_string_line("Initial reset failed!");
		return 999.999;
	}
	
	uint8_t addr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		
	if(!ow_search(addr, cfg))
	{
		return 999.999;
	}
	
	return ow_read_temperature_ds18x2x(cfg, addr);
}

#endif // F_1WIRE