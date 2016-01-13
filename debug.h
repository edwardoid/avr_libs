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

#ifndef DEBUG_H
#define DEBUG_H

#include "usart.h"
#include "time_utils_counters.h"

#define ENABLE_USART_DEBUGGING 1

#ifdef ENABLE_USART_DEBUGGING
#define DBG_MSG(msg) usart_write_byte('['); usart_write_u32num(tu_millis()); usart_write_string("]: "); usart_write_string_line(msg);
#define DBG_WRN(wrn) usart_write_byte('['); usart_write_u32num(tu_millis()); usart_write_string("] ?: "); usart_write_string_line(wrn);
#define DBG_ERR(err) usart_write_byte('['); usart_write_u32num(tu_millis()); usart_write_string("] !: "); usart_write_string_line(err);	
#else
#define DBG_MSG(msg) /* msg */
#define DBG_WRN(wrn) /* wrn */
#define DBG_ERR(err) /* err */
#endif

#endif // DEBUB_H