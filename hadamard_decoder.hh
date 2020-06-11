/*
Soft decoder for augmented Hadamard codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <int K>
class HadamardDecoder
{
	static const int W = 1 << K;
	static const int N = 1 << (K - 1);
	int8_t mod[W*N];

	static bool parity(unsigned x)
	{
		x ^= x >> 16;
		x ^= x >> 8;
		x ^= x >> 4;
		x ^= x >> 2;
		x ^= x >> 1;
		return x & 1;
	}
public:
	HadamardDecoder()
	{
		for (int msg = 0; msg < W; ++msg)
			for (int i = 0; i < N; ++i)
				mod[msg*N+i] = 1 - 2 * parity(msg&(i|N));
	}
	int operator()(const int8_t *code)
	{
		int word = 0, best = 0, next = 0;
		for (int msg = 0; msg < W; ++msg) {
			int sum = 0;
			for (int i = 0; i < N; ++i)
				sum += mod[msg*N+i] * code[i];
			if (sum > best) {
				next = best;
				best = sum;
				word = msg;
			} else if (sum > next) {
				next = sum;
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

}

