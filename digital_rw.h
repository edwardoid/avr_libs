#ifndef READ_DIGITAL_VALUE_H
#define READ_DIGITAL_VALUE_H

#include <avr/io.h>

#define read_digital(port, pin) (bit_is_set(port, pin))
#define rd(port, pin) read_digital(port, pin)

#define write_digital_high(port, pin) sbi(port, pin)
#define wdh(port, pin) write_digital_high(port, pin)

#define write_digital_low(port, pin) cbi(port, pin)
#define wdl(port, pin) write_digital_low(port, pin)

#define _HIGH 1
#define _LOW 0

#define digital_toggle(port, pin) (port ^= _BV(pin)
#define write_digital(port, pin, val) (val != _LOW ? wdh(port, pin) : wdl(port, bin)

#endif // READ_DIGITAL_VALUE