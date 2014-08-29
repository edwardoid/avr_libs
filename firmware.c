#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "bitman.h"
#include "1wire.h"
#include <stdio.h>
#include "time_utils_delays.h"
#include "pwm.h"
//#include "time_utils_counters.h"

#define WORKING		0x00
#define SETIING_MAX	0x01
#define SETTING_MIN 0x02

double get_temp_sensor(ow_conf* cfg, uint8_t num)
{
	if(!ow_reset())
	{
		uart_write_string_line("Initial reset failed!");
		return 999.999;
	}
	
	uint8_t addr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		
	if(!ow_search(addr, cfg))
	{
		uart_write_string_line(cfg->msg);
		return 999.999;
	}
	
	return ow_read_temperature_ds18x2x(addr);
}

int main()
{
//	tu_counters_init();
	uart_init(_9600_UBBRH, _9600_UBBRL);

	uart_write_string_line("Starting....");
	
    set_bit(DDRB, PB1);
    set_bit(DDRB, PB2);
   
    pwm_t1_set_mode_f_10bit();
    pwm_t1_set_non_inverted(PB1); 
    pwm_t1_set_non_inverted(PB2); 
    pwm_t1_set_prescaling_256();

    int i = 3;
    int dir = 1;
    while(1)
    {
        pwm_t1_set_duty(PB1, i);
        pwm_t1_set_duty(PB2, 1024 - i);
        i += dir;
        if(i == 1024)
            dir = 1;
        else if(i == 3)
            dir = -1;
        _delay_ms(5);     
    }
	return 0;
}
