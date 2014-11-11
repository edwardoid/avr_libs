#include "screen.h"


#define QUESTION_RESULT_NOT_ANSWERED	0
#define QUESTION_RESULT_YES				1
#define QUESTION_RESULT_NO				2

struct question_box_t
{
	struct screen_t		box;
	struct screen_t*	caller;
	const char*			question;
	uint8_t				result;
};

extern struct question_box_t question_screen;

void draw_question();

uint8_t question_screen_event_handler(struct event_t* event);