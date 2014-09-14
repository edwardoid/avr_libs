#include "spi.h"
#include "bitman.h"
#include <avr/io.h>
#include <stdlib.h>

uint8_t spi_set_clock(uint8_t clk)
{
	switch(clk)
	{
		case SPI_DIV_CLK_2:
		{
			  set_bit(SPSR, SPI2X);
			clear_bit(SPCR, SPR1);
			clear_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_4:
		{
			clear_bit(SPSR, SPI2X);
			clear_bit(SPCR, SPR1);
			clear_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_8:
		{
			  set_bit(SPSR, SPI2X);
			clear_bit(SPCR, SPR1);
			  set_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_16:
		{
			clear_bit(SPSR, SPI2X);
			clear_bit(SPCR, SPR1);
			  set_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_32:
		{
			  set_bit(SPSR, SPI2X);
			  set_bit(SPCR, SPR1);
			clear_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_64:
		{
			clear_bit(SPSR, SPI2X);
			  set_bit(SPCR, SPR1);
			clear_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_64_1:
		{
			  set_bit(SPSR, SPI2X);
			  set_bit(SPCR, SPR1);
			  set_bit(SPCR, SPR0);
			break;
		}
		
		case SPI_DIV_CLK_128:
		{
			clear_bit(SPSR, SPI2X);
			  set_bit(SPCR, SPR1);
			  set_bit(SPCR, SPR0);
			break;
		}
		
		default:
		{
			return -1;
		}
	}
	
	return 0;
}

void spi_set_mode(uint8_t mode)
{
	switch(mode)
	{
		case SPI_MODE_3:
		{
			set_bit(SPCR, CPOL);
			set_bit(SPCR, CPHA);
			break;
		}
		case SPI_MODE_2:
		{
			set_bit(SPCR, CPOL);
			clear_bit(SPCR, CPHA);
			break;
		}
		case SPI_MODE_1:
		{
			clear_bit(SPCR, CPOL);
			set_bit(SPCR, CPHA);
			break;
		}
		
		case SPI_MODE_0:
		default:
		{
			clear_bit(SPCR, CPOL);
			clear_bit(SPCR, CPHA);
		}
	}
}

uint8_t	spi_init_as_master_ex(uint8_t* ss_pins, uint8_t count, ddr_ptr_t ss_ddr, uint8_t clk, uint8_t mode)
{
	if((ss_pins == NULL || ss_ddr == NULL) && count != 1)
		return -1;
	
	set_bit(SPI_DDR, SPI_SCK);
	set_bit(SPI_DDR, SPI_MOSI);
	clear_bit(SPI_DDR, SPI_MISO);
	if(count == 1 && ss_pins == NULL && ss_ddr == NULL)
	{
		set_bit(SPI_DDR, SPI_SS);
	}
	else
	{
		uint8_t i = 0;
		for(; i != count; ++i)
			set_bit(*ss_ddr, ss_pins[i]);
	}
		
	set_bit(SPCR, MSTR); // set as master
	
	clk = spi_set_clock(clk);
	if(clk != 0)
		return clk;
	spi_set_mode(mode);
	set_bit(SPCR, SPE); // enable SPI
	return 0;
}

uint8_t	spi_init_as_slave(uint8_t clk, uint8_t mode)
{
	clear_bit(SPI_DDR, SPI_SCK);
	clear_bit(SPI_DDR, SPI_MOSI);
	set_bit(SPI_DDR, SPI_MISO);
	clear_bit(SPI_DDR, SPI_SS);
	clear_bit(SPCR, MSTR);
	clk = spi_set_clock(clk);
	//spi_set_mode(mode);
	set_bit(SPCR, SPE);
	return 0;
}


void spi_write_byte_ss(char data, uint8_t ss_pin, port_ptr_t ss_port)
{
	clear_bit(*ss_port, ss_pin);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	set_bit(*ss_port, ss_pin);

}

void spi_write_ss(char* buff, uint8_t sz, uint8_t ss_pin, port_ptr_t ss_port)
{
	uint8_t i = 0;
	for(; i < sz; ++i)
	{
		spi_write_byte_ss(buff[i], ss_pin, ss_port);
		--sz;
	}
}


char spi_read()
{
	while(!(SPSR & (1<<SPIF)));    // wait until all data is received
    return SPDR;
}
