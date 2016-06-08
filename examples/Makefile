.PHONY: all socket_controller

MAKE=make

all: rc522

socket_controller:
	$(MAKE) -C socket_controller all

clean:
	cd socket_controller && $(MAKE) clean
	cd rc522_test && $(MAKE) clean

rc522:
	cd rc522_test && $(MAKE) all
