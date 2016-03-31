#define F_CPU 20000000UL

#include <lib_ex_config.h>


#include <rc522/rc522_conf.h>
#include <rc522/rc522.h>
#include <spi/spi.h>
#include <usart/usart.h>
#include <time_utils_delays.h>

int main()
{
	usart_init(9600);
	
	usart_write_string_line("Starting....");
    
#if defined(F_RC522)
	rc522_cfg_t cfg;
	cfg.ss.ddr = &SPI_DDR;
	cfg.ss.port = &SPI_PORT;
	cfg.ss.pin = SPI_SS;
	rc522_init(&cfg);
	forever
	{
		
	}
#endif
}