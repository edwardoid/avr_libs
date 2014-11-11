/*
 * display_settings_screen.c
 *
 * Created: 11/10/2014 7:11:51 PM
 *  Author: eduards
 */ 

#include "display_settings_screen.h"
#include "../config.h"
#include "../pcd8544.h"
#include "../pcd8544_progress.h"
#include "../pcd8544drawing.h"
#include "../pcd8544bitmaps.h"
#include "events.h"
#include "state.h"
#include "main_screen.h"
#include "../my_stdlib.h"
#include "../pcd8544_font.h"
//#include "../pcd8544_tiny_font.h"
#include "../pcd8544_default_font.h"
#include "question_screen.h"

uint8_t contrast = 0x40;
int8_t has_changes = 0;

struct screen_t display_settings_screen = 
{
	.draw = draw_display_settings_screen,
	.event_handler = display_settings_event_handler
};

void draw_display_settings_screen()
{
	pcd8544_fill(0);
	redraw_notification_area();
	
	pcd8544_print_str(2, 32 - default_font.height - 1, "contrast", &default_font, 1, ALIGN_CENTER);
	pcd8544_draw_progress(6, 32, PCD8544_WIDTH - 10 - 16, contrast, 1);
	pcd8544_draw_bitmap(PCD8544_WIDTH - 16, 28, contrast_icon, 16, 16, 1);
	pcd8544_set_contrast(contrast);
	pcd8544_render();
}

uint8_t display_settings_event_handler(struct event_t* event)
{
	if(event != NULL)
	{
		if(event->emitter == BUTTON_CENTER && event->data == BUTTON_LONG_PRESSED)
		{
			if(has_changes)
			{
				current_state.current_screen = &(question_screen.box);
				question_screen.question = "Contrast?";
				question_screen.caller = &display_settings_screen;
				question_screen.result = QUESTION_RESULT_NOT_ANSWERED;
			}
			else
			{
				current_state.current_screen = &main_screen;
			}
			return SIGNAL_REDRAW;
		}
		else if(event->emitter == BUTTON_LEFT && event->data != BUTTON_RELEASED && contrast != 1)
		{
			contrast -= 1;
			pcd8544_set_contrast(contrast);
			has_changes -= 1;
			return SIGNAL_REDRAW;
		}
		else if(event->emitter == BUTTON_RIGHT && event->data != BUTTON_RELEASED && contrast != 100)
		{
			contrast += 1;
			pcd8544_set_contrast(contrast);
			has_changes += 1;
			return SIGNAL_REDRAW;
		}
	}
	
	return 0;
}