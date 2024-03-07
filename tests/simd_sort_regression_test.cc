/*
Regression Test for the SIMD sorting wrappers

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include "simd.hh"
#include "simd_sort.hh"

int main()
{
	const int WIDTH = 32;
	typedef int32_t TYPE;
	typedef SIMD<TYPE, WIDTH>::uint_type UINT;
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand = std::bind(distribution(0, WIDTH), generator(rd()));
	for (int loop = 0; loop < 1000000; ++loop) {
		SIMD<TYPE, WIDTH> a, b, c;
		for (int i = 0; i < WIDTH; ++i)
			a.v[i] = rand();
		b = vsort(a);
		c = a;
		std::sort(c.v, c.v+WIDTH);
		for (int i = 0; i < WIDTH; ++i)
			assert(b.v[i] == c.v[i]);
		SIMD<UINT, WIDTH> d, e;
		d = vorder(a);
		for (int i = 0; i < WIDTH; ++i)
			e.v[i] = i;
		std::stable_sort(e.v, e.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
		for (int i = 0; i < WIDTH; ++i)
			assert(d.v[i] == e.v[i]);
	}
	std::cerr << "SIMD sort regression test passed!" << std::endl;
	return 0;
}

