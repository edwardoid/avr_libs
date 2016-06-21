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

#ifndef TIMERS_H
#define TIMERS_H

#include <lib_ex_config.h>
#include "my_types.h"

#if defined(F_TIMERS)

#define TIMER_PRESCALE_0 1
#define TIMER_PRESCALE_8 8
#define TIMER_PRESCALE_64 64
#define TIMER_PRESCALE_256 256
#define TIMER_PRESCALE_1024 1024 

#include "timer_modes.h"

#if defined(F_TIMERS_ENABLE_CALLBACKS)
void timers_start_0_normal(uint16_t prescale, callback_t cb); // Normal Mode upto 0xFF
void timers_start_0_ctc(uint8_t cycle, uint16_t prescale, callback_t cb); // CTC mode, up to OCR0A 


void timers_start_1_normal(uint16_t prescale, callback_t cb);
void timers_start_1(uint16_t cycle, uint8_t mode, uint16_t prescale, callback_t cb);

void timers_start_2_normal(uint16_t prescale, callback_t cb); // Normal Mode upto 0xFF
void timers_start_2_ctc(uint8_t cycle, uint16_t prescale, callback_t cb); // CTC mode, up to OCR0A 

#else
void timers_start_0_normal(uint16_t prescale);
void timers_start_0_crc(uint8_t cycle, uint16_t prescale);

void timers_start_1_normal(uint16_t prescale);
void timers_start_1(uint16_t cycle, uint8_t mode, uint16_t prescale);


void timers_start_2_normal(uint16_t prescale);
void timers_start_2_ctc(uint8_t cycle, uint16_t prescale);

#endif // F_TIMERS_ENABLE_CALLBACKS

void timers_set_prescaler_0(uint16_t prescale);
void timers_set_prescaler_1(uint16_t prescale);
void timers_set_prescaler_2(uint16_t prescale);

uint16_t timers_get_prescaler_0();
uint16_t timers_get_prescaler_1();
uint16_t timers_get_prescaler_2();

void timers_set_cycle_0(uint8_t cycle);
void timers_set_cycle_1(uint16_t cycle);
void timers_set_cycle_2(uint8_t cycle);

void timers_set_timeout_ms_0(uint32_t ms);
void timers_set_timeout_ms_1(uint32_t ms);
void timers_set_timeout_ms_2(uint32_t ms);

#endif // F_TIMERS

#endif // TIMERS_H