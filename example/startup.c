#include "startup.h"

#include "../usart.h"
#include "../spi.h"
#include "../pcd8544.h"
#include "../1wire.h"
#include "state.h"
#include "main_screen.h"

void setup_hardware()
{
	usart_init(9600);
	
	usart_write_string_line("Starting....");
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
	
	pcd8544_init(&cfg, 0x4, 50);
	pcd8544_fill(0);
	pcd8544_render();
	current_state.current_screen = &main_screen;
}