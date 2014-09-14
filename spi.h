#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "my_types.h"

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_SCK		PB5
#define SPI_MISO	PB4
#define SPI_MOSI	PB3
#define SPI_SS		PB2

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

uint8_t	spi_init_as_master_ex(uint8_t* ss_pins, uint8_t count, volatile ddr_ptr_t ss_ddr, uint8_t clk, uint8_t mode);
#define spi_init_as_master(clk, mode) spi_init_as_master_ex(NULL, 1, NULL, clk, mode)

void	spi_set_mode(uint8_t mode);
uint8_t spi_set_clock(uint8_t clk);

uint8_t	spi_init_as_slave(uint8_t clk, uint8_t mode);

void spi_write_byte_ss(char data, uint8_t ss_pin, port_ptr_t ss_port);
#define spi_write_byte(data) spi_write_byte_ss( (data), SPI_SS, & SPI_PORT);

void spi_write_ss(char* buff, uint8_t sz, uint8_t ss_pin, port_ptr_t ss_port);
#define spi_write(buff, sz) spi_write_ss( buff, sz, SPI_SS, & SPI_PORT);

char spi_read();

#endif // SPI_H