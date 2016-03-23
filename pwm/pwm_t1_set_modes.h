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

#ifndef PWM_T1_SET_MODES_H
#define PWM_T1_SET_MODES_H

#include <avr/io.h>

#include "config.h"
#ifdef F_PWM

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

#endif // F_PWM

#endif // PWM_T1_SET_MODES_H
