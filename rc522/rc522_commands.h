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

#ifndef RC522_COMMANDS_H

#include <lib_ex_config.h>

#if defined(F_RC522)

#include <bytes_manip.h>

#define RC522_CMD_Idle			b0000
#define RC522_CMD_Mem			b0001
#define RC522_CMD_GenRndId		b0010
#define RC522_CMD_CalcCRC		b0011
#define RC522_CMD_Transmit		b0100
#define RC522_CMD_NoCmdChange 	b0111
#define RC522_CMD_Receive		b1000
#define RC522_CMD_Transceive	b1100
#define RC522_CMD_MFAuthent		b1110
#define RC522_CMD_SoftReset		b1111

#endif // F_RC522

#endif // RC522_COMMANDS_H