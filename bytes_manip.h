#ifndef BYTES_MANIP_H
#define BYTES_MANIP_H

#include <avr/sfr_defs.h>

#define concat_bytes(high, low) (((high) << 8) | (low))
#define high_byte(dword) (((dword) >> 8) & 0xFF)
#define low_byte(dword) ((dword) & 0xFF)

#endif // BYTES_MANIP_H