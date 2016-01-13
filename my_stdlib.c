/*
 * CFile1.c
 *
 * Created: 11/11/2014 6:23:52 PM
 *  Author: eduards
 */ 

#include "my_stdlib.h"

#include "config.h"
#include "my_types.h"
#include "usart.h"

#ifndef USE_STD_LIB

uint8_t	get_digits_count(int32_t val)
{
	if(val < 0)
		val = -val;
	uint8_t digits_count = val == 0 ? 1 : 0;
	
	while(val != 0)
	{
		++digits_count;
		val /= 10;
	}
	
	return digits_count;
}

void itoa(int32_t val, char* str)
{
	uint8_t digits_count = val == 0 ? 1 : 0;
	int32_t n = val;
	
	if(val < 0)
	{
		*str = '-';
		++str;
		val *= -1;
	}
	
	while(n != 0)
	{
		++digits_count;
		n /= 10;
	}
	
	str += digits_count;
	
	while(digits_count-- > 0)
	{
		*(--str) = '0' + val % 10;
		val /= 10;
	}
}

void	ftoa(float	val, uint8_t precision, char*	str)
{
	if(str == NULL)
		return;
		
	uint8_t dc = get_digits_count(val);
	
	itoa(val, str);
	usart_write_string(str);
	if(val < 0)
		val = -val;
	
	val -= (int16_t)(val);
	
	while(precision--)
	{
		val *= 10;
	}
	
	str += dc;
	*str = '.';
	
	itoa(val, ++str);
}

void*	memset(void * buff, int val, uint16_t sz)
{
	while(sz--)
	{
		*((byte*)buff++) = val;
	}
	
	return buff - sz;
}

#endif // USE_STD_LIB