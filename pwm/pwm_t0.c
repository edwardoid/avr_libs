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

#include <avr/io.h>
#include <pwm/pwm.h>
#include <pwm/pwm_t0.h>

void pwm_t0_start(uint8_t mode, uint8_t top, uint8_t prescale)
{
	switch(mode)
	{
		case PWM_T0_PHASE_CORRECTED:
		{
			clear_bit(TCCR0B, WGM02);
			clear_mask(TCCR0A, _BV(WGM01) | _BV(WGM00));
			set_bit(TCCR0A, WGM00);
			break;
		}
		case PWM_T0_FAST:
		{
			clear_bit(TCCR0B, WGM02);
			set_mask(TCCR0A, _BV(WGM01) | _BV(WGM00));
			break;
		}
		case PWM_T0_PHASE_CORRECTED_OCR0A:
		{
			set_bit(TCCR0B, WGM02);
			clear_mask(TCCR0A, _BV(WGM01) | _BV(WGM00));
			set_bit(TCCR0A, WGM00);
			break;
		}
		case PWM_T0_FAST_OCR0A:
		{
			set_bit(TCCR0B, WGM02);
			set_mask(TCCR0A, _BV(WGM01) | _BV(WGM00));
			break;
		}
	}
	set_bit(TCCR0A, COM0A1);
	pwm_t0_set_top(top);
	timers_set_prescaler_0(prescale);
}

void pwm_t0_set_invert_mode(uint8_t invert)
{
	if(invert == PWM_INVERTED)
	{
		set_bit(TCCR0A, COM0A0);
	}
	else
	{
		clear_bit(TCCR0A, COM0A0);
	}
}

void pwm_t0_set_top(uint8_t top)
{
	OCR0A = top;
}

#endif // F_PWM