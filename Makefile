LIB_NAME := libex
SRC_DIR :=.
OUT_DIR :=./out
MCU=atmega8
MCU_DEF := -D__AVR_ATmega8__
CLOCK=20000000UL
TOOLCHAIN := D:\Programs\WinAvr\avr
CC := avr-gcc
AR := avr-ar
RN := rm
MKDIR := mkdir
OBJ_DUMP := avr-objdump
SIZE := avr-size
DU := du

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/*.c))

OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/usart/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/spi/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/1wire/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/sd/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/pwm/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/adc/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/pcd8544/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/nrf2l01/*.c))

OBJECT_DIRS += $(addprefix $(OUT_DIR)/,$(dir $(OBJECTS)))
OBJECT_FILES += $(addprefix $(OUT_DIR)/,$(OBJECTS))

HEADERS = $(wildcard *.h)
INCLUDES := -I$(SRC_DIR)

CFLAGS  = -Os $(MCU_DEF) -DF_CPU=$(CLOCK) $(INCLUDES)
CFLAGS += -std=gnu99 -Wall -funsigned-char -funsigned-bitfields
CFLAGS += -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
CFLAGS += -Wall

%.o: %.c $(HEADERS) prepare
	$(CC) $(CFLAGS) -mmcu=$(MCU) -c $< -o $(OUT_DIR)/$@

all: prepare libs	

libs: $(OBJECTS) $(SUBLIBS)
	$(AR) -r $(OUT_DIR)/$(LIB_NAME).a $(addprefix $(OUT_DIR)/,$(OBJECTS))
	$(OBJ_DUMP) -h -S $(OUT_DIR)/$(LIB_NAME).a 
	$(SIZE) --format=avr --mcu=$(MCU) $(OUT_DIR)/$(LIB_NAME).a
	$(DU) -h $(OUT_DIR)/$(LIB_NAME).a

prepare:
	$(MKDIR) -p $(OBJECT_DIRS)


examples: socket_controller

socket_controller: libs 
	make -C $(SRC_DIR)/example

clean: prepare
	$(RM) -f $(OUT_DIR)/$(LIB_NAME).a
	$(RM) -f $(OBJECT_FILES)

.PHONY: all libs clean prepare examples socket_controller