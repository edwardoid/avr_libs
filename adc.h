#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include "bitman.h"
#include <stdint.h>

#define ADC_SRC_AREF	1
#define ADC_SRC_AVCC	2
#define ADC_SRC_INT		3

void		adc_select_source(uint8_t source);
uint16_t 	adc_read(uint8_t pin);

#endif // ADC_H
