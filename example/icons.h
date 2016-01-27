#ifndef ICONS_H
#define ICONS_H

#include <avr/pgmspace.h>

static const char min_icon[3 * 16] PROGMEM = {
	0xFE, 0x01, 0x01, 0xA1, 0x31, 0x41, 0x89, 0xF5, 0xF5, 0x89, 0x41, 0x31, 0xA1, 0x01, 0x01, 0xFE,
	0xFF, 0x00, 0x03, 0x14, 0x33, 0x0B, 0x47, 0xBF, 0xBF, 0x47, 0x0B, 0x33, 0x14, 0x03, 0x00, 0xFF,
	0x01, 0x72, 0x0A, 0x12, 0x0A, 0x72, 0x02, 0x7A, 0x02, 0x7A, 0x0A, 0x12, 0x22, 0x7A, 0x02, 0x01
};

static const char min_selected_icon[3 * 16] PROGMEM = {
	0xFE, 0xFF, 0xFF, 0x5F, 0xCF, 0xBF, 0x77, 0x0B, 0x0B, 0x77, 0xBF, 0xCF, 0x5F, 0xFF, 0xFF, 0xFE,
	0xFF, 0xFF, 0xFC, 0xEB, 0xCC, 0xF4, 0xB8, 0x60, 0x60, 0xB8, 0xF4, 0xCC, 0xEB, 0xFC, 0xFF, 0xFF,
	0x01, 0x73, 0x0B, 0x13, 0x0B, 0x73, 0x03, 0x7B, 0x03, 0x7B, 0x0B, 0x13, 0x23, 0x7B, 0x03, 0x01
};

static const char max_icon[3 * 16] PROGMEM = {
	0xFE, 0x01, 0x01, 0x81, 0x01, 0x21, 0x81, 0xC1, 0xD1, 0x81, 0x01, 0x21, 0x81, 0x01, 0x01, 0xFE,
	0xFF, 0x00, 0x00, 0x04, 0x00, 0x13, 0x07, 0x0F, 0x2F, 0x07, 0x03, 0x10, 0x04, 0x00, 0x00, 0xFF,
	0x01, 0x72, 0x0A, 0x12, 0x0A, 0x72, 0x02, 0x72, 0x2A, 0x72, 0x02, 0x4A, 0x32, 0x4A, 0x02, 0x01
};

static const char max_selected_icon[3 * 16] PROGMEM = {
	0xFE, 0xFF, 0xFF, 0x7F, 0xFF, 0xDF, 0x7F, 0x3F, 0x2F, 0x7F, 0xFF, 0xDF, 0x7F, 0xFF, 0xFF, 0xFE,
	0xFF, 0xFF, 0xFF, 0xFB, 0xFF, 0xEC, 0xF8, 0xF0, 0xD0, 0xF8, 0xFC, 0xEF, 0xFB, 0xFF, 0xFF, 0xFF,
	0x01, 0x73, 0x0B, 0x13, 0x0B, 0x73, 0x03, 0x73, 0x2B, 0x73, 0x03, 0x4B, 0x33, 0x4B, 0x03, 0x01
};

static const char delta_icon[3 * 16] PROGMEM = {
	0xFE, 0x01, 0x01, 0x01, 0x81, 0x41, 0x41, 0x41, 0x41, 0x41, 0x21, 0x01, 0x01, 0x01, 0x01, 0xFE,
	0xFF, 0x00, 0x00, 0x00, 0x30, 0x4D, 0x86, 0x84, 0x84, 0x84, 0x48, 0x30, 0x00, 0x00, 0x00, 0xFF,
	0x01, 0x7A, 0x4A, 0x32, 0x02, 0x7A, 0x42, 0x02, 0x0A, 0x7A, 0x0A, 0x02, 0x72, 0x2A, 0x72, 0x01
};

static const char delta_selected_icon[3 * 16] PROGMEM = {
	0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
	0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x92, 0x79, 0x7B, 0x7B, 0x7B, 0xB7, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x01, 0x7B, 0x4B, 0x33, 0x03, 0x7B, 0x43, 0x03, 0x0B, 0x7B, 0x0B, 0x03, 0x73, 0x2B, 0x73, 0x01
};

static const char contrast_icon[3 * 16] PROGMEM = {
	0xFE, 0x01, 0x01, 0x01, 0xC1, 0x21, 0x21, 0x11, 0xF1, 0xE1, 0xE1, 0xC1, 0x01, 0x01, 0x01, 0xFE,
	0xFF, 0x00, 0x00, 0x03, 0x0C, 0x10, 0x10, 0x20, 0x3F, 0x1F, 0x1F, 0x0F, 0x03, 0x00, 0x00, 0xFF,
	0x01, 0x7A, 0x4A, 0x32, 0x02, 0x52, 0x6A, 0x6A, 0x02, 0x7A, 0x2A, 0x12, 0x02, 0x7A, 0x42, 0x01
};


static const char conttrast_selected_icon[3 * 16] PROGMEM = {
	0xFE, 0xFF, 0xFF, 0x7F, 0xBF, 0xBF, 0xDF, 0x1F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
	0xFF, 0xFF, 0xF9, 0xE6, 0xDF, 0xDF, 0xBF, 0x80, 0xC0, 0xC0, 0xE0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF,
	0x01, 0x7B, 0x4B, 0x33, 0x03, 0x53, 0x6B, 0x6B, 0x03, 0x7B, 0x2B, 0x13, 0x03, 0x7B, 0x43, 0x01
};

#endif // ICONS_H