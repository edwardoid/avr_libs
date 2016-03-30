#include "question_screen.h"

#include <lib_ex_config.h>

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC) && defined(F_PCD8544)

#include "events.h"
#include "state.h"
#include <pcd8544/pcd8544.h>
#include <my_stdlib.h>
#include <usart/usart.h>
#include <pcd8544/pcd8544_default_font.h>
#include <pcd8544/pcd8544drawing.h>

struct question_box_t question_screen = 
{
	.box = {
			.draw = draw_question,
			.event_handler = question_screen_event_handler	
		},
	.caller = NULL,
	.question = "",
	.result = QUESTION_RESULT_NOT_ANSWERED
};

void draw_question()
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
	
	uint8_t len = pcd8544_get_str_length_in_px(question_screen.question, &default_font) + 4;
	uint8_t left = (PCD8544_WIDTH - len) >> 1;
	uint8_t top = (PCD8544_HEIGHT - default_font.height - 4) >> 1;
	pcd8544_fill_rounded_rect(top, left, top + default_font.height + 2, left + len + 2, 2, 0);
	pcd8544_draw_rounded_rect(top, left, top + default_font.height + 2, left + len + 2, 2, 1);
	//pcd8544_print_str(left + 2, top + 2, question_screen.question, &default_font, 1, ALIGN_CENTER);
	//pcd8544_fill_rounded_rect(PCD8544_HEIGHT - default_font.height - 4, 2, PCD8544_HEIGHT - 1, pcd8544_get_str_length_in_px(YES, &default_font) + 4, 2, 0);
	//pcd8544_draw_rounded_rect(PCD8544_HEIGHT - default_font.height - 4, 2, PCD8544_HEIGHT - 1, pcd8544_get_str_length_in_px(YES, &default_font) + 4, 2, 1);
	//pcd8544_print_str(2, PCD8544_HEIGHT - default_font.height - 2, YES, &default_font, 1, ALIGN_LEFT);
	
	pcd8544_render();
}

uint8_t question_screen_event_handler(struct event_t* event)
{
	if(event->emitter == BUTTON_LEFT && event->data == BUTTON_RELEASED)
	{
		if(question_screen.caller != NULL)
		{
			current_state.current_screen = question_screen.caller;
			question_screen.result = QUESTION_RESULT_NO;
			return SIGNAL_REDRAW;
		}		
	}
	else if(event->emitter == BUTTON_RIGHT && event->data == BUTTON_RELEASED)
	{
		if(question_screen.caller != NULL)
		{
			current_state.current_screen = question_screen.caller;
			question_screen.result = QUESTION_RESULT_YES;
			return SIGNAL_REDRAW;
		}
	}
	
	return 0;
}

#endif // F_EXAMPLES