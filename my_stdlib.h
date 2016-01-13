/*
 * my_stdlib.h
 *
 * Created: 11/11/2014 6:16:56 PM
 *  Author: eduards
 */ 


#ifndef MY_STDLIB_H
#define MY_STDLIB_H

#include "config.h"
#include <stdint.h>

#define my_abs(v) ((v > 0) ? (v) : -(v))


#ifdef USE_STDLIB

#include <stdlib.h>

#else

void*	memset(void * buff, int val, uint16_t sz);
void	itoa(int32_t val, char * str);
#ifndef NULL
#define NULL 0
#endif // NULL


#endif // USE_STDLIB

void	ftoa(float	val, uint8_t precision, char*	str);

#endif /* MY_STDLIB_H_ */