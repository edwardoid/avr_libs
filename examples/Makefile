.PHONY: all socket_controller rc522 adc servo

MAKE=make

all: servo

socket_controller:
	$(MAKE) -C socket_controller all

clean:
	cd socket_controller && $(MAKE) clean
	cd adc && $(MAKE) clean

rc522:
	cd rc522_test && $(MAKE) all

adc:
	cd adc && $(MAKE) all

servo:
	cd servo && $(MAKE) all