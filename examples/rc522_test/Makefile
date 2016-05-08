EXAMPLE_NAME = rc522_test
SRC_DIR = .
OUT_DIR = ./out
MCU=atmega328p
MCU_DEF = -D__AVR_ATmega328P__
CLOCK :=20000000UL
TOOLCHAIN = D:\Programs\WinAvr\avr
CC = avr-gcc
RM = rm
OBJ_DUMP = avr-objdump
SIZE = avr-size
OBJ_CPY = avr-objcopy
MKDIR = mkdir
AR = avr-ar
PROGRAMMER = usbasp

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
	@ $(CC) $(CFLAGS) -mmcu=$(MCU) -c $< -o $@

all: flash

app: $(OBJECTS)
	@ $(CC) $(BIN_CFLAGS) $(OBJECTS) $(LFLAGS) -o $(OUT_DIR)/$(EXAMPLE_NAME).elf
	@ $(OBJ_CPY) -O ihex -R .eeprom $(OUT_DIR)/$(EXAMPLE_NAME).elf $(OUT_DIR)/$(EXAMPLE_NAME).hex

size: app
	@ $(SIZE) --format=avr --mcu=$(MCU) $(OUT_DIR)/$(EXAMPLE_NAME).elf

flash: app
	@echo "Uploading..."
	avrdude -c $(PROGRAMMER) -p $(MCU) -U flash:w:"$(OUT_DIR)/$(EXAMPLE_NAME).hex"

prepare:
	@ $(MKDIR) -p $(OUT_DIR)

clean: prepare
	@ $(RM) -f $(OBJECTS)
	@ $(RM) -f $(OUT_DIR)/$(EXAMPLE_NAME).elf
	@ $(RM) -f $(OUT_DIR)/$(EXAMPLE_NAME).hex
	
.PHONY: all size flash app clean prepare