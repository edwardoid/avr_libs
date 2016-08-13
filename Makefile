LIB_NAME := libex
SRC_DIR := .
OUT_DIR :=./out/
#MCU=atmega328p
#MCU_DEF = -D__AVR_ATmega328P__
#CLOCK :=20000000UL
MCU=atmega8
MCU_DEF = -D__AVR_ATmega8A__
CLOCK :=16000000UL
TOOLCHAIN := E:\Programs\avr-toolchain\avr8-gnu-toolchain\avr
CC := avr-gcc
AR := avr-ar
RN := rm
MKDIR := mkdir
OBJ_DUMP := avr-objdump
SIZE := avr-size	
DU := du
MAKE=make

SOURCES_SUBDIRS = timers
SOURCES_SUBDIRS += usart
SOURCES_SUBDIRS += spi
SOURCES_SUBDIRS += 1wire
SOURCES_SUBDIRS += sd
SOURCES_SUBDIRS += pwm
SOURCES_SUBDIRS += adc
SOURCES_SUBDIRS += pcd8544
SOURCES_SUBDIRS += nrf24l01
SOURCES_SUBDIRS += rc522


OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/*.c))
OBJECTS += $(foreach dir,$(SOURCES_SUBDIRS), $(patsubst %.c, %.o, $(wildcard $(dir)/*.c)))

OBJECT_DIRS = $(addprefix $(OUT_DIR),$(SOURCES_SUBDIRS))
OBJECT_FILES += $(addprefix $(OUT_DIR),$(OBJECTS))

HEADERS = $(wildcard *.h)
INCLUDES = -I$(SRC_DIR)
INCLUDES += -I$(TOOLCHAIN)\include

CFLAGS  = -Os $(MCU_DEF) -DF_CPU=$(CLOCK) $(INCLUDES)
CFLAGS += -std=gnu99 -Wall -funsigned-char -funsigned-bitfields
CFLAGS += -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
CFLAGS += -Wall

%.o: %.c $(HEADERS) prepare
	@ echo $<
	@ $(CC) $(CFLAGS) -mmcu=$(MCU) -c $< -o $(OUT_DIR)/$@


all: prepare libs


libs: $(OBJECTS) $(SUBLIBS)
	@ $(AR) -r $(OUT_DIR)$(LIB_NAME).a  $(addprefix $(OUT_DIR),$(OBJECTS))
#	$(OBJ_DUMP) -h -S $(OUT_DIR)/$(LIB_NAME).a
#	$(SIZE) --format=avr --mcu=$(MCU) $(OUT_DIR)/$(LIB_NAME).a
#	@ $(DU) -h $(OUT_DIR)/$(LIB_NAME).a

prepare:	
	$(MKDIR) -p $(OBJECT_DIRS)


examples: libs
	@echo "Examples"
	@ cd $(SRC_DIR)/examples && $(MAKE) all

clean: prepare
	@ $(RM) -f $(OUT_DIR)$(LIB_NAME).a
	@ $(RM) -f $(OBJECT_FILES)
	@ cd $(SRC_DIR)/examples && $(MAKE) clean

.PHONY: all libs clean prepare examples socket_controller
