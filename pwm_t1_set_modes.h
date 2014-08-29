#ifndef PWM_T1_SET_MODES_H
#define PWM_T1_SET_MODES_H

#include <avr/io.h>
#include "bitman.h"

#define pwm_t1_disable()                   clear_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                           clear_bit(TCCR1A, WGM11);    clear_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pc_1()               set_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);    clear_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pc_2()               set_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);      set_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pc_8bit()          clear_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                           clear_bit(TCCR1A, WGM11);    set_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pc_9bit()          clear_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                           set_bit(TCCR1A, WGM11);    clear_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pc_10bit()         clear_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);      set_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_f_1()                set_bit(TCCR1B, WGM13);      set_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);    clear_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_f_2()                set_bit(TCCR1B, WGM13);      set_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);      set_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_f_8bit()           clear_bit(TCCR1B, WGM13);      set_bit(TCCR1B, WGM12);   \
                                           clear_bit(TCCR1A, WGM11);      set_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_f_9bit()           clear_bit(TCCR1B, WGM13);      set_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);    clear_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_f_10bit()          clear_bit(TCCR1B, WGM13);      set_bit(TCCR1B, WGM12);   \
                                             set_bit(TCCR1A, WGM11);      set_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pfc_1()              set_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                           clear_bit(TCCR1A, WGM11);    clear_bit(TCCR1A, WGM10);

#define pwm_t1_set_mode_pfc_2()              set_bit(TCCR1B, WGM13);    clear_bit(TCCR1B, WGM12);   \
                                           clear_bit(TCCR1A, WGM11);      set_bit(TCCR1A, WGM10);

#include <inttypes.h>

#endif // PWM_T1_SET_MODES_H
