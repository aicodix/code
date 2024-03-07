/*
Sorting for SIMD types

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

template <typename TYPE, int WIDTH>
static inline SIMD<typename SIMD<TYPE, WIDTH>::uint_type, WIDTH> vorder(SIMD<TYPE, WIDTH> a)
{
	SIMD<typename SIMD<TYPE, WIDTH>::uint_type, WIDTH> p;
	p.v[0] = 0;
	for (int i = 1, j; i < WIDTH; ++i) {
		TYPE t = a.v[i];
		for (j = i; j > 0 && a.v[j-1] > t; --j) {
			a.v[j] = a.v[j-1];
			p.v[j] = p.v[j-1];
		}
		a.v[j] = t;
		p.v[j] = i;
	}
	return p;
}

template <typename TYPE, int WIDTH>
static inline SIMD<TYPE, WIDTH> vsort(SIMD<TYPE, WIDTH> a)
{
	for (int i = 1, j; i < WIDTH; ++i) {
		TYPE t = a.v[i];
		for (j = i; j > 0 && a.v[j-1] > t; --j)
			a.v[j] = a.v[j-1];
		a.v[j] = t;
	}
	return a;
}

