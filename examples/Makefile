.PHONY: all socket_controller rc522 adc timers servo

MAKE=make

all: timers

socket_controller:
	$(MAKE) -C socket_controller all

clean:
	cd socket_controller && $(MAKE) clean
	cd adc && $(MAKE) clean

rc522:
	cd rc522_test && $(MAKE) all

adc:
	cd adc && $(MAKE) all

timers:
	cd timers && $(MAKE) all

servo:
	cd servo && $(MAKE) all