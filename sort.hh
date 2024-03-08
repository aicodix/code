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

template <typename TYPE, int MAX_N, int M = 32>
class MergeSort
{
	TYPE tmp[MAX_N];
	void merge(TYPE *a, int n, int left, int right, int end)
	{
		if (right > n)
			right = n;
		if (end > n)
			end = n;
		for (int i = left, j = right, k = left; k < end; ++k)
			tmp[k] = (i < right && (j >= end || a[i] <= a[j])) ? a[i++] : a[j++];
	}
public:
	void operator()(TYPE *a, int n)
	{
		for (int i = 0; i < n; i += M)
			insertion_sort(a+i, i > n-M ? n-i : M);
		for (int l = M; l < n; l *= 2) {
			for (int i = 0; i < n; i += 2*l)
				merge(a, n, i, i+l, i+2*l);
			for (int i = 0; i < n; ++i)
				a[i] = tmp[i];
		}
	}
};

}

