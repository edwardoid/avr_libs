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
    uint8_t test_pipe = 1;
	nrf24l01_conf_t first;
	first.ce_port= &PORTD;
	first.ce_pin = PD6;
	first.ss_port = &PORTC;
	first.ss_pin = PC4;
    first.pipe_addr_len = 0;
    first.payload_lengths[4] = 5;
	usart_write_string_line("Initializing..");
	
	DBG_VALUE("First: Init: ", nrf24l01_init(&first, &DDRD, &DDRC));
	DBG_VALUE("First: Speed: ", nrf24l01_get_speed(&first));
	DBG_VALUE("First: Channel: ", nrf24l01_get_channel(&first));
	DBG_VALUE("First: Status: ", nrf24l01_get_status(&first));
    first.pipe_addrs[3] = 0xBB;
    nrf24l01_print_addresses(&first);
	
	nrf24l01_conf_t second;
	second.ce_port= &PORTD;
	second.ce_pin = PD5;
	second.ss_port = &PORTC;
	second.ss_pin = PC3;
    second.pipe_addr_len = 0;
	second.pipe_addrs[3] = 0xBB;
    second.payload_lengths[4] = 5;
    usart_write_string_line("Initializing..");
	
	DBG_VALUE("Second: Init: ", nrf24l01_init(&second, &DDRD, &DDRC));
	DBG_VALUE("Second: Speed: ", nrf24l01_get_speed(&second));
	DBG_VALUE("Second: Channel: ", nrf24l01_get_channel(&second));
	DBG_VALUE("Second: Status: ", nrf24l01_get_status(&second));
    nrf24l01_print_addresses(&second);
    
    nrf24l01_conf_t* writer = &second;
    nrf24l01_conf_t* reader = &first;
    while(1)
    {
        DBG_VALUE("Preparing for read: ", nrf24l01_prepare_for_read(&reader, test_pipe, 0));
        
        // Second -> First
        usart_write_string_line("Writing...");
        DBG_VALUE("Prepating to write: ", nrf24l01_prepare_for_write(writer, test_pipe));
        DBG_VALUE("Write result: ", nrf24l01_write_ack(writer, test_pipe, "Hello", 5));
        byte rcv = 0;
        
        uint8_t pp = 0xFF;
        uint8_t i = 0;
        while(!nrf24l01_data_available(reader, &pp))
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
        rcv = nrf24l01_read_byte(reader, test_pipe);
        DBG_VALUE("Reader Status: ", nrf24l01_get_status(reader));
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