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

#include <lib_ex_config.h>

#ifndef USART_CONFIG_H
#define USART_CONFIG_H

#ifdef F_USART

#include <avr/io.h>

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__)

#define TX_ENABLE_BIT			TXEN
#define RX_ENABLE_BIT			RXEN
#define USART_CTL_A				UCSRA
#define USART_CTL_B				UCSRB
#define USART_CTL_C				UCSRC
#define USART_BR_HI_REG			UBRRH
#define USART_BR_LO_REG			UBRRL
#define USART_BR_SEL			URSEL
#define USART_FRAME_BIT0		UCSZ0
#define USART_FRAME_BIT1		UCSZ1
#define USART_DATA_REG			UDR
#define USART_DATA_READY_BIT	UDRE
#define USART_GOT_DATA_BIT		RXC

#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

#define TX_ENABLE_BIT			TXEN0
#define RX_ENABLE_BIT			RXEN0
#define USART_CTL_A				UCSR0A
#define USART_CTL_B				UCSR0B
#define USART_CTL_C				UCSR0C
#define USART_BR_HI_REG			UBRR0H
#define USART_BR_LO_REG			UBRR0L
#define USART_BR_SEL			UMSEL01
#define USART_FRAME_BIT0		UCSZ00
#define USART_FRAME_BIT1		UCSZ01
#define USART_DATA_REG			UDR0
#define USART_DATA_READY_BIT	UDRE0
#define USART_GOT_DATA_BIT		RXC0

#endif // __AVR_ATmega328

#endif // F_USART



#endif /* USART_CONFIG_H */