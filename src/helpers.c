/*
 * src/helpers.c
 * (c) 2022 Jonas Gunz <himself@jonasgunz.de>
 * License: MIT
 */

#include "helpers.h"

uint8_t hex_to_byte(char c[]) {
	uint8_t ret = hex_to_halfbyte(c[1]);
	ret += 16 * hex_to_halfbyte(c[0]);

	return ret;
}

uint8_t hex_to_halfbyte(char c) {
	uint8_t ret = 0;

	if ( c >= 48 && c <= 57)
		ret = c-48;
	else if (c >= 65 && c <= 70)
		ret = c-55;
	else if (c >= 97 && c <= 102)
		ret = c-87;

	return ret;
}
