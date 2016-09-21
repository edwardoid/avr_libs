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

#ifndef SPI_H
#define SPI_H

#include <lib_ex_config.h>

#if defined(F_SPI)

#include <avr/io.h>

#include <stdint.h>
#include <avr/io.h>
#include <my_types.h>
#include <my_stdlib.h>
#include <bitman.h>

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_SCK		PB5
#define SPI_MISO	PB4
#define SPI_MOSI	PB3
#define SPI_SS		PB2

#define SPI_DIV_CLK_0		0
#define SPI_DIV_CLK_2		2
#define SPI_DIV_CLK_4		4
#define SPI_DIV_CLK_8		8
#define SPI_DIV_CLK_16		16
#define SPI_DIV_CLK_32		32
#define SPI_DIV_CLK_64		64
#define SPI_DIV_CLK_64_1	65
#define SPI_DIV_CLK_128		128

#define SPI_MODE_0	0
#define SPI_MODE_1	1
#define SPI_MODE_2	2
#define SPI_MODE_3	3

//#define SPI_USE_BUFFER
#define SPI_BUFFER_LEN	256


/**
	Initializes SPI protocol
	@param ss_pins pins connected to the slaves
	@param count ss_pins array length(slaves count)
	@param ss_ddr DDR for slaves. if NULL DDRB will be used
	@param clk speed. @see spi_set_clock
	@param mode connectin mode. @see spi_set_mode()
	@return 0 if SPI was successfully initialized. uint8_t(-1) if failed.
*/
uint8_t	spi_init_as_master_ex(uint8_t* ss_pins, uint8_t count, ddr_ptr_t ss_ddr, uint8_t clk, uint8_t mode);

/**
	Initialize SPI on AVR with 1 slave connected to the DDRB and PB2 @see spi_init_as_master_ex()
*/
FORCE uint8_t spi_init_as_master(uint8_t clk, uint8_t mode)
{
	return spi_init_as_master_ex(NULL, 1, NULL, clk, mode);
}

void	spi_set_master_bit_first(uint8_t val);

void	spi_set_mode(uint8_t mode);

/**
	Sets connection speed.
	@param clk Clock(speed). Valid values are: SPI_DIV_CLK_0 ... SPI_DIV_CLK_128
*/
uint8_t spi_set_clock(uint8_t clk);

uint8_t	spi_init_as_slave(uint8_t clk, uint8_t mode);

char spi_write_byte_ss(byte data, uint8_t ss_pin, port_ptr_t ss_port);

FORCE char spi_write_byte(byte data)
{
	return spi_write_byte_ss( (data), SPI_SS, & SPI_PORT);
}

byte spi_write_ss(byte* buff, uint8_t sz, uint8_t ss_pin, port_ptr_t ss_port);

FORCE byte spi_write(byte* buff, uint8_t sz)
{
	return spi_write_ss( buff, sz, SPI_SS, & SPI_PORT);
}

byte spi_read();

FORCE uint8_t spi_enabeld()
{
	return test_bit(SPSR, SPE);
}

FORCE uint8_t spi_is_master()
{
	return test_bit(SPSR, MSTR);
}

FORCE uint8_t spi_is_slave()
{
	return !(test_bit(SPSR, MSTR));
}

#endif // F_SPI

#endif // SPI_H
