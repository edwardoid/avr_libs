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

#include "screensaver.h"

#include "../config.h"
#ifdef F_EXAMPLES

#include <pcd8544/pcd8544drawing.h>
#include <pcd8544/pcd8544_font.h>
#include <pcd8544/pcd8544_ubuntu_font.h>
#include <my_stdlib.h>
#include "main_screen.h"
#include "state.h"

/*
struct screensaver_t screensaver = 
{
	.last_timer_tick = 0,
	.prev_screen = &main_screen,
	.saver = {
		.draw = draw_screensaver,
		.event_handler = screensaver_events_handler
	}
};


void draw_screensaver()
{
	pcd8544_fill(0xFF);
	char tmp[] = "\0\0\0\0\0";
	ftoa(current_state.temperature, 2, tmp);
	pcd8544_print_str(0, 0, tmp, &ubuntu_font, 0, ALIGN_CENTER); 
}

void screensaver_events_handler(struct event_t* e)
{
	if(e == NULL)
		return;
		
	if(e->emitter == BUTTON_LEFT || e->emitter == BUTTON_RIGHT || e->emitter == BUTTON_CENTER)
	{
		current_state.current_screen = screensaver.prev_screen == NULL ? &main_screen : screensaver.prev_screen;
		screensaver.last_timer_tick = -1;
		add_redraw_event();
	}
}
*/

#endif // F_EXAMPLES