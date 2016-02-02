#ifndef SCREEN_H
#define SCREEN_H

#include "../config.h"
#ifdef F_EXAMPLES

#include "events.h"
#include <inttypes.h>
#include "../my_types.h"

typedef void (*draw_function_ptr)();
typedef uint8_t (*event_handler_function_ptr)(struct event_t*);

struct screen_t
{
	draw_function_ptr draw;
	event_handler_function_ptr event_handler;	
};

uint8_t print_socket_status_in_notification_area(uint8_t color);
uint8_t print_temp_in_notification_area(float temp, uint8_t color);
void redraw_notification_area();
void draw_button(uint8_t x, uint8_t y, const byte* icon, uint8_t w, uint8_t h, uint8_t pressed);

#endif // F_EXAMPLES
#endif /* SCREEN_H_ */