#include "startup.h"
#include "state.h"
#include "events.h"
#include "../my_stdlib.h"
#include "../utils.h"
#include <avr/io.h>
#include "../bitman.h"
#include "../time_utils_delays.h"

uint8_t pressed = 0;

int main()
{
	setup_hardware();
	current_state.current_screen->draw();
	set_as_input(DDRC, 5);
	set_as_input(DDRC, 4);
	set_as_input(DDRC, 3);
	forever
	{
		struct event_t e;
		if(NOTHING_HAPPENED != poll_hardware(&e) && e.emitter != NOTHING_HAPPENED)
		{
			if(current_state.current_screen->event_handler != NULL)
			{
				switch(current_state.current_screen->event_handler(&e))
				{
					case SIGNAL_REDRAW:
					{
						current_state.current_screen->draw();
						break;
					}
					
					default:
					{
						
					}
				}
			}
			
			global_event_handler(&e);	
		}
	}	
}