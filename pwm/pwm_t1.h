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

#include "my_types.h"

#define PWM_T1_PHASE_CORRECTED_8BIT				(_BV(WGM10))										/* TOP = 0x00FF */
#define PWM_T1_PHASE_CORRECTED_9BIT 			(_BV(WGM11))										/* TOP = 0x01FF */
#define PWM_T1_PHASE_CORRECTED_10BIT 			(_BV(WGM11)) | (_BV(WGM10)))						/* TOP = 0x03FF */
#define PWM_T1_FAST_8BIT 						(_BV(WGM12) | _BV(WGM10) )							/* top = 0xFF */
#define PWM_T1_FAST_9BIT						(_BV(WGM12) | _BV(WGM11))							/* top = 0xFF */
#define PWM_T1_FAST_10BIT						(_BV(WGM12) | _BV(WGM11) | _BV(WGM10))				/* top = 0xFF */
#define PWM_T1_PHASE_FREQ_CORRECTED_ICR1		(_BV(WGM13))										/* TOP = ICR1 */
#define PWM_T1_PHASE_FREQ_CORRECTED_OCR1A		(_BV(WGM13) | _BV(WGM10))							/* TOP = OCR1A */
#define PWM_T1_PHASE_CORRECTED_ICR1 			(_BV(WGM13) | _BV(WGM11))							/* TOP = ICR1 */
#define PWM_T1_PHASE_CORRECTED_OCR1A 			(_BV(WGM13) | _BV(WGM11) | _BV(WGM10))				/* TOP = OCR1A */
#define PWM_T1_FAST_ICR1 						(_BV(WGM13) | _BV(WGM12) | _BV(WGM10))				/* top = ICR1 */
#define PWM_T1_FAST_OCR1A 						(_BV(WGM13) | _BV(WGM12) | _BV(WGM11) | _BV(WGM10))	/* top = OCR1A */


#define PWM_T1_NON_INVERTED_OC1A (_BV(COM1A1)) 
#define PWM_T1_INVERTED_OC1A (_BV(COM1A1) | _BV(COM1A0))


#define PWM_T1_NON_INVERTED_OC1B (_BV(COM1B1))
#define PWM_T1_INVERTED_OC1B (_BV(COM1B1) | _BV(COM1B0))

void pwm_t1_start(uint8_t mode, uint16_t top, uint8_t prescale);
void pwm_t1_stop();
void pwm_t1_set_invert_mode(uint8_t invert);
void pwm_t1_set_top(uint8_t top);
void pwm_t1_set_duty(uint8_t duty); /* 0..100 */

#endif // F_PWM

#endif // PWM_T1_H
