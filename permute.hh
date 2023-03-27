/*
Reversible permutations

Copyright 2023 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "xorshift.hh"

namespace CODE {

template <int SIZE>
class ReversibleFisherYatesShuffle
{
	int seq[SIZE-1];
public:
	ReversibleFisherYatesShuffle()
	{
		CODE::Xorshift32 prng;
		for (int i = 0; i < SIZE-1; ++i)
			seq[i] = i + prng() % (SIZE - i);
	}
	template <typename TYPE>
	void forward(TYPE *array)
	{
		for (int i = 0; i < SIZE-1; ++i)
			std::swap(array[i], array[seq[i]]);
	}
	template <typename TYPE>
	void reverse(TYPE *array)
	{
		for (int i = SIZE-2; i >= 0; --i)
			std::swap(array[i], array[seq[i]]);
	}
};

template <int SIZE, typename TYPE>
static void BitReversalPermute(TYPE *array)
{
	static_assert(SIZE > 0 && (SIZE & (SIZE - 1)) == 0, "SIZE not power of two");
	for (int i = 0, j = 0; i < SIZE - 1; ++i) {
		if (i < j)
			std::swap(array[i], array[j]);
		int k = SIZE >> 1;
		while (j & k) {
			j ^= k;
			k >>= 1;
		}
		j ^= k;
	}
}

}

