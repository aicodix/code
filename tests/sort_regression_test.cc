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
	unsigned seed = 42;
	if (1) {
		std::random_device rd;
		seed = rd();
	}
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand = std::bind(distribution(1, MAX_N), generator(seed));
	typedef std::pair<int, int> Pair;
	Pair a[MAX_N], b[MAX_N], c[MAX_N];
	int d[MAX_N], e[MAX_N];
	auto comp = [](Pair a, Pair b){ return a.second < b.second; };
	CODE::MergeSort<Pair, MAX_N> merge_sort;
	for (int loop = 0; loop < 1000000; ++loop) {
		int size = rand();
		for (int i = 0; i < size; ++i)
			a[i].first = b[i].first = c[i].first = e[i] = i;
		for (int i = 0; i < size; ++i)
			a[i].second = b[i].second = c[i].second = d[i] = rand();
		std::stable_sort(a, a+size, comp);
		CODE::insertion_sort(b, size, comp);
		for (int i = 0; i < size; ++i)
			assert(a[i] == b[i]);
		merge_sort(c, size, comp);
		for (int i = 0; i < size; ++i)
			assert(a[i] == c[i]);
		CODE::insertion_sort(e, d, size);
		for (int i = 0; i < size; ++i)
			assert(a[i].first == e[i]);
	}
	std::cerr << "Sorting regression test passed!" << std::endl;
	return 0;
}

