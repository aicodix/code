/*
Mersenne 2^31-1 Prime Field Packing

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "mersenne_31.hh"

namespace CODE {

struct MersennePacking
{
	typedef Mersenne31 M31;
	void pack(uint32_t *dst, const uint8_t *src, int count, int bytes)
	{
		uint64_t acc = 0;
		for (int i = 0, j = 0, k = 0; i < count; i++) {
			while (k < 31 && j < bytes) {
				acc |= uint64_t(src[j++]) << k;
				k += 8;
			}
			dst[i] = acc & 0x7FFFFFFF;
			acc >>= 31;
			k -= 31;
		}
	}
	void unpack(uint8_t *dst, const uint32_t *src, int count, int bytes)
	{
		uint64_t acc = 0;
		for (int i = 0, j = 0, k = 0; i < count; i++) {
			acc |= uint64_t(src[i]) << k;
			k += 31;
			while ((k >= 8 || i == count - 1) && j < bytes) {
				dst[j++] = acc & 255;
				acc >>= 8;
				k -= 8;
			}
		}
	}
};

}

