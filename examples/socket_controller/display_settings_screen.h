/*
 * display_settings_screen.h
 *
 * Created: 11/10/2014 7:10:50 PM
 *  Author: eduards
 */ 


#ifndef DISPLAY_SETTINGS_SCREEN_H
#define DISPLAY_SETTINGS_SCREEN_H

#include <lib_ex_config.h>

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC)

#include "screen.h"

void draw_display_settings_screen();

uint8_t display_settings_event_handler(struct event_t* event);

extern struct screen_t display_settings_screen;


#endif // F_EXAMPLES

#endif /* DISPLAY_SETTINGS_SCREEN_H */