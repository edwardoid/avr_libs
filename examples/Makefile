.PHONY: all socket_controller

all: rc522

socket_controller:
	make -C socket_controller all

clean: 
	cd socket_controller && make clean
	cd rc522_test && make clean

rc522:
	cd rc522_test && make all