#include "startup.h"
#include "state.h"
#include "events.h"

#include <lib_ex_config.h>
#include <my_stdlib.h>
#include <utils.h>
#include <avr/io.h>
#include <bitman.h>
#include <time_utils_delays.h>
#include <adc/adc.h>
#include <usart/usart.h>
#include <nrf24l01/nrf24l01.h>
#include <spi/spi.h>
#include <debug.h>

uint8_t pressed = 0;

#define ROLETX 1
#define ROLERX 0

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC)

void run()
{
	setup_hardware();
	current_state.current_screen->draw();
	set_as_input(DDRC, 5);
	set_as_input(DDRC, 4);
	set_as_input(DDRC, 3);
	forever
	{
		struct event_t e;
		if(NOTHING_HAPPENED != poll_hardware(&e) && e.emitter != NOTHING_HAPPENED)
		{
			if(current_state.current_screen->event_handler != NULL)
			{
				switch(current_state.current_screen->event_handler(&e))
				{
					case SIGNAL_REDRAW:
					{
						current_state.current_screen->draw();
						break;
					}
					
					default:
					{
						
					}
				}
			}
			
			global_event_handler(&e);
		}
	}
}

#endif // F_EXAMPLES

#if defined(F_NRF24L01)

void rf24test()
{
    uint8_t test_pipe = 0;
    byte* payload = "qwerasd1qwerasd2qwerasd3qwerasd4";
    byte* rx_pld = "11111111111111111111111111111111";
    uint8_t payload_len = strlen(payload);
    memset(rx_pld, '\0', payload_len);
	nrf24l01_conf_t writer;
    nrf24l01_init_config(&writer);
	writer.ce_port= &PORTD;
	writer.ce_pin = PD6;
	writer.ss_port = &PORTC;
	writer.ss_pin = PC4;
    writer.pipe_addr_len = 0;
    usart_write_string_line("Initializing..");
	
	DBG_VALUE("Writer: Init: ", nrf24l01_init(&writer, &DDRD, &DDRC));
	DBG_VALUE("Writer: Speed: ", nrf24l01_get_speed(&writer));
	DBG_VALUE("Writer: Channel: ", nrf24l01_get_channel(&writer));
	DBG_VALUE("Writer: Status: ", nrf24l01_get_status(&writer));
    nrf24l01_set_retries(&writer, 4, 2);
    nrf24l01_set_power_amplifier(&writer, NRF24L01_PA_MAX);
    nrf24l01_enable_dynamic_payload_on_pipe(&writer, test_pipe, NRF24L01_ENABLE);
    nrf24l01_print_addresses(&writer);
	
    usart_write_string_line("Preparing for write:");
    nrf24l01_prepare_for_write_to_addr(&writer, (byte*) "2Node", payload_len,NRF24L01_DISABLE);
    nrf24l01_print_status(&writer);

    
    while(1)
    {
        nrf24l01_set_retries(&writer, 1, 1);
        usart_write_string_line("Writing:");
        nrf24l01_write(&writer, payload, payload_len - 5);
        nrf24l01_print_status(&writer);
        
        usart_write_string_line("Ending writing:");
        nrf24l01_end_writing_keep_irq(&writer);
        nrf24l01_print_status(&writer);
        usart_read_byte();
    }
}

#else
#define rf24test()
#endif

int main()
{
	usart_init(9600);
	
	usart_write_string_line("Starting....");
    tu_delay_ms(500);
	rf24test();
	//run();
	return 0;
}