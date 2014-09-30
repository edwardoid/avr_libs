#include "spi.h"
#include "bitman.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

#ifdef SPI_USE_BUFFER
#include <avr/interrupt.h>
#include "time_utils_delays.h"

volatile char spi_buffer[SPI_BUFFER_LEN];
volatile uint8_t spi_bytes_available = 0;
volatile char* spi_read_cursor = NULL;
volatile char* spi_write_cursor = NULL;

#define spi_incr_cursor(cur)									\
	if(cur != NULL || cur + 1 == spi_buffer + SPI_BUFFER_LEN)	\
	{															\
		cur = spi_buffer;										\
	}															\
	else														\
	{															\
		++cur;													\
	}

	
#endif // SPI_USE_BUFFER




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
			clear_bit(SPSR, SPI2X);
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
	set_as_input(SPI_DDR, SPI_MISO);
	if(count == 1 && ss_pins == NULL && ss_ddr == NULL)
	{
		set_as_output(SPI_DDR, SPI_SS);
	}
	else
	{
		uint8_t i = 0;
		for(; i != count; ++i)
			set_as_output(*ss_ddr, ss_pins[i]);
	}
		
	set_bit(SPCR, MSTR); // set as master
	
	clk = spi_set_clock(clk);
	if(clk != 0)
		return clk;
	spi_set_mode(mode);

#ifdef SPI_USE_BUFFER
	tu_counters_init();
	memset((void*)spi_buffer, SPI_BUFFER_LEN, 0x0);
	set_bit(SPCR, SPIE);
	sei();
#endif // SPI_BUFFER_LEN

	set_bit(SPCR, SPE); // enable SPI
	return 0;
}

uint8_t	spi_init_as_slave(uint8_t clk, uint8_t mode)
{
	set_bit(DDRD, PD6);
	set_bit(DDRD, PD7);
	clear_bit(PORTD, PD7);
	clear_bit(PORTD, PD7);
		
	clear_bit(SPI_DDR, SPI_SCK);
	clear_bit(SPI_DDR, SPI_MOSI);
	set_as_output(SPI_DDR, SPI_MISO);
	set_as_input(SPI_DDR, SPI_SS);
	clear_bit(SPCR, MSTR);
	clk = spi_set_clock(clk);
	spi_set_mode(mode);

#ifdef SPI_USE_BUFFER
	memset((void*)spi_buffer, SPI_BUFFER_LEN, 0x0);
	set_bit(SPCR, SPIE);
	sei();
#endif // SPI_USE_BUFFER

	set_bit(SPCR, SPE);
	return 0;
}


void spi_write_byte_ss(char data, uint8_t ss_pin, port_ptr_t ss_port)
{
	if(test_bit(SPCR, MSTR) && ss_port != NULL)
		clear_bit(*ss_port, ss_pin);
	SPDR = data;
	
	while(!(SPSR & (1<<SPIF)));
	
	data = SPDR;
	
	if(test_bit(SPCR, MSTR) && ss_port != NULL)
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

#ifdef SPI_USE_BUFFER
char spi_read()
{
	toggle_bit(PORTD, PD7);
	while(spi_read_cursor == NULL || spi_write_cursor == NULL || spi_bytes_available == 0)
		tu_delay_us(100);
	toggle_bit(PORTD, PD7);
	byte data = *spi_read_cursor;
	--spi_bytes_available;
	spi_incr_cursor(spi_read_cursor);
	return data;
}

#else
char spi_read()
{
	while(!(SPSR & (1<<SPIF)));    // wait until all data is received
    return SPDR;
}
#endif // SPI_BUFFER_LEN

#ifdef SPI_USE_BUFFER
ISR (SPI_STC_vect)
{
	if(spi_write_cursor == NULL)
	{
		spi_write_cursor = spi_buffer;
		spi_read_cursor = spi_buffer;
	}
	
	*spi_write_cursor = SPDR;
	++spi_bytes_available;
	spi_incr_cursor(spi_write_cursor);		
	toggle_bit(PORTD, PD6);
	tu_delay_ms(10);
	toggle_bit(PORTD, PD6);
}

#endif