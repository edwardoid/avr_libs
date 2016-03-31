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

#include "value_change_screen.h"

#include <lib_ex_config.h>

#ifdef F_EXAMPLES

#include "events.h"
#include "question.h"
#include "state.h"
#include "main_screen.h"
#include <pcd8544/pcd8544_progress.h>
#include <pcd8544/pcd8544drawing.h>
#include <my_stdlib.h>
#include <pcd8544/pcd8544_font.h>
/*
struct value_change_screen_t value_change_screen = 
{
	.screen =
	{
		.draw = draw_value_change_screen,
		.event_handler = value_change_screen_events_handler
	},
	.parameter_name = NULL,
	.current_value = 0.0,
	.min = 0.0,
	.max = 100.0,
	.step = 1.0
};

void draw_value_change_screen()
{
	pcd8544_fill(0);
	redraw_notification_area();
	
	uint8_t y = 14;
	if(value_change_screen.parameter_name != NULL)
	{
		pcd8544_print_str(0, y, value_change_screen.parameter_name, &ubuntu_font, 1, ALIGN_CENTER);
	}
	
	float new_val = value_change_screen.current_value + value_change_screen.difference;
	
	uint8_t len = pcd8577_print_calc_style_num_length_in_px(new_val) + 2 + (new_val < 0 ? 1 : 0);
	
	y += ubuntu_font.height + 4;
	
	pcd8577_print_calc_style_fnum((PCD8544_WIDTH - len) / 2, y, new_val, 1);
	
	y += 6;
	pcd8544_draw_progress(2, y, PCD8544_WIDTH - 4, (100. * (new_val - value_change_screen.min)) / (value_change_screen.max - value_change_screen.min), 0, 1);
	
	if(value_change_screen.apply != NULL)
		value_change_screen.apply(value_change_screen.current_value + value_change_screen.difference);
}

void value_change_screen_events_handler(struct event_t* e)
{
	if(e == NULL);
	
	if(e->emitter == TEMP_SENSOR && e->data == TEMP_CHANGED)
	{
		add_redraw_event();
	}
	else if(e->emitter == BUTTON_LEFT && (e->data == BUTTON_PRESSED || e->data == BUTTON_LONG_PRESSED))
	{
		value_change_screen.difference -= value_change_screen.step;
		float new_val = value_change_screen.current_value + value_change_screen.difference;
		if(new_val > value_change_screen.max)
		{
			value_change_screen.difference = value_change_screen.max - value_change_screen.current_value;
		}
		else if (new_val < value_change_screen.min)
		{
			value_change_screen.difference = value_change_screen.current_value - value_change_screen.min;
		}
		add_redraw_event();
	}
	else if(e->emitter == BUTTON_RIGHT && (e->data == BUTTON_PRESSED || e->data == BUTTON_LONG_PRESSED))
	{
		value_change_screen.difference += value_change_screen.step;
		float new_val = value_change_screen.current_value + value_change_screen.difference;
		if(new_val > value_change_screen.max)
		{
			value_change_screen.difference = value_change_screen.max - value_change_screen.current_value;
		}
		else if (new_val < value_change_screen.min)
		{
			value_change_screen.difference = value_change_screen.current_value - value_change_screen.min;
		}
		add_redraw_event();
	}
	else if(e->emitter == BUTTON_CENTER && e->data == BUTTON_PRESSED)
	{
		if(fabs(value_change_screen.difference) >= fabs(value_change_screen.step))
		{
			int answer = question("Save?");
			if(answer == QUESTION_RESULT_YES)
			{
				value_change_screen.current_value += value_change_screen.difference;
				add_event(VALUE_CHANGE, VALUE_CHANGED);
			}
		}
		else
		{
			current_state.current_screen = &main_screen;
			add_redraw_event();
		}
	}
}
*/

#endif // F_EXAMPLES