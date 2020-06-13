/*
Test for the Simplex codes encoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "simplex_encoder.hh"

template <int K>
void simplex_test()
{
	CODE::SimplexEncoder<K> encode;
	const int W = 1 << K;
	const int N = (1 << K) - 1;
	const int D = 1 << (K - 1);
	int hist[N+1] = { 0 };
	for (int j = 0; j < W; ++j) {
		int8_t cj[N];
		encode(cj, j);
		for (int i = j + 1; i < W; ++i) {
			int8_t ci[N];
			encode(ci, i);
			int d = 0;
			for (int n = 0; n < N; ++n)
				d += ci[n] != cj[n];
			++hist[d];
		}
	}
	std::cout << "Simplex(" << N << ", " << K << ", " << D << ") hist:";
	for (int i = 0; i < N + 1; ++i)
		if (hist[i])
			std::cout << " " << i << ":" << hist[i];
	std::cout << std::endl;
	int d = 0;
	while (!hist[d])
		++d;
	assert(d == D);
}

int main()
{
	if (1) {
		simplex_test<2>();
	}
	if (1) {
		simplex_test<3>();
	}
	if (1) {
		simplex_test<4>();
	}
	if (1) {
		simplex_test<5>();
	}
	if (1) {
		simplex_test<6>();
	}
	if (1) {
		simplex_test<7>();
	}
	if (1) {
		simplex_test<8>();
	}
	std::cerr << "Simplex encoder test passed!" << std::endl;
	return 0;
}

