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

#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <lib_ex_config.h>
	

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC) && defined(F_PCD8544)

#include "screen.h"
#include "events.h"
#include <pcd8544/pcd8544_font.h>
#include <avr/pgmspace.h>

void draw_screensaver();
void screensaver_events_handler(struct event_t* e);

struct screensaver_t
{
	uint16_t			last_timer_tick;
	struct screen_t*	prev_screen;
	struct screen_t		saver;	
};

extern struct screensaver_t screensaver;

#endif // F_EXAMPLES

#endif /* SCREENSAVER_H */