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

#include "question.h"
#include "events.h"
#include "state.h"
#include "../pcd8544.h"
#include "../my_stdlib.h"
#include "../usart.h"
#include "../pcd8544_default_font.h"
#include "../pcd8544drawing.h"
#include "../utils.h"

/*
static const char* YES = "YES";
static const char* NO = "NO";

void draw_question(const char* q)
{
	uint8_t x = 0;
	uint8_t y = 14;
	for (; x < PCD8544_WIDTH; ++x)
	{
		for (y = 12; y < PCD8544_HEIGHT; ++y)
		{
			if(x % 2 ^ y % 2)
				pcd8544_set_pixel(x, y, 1);
		}
	}
	
	uint8_t len = pcd8544_get_str_length_in_px(q, &default_font) + 4;
	uint8_t left = (PCD8544_WIDTH - len) >> 1;
	uint8_t top = (PCD8544_HEIGHT - default_font.height - 4) >> 1;
	
	// Question box and message
	pcd8544_fill_rounded_rect(top, left, top + default_font.height + 2, PCD8544_WIDTH - left, 2, 0);
	pcd8544_draw_rounded_rect(top, left, top + default_font.height + 2, PCD8544_WIDTH - left, 2, 1);
	pcd8544_print_str(left + 2, top + 2, q, &default_font, 1, ALIGN_CENTER);
	
	
	len = pcd8544_get_str_length_in_px(YES, &default_font);
	top = PCD8544_HEIGHT - default_font.height - 4;
	
	// YES button
	pcd8544_fill_rounded_rect(top, 1, PCD8544_HEIGHT - 1, len + 4, 2, 0);
	pcd8544_draw_rounded_rect(top, 1, PCD8544_HEIGHT - 1, len + 4, 2, 1);
	pcd8544_print_str(3, top + 3, YES, &default_font, 1, ALIGN_LEFT);
	// NO button
	len = pcd8544_get_str_length_in_px(NO, &default_font);
	pcd8544_fill_rounded_rect(top, PCD8544_WIDTH - len - 5, PCD8544_HEIGHT - 1, PCD8544_WIDTH - 2, 2, 0);
	pcd8544_draw_rounded_rect(top, PCD8544_WIDTH - len - 5, PCD8544_HEIGHT - 1, PCD8544_WIDTH - 2, 2, 1);
	pcd8544_print_str(PCD8544_WIDTH - len - 3, top + 3, NO, &default_font, 1, ALIGN_LEFT);
	
	
	pcd8544_render();
}

uint8_t question(const char* str)
{
	struct event_t e;
	draw_question(str);
	forever
	{
		get_event(&e);
		if(e.emitter == NOTHING_HAPPENED)
			continue;
		if(e.data != BUTTON_RELEASED)
			continue;	
		if(e.emitter == BUTTON_LEFT)
		{
			return QUESTION_RESULT_YES;
		}
		else if(e.emitter == BUTTON_RIGHT)
		{
			return QUESTION_RESULT_NO;
		}
	}
}
*/