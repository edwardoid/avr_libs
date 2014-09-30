#ifndef TIME_UTILS_COUNTERS_H
#define TIME_UTILS_COUNTERS_H

#include <inttypes.h>
#include <avr/interrupt.h>

#define tu_counters_init() TCCR0 |= (_BV(CS01) | _BV(CS00)); TIMSK |= _BV(TOIE0); sei();

uint32_t tu_millis();

#endif // TIME_UTILS_COUNTERS_H
