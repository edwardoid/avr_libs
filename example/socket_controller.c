#include "startup.h"
#include "state.h"
#include "events.h"

#include "../config.h"
#include "../my_stdlib.h"
#include "../utils.h"
#include <avr/io.h>
#include "../bitman.h"
#include "../time_utils_delays.h"
#include "../adc.h"
#include "../usart.h"
#include "../nrf24l01.h"
#include "../spi.h"
#include "../debug.h"

uint8_t pressed = 0;

#define ROLETX 1
#define ROLERX 0

#ifdef F_EXAMPLES
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

void rf24test()
{
    uint8_t test_pipe = 0;
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
    nrf24l01_set_power_amplifier(&writer, NRF24L01_PA_MAX);
    nrf24l01_enable_dynamic_payload_on_pipe(&writer, test_pipe, NRF24L01_ENABLE);
    nrf24l01_print_addresses(&writer);
	
	nrf24l01_conf_t reader;
	nrf24l01_init_config(&reader);
    reader.ce_port= &PORTD;
	reader.ce_pin = PD5;
	reader.ss_port = &PORTC;
	reader.ss_pin = PC3;
    reader.pipe_addr_len = 0;
	usart_write_string_line("Initializing..");
	
	DBG_VALUE("Reader: Init: ", nrf24l01_init(&reader, &DDRD, &DDRC));
	DBG_VALUE("Reader: Speed: ", nrf24l01_get_speed(&reader));
	DBG_VALUE("Reader: Channel: ", nrf24l01_get_channel(&reader));
	DBG_VALUE("Reader: Status: ", nrf24l01_get_status(&reader));
    nrf24l01_print_addresses(&reader);
    
    while(1)
    {
        usart_write_string_line("Preparing for read:");
        nrf24l01_prepare_for_read(&reader, test_pipe);
        nrf24l01_print_status(&reader);
        // reader -> writer
        usart_write_string_line("Preparing for write:");
        nrf24l01_prepare_for_write(&writer, test_pipe, NRF24L01_DISABLE);
        nrf24l01_print_status(&writer);
        
        
        usart_write_string_line("Writing:");
        nrf24l01_write(&writer, "Hello!", strlen("Hello!"));
        nrf24l01_print_status(&writer);
        
        usart_write_string_line("Ending writing:");
        nrf24l01_end_writing(&writer);
        nrf24l01_print_status(&writer);
        
        byte rcv = 0;
        
        uint8_t pp = 0xFF;
        uint8_t i = 0;
        while(!nrf24l01_data_available(&reader, &pp))
        {
            if (i == 80)
            {
                i = 0;
                usart_write_byte('\n');
                break;
            }
            tu_delay_ms(100);
            usart_write_byte('.');
            
            ++i;
        }
        DBG_VALUE("\nAvailable on ", pp);
        usart_write_string_line("Receiving...");
        rcv = nrf24l01_read_byte(&reader, test_pipe);
        DBG_VALUE("Reader Status: ", nrf24l01_get_status(&reader));
        DBG_VALUE("\nGot", rcv);
        usart_write_string_line(rcv);
        usart_read_byte();
    }
}

int main()
{
	usart_init(9600);
	
	usart_write_string_line("Starting....");
    tu_delay_ms(500);
	rf24test();
	//run();
	return 0;
}