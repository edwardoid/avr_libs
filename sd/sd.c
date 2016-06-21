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

#include "sd.h"

#ifdef F_SD

int8_t sd_send_command(uint8_t cmd, uint32_t args,port_ptr_t port, uint8_t pin, char* resp)
{
	if(resp != NULL)
	{
		*resp = 0xFF;
	}
		
	spi_write_byte_ss((char)cmd | 0x40, pin, port);
	spi_write_byte_ss((args >> 24) && 0xFF, pin, port);
	spi_write_byte_ss((args >> 16) && 0xFF, pin, port);
	spi_write_byte_ss((args >> 8) && 0xFF, pin, port);
	spi_write_byte_ss((args >> 0) && 0xFF, pin, port);
	
	char r = 0xFF;
	if(cmd == SD_CMD_SEND_IF_COND)
		r = spi_write_byte_ss(0x87, pin, port);
	else
		r = spi_write_byte_ss(0x95, pin, port);
	
	
	usart_write_num(r);
	uint32_t rsp_start = tu_millis();
	do 
	{
		usart_write_num(r);
			
		if(r != 0xFF)
		{
			if(resp != NULL)
				*resp = r;
			return 0;
		}
			
		r = spi_write_byte_ss(0xFF, pin, port);
	}
	while((tu_millis() - rsp_start) < SD_CMD_RESP_MAX_TIMEOUT);
	
	return 1;
}


int8_t sd_init(ddr_ptr_t ddr, port_ptr_t port, uint8_t pin)
{
	if((spi_enabeld() && spi_is_slave()) || !spi_enabeld())
		spi_init_as_master_ex(&pin, 1, ddr, SPI_DIV_CLK_128, SPI_MODE_0);

	uint8_t i = 0;
	for(i = 0; i < 10; ++i)
	{
		spi_write_byte_ss(0xFF, pin, port);
	}
	
	char r = 0x00;
	if(0 != sd_send_command(SD_CMD_GO_IDLE_STATE, 0, port, pin, &r))
		return 1;
	return 0;
}

#endif