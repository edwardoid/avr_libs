/*
	avr_libs
	Copyright (C) 2014  Edward Sargsyan

	avr_libs is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	avr_libs is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with avr_libs.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PWM_T1_H
#define PWM_T1_H

#include <lib_ex_config.h>

#ifdef F_PWM

#define pwm_t1_timer_from_pin(pin)              (*(pin == PB1 ? &OCR1A: &OCR1B))
#define pwm_t1_timer_control_from_pin(pin)      (*(pin == PB1 ? &TCCR1A: &TCCR1B))
#define pwm_t1_comparator_from_pin(pin, num)    (pin == PB1 ? COM1A ## num : COM1B ## num )

#define pwm_t1_set_duty_perc(pin, val)        pwm_t1_set_duty(timer, (uint8_t) ( val##. / 250.))
#define pwm_t1_set_duty(pin, val)             pwm_t1_timer_from_pin(pin) = val; 

//  Prescaling
#define pwm_disable_t1()                        TCCR1B &= (_BV(CS10) | _BV(CS11) | _BV(CS12));
#define pwm_t1_set_prescaling_0()                 set_bit(TCCR1B, CS10); clear_bit(TCCR1B, CS11); clear_bit(TCCR1B, CS12);
#define pwm_t1_set_prescaling_8()               clear_bit(TCCR1B, CS10);   set_bit(TCCR1B, CS11); clear_bit(TCCR1B, CS12);
#define pwm_t1_set_prescaling_64()                set_bit(TCCR1B, CS10);    set_bit(TCCR1B, CS11);   clear_bit(TCCR1B, CS12);
#define pwm_t1_set_prescaling_256()             clear_bit(TCCR1B, CS10);  clear_bit(TCCR1B, CS11);     set_bit(TCCR1B, CS12);
#define pwm_t1_set_prescaling_1024()              set_bit(TCCR1B, CS10);  clear_bit(TCCR1B, CS11);     set_bit(TCCR1B, CS12);

#include "pwm_t1_set_modes.h"

#define pwm_t1_set_inverted(pin)           set_bit(TCCR1A, pwm_t1_comparator_from_pin(pin, 1));    \
                                           set_bit(TCCR1A, pwm_t1_comparator_from_pin(pin, 0));

#define pwm_t1_set_non_inverted(pin)       set_bit(TCCR1A, pwm_t1_comparator_from_pin(pin, 1));    \
                                           clear_bit(TCCR1A, pwm_t1_comparator_from_pin(pin, 0));


#define pwm_enable_t1(pin)                      set_bit(DDRB, pin);                   \
                                                pwm_t1_set_duty(pin, 0)               \
                                                pwm_t1_set_non_inverted(pin);         \
                                                pwm_t1_set_mode_pc_10bit();           \
                                                pwm_t1_set_prescaling_8();


#endif // F_PWM

#endif // PWM_T1_H
