#include "startup.h"

#include "../config.h"
#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_PCD8544)

#include "../usart.h"
#include "../spi.h"
#include "../pcd8544.h"
#include "../1wire.h"
#include "../adc.h"
#include "state.h"
#include "main_screen.h"

void setup_hardware()
{
	uint16_t v = ADCL;
	usart_write_num((ADCH << 8) + v);
	usart_write_byte('\n');
	
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
	
	adc_divide_by(ADC_DIV_128);
	adc_select_reference(ADC_SRC_AVCC);
	
	pcd8544_init(&cfg, 0x4, 50);
	pcd8544_fill(0);
	pcd8544_render();
	current_state.current_screen = &main_screen;
}

#endif // F_EXAMPLES