/*
	avr_libs
	Copyright (C) 2014  Edward Sargsyan

	avr_libs is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	avr_libs is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with avr_libs.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ONE_WIRE_H
#define ONE_WIRE_H

#include "my_types.h"

/*
#ifndef OW_PORT
#define OW_PORT PORTB
#endif


#ifndef OW_PB
#define OW_PB PB2
#endif

#ifndef OW_PIN
#define OW_PIN PINB
#endif

#ifndef OW_DDR
#define OW_DDR DDRB
#endif
*/
#define OW_TRUE				0x01
#define OW_FALSE			0x00

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 1
#endif

#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 1
#endif
	
#include <inttypes.h>

#define OW_SEARCH_ROM_CMD		0xF0
#define OW_SELECT_ROM_CMD		0x55
#define	OW_READ_ROM_CMD			0x33
#define OW_SKIP_ROM_CMD			0xCC
#define OW_ALARM_SEARCH_CMD		0xEC
#define OW_START_TEMP_CONV_CMD	0x44
#define OW_WRITE_SCRATCHPAD_CMD	0x4E
#define OW_READ_SCRATCHPAD_CMD	0xBE
#define OW_COPY_SCRATCHPAD_CMD	0x48
#define	OW_RECALL_E				0xB8
#define OW_READ_POWER_SUPPY_CMD 0xB4



#define OW_ADDR_DEV_DS18S20	0x10
#define OW_ADDR_DEV_DS18B20	0x28
#define OW_ADDR_DEV_DS1822	0x22

typedef struct
{
	uint8_t LastDiscrepancy;
	uint8_t LastFamilyDiscrepancy;
	uint8_t LastDeviceFlag;
	uint8_t ROM_NO[8];
	ddr_ptr_t	ddr;
	port_ptr_t	port;
	pin_ptr_t	pin;
	uint8_t		pin_num;
} ow_conf;
													

void ow_init_config(ow_conf* cfg);

void ow_init(ow_conf* cfg);

uint8_t	ow_reset(ow_conf* cfg);

void	ow_write_bit(ow_conf* cfg, uint8_t v);

#define ow_write(cfg, v) ow_write_ex(cfg, v, 0);

void ow_write_ex(ow_conf* cfg, uint8_t v, uint8_t pwr);

void ow_write_bytes(ow_conf* cfg, const uint8_t *buf, uint16_t count);

uint8_t ow_read_bit(ow_conf* cfg);

// Read a byte
//
uint8_t ow_read(ow_conf* cfg);

void ow_read_bytes(ow_conf* cfg, uint8_t *buf, uint16_t count);

void ow_select(ow_conf* cfg, const uint8_t rom[8]);

void ow_skip();

void ow_depower();

void ow_reset_search(ow_conf* cfg);

void ow_target_search(uint8_t family_code, ow_conf* cfg);

uint8_t ow_search(uint8_t *newAddr, ow_conf* cfg);

uint8_t ow_crc8(const uint8_t *addr, uint8_t len);

#if ONEWIRE_CRC16
uint8_t ow_check_crc16(	const	uint8_t*	input,
								uint16_t	len,
						const	uint8_t*	inverted_crc,
								uint16_t	crc_val);

uint16_t ow_crc16(const uint8_t* input, uint16_t len, uint16_t crc_val);


uint8_t	ow_read_scratchpad(ow_conf* cfg, const uint8_t* rom /* [8] */ , uint8_t* data, uint8_t count);

double ow_read_temperature_ds18x2x(ow_conf* cfg, const uint8_t* rom);

#endif // ONEWIRE_CRC16

#endif // ONE_WIRE_H
