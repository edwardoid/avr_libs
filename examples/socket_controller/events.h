/*
 * events.h
 *
 * Created: 11/9/2014 3:05:35 AM
 *  Author: eduards
 */ 


#ifndef EVENTS_H
#define EVENTS_H

#include <lib_ex_config.h>

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC)

#include <inttypes.h>

#include <1wire/1wire.h>
#include <avr/io.h>

#define NOTHING_HAPPENED	0

#define BUTTON_ADC ADC5

#define BUTTON_LEFT			5
#define BUTTON_RIGHT		4
#define BUTTON_CENTER		3
#define TEMP_SENSOR			2


#define BUTTON_PRESSED		1
#define BUTTON_LONG_PRESSED	3
#define BUTTON_RELEASED		4
#define TEMP_CHANGED		5

#define SIGNAL_REDRAW		1
#define CONTEXT_CHANGE		2

struct event_t
{
	uint8_t	emitter;
	int32_t	data;	
};

uint8_t	poll_hardware(struct event_t* res);

uint8_t	global_event_handler(struct event_t* event);

#else
#define poll_hardware(x) 0
#define global_event_handler(x) 0

#endif // F_EXAMPLES



#endif /* EVENTS_H_ */