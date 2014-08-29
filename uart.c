#include "uart.h"
#include <avr/io.h>
#include <stdio.h>

void uart_init (int BAUDRATE_H, int BAUDRATE_L)
{
	UBRRH = BAUDRATE_H;
	UBRRL = BAUDRATE_L;

	UCSRB|= (1<<TXEN)|(1<<RXEN);
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

char	uart_read_byte()
{
	while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }

   //Now USART has got data from host
   //and is available is buffer

   return UDR;
}
	
void	uart_write_byte(char data)
{
	while(!(UCSRA & (1<<UDRE)));

   //Now write the data to USART buffer

   UDR=data;
}

void	uart_write_string(const char* s)
{
	while (*s)
		uart_write_byte(*s++);
}

void	uart_write_num(int num)
{
	char str[32] = {};
	sprintf(str, "%d", num);
	uart_write_string(str);
}

void	uart_write_num_hex(int num)
{
	char str[32] = {};
	sprintf(str, "%2X", num);
	uart_write_string(str);
}

void	uart_write_string_line(const char* s)
{
	while (*s)
		uart_write_byte(*s++);
	uart_write_byte('\n');
}
