#include "1wire.h"

#include "time_utils.h"
#include <avr/interrupt.h>
#include "bitman.h"

#define ow_set_ow_as_input() clear_bit(OW_DDR, OW_PB)
#define ow_set_ow_as_output() set_bit(OW_DDR, OW_PB)
#define ow_read_ow_wire() test_bit(OW_PIN, OW_PB)
#define ow_write_ow_1() set_bit(OW_PORT, OW_PB)
#define ow_write_ow_0() clear_bit(OW_PORT, OW_PB)

#include "uart.h"

void ow_init(ow_conf* cfg)
{
	ow_set_ow_as_input();
	ow_reset_search(cfg);
}

void ow_reset_search(ow_conf* cfg)
{
	// reset the search state
	cfg->LastDiscrepancy = 0;
	cfg->LastDeviceFlag = OW_FALSE;
	cfg->LastFamilyDiscrepancy = 0;
	int i = 7;
	for(; ; i--)
	{
		cfg->ROM_NO[i] = 0;
		if ( i == 0)
			break;
	}
}

void ow_init_config(ow_conf* cfg)
{
	cfg->LastDiscrepancy = 0;
	cfg->LastFamilyDiscrepancy = 0;
	cfg->LastDeviceFlag = 0;
	
	uint8_t i = 8;
	
	while(--i)
	{
		cfg->ROM_NO[i] = 0;
	}
	
	cfg->msg = 0;
}

uint8_t	ow_reset()
{
	uint8_t r = 0;
	ow_write_ow_0();
	ow_set_ow_as_output();
	tu_delay_us(480);
	ow_set_ow_as_input();
	r = ow_read_ow_wire();
	tu_delay_us(420);
	return !r;
}

void	ow_write_bit(uint8_t v)
{
	ow_write_ow_0();
	ow_set_ow_as_output();
	tu_delay_us(1);
	if(v)
		ow_set_ow_as_input();
	tu_delay_us(60);
	ow_set_ow_as_input();
}

void	ow_write_ex(uint8_t v, uint8_t pwr)
{
	uint8_t i = 8;
	while(i--)
	{
		ow_write_bit(v & 0x01);
		v >>= 1;
	}
	
	if(!pwr)
	{
		cli();
		ow_set_ow_as_input();
		ow_write_ow_0();
	
		sei();
	}
}

void ow_write_bytes(const uint8_t *buf, uint16_t count)
{
	uint16_t i = 0;
	for ( ; i < count ; i++)
		ow_write(buf[i]);
}

uint8_t ow_read_bit(void)
{
	uint8_t b = 0;
	ow_write_ow_0();
	ow_set_ow_as_output();
	tu_delay_us(1);
	
	ow_set_ow_as_input();
	tu_delay_us(14);
	
	if(ow_read_ow_wire())
		b = 1;
	tu_delay_us(45);
	return b;
}

uint8_t ow_read() {
	
    uint8_t bitMask = 0xFF;
    uint8_t r = 0;

    for (bitMask = 0x01; bitMask; bitMask <<= 1)
	{
		if (ow_read_bit())
			r |= bitMask;
    }
    return r;
}

void ow_read_bytes(uint8_t *buf, uint16_t count)
{
	uint16_t i = 0;
	for ( ; i < count ; i++)
		buf[i] = ow_read();
}

void ow_select(const uint8_t rom[8])
{
    uint8_t i = 0;

    ow_write(OW_SELECT_ROM_CMD);           // Choose ROM

    for (; i < 8; i++)
		ow_write(rom[i]);
}

void ow_skip()
{
    ow_write(OW_SKIP_ROM_CMD);           // Skip ROM
}

void ow_depower()
{
	cli();
	ow_set_ow_as_input();
	sei();
}

void ow_target_search(uint8_t family_code, ow_conf* cfg)
{
// set the search state to find SearchFamily type devices
	cfg->ROM_NO[0] = family_code;
	uint8_t i = 1;

	for (; i < 8; i++)
	  cfg->ROM_NO[i] = 0;
	cfg->LastDiscrepancy = 64;
	cfg->LastFamilyDiscrepancy = 0;
	cfg->LastDeviceFlag = OW_FALSE;
}

