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
	const int MAX_N = 128;
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand = std::bind(distribution(1, MAX_N), generator(rd()));
	int a[MAX_N], b[MAX_N], c[MAX_N], d[MAX_N], e[MAX_N], f[MAX_N], g[MAX_N];
	CODE::MergeSort<int, MAX_N> merge_sort;
	for (int loop = 0; loop < 1000000; ++loop) {
		int size = rand();
		for (int i = 0; i < size; ++i)
			a[i] = b[i] = c[i] = d[i] = e[i] = rand();
		std::sort(a, a+size);
		CODE::insertion_sort(b, size);
		for (int i = 0; i < size; ++i)
			assert(a[i] == b[i]);
		merge_sort(c, size);
		for (int i = 0; i < size; ++i)
			assert(a[i] == c[i]);
		for (int i = 0; i < size; ++i)
			f[i] = i;
		std::stable_sort(f, f+size, [d](int i, int j){ return d[i] < d[j]; });
		CODE::insertion_sort(g, e, size);
		for (int i = 0; i < size; ++i)
			assert(f[i] == g[i]);
	}
	std::cerr << "Sorting regression test passed!" << std::endl;
	return 0;
}

