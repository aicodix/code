/*
Construct reliability sequences for polar codes

Copyright 2023 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <limits>
#include "sort.hh"

namespace CODE {

// erasure probabilities above one half are stored as their negated
// complement, as values close to one would otherwise round to one.
// squares are kept above zero, so the sign survives underflow and
// doesn't depend on signed zeros, which -ffast-math ignores
template <int MAX_M>
class BhattacharyyaSequence
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
	MergeSort<int, 1<<MAX_M> sort;
public:
	void operator()(int *sequence, int level, double erasure_probability = std::exp(-1.))
	{
		assert(level <= MAX_M);
		int length = 1 << level;
		compute(complement(erasure_probability), 0, length / 2);
		for (int i = 0; i < length; ++i)
			sequence[i] = i;
		// least reliable first, ties in the same order as ReedMullerSequence
		sort(sequence, length, [this](int a, int b){
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
	}
};

template <int MAX_M>
class ReedMullerSequence
{
	MergeSort<int, 1<<MAX_M> sort;
public:
	void operator()(int *sequence, int level)
	{
		assert(level <= MAX_M);
		int length = 1 << level;
		for (int i = 0; i < length; ++i)
			sequence[i] = i;
		sort(sequence, length, [](int a, int b){
			int x = __builtin_popcount(a);
			int y = __builtin_popcount(b);
			if (x != y)
				return x < y;
			return a < b;
		});
	}
};

}