uint8_t ow_search(uint8_t *newAddr, ow_conf* cfg)
{
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;

	unsigned char rom_byte_mask, search_direction;

	// initialize for search
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;

	// if the last call was not the last one
	if (! (cfg->LastDeviceFlag))
	{
		// 1-Wire reset
		if (!ow_reset())
		{
			// reset the search
			cfg->LastDiscrepancy = 0;
			cfg->LastDeviceFlag = OW_FALSE;
			cfg->LastFamilyDiscrepancy = 0;
			cfg->msg = "Reset failed!";
			return OW_FALSE;
		}

		// issue the search command
		ow_write(OW_SEARCH_ROM_CMD);

		// loop to do the search
		do
		{
			// read a bit and its complement
			id_bit = ow_read_bit();
			cmp_id_bit = ow_read_bit();

			// check for no devices on 1-wire
			if ((id_bit == 1) && (cmp_id_bit == 1))
			{
				break;
			}
			else
			{
				// all devices coupled have 0 or 1
				if (id_bit != cmp_id_bit)
					search_direction = id_bit;  // bit write value for search
				else
				{
					if (id_bit_number < cfg->LastDiscrepancy)
						search_direction = (((cfg->ROM_NO[rom_byte_number]) & rom_byte_mask) > 0);
					else
						search_direction = (id_bit_number == cfg->LastDiscrepancy);

					// if 0 was picked then record its position in LastZero
					if (search_direction == 0)
					{
						last_zero = id_bit_number;

						// check for Last discrepancy in family
						if (last_zero < 9)
							cfg->LastFamilyDiscrepancy = last_zero;
					}
				}

				// with mask rom_byte_mask
				if (search_direction == 1)
					cfg->ROM_NO[rom_byte_number] |= rom_byte_mask;
				else
					cfg->ROM_NO[rom_byte_number] &= ~rom_byte_mask;

				// serial number search direction write bit
				ow_write_bit(search_direction);
				// increment the byte counter id_bit_number
				// and shift the mask rom_byte_mask
				id_bit_number++;
				rom_byte_mask <<= 1;

					// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
				if (rom_byte_mask == 0)
				{
					rom_byte_number++;
					rom_byte_mask = 1;
				}
			}
		}
		while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7
			
		// if the search was successful then
		if (!(id_bit_number < 65))
		{
			// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
			cfg->LastDiscrepancy = last_zero;

			// check for last device
			if (cfg->LastDiscrepancy == 0)
				cfg->LastDeviceFlag = OW_TRUE;

			search_result = OW_TRUE;
		}
	}

	// if no device found then reset counters so next 'search' will be like a first
	if (!search_result || !(cfg->ROM_NO[0]))
	{
		cfg->msg = "No devices on the bus";
		cfg->LastDiscrepancy = 0;
		cfg->LastDeviceFlag = OW_FALSE;
		cfg->LastFamilyDiscrepancy = 0;
		search_result = OW_FALSE;
	}
	
	uint8_t i = 0;
	for (; i < 8; i++)
		newAddr[i] = cfg->ROM_NO[i];
	return search_result;
}

uint8_t	ow_read_scratchpad(const uint8_t* rom, uint8_t* data, uint8_t count)
{
	if(!ow_reset())
	{
		return OW_FALSE;
	}
	
	ow_select(rom);
	ow_write(OW_READ_SCRATCHPAD_CMD);
	
	uint8_t i = 0;
	for(; i < count; ++i)
	{
		data[i] = ow_read();
	}
	
	return OW_TRUE;
}

double ow_read_temperature_ds18x2x(const uint8_t* rom)
{
	if(ow_crc8(rom, 7) != rom[7])
	{
		uart_write_string_line("CRC is not valid");
		return -999.999;
	}
		
	if(!ow_reset())
	{
		return -999.999;
	}
	
	ow_select(rom);
	ow_write_ex(OW_START_TEMP_CONV_CMD, 1);
	
	tu_delay_ms(600);
	
	
	uint8_t data[12];
	
	if(OW_FALSE == ow_read_scratchpad(rom, data, 9))
	{
		return -999.999;
	}
	
	
	uint8_t crc = ow_crc8(data, 8);
	if(crc != data[8])
	{
		return -999.999;
	}
	
	int16_t raw = (data[1] << 8) | data[0];
	return (double)raw / 16.0;
}

#ifdef ONEWIRE_CRC8_TABLE
// This table comes from Dallas sample code where it is freely reusable,
// though Copyright (C) 2000 Dallas Semiconductor Corporation
static const uint8_t dscrc_table[] = {
      0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
     35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
     70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
     17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
     50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
     87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//
uint8_t ow_crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		crc = dscrc_table[crc ^ *addr++];
	}
	return crc;
}
#else // ONEWIRE_CRC8_TABLE
//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t ow_crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;
	
	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
#endif

#if ONEWIRE_CRC16
uint8_t ow_check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc_val)
{
    crc_val = ~ ow_crc16(input, len, crc_val);
    return (crc_val & 0xFF) == inverted_crc[0] && (crc_val >> 8) == inverted_crc[1];
}

uint16_t ow_crc16(const uint8_t* input, uint16_t len, uint16_t crc_val)
{
    static const uint8_t oddparity[16] =
        { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

	uint16_t i = 0;
    for ( ; i < len ; i++)
	{
		// Even though we're just copying a byte from the input,
		// we'll be doing 16-bit computation with it.
		uint16_t cdata = input[i];
		cdata = (cdata ^ crc_val) & 0xff;
		crc_val >>= 8;

		if (oddparity[cdata & 0x0F] ^ oddparity[cdata >> 4])
			crc_val ^= 0xC001;

		cdata <<= 6;
		crc_val ^= cdata;
		cdata <<= 1;
		crc_val ^= cdata;
	}
    return crc_val;
}

#endif // ONEWIRE_CRC16