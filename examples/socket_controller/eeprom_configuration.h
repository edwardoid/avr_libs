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

#ifndef EEPROM_CONFIGURATION_H
#define EEPROM_CONFIGURATION_H

#include "config.h"

#ifdef F_EXAMPLES

#include <inttypes.h>

#define EEPROM_INIT_FLAG_ADDR ((void*)0)
#define EEPROM_SETTINGS_ADDR ((void*)((EEPROM_INIT_FLAG_ADDR + sizeof(uint8_t))))

#endif // F_EXAMPLES

#endif /* EEPROM_CONFIGURATION_H_ */