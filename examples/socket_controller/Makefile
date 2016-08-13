EXAMPLE_NAME := socket_controller
SRC_DIR :=.
OUT_DIR :=./out
#MCU=atmega328p
#MCU_DEF = -D__AVR_ATmega328P__
#CLOCK :=20000000UL
MCU=atmega8
MCU_DEF = -D__AVR_ATmega8A__
CLOCK :=16000000UL
TOOLCHAIN := D:\Programs\WinAvr\avr
CC := avr-gcc
RM := rm
OBJ_DUMP := avr-objdump
SIZE := avr-size
MKDIR := mkdir
AR := avr-ar

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/*.c))

HEADERS = $(wildcard *.h)
INCLUDES := -I$(SRC_DIR)/../..

LIBDIRS = -L./../../out
LIBS = -lex

CFLAGS  = -Os $(MCU_DEF) -DF_CPU=$(CLOCK) $(INCLUDES)
CFLAGS += -std=gnu99 -Wall -funsigned-char -funsigned-bitfields
CFLAGS += -fpack-struct -fshort-enums -ffunction-sections
CFLAGS += -fdata-sections
BIN_CFLAGS := -mmcu=$(MCU)
LFLAGS = $(LIBDIRS) $(LIBS) -Wl,--gc-sections

%.o: %.c $(HEADERS) prepare
	$(CC) $(CFLAGS) -mmcu=$(MCU) -c $< -o $@

all: prepare app	

app: $(OBJECTS)
	$(CC) $(BIN_CFLAGS) $(OBJECTS) $(LFLAGS) -o $(OUT_DIR)/$(EXAMPLE_NAME).elf
#	$(OBJ_DUMP) -h -S $(OUT_DIR)/$(EXAMPLE_NAME).elf
	$(SIZE) --format=avr --mcu=$(MCU) $(OUT_DIR)/$(EXAMPLE_NAME).elf

prepare:
	$(MKDIR) -p $(OUT_DIR)

clean: prepare
	$(RM) -f $(OBJECTS)
	$(RM) -f $(OUT_DIR)/$(OUT_DIR)/*
	
.PHONY: all app clean prepare