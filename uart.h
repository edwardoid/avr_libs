#ifndef UART_H
#define UART_H

#include "bitman.h"

#define _4800_UBBRH 0x00
#define _4800_UBBRL 0xCF
#define _4800_UBBR concat_bytes(_4800_UBBRH, _4800_UBBRL) 

#define _9600_UBBRH 0x00
#define _9600_UBBRL 0x67
#define _9600_UBBR concat_bytes(_9600_UBBRH, _9600_UBBRL) 

#define _19200_UBBRH 0x00
#define _19200_UBBRL 0x33
#define _19200_UBBR concat_bytes(_19200_UBBRH, _19200_UBBRL)

#define _38400_UBBRH 0x00
#define _38400_UBBRL 0x19
#define _38400_UBBR concat_bytes(_38400_UBBRH, _38400_UBBRL) 

#define _57600_UBBRH 0x00
#define _57600_UBBRL 0x10
#define _57600_UBBR concat_bytes(_57600_UBBRH, _57600_UBBRL) 

#define _115200_UBBRH 0x00
#define _115200_UBBRL 0x08
#define _115200_UBBR concat_bytes(_115200_UBBRH, _115200_UBBRL) 

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

void	uart_write_string(char* s)
{
	while (*s)
		uart_write_byte(*s++);
}

#endif // UART_H
