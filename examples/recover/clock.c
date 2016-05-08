#include <avr/io.h>

int main(void) {
  DDRB = 0xFF;
	while(1) {
    	PORTB ^= 0xFF;
	}
}