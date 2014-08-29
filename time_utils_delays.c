#include "time_utils_delays.h"

inline __attribute__((gnu_inline)) void tu_delay_cycles(uint32_t cycles)
{
	while(--cycles) asm volatile("nop");
};