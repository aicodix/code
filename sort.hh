/*
Some stable sorting algorithms

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <typename TYPE>
static void insertion_sort(TYPE *a, int n)
{
	for (int i = 1, j; i < n; ++i) {
		TYPE t = a[i];
		for (j = i; j > 0 && a[j-1] > t; --j)
			a[j] = a[j-1];
		a[j] = t;
	}
}

template <typename INDEX, typename TYPE>
static void insertion_sort(INDEX *p, TYPE *a, int n)
{
	p[0] = 0;
	for (int i = 1, j; i < n; ++i) {
		TYPE t = a[i];
		for (j = i; j > 0 && a[j-1] > t; --j) {
			a[j] = a[j-1];
			p[j] = p[j-1];
		}
		a[j] = t;
		p[j] = i;
	}
}

}

