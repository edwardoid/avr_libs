#include "state.h"

#include <lib_ex_config.h>

#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_1WIRE) && defined(F_ADC) && defined(F_PCD8544)

#include "main_screen.h"

struct state_t current_state =
{
	.is_auto_on = 1,
	.temperature = -99.99,
	.min_temp = 100,
	.max_temp = 0,
	.is_socket_on = 0,
	.current_screen = &main_screen
};

#endif // F_EXAMPLES