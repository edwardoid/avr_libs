avr_libs
========
Set of useful functions and libraries for Atmel AVR microcontrollers

Structure
=========
1. Soms functions are **declared** and **defined** in separate header files. So you need to include them sequentially.
2. All functions does not check previous states(if it's not necessary), so take care about checks and preparations.
3. Some functions require interrupt handlers, so sometimes you will not be able to use you own interrups. [ISR](http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html#gad28590624d422cdf30d626e0a506255f). This interrupt handlers will be registered when you include function which needs interrupt results.
	
Supporting microcontrollers
============================
At this moment the library will be tested on Atmega8. Atmega328 and Atmega168 are very similar with Atmega8, so you will be able to use the library with theese microcontrollers as well. Also some functionality will be tested with Attiny85(similar with Attiny45).