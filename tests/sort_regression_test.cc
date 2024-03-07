/*
Regression Test for sorting

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include "sort.hh"

int main()
{
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand = std::bind(distribution(1, 32), generator(rd()));
	int a[32], b[32], c[32], d[32], e[32], f[32];
	for (int loop = 0; loop < 1000000; ++loop) {
		int size = rand();
		for (int i = 0; i < size; ++i)
			a[i] = b[i] = c[i] = d[i] = rand();
		std::sort(a, a+size);
		insertion_sort(b, size);
		for (int i = 0; i < size; ++i)
			assert(a[i] == b[i]);
		for (int i = 0; i < size; ++i)
			e[i] = i;
		std::stable_sort(e, e+size, [c](int i, int j){ return c[i] < c[j]; });
		insertion_sort(f, d, size);
		for (int i = 0; i < size; ++i)
			assert(e[i] == f[i]);
	}
	std::cerr << "Sorting regression test passed!" << std::endl;
	return 0;
}

