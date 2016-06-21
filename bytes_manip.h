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

#ifndef BYTES_MANIP_H
#define BYTES_MANIP_H

#include <avr/sfr_defs.h>



#define concat_bytes(high, low) (((high) << 8) | (low))
#define high_byte(dword) (((dword) >> 8) & 	0xFF)
#define low_byte(dword) ((dword) & 	0xFF)

// Short

#define b0000 	0x00
#define b0001 	0x01
#define b0010 	0x02
#define b0011 	0x03
#define b0100 	0x04
#define b0101 	0x05
#define b0110 	0x06
#define b0111 	0x07
#define b1000 	0x08
#define b1001 	0x09
#define b1010 	0x0a
#define b1011 	0x0b
#define b1100 	0x0c
#define b1101 	0x0d
#define b1110 	0x0e
#define b1111 	0x0f

// Long
#define b00000000 	0x00
#define b00000001 	0x01
#define b00000010 	0x02
#define b00000011 	0x03
#define b00000100 	0x04
#define b00000101 	0x05
#define b00000110 	0x06
#define b00000111 	0x07
#define b00001000 	0x08
#define b00001001 	0x09
#define b00001010 	0x0a
#define b00001011 	0x0b
#define b00001100 	0x0c
#define b00001101 	0x0d
#define b00001110 	0x0e
#define b00001111 	0x0f
#define b00010000 	0x10
#define b00010001 	0x11
#define b00010010 	0x12
#define b00010011 	0x13
#define b00010100 	0x14
#define b00010101 	0x15
#define b00010110 	0x16
#define b00010111 	0x17
#define b00011000 	0x18
#define b00011001 	0x19
#define b00011010 	0x1a
#define b00011011 	0x1b
#define b00011100 	0x1c
#define b00011101 	0x1d
#define b00011110 	0x1e
#define b00011111 	0x1f
#define b00100000 	0x20
#define b00100001 	0x21
#define b00100010 	0x22
#define b00100011 	0x23
#define b00100100 	0x24
#define b00100101 	0x25
#define b00100110 	0x26
#define b00100111 	0x27
#define b00101000 	0x28
#define b00101001 	0x29
#define b00101010 	0x2a
#define b00101011 	0x2b
#define b00101100 	0x2c
#define b00101101 	0x2d
#define b00101110 	0x2e
#define b00101111 	0x2f
#define b00110000 	0x30
#define b00110001 	0x31
#define b00110010 	0x32
#define b00110011 	0x33
#define b00110100 	0x34
#define b00110101 	0x35
#define b00110110 	0x36
#define b00110111 	0x37
#define b00111000 	0x38
#define b00111001 	0x39
#define b00111010 	0x3a
#define b00111011 	0x3b
#define b00111100 	0x3c
#define b00111101 	0x3d
#define b00111110 	0x3e
#define b00111111 	0x3f
#define b01000000 	0x40
#define b01000001 	0x41
#define b01000010 	0x42
#define b01000011 	0x43
#define b01000100 	0x44
#define b01000101 	0x45
#define b01000110 	0x46
#define b01000111 	0x47
#define b01001000 	0x48
#define b01001001 	0x49
#define b01001010 	0x4a
#define b01001011 	0x4b
#define b01001100 	0x4c
#define b01001101 	0x4d
#define b01001110 	0x4e
#define b01001111 	0x4f
#define b01010000 	0x50
#define b01010001 	0x51
#define b01010010 	0x52
#define b01010011 	0x53
#define b01010100 	0x54
#define b01010101 	0x55
#define b01010110 	0x56
#define b01010111 	0x57
#define b01011000 	0x58
#define b01011001 	0x59
#define b01011010 	0x5a
#define b01011011 	0x5b
#define b01011100 	0x5c
#define b01011101 	0x5d
#define b01011110 	0x5e
#define b01011111 	0x5f
#define b01100000 	0x60
#define b01100001 	0x61
#define b01100010 	0x62
#define b01100011 	0x63
#define b01100100 	0x64
#define b01100101 	0x65
#define b01100110 	0x66
#define b01100111 	0x67
#define b01101000 	0x68
#define b01101001 	0x69
#define b01101010 	0x6a
#define b01101011 	0x6b
#define b01101100 	0x6c
#define b01101101 	0x6d
#define b01101110 	0x6e
#define b01101111 	0x6f
#define b01110000 	0x70
#define b01110001 	0x71
#define b01110010 	0x72
#define b01110011 	0x73
#define b01110100 	0x74
#define b01110101 	0x75
#define b01110110 	0x76
#define b01110111 	0x77
#define b01111000 	0x78
#define b01111001 	0x79
#define b01111010 	0x7a
#define b01111011 	0x7b
#define b01111100 	0x7c
#define b01111101 	0x7d
#define b01111110 	0x7e
#define b01111111 	0x7f
#define b10000000 	0x80
#define b10000001 	0x81
#define b10000010 	0x82
#define b10000011 	0x83
#define b10000100 	0x84
#define b10000101 	0x85
#define b10000110 	0x86
#define b10000111 	0x87
#define b10001000 	0x88
#define b10001001 	0x89
#define b10001010 	0x8a
#define b10001011 	0x8b
#define b10001100 	0x8c
#define b10001101 	0x8d
#define b10001110 	0x8e
#define b10001111 	0x8f
#define b10010000 	0x90
#define b10010001 	0x91
#define b10010010 	0x92
#define b10010011 	0x93
#define b10010100 	0x94
#define b10010101 	0x95
#define b10010110 	0x96
#define b10010111 	0x97
#define b10011000 	0x98
#define b10011001 	0x99
#define b10011010 	0x9a
#define b10011011 	0x9b
#define b10011100 	0x9c
#define b10011101 	0x9d
#define b10011110 	0x9e
#define b10011111 	0x9f
#define b10100000 	0xa0
#define b10100001 	0xa1
#define b10100010 	0xa2
#define b10100011 	0xa3
#define b10100100 	0xa4
#define b10100101 	0xa5
#define b10100110 	0xa6
#define b10100111 	0xa7
#define b10101000 	0xa8
#define b10101001 	0xa9
#define b10101010 	0xaa
#define b10101011 	0xab
#define b10101100 	0xac
#define b10101101 	0xad
#define b10101110 	0xae
#define b10101111 	0xaf
#define b10110000 	0xb0
#define b10110001 	0xb1
#define b10110010 	0xb2
#define b10110011 	0xb3
#define b10110100 	0xb4
#define b10110101 	0xb5
#define b10110110 	0xb6
#define b10110111 	0xb7
#define b10111000 	0xb8
#define b10111001 	0xb9
#define b10111010 	0xba
#define b10111011 	0xbb
#define b10111100 	0xbc
#define b10111101 	0xbd
#define b10111110 	0xbe
#define b10111111 	0xbf
#define b11000000 	0xc0
#define b11000001 	0xc1
#define b11000010 	0xc2
#define b11000011 	0xc3
#define b11000100 	0xc4
#define b11000101 	0xc5
#define b11000110 	0xc6
#define b11000111 	0xc7
#define b11001000 	0xc8
#define b11001001 	0xc9
#define b11001010 	0xca
#define b11001011 	0xcb
#define b11001100 	0xcc
#define b11001101 	0xcd
#define b11001110 	0xce
#define b11001111 	0xcf
#define b11010000 	0xd0
#define b11010001 	0xd1
#define b11010010 	0xd2
#define b11010011 	0xd3
#define b11010100 	0xd4
#define b11010101 	0xd5
#define b11010110 	0xd6
#define b11010111 	0xd7
#define b11011000 	0xd8
#define b11011001 	0xd9
#define b11011010 	0xda
#define b11011011 	0xdb
#define b11011100 	0xdc
#define b11011101 	0xdd
#define b11011110 	0xde
#define b11011111 	0xdf
#define b11100000 	0xe0
#define b11100001 	0xe1
#define b11100010 	0xe2
#define b11100011 	0xe3
#define b11100100 	0xe4
#define b11100101 	0xe5
#define b11100110 	0xe6
#define b11100111 	0xe7
#define b11101000 	0xe8
#define b11101001 	0xe9
#define b11101010 	0xea
#define b11101011 	0xeb
#define b11101100 	0xec
#define b11101101 	0xed
#define b11101110 	0xee
#define b11101111 	0xef
#define b11110000 	0xf0
#define b11110001 	0xf1
#define b11110010 	0xf2
#define b11110011 	0xf3
#define b11110100 	0xf4
#define b11110101 	0xf5
#define b11110110 	0xf6
#define b11110111 	0xf7
#define b11111000 	0xf8
#define b11111001 	0xf9
#define b11111010 	0xfa
#define b11111011 	0xfb
#define b11111100 	0xfc
#define b11111101 	0xfd
#define b11111110 	0xfe
#define b11111111 	0xff

#endif // BYTES_MANIP_H