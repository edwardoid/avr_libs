.PHONY: all socket_controller rc522 adc timers servo nrf24l01

MAKE=make

all: nrf24l01

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


nrf24l01:
	cd nrf24l01 && $(MAKE) all