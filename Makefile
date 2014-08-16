CC=avr-gcc
OBJ_CP=avr-objcopy
DUDE=avrdude
DUDE_MC=atmega8
CC_MC=atmega8
AVR_DEF=__AVR_ATmega8__
HEX_FILE=firmware.hex
LOW_F=0xFF
HIGH_F=0xC9
SRC_DIR=./
OBJ_DIR=./
DEP_DIR=./
OPT_LEVEL=-Os
W_LEVEL=-Wall
CLOCK_F=16000000UL
SRC_FILES:=$(wildcard $(SRC_DIR)*.c)
OBJ_FILES:=$(patsubst $(SRC_DIR), $(OBJ_DIR),$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES)))
ELF_FILES:=$(patsubst $(SRC_DIR), $(OBJ_DIR),$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.elf,$(SRC_FILES)))
DEFILES:=$(patsubst $(SRC_DIR), $(DEP_DIR),$(patsubst $(SRC_DIR)%.c,$(DEP_DIR)%.o,$(SRC_FILES)))


PHONY: all
all: flash


.PHONY: compile
compile: $(ELF_FILES)
	$(OBJ_CP) -j .text -j .data $(ELF_FILES) $(HEX_FILE)

$(OBJ_DIR)%.elf: $(SRC_DIR)%.c
	$(CC) $(OPT_LEVEL) $(W_LEVEL) -mmcu=$(CC_MC) $< -DF_CPU=$(CLOCK_F) -D$(AVR_DEF) -o $@ 

$(DEP_DIR)%.dep: $(SRC_DIR)%.c
	$(CC) -MM $< -MT "$@ $(patsubst $(DEP_DIR)%.dep,$(OBJ_DIR)%o,$@)" -mmcu=$(CC_MC) -DF_CPU=$(CLOCK_F) -D$(AR_DEF) -o $@


-include $(DEP_FILES)

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.elf
	rm -f $(HEX_FILE)

.PHONY: fuses
fuses:
	sudo $(DUDE) -c usbasp -p $(DUDE_MC) -U lfuse:w:$(LOW_F):m -U hfuse:w:$(HIGH_F):m

.PHONY: flash
flash: compile
	sudo $(DUDE) -c usbasp -p $(DUDE_MC) -B 10 -u -U flash:w:$(HEX_FILE)
