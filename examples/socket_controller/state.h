/*
 * state.h
 *
 * Created: 11/9/2014 2:35:38 AM
 *  Author: eduards
 */ 


#ifndef STATE_H
#define STATE_H

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC) && defined(F_PCD8544)

#include <inttypes.h>
#include "screen.h"

struct state_t
{
	uint8_t is_auto_on;
	float	temperature;
	float	min_temp;
	float	max_temp;
	uint8_t	is_socket_on;
	struct screen_t* current_screen;
};


extern struct state_t current_state;


#endif

#endif /* STATE_H_ */