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
#include "uart.h"
#include "bitman.h"
#include "1wire.h"
#include <stdio.h>
#include "time_utils_delays.h"
#include "pwm.h"
#include "spi.h"
#include "sd.h"
#include <stdlib.h>
#include "time_utils_counters.h"
#include <stdint.h>
#include <string.h>
#include "pcd8544.h"

#define WORKING		0x00
#define SETIING_MAX	0x01
#define SETTING_MIN 0x02

#ifdef F_1WIRE
double get_temp_sensor(ow_conf* cfg, uint8_t num)
{
	if(!ow_reset(cfg))
	{
		uart_write_string_line("Initial reset failed!");
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

int main()
{
	uart_init(_9600_UBBRH, _9600_UBBRL);

	uart_write_string_line("Starting....");

	pcd8544_cfg_t cfg;
	cfg.dc.ddr = &DDRB;
	cfg.dc.port = &PORTB;
	cfg.dc.pin = PB1;
	
	cfg.ss.ddr = &SPI_DDR;
	cfg.ss.port = &SPI_PORT;
	cfg.ss.pin = SPI_SS;
	
	cfg.rst.ddr = & DDRB;
	cfg.rst.port = & PORTB;
	cfg.rst.pin = PB0;
	
	pcd8544_init(&cfg, 0x4, 0x40);
	pcd8544_fill(0);
	int16_t i  = 0;
	pcd8544_draw_text(0, 0, "Hello, Luskin!");
	pcd8544_draw_line(0, 0, 10, 10, 1);
	while(1 || i)
	{
		pcd8544_render();
		tu_delay_ms(100);
	}
}

#undef SPI_TEST
#ifdef SPI_TEST
#define  MASTER_CODE 
int main()
{
	uart_init(_9600_UBBRH, _9600_UBBRL);

	uart_write_string_line("Starting....");
#ifdef MASTER_CODE
	char i = 5;
	
	if(0 != spi_init_as_master(SPI_DIV_CLK_4, SPI_MODE_0))
		uart_write_string_line("SPI initialization failed");

	while(1)
	{
		//uart_write_string_line("Writing");
		spi_write_byte(i + 10);
		tu_delay_ms(1000);
		++i;
	}
#else
	if(0 != spi_init_as_slave(SPI_DIV_CLK_128, SPI_MODE_0))
		uart_write_string_line("SPI initialization failed");
	
	while(1)
	{
		int i = spi_read();
		uart_write_byte('?');
		char msg[256] = {};
		memset(msg, 0, 256);
		sprintf(msg, "Got: %d\n", i);
		uart_write_string_line(msg);
	}
	
#endif
	return 0;
}

#endif // SPI_TEST