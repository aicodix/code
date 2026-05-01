/*
Mersenne 2^31-1 Prime Field Packing

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "mersenne_31.hh"

namespace CODE {

struct MersennePacking
{
	static void pack(Mersenne31 *dst, const uint8_t *src, int count, int bytes)
	{
		uint64_t acc = 0;
		for (int i = 0, j = 0, k = 0; i < count; i++) {
			while (k < 31 && j < bytes) {
				acc |= uint64_t(src[j++]) << k;
				k += 8;
			}
			dst[i] = Mersenne31(acc & 0x7FFFFFFF);
			acc >>= 31;
			k -= 31;
		}
	}
	static void unpack(uint8_t *dst, const Mersenne31 *src, int count, int bytes)
	{
		uint64_t acc = 0;
		for (int i = 0, j = 0, k = 0; i < count; i++) {
			acc |= uint64_t(src[i].v) << k;
			k += 31;
			while ((k >= 8 || i == count - 1) && j < bytes) {
				dst[j++] = acc & 255;
				acc >>= 8;
				k -= 8;
			}
		}
	}
	static void decode(uint8_t *dst, const Mersenne31 *src, int bytes)
	{
		int count = (bytes * 8 + 30) / 31;
		Mersenne31 sub = *src++;
		uint64_t acc = 0;
		for (int i = 0, j = 0, k = 0; i < count; i++) {
			uint64_t val = sub == src[i] ? 0x7FFFFFFF : src[i]();
			acc |= val << k;
			k += 31;
			while ((k >= 8 || i == count - 1) && j < bytes) {
				dst[j++] = acc & 255;
				acc >>= 8;
				k -= 8;
			}
		}
	}
};

template <int MAX_BYTES>
struct MersenneRemapping
{
	typedef unsigned used_word;
	static constexpr int max_count = (MAX_BYTES * 8 + 30) / 31;
	static constexpr int used_width = 8 * sizeof(used_word);
	static constexpr int used_length = (max_count + used_width - 1) / used_width;
	used_word used_values[used_length];
	Mersenne31 find_unused(const Mersenne31 *dst, int count)
	{
		int limit = (count + used_width - 1) / used_width;
		for (int i = 0; i < limit; ++i)
			used_values[i] = 0;
		for (int i = 0; i < count; ++i)
			if (int(dst[i].v)/used_width < limit)
				used_values[dst[i].v/used_width] |= 1 << dst[i].v%used_width;
		int s = 0;
		while (s/used_width < limit && used_values[s/used_width] & 1 << s%used_width)
			++s;
		return Mersenne31(s);
	}
	void encode(Mersenne31 *dst, const uint8_t *src, int bytes)
	{
		assert(bytes <= MAX_BYTES);
		int count = (bytes * 8 + 30) / 31;
		MersennePacking::pack(dst+1, src, count, bytes);
		Mersenne31 sub = find_unused(dst+1, count);
		*dst++ = sub;
		for (int i = 0; i < count; ++i)
			if (dst[i].v == 0x7FFFFFFF)
				dst[i] = sub;
	}
};

}

