#include "../config.h"

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE)

#include "events.h"
#include "../my_stdlib.h"
#include "../my_types.h"
#include "../bitman.h"
#include "../time_utils_delays.h"
#include "../adc.h"
#include <avr/io.h>

struct button_t
{
	uint16_t		min;
	uint16_t		max;
	uint8_t		last_state;
};


struct button_t left = 
{
	.min= 470,
	.max = 540,
	.last_state = BUTTON_RELEASED
};

struct button_t center =
{
	.min= 630,
	.max = 740,
	.last_state = BUTTON_RELEASED
};

struct button_t right =
{
	.min= 800,
	.max = 900,
	.last_state = BUTTON_RELEASED
};

 ow_conf_t temp =
 {
	 .LastDeviceFlag = 0,
	 .LastDiscrepancy = 0,
	 .LastFamilyDiscrepancy = 0,
	 .ddr = &DDRD,
	 .port = &PORTD,
	 .pin = &PIND,
	 .pin_num = PD7
 };
 
uint8_t temp_rom[]  = { 0, 0, 0, 0,    0, 0, 0, 0 };

int8_t button_left_last_state	= BUTTON_RELEASED;
int8_t button_right_last_state	= BUTTON_RELEASED;
int8_t button_center_last_state	= BUTTON_RELEASED;

float	temp_sensor_last_value	= 99.99;

#define IN_RANGE(val, button) ( (button->min < val) && (button->max > val ))

uint8_t	poll_button(struct button_t* bttn)
{
	uint16_t adc = adc_read(BUTTON_ADC);
	if(bttn->last_state == BUTTON_RELEASED && IN_RANGE(adc, bttn))
	{
		bttn->last_state = BUTTON_PRESSED;
	}
	else if((bttn->last_state == BUTTON_PRESSED || bttn->last_state == BUTTON_LONG_PRESSED) && IN_RANGE(adc, bttn))
	{
		bttn->last_state = BUTTON_RELEASED;
	}
	else
	{
		return NOTHING_HAPPENED;
	}
	return bttn->last_state;
}

uint8_t poll_button_for_long_press(struct button_t* bttn)
{
	if(bttn == NULL)
	{
		return NOTHING_HAPPENED;
	}
		
	poll_button(bttn);
	if(bttn->last_state == BUTTON_PRESSED || bttn->last_state == BUTTON_LONG_PRESSED)
	{
		uint8_t poll_count = 5;
		while(poll_count--)
		{
			tu_delay_ms(100);
			poll_button(bttn);
			if(bttn->last_state == BUTTON_RELEASED)
			{
				bttn->last_state = BUTTON_RELEASED;
				return BUTTON_RELEASED;
			}
		}	
	}
	
	bttn->last_state = BUTTON_LONG_PRESSED;
	return bttn->last_state;
}

uint8_t poll_temp_sensor()
{
	ow_conf_t cfg;
	memset(&cfg, 0, sizeof(ow_conf_t));
	ow_init_config(&cfg);
	cfg.ddr = &DDRD;
	cfg.port = &PORTD;
	cfg.pin = &PIND;
	cfg.pin_num = PD7;
	ow_init(&cfg);
	if(!ow_reset(&cfg))
	{
		return NOTHING_HAPPENED;
	}
	if(!ow_search(temp_rom, &cfg))
	{
		return NOTHING_HAPPENED;
	}
	temp_sensor_last_value = ow_read_temperature_ds18x2x(&temp, temp_rom);
	return TEMP_CHANGED;
}

uint8_t poll_hardware(struct event_t* res)
{
	uint32_t val= (uint32_t) adc_read(BUTTON_ADC);
	usart_write_num(val);
	usart_write_byte('\n');
	if(res != NULL)
	{
		res->emitter = NOTHING_HAPPENED;
		res->data = 0;
		
		if(poll_button(&left) != NOTHING_HAPPENED)
		{
			usart_write_string_line("LEFT");
			res->emitter = BUTTON_LEFT;
			res->data = left.last_state;
		}
		else if(poll_button(&right) != NOTHING_HAPPENED)
		{
			usart_write_string_line("RIGHT");
			res->emitter = BUTTON_RIGHT;
			res->data = right.last_state;
		}
		else if(poll_button(&center) != NOTHING_HAPPENED)
		{
			usart_write_string_line("CENTER");
			if(center.last_state != BUTTON_RELEASED)
			{
				poll_button_for_long_press(&center);
			}
			res->emitter = BUTTON_CENTER;
			res->data = center.last_state;
		}
		else if(poll_temp_sensor() != NOTHING_HAPPENED)
		{
			res->emitter = TEMP_SENSOR;
			temp_sensor_last_value += 1;
			res->data = temp_sensor_last_value;
		}
		
		return res->emitter;
	}
	return NOTHING_HAPPENED;
}

uint8_t global_event_handler(struct event_t* event)
{
	return 0;
}

#endif // F_EXAMPLES