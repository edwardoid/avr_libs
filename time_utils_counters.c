#include "time_utils_counters.h"

#include <avr/interrupt.h>

#define ST_CLOCK_CYCLES_TO_MICROSECONDS(a) ( ((a) * 1000L) / (F_CPU / 1000L) )

#define ST_MICROSECONDS_PER_TIMER0_OVERFLOW (ST_CLOCK_CYCLES_TO_MICROSECONDS(64 * 256))

#define ST_MILLIS_INC (ST_MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

#define ST_FRACT_INC ((ST_MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define ST_FRACT_MAX (1000 >> 3)


volatile static uint32_t tu_timer0_millis = 0;
static uint8_t tu_timer0_fract = 0;

inline __attribute__((gnu_inline)) uint32_t tu_millis()
{
    uint32_t m;

    uint8_t oldSREG = SREG;

    // disable interrupts while we read st_timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to st_timer0_millis)
    cli();
    m = tu_timer0_millis;

    SREG = oldSREG; // restore sei() if it was enabled

    return m;	
}

ISR(TIMER0_OVF_vect)
{
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = tu_timer0_millis;
    uint8_t f = tu_timer0_fract;

    m += ST_MILLIS_INC;
    f += ST_FRACT_INC;
    if (f >= ST_FRACT_MAX) {
        f -= ST_FRACT_MAX;
        m += 1;
    }

    tu_timer0_fract = f;
    tu_timer0_millis = m;
}