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

#ifndef PWM_H
#define PWM_H

#include <lib_ex_config.h>

#ifdef F_PWM

#define PWM_INVERTED 1
#define PWM_NON_INVERTED 0

#include "pwm_t2.h"
#include "pwm_t1.h"

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#include "pwm_t0.h"
#endif // 328p specific

#endif // F_PWM

#endif // PWM_H
