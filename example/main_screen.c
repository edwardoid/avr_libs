#include "main_screen.h"

#include "../config.h"

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_PCD8544)

#include "../pcd8544.h"
#include "../pcd8544bitmaps.h"
#include "../my_stdlib.h"
#include "../bitman.h"
#include "state.h"
//#include "display_settings_screen.h"

uint8_t menu_selection = 0;
uint8_t screen_is_about_to_be_changed = 0;

struct screen_t main_screen =
{
	.draw = draw_main_screen,
	.event_handler = main_screen_event_handler
};

void draw_main_screen()
{
	pcd8544_fill(0);
	redraw_notification_area();
	draw_button(6, 14, socket, 32, 32, test_bit(menu_selection, 1));
	draw_button(46, 14, display, 32, 32, test_bit(menu_selection, 2));
	pcd8544_render();
}

uint8_t main_screen_event_handler(struct event_t* event)
{
	if(event == NULL || event->emitter == NOTHING_HAPPENED)
		return 0;

	if(event->emitter == BUTTON_LEFT && event->data == BUTTON_PRESSED)
	{
		menu_selection = 0;
		set_bit(menu_selection, 1);
		return SIGNAL_REDRAW;
	}
	else if(event->emitter == BUTTON_RIGHT && event->data == BUTTON_PRESSED)
	{
		menu_selection = 0;
		set_bit(menu_selection, 2);
		return SIGNAL_REDRAW;
	}
	else if(event->emitter == BUTTON_CENTER)
	{
		if(event->data == BUTTON_RELEASED)
		{
			if(screen_is_about_to_be_changed)
			{
				if(test_bit(menu_selection, 2))
				{
					//current_state.current_screen = &display_settings_screen;
					//screen_is_about_to_be_changed = 0;
				}
			}
			else
			{
				menu_selection = 0;
			}
			
			return SIGNAL_REDRAW;
		}
		else if(event->data == BUTTON_LONG_PRESSED)
		{
			screen_is_about_to_be_changed = 1;
		}
	}
	else if (event->emitter == TEMP_SENSOR)
	{
		current_state.temperature = event->data;
		return SIGNAL_REDRAW;
	}
	
	
	return 0;
}

#endif // F_EXAMPLES