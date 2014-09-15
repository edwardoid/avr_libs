#include "time_utils_counters.h"

#include <avr/interrupt.h>

#define TU_CLOCK_CYCLES_TO_MICROSECONDS(a) ( ((a) * 1000L) / (F_CPU / 1000L) )

#define TU_MICROSECONDS_PER_TIMER0_OVERFLOW (TU_CLOCK_CYCLES_TO_MICROSECONDS(64 * 256))

#define TU_MILLIS_INC (TU_MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

#define TU_FRACT_INC ((TU_MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define TU_FRACT_MAX (1000 >> 3)


volatile static uint32_t tu_timer0_millis = 0;
static uint8_t tu_timer0_fract = 0;

inline __attribute__((gnu_inline)) uint32_t tu_millis()
{
    uint32_t val;

    uint8_t prev_int_status = SREG;

    // disable interrupts while we read TU_timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to TU_timer0_millis)
    cli();
    val = tu_timer0_millis;

    SREG = prev_int_status; // restore sei() if it was enabled

    return val;	
}

ISR(TIMER0_OVF_vect)
{
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = tu_timer0_millis;
    uint8_t f = tu_timer0_fract;

    m += TU_MILLIS_INC;
    f += TU_FRACT_INC;
    if (f >= TU_FRACT_MAX) {
        f -= TU_FRACT_MAX;
        m += 1;
    }

    tu_timer0_fract = f;
    tu_timer0_millis = m;
}