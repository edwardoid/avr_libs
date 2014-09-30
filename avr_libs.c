#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
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

#define WORKING		0x00
#define SETIING_MAX	0x01
#define SETTING_MIN 0x02

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


#define  MASTER_CODE 
int main()
{
	uart_init(_9600_UBBRH, _9600_UBBRL);

	uart_write_string_line("Starting....");
#ifdef MASTER_CODE
	char i = 5;
	
	if(0 != spi_init_as_master(SPI_DIV_CLK_128, SPI_MODE_0))
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
