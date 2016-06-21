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

#ifndef RC522_CONF_H
#define RC522_CONF_H

#if defined(F_RC522) && defined(F_SPI)

#include <my_types.h>

typedef struct
{
	pin_cfg_t ss;
} rc522_cfg_t;


#endif // F_RC522 && F_SPI

#endif // RC522_CONF_H