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

#include <lib_ex_config.h>

#if defined(F_PWM)

#include "pwm_t1.h"
#include "bitmaN.h"
#include <avr/io.h>
#include <my_types.h>
#include "timers/timers.h"

#define PWM_T1_DISABLED 0

static volatile uint8_t current_pwm_mode = 0; // Disabled


#define PWM_T1_WGM11_WGM10_MASK (_BV(WGM11) | _BV(WGM10))
#define PWM_T1_WGM13_WGM12_MASK (_BV(WGM13) | _BV(WGM12))


void pwm_t1_start(uint8_t mode, uint16_t top, uint8_t prescale)
{
	pwm_t1_stop();

	if(mode == PWM_T1_DISABLED)
	{
		return;
	}

	set_mask(TCCR1A, PWM_T1_WGM11_WGM10_MASK & mode);
	set_mask(TCCR1B, PWM_T1_WGM13_WGM12_MASK & mode);

	timers_set_prescaler_1(prescale);
}

void pwm_t1_stop()
{
	clear_mask(TCCR1A, PWM_T1_WGM11_WGM10_MASK);
	clear_mask(TCCR1B, PWM_T1_WGM13_WGM12_MASK);
}

void pwm_t1_set_invert_mode(uint8_t invert)
{
	clear_mask(TCCR1A, PWM_T1_INVERTED_OC1A | PWM_T1_INVERTED_OC1B);
	set_mask(TCCR1A, invert);
}

void pwm_t1_set_top(uint8_t top);
void pwm_t1_set_duty(uint8_t duty); /* 0..100 */


#endif // F_PWM 