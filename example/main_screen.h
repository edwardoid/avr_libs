/*
 * main_screen.h
 *
 * Created: 11/9/2014 2:22:01 AM
 *  Author: eduards
 */ 


#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "screen.h"

void draw_main_screen();

uint8_t main_screen_event_handler(struct event_t* event);

extern struct screen_t main_screen;

#endif /* MAIN_SCREEN_H_ */