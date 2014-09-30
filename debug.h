#ifndef DEBUG_H
#define DEBUG_H

#include "uart.h"
#include "time_utils_counters.h"

#define ENABLE_UART_DEBUGGING 1

#ifdef ENABLE_UART_DEBUGGING
#define DBG_MSG(msg) uart_write_byte('['); uart_write_u32num(tu_millis()); uart_write_string("] MSG: "); uart_write_string_line(msg);
#define DBG_WRN(wrn) uart_write_byte('['); uart_write_u32num(tu_millis()); uart_write_string("] WRN: "); uart_write_string_line(wrn);
#define DBG_ERR(err) uart_write_byte('['); uart_write_u32num(tu_millis()); uart_write_string("] ERR: "); uart_write_string_line(err);	
#else
#define DBG_MSG(msg) /* msg */
#define DBG_WRN(wrn) /* wrn */
#define DBG_ERR(err) /* err */
#endif

#endif // DEBUB_H