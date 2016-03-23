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

#ifndef SD_H
#define SD_H

#include "config.h"

#ifdef F_SD

#include "my_types.h"
#include <spi/spi.h>
#include "debug.h"
#include "sd_cmd.h"

#define TOKEN_WAIT_MAX_TIMEOUT 1000
#define SD_CMD_RESP_MAX_TIMEOUT 3000
#define SD_IDLE_WAIT_MAX_TIMEOUT 3000

#define SD_R1	1
#define SD_R1B	2
#define SD_R2	3
#define SD_R3	4

#define SD_CRC 0x95

#define SD_UNKNOWN_TYPE 0
#define SD_MMC	(1 << 0)
#define SD_SD	(1 << 2)
#define SD_SDHC (1 << 3)

int8_t	sd_init(ddr_ptr_t ddr, port_ptr_t port, uint8_t pin);

int8_t sd_send_command(uint8_t cmd, uint32_t args,port_ptr_t port, uint8_t pin, char* resp);

#endif // F_SD

#endif // SD_H