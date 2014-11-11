#include "startup.h"

#include "../uart.h"
#include "../spi.h"
#include "../pcd8544.h"
#include "state.h"
#include "main_screen.h"

void setup_hardware()
{
	uart_init(_9600_UBBRH, _9600_UBBRL);
	
	//uart_write_string_line("Starting....");
	
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
	pcd8544_render();
	current_state.current_screen = &main_screen;
}