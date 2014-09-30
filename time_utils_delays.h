#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <util/delay.h>
#define LOOP_CYCLES 8 				//Number of cycles that the loop takes

#define tu_delay_us(num) tu_delay_cycles(num / ( 8 * ( 1 / (F_CPU / 1000000.))))
#define tu_delay_ms(num) tu_delay_cycles(num / ( 8 * ( 1 / (F_CPU / 1000.   ))))

#include <inttypes.h>
void tu_delay_cycles(uint32_t cycles);

#endif // TIME_UTILS_H