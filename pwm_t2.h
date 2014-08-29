#ifndef PWM_T2_H
#define PWM_T2_H

#include "bitman.h"
#include <inttypes.h>
#include <avr/io.h>

#define pwm_t2_set_duty_perc(val)               pwm_t2_set_duty((uint8_t) ( val##. / 250.))
#define pwm_t2_set_duty(val)                    OCR2 = val; 

#define pwm_t2_set_prescaling_0()                 set_bit(TCCR2, CS20); clear_bit(TCCR2, CS21); clear_bit(TCCR2, CS22);
#define pwm_t2_set_prescaling_8()               clear_bit(TCCR2, CS20);   set_bit(TCCR2, CS21); clear_bit(TCCR2, CS22);
#define pwm_t2_set_prescaling_32()                set_bit(TCCR2, CS20); set_bit(TCCR2, CS21);   clear_bit(TCCR2, CS22);
#define pwm_t2_set_prescaling_64()              clear_bit(TCCR2, CS20); clear_bit(TCCR2, CS21);   set_bit(TCCR2, CS22);
#define pwm_t2_set_prescaling_128()               set_bit(TCCR2, CS20); clear_bit(TCCR2, CS21);   set_bit(TCCR2, CS22);
#define pwm_t2_set_prescaling_256()             clear_bit(TCCR2, CS20);   set_bit(TCCR2, CS21);   set_bit(TCCR2, CS22);
#define pwm_t2_set_prescaling_1024()              set_bit(TCCR2, CS20);   set_bit(TCCR2, CS21);   set_bit(TCCR2, CS22);

#define pwm_t2_set_mode_fast()                  TCCR2 |= _BV(WGM21) | _BV(WGM20);
#define pwm_t2_set_mode_phase_corr()            set_bit(TCCR2, WGM21); clear_bit(TCCR2, WGM20);

#define pwm_t2_set_fast_inverted()              set_bit(TCCR2, COM21);   set_bit(TCCR2, COM20);
#define pwm_t2_set_fast_non_inverted()          set_bit(TCCR2, COM21); clear_bit(TCCR2, COM20);

#define pwm_enable_t2()                         set_bit(DDRB, PB3);             \
                                                pwm_t2_set_duty(0);             \
                                                pwm_t2_set_fast_non_inverted()  \
                                                pwm_t2_set_mode_fast()          \
                                                pwm_t2_set_prescaling_8()



#endif // PWM_T2_H
