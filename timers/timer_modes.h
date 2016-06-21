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

#ifndef TIMER_MODES_H
#define TIMER_MODES_H

#include <lib_ex_config.h>

#if defined(F_TIMERS)


/*
	Can't set duty, will be triggered on MAX
*/
#define TIMER_NORMAL_MODE 0

/**
	Clear Timer on Compare
	Description:
	When the prescaler receives a pulse from a clock cycle
	and passes it onto the Control Logic.  The Control Logic
	increments the TCNTn register by 1.  The TCNTn register
	is compared to the OCRn register, when a compare match
	occurs the TOVn bit is set in the TIFR register.
*/

#define TIMER0_NORMAL 0      /* Top 0xFF */
#define TIMER0_CTC _BV(WGM01)/* Top OCR0A */

#define TIMER1_CTC_OCR1A_MODE 4
#define TIMER1_CTC_ICR1_MODE 12

#define TIMER2_CTC_MODE 2


#endif // F_TIMERS

#endif // TIMER_MODES_H