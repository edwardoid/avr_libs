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

#ifndef VALUE_CHANGE_SCREEN
#define VALUE_CHANGE_SCREEN

#include "../config.h"
#ifdef F_EXAMPLES

#include "screen.h"
/*
void draw_value_change_screen();
void value_change_screen_events_handler(struct event_t* e);

typedef void (*apply_function_ptr)(float);

struct value_change_screen_t
{
	struct screen_t	screen;
	const	char*	parameter_name;
	float			current_value;
	float			min;
	float			max;
	float			step;
	float			difference;
	apply_function_ptr	apply;
};

extern struct value_change_screen_t value_change_screen;
*/

#endif // F_EXAMPLES

#endif // VALUE_CHANGE_SCREEN