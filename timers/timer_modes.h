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

// Timer/Counter0 [
#define TIMER0_NORMAL 0x0    /* Top 0xFF */
#define TIMER0_CTC _BV(WGM01)/* Top OCR0A */
// ] Timer/Counter0

// Timer/Counter1 [
#define TIMER1_NORMAL 0x0									/* Top 0xFFFF */
#define TIMER1_CTC_OCR1A	(_BV(WGM12))				/* Top OCR1A */
#define TIMER1_CTC_ICR1		(_BV(WGM13) | _BV(WGM12))	/* Top ICR1 */
// ] Timer/Counter1

// Timer/Counter2 [
#define TIMER2_NORMAL	0x00			/* Top 0xFF */
#define TIMER2_CTC		(_BV(WGM21))	/* Top OCR2 */
// ] Timer/Counter2


#endif // F_TIMERS

#endif // TIMER_MODES_H
