#include "events.h"
#include "../my_stdlib.h"
#include "../my_types.h"
#include "../bitman.h"
#include "../time_utils_delays.h"
#include <avr/io.h>

struct button_t
{
	ddr_ptr_t	ddr;
	pin_ptr_t	pin;
	uint8_t		num;
	uint8_t		last_state;
};


struct button_t left = 
{
	.ddr = &DDRC,
	.pin = &PINC,
	.num = BUTTON_LEFT,
	.last_state = BUTTON_RELEASED
};

struct button_t center =
{
	.ddr = &DDRC,
	.pin = &PINC,
	.num = BUTTON_CENTER,
	.last_state = BUTTON_RELEASED
};

struct button_t right =
{
	.ddr = &DDRC,
	.pin = &PINC,
	.num = BUTTON_RIGHT,
	.last_state = BUTTON_RELEASED
};

int8_t button_left_last_state	= BUTTON_RELEASED;
int8_t button_right_last_state	= BUTTON_RELEASED;
int8_t button_center_last_state	= BUTTON_RELEASED;

float	temp_sensor_last_value	= 99.99;

uint8_t	poll_button(struct button_t* bttn)
{
	uint8_t res = test_bit(*bttn->pin, bttn->num);
	
	if(bttn->last_state == BUTTON_RELEASED && res)
	{
		bttn->last_state = BUTTON_PRESSED;
	}
	else if((bttn->last_state == BUTTON_PRESSED || bttn->last_state == BUTTON_LONG_PRESSED) && !res)
	{
		bttn->last_state = BUTTON_RELEASED;
	}
	else
		return NOTHING_HAPPENED;
	
	return bttn->last_state;
}

uint8_t poll_button_for_long_press(struct button_t* bttn)
{
	if(bttn == NULL)
		return NOTHING_HAPPENED;
		
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

uint8_t poll_hardware(struct event_t* res)
{
	if(res != NULL)
	{
		res->emitter = NOTHING_HAPPENED;
		res->data = 0;
		
		if(poll_button(&left) != NOTHING_HAPPENED)
		{
			res->emitter = left.num;
			res->data = left.last_state;
		}
		else if(poll_button(&right) != NOTHING_HAPPENED)
		{
			res->emitter = right.num;
			res->data = right.last_state;
		}
		else if(poll_button(&center) != NOTHING_HAPPENED)
		{
			if(center.last_state != BUTTON_RELEASED)
				poll_button_for_long_press(&center);
			res->emitter = center.num;
			res->data = center.last_state;
		}
		
		return res->emitter;
	}
	return NOTHING_HAPPENED;
}

uint8_t global_event_handler(struct event_t* event)
{
	return 0;
}