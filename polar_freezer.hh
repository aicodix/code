/*
Bit freezers for polar codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <limits>
#include <algorithm>

namespace CODE {

// erasure probabilities above one half are stored as their negated
// complement, as values close to one would otherwise round to one.
// squares are kept above zero, so the sign survives underflow and
// doesn't depend on signed zeros, which -ffast-math ignores
class PolarFreezer
{
	static double complement(double pe)
	{
		bool comp = pe > 0.5;
		if (comp)
			pe = 1 - pe;
		if (pe < std::numeric_limits<double>::min())
			pe = std::numeric_limits<double>::min();
		return comp ? -pe : pe;
	}
	static bool get_bit(const uint32_t *bits, int idx)
	{
		return (bits[idx/32] >> (idx%32)) & 1;
	}
	static void set_bit(uint32_t *bits, int idx, bool val)
	{
		bits[idx/32] &= ~(1 << (idx%32));
		bits[idx/32] |= (uint32_t)val << (idx%32);
	}
	static void freeze(uint32_t *bits, double pe, double th, int i, int h)
	{
		if (h) {
			bool comp = pe < 0;
			double x = comp ? -pe : pe;
			double square = x * x;
			if (square < std::numeric_limits<double>::min())
				square = std::numeric_limits<double>::min();
			double grow = x * (2-x);
			bool flip = grow > 0.5;
			if (flip)
				grow = (1-x) * (1-x);
			if (comp != flip)
				grow = -grow;
			if (comp)
				square = -square;
			freeze(bits, comp ? square : grow, th, i, h/2);
			freeze(bits, comp ? grow : square, th, i+h, h/2);
		} else {
			// z > th, with z = 1 + pe for the negated complement
			set_bit(bits, i, pe < 0 ? -pe < 1 - th : pe > th);
		}
	}
public:
	int operator()(uint32_t *frozen_bits, int level, double erasure_probability = 0.5, double freezing_threshold = 0.5)
	{
		int length = 1 << level;
		freeze(frozen_bits, complement(erasure_probability), freezing_threshold, 0, length / 2);
		int K = length;
		for (int i = 0; i < length; ++i)
			K -= (frozen_bits[i/32] >> (i%32)) & 1;
		return K;
	}
};

// erasure probabilities above one half are stored as their negated
// complement, as values close to one would otherwise round to one.
// squares are kept above zero, so the sign survives underflow and
// doesn't depend on signed zeros, which -ffast-math ignores
template <int MAX_M>
class BhattacharyyaFreezer
{
	static double complement(double pe)
	{
		bool comp = pe > 0.5;
		if (comp)
			pe = 1 - pe;
		if (pe < std::numeric_limits<double>::min())
			pe = std::numeric_limits<double>::min();
		return comp ? -pe : pe;
	}
	static void inform_bit(uint32_t *bits, int idx)
	{
		bits[idx/32] &= ~(1 << (idx%32));
	}
	static void frozen_bit(uint32_t *bits, int idx)
	{
		bits[idx/32] |= 1 << (idx%32);
	}
	void compute(double pe, int i, int h)
	{
		if (h) {
			bool comp = pe < 0;
			double x = comp ? -pe : pe;
			double square = x * x;
			if (square < std::numeric_limits<double>::min())
				square = std::numeric_limits<double>::min();
			double grow = x * (2-x);
			bool flip = grow > 0.5;
			if (flip)
				grow = (1-x) * (1-x);
			if (comp != flip)
				grow = -grow;
			if (comp)
				square = -square;
			compute(comp ? square : grow, i, h/2);
			compute(comp ? grow : square, i+h, h/2);
		} else {
			prob[i] = pe;
		}
	}
	double prob[1<<MAX_M];
	int index[1<<MAX_M];
public:
	void operator()(uint32_t *frozen_bits, int level, int K, double erasure_probability = std::exp(-1.))
	{
		assert(level <= MAX_M);
		int length = 1 << level;
		compute(complement(erasure_probability), 0, length / 2);
		for (int i = 0; i < length; ++i)
			index[i] = i;
		// least reliable first, ties in the same order as ReedMullerSequence
		std::nth_element(index, index+length-K, index+length, [this](int a, int b){
			bool na = prob[a] < 0, nb = prob[b] < 0;
			if (na != nb)
				return na;
			if (prob[a] != prob[b])
				return prob[a] > prob[b];
			int x = __builtin_popcount(a);
			int y = __builtin_popcount(b);
			if (x != y)
				return x < y;
			return a < b;
		});
		for (int i = 0; i < length-K; ++i)
			frozen_bit(frozen_bits, index[i]);
		for (int i = length-K; i < length; ++i)
			inform_bit(frozen_bits, index[i]);
	}
};

}

