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

#include <lib_ex_config.h>

#if defined(F_EXAMPLES)

#include <usart/usart.h>
#include <avr/io.h>
#include <nrf24l01/nrf24l01.h>
#include <debug.h>
#include <time_utils_delays.h>

#define abs(x) ((x > 0) ? (x) : (-(x)))

uint8_t test_pipe = 2;

uint8_t write_mode = 1;

void write(nrf24l01_conf_t* writer, char* data, uint8_t len)
{
    usart_write_string_line("Preparing for write:");
    nrf24l01_prepare_for_write(writer, test_pipe, NRF24L01_DISABLE);
    
    usart_write_string_line("Writing:");
    nrf24l01_write(writer, data, len);
    
    usart_write_string_line("Finishing");
    nrf24l01_end_writing(writer);
    usart_write_string_line("Done");
}

uint8_t read(nrf24l01_conf_t* reader, char* data, uint8_t len)
{
    nrf24l01_prepare_for_read(reader, test_pipe);
    tu_delay_ms(20000UL);
    usart_write_string_line("Preparing for read:");
    nrf24l01_print_status(reader);
    uint8_t pos = 0;
    uint8_t pp = 0xFF;
    while(nrf24l01_data_available(reader, &pp));
    nrf24l01_read_payload(reader, data, 4);
    usart_write_byte('\n');
    return pos;    
}

void rf24test()
{
	nrf24l01_conf_t writer;
    nrf24l01_init_config(&writer);
	writer.ce_port= &PORTB;
	writer.ce_pin = PB1;
	writer.ss_port = &PORTB;
	writer.ss_pin = PB2;
    writer.pipe_addr_len = 0;
    usart_write_string_line("Initializing writer..");
	
	
	DBG_VALUE("Writer: Init: ", nrf24l01_init(&writer, &DDRD, &DDRC));
	DBG_VALUE("Writer: Speed: ", nrf24l01_get_speed(&writer));
	DBG_VALUE("Writer: Channel: ", nrf24l01_get_channel(&writer));
	DBG_VALUE("Writer: Status: ", nrf24l01_get_status(&writer));
    
    nrf24l01_set_power_amplifier(&writer, NRF24L01_PA_MAX);
    nrf24l01_enable_dynamic_payload_on_pipe(&writer, test_pipe, NRF24L01_DISABLE);
    nrf24l01_print_addresses(&writer);
	
    char data[5] = "Ping"; 

    if (write_mode)
    {
        write(&writer, data, 5);
        memset(data, 0, 5);
    }
	

    while(1)
    {
            
        uint8_t len = read(&writer, data, 5); 
        if(len == 0)
        {
            continue;
        }       
        usart_write_string("Got: ");
        usart_write_string_line(data);
        usart_read_byte();
        write(&writer, data, len);
    }
}

int main()
{
	usart_init(9600UL);
	tu_delay_ms(1000);
	usart_write_string_line("NRF24l01(+) example.");
	rf24test();
	test_lib();
    while(1);
}


#endif