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


#ifndef PWM_T0_H
#define PWM_T0_H

#include <lib_ex_config.h>

#ifdef F_PWM

#define PWM_T0_PHASE_CORRECTED 1 /* TOP = 0xFF */
#define PWM_T0_FAST 3 /* top = 0xFF */
#define PWM_T0_PHASE_CORRECTED_OCR0A 5 /* top = OCRA */
#define PWM_T0_FAST_OCR0A 7

void pwm_t0_start(uint8_t mode, uint8_t top, uint8_t prescale);
void pwm_t0_set_invert_mode(uint8_t invert);
void pwm_t0_set_top(uint8_t top);
void pwm_t0_set_duty(uint8_t duty); /* 0..100 */

#endif // F_PWM

#endif // PWM_T0_H
