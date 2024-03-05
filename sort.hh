/*
Sorting for SIMD types

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <algorithm>

template <int WIDTH>
static inline SIMD<uint32_t, WIDTH> vorder(SIMD<float, WIDTH> a)
{
	SIMD<uint32_t, WIDTH> tmp;
	for (int i = 0; i < WIDTH; ++i)
		tmp.v[i] = i;
	std::stable_sort(tmp.v, tmp.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
	return tmp;
}

template <int WIDTH>
static inline SIMD<uint64_t, WIDTH> vorder(SIMD<double, WIDTH> a)
{
	SIMD<uint64_t, WIDTH> tmp;
	for (int i = 0; i < WIDTH; ++i)
		tmp.v[i] = i;
	std::stable_sort(tmp.v, tmp.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
	return tmp;
}

template <int WIDTH>
static inline SIMD<uint8_t, WIDTH> vorder(SIMD<int8_t, WIDTH> a)
{
	SIMD<uint8_t, WIDTH> tmp;
	for (int i = 0; i < WIDTH; ++i)
		tmp.v[i] = i;
	std::stable_sort(tmp.v, tmp.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
	return tmp;
}

template <int WIDTH>
static inline SIMD<uint16_t, WIDTH> vorder(SIMD<int16_t, WIDTH> a)
{
	SIMD<uint16_t, WIDTH> tmp;
	for (int i = 0; i < WIDTH; ++i)
		tmp.v[i] = i;
	std::stable_sort(tmp.v, tmp.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
	return tmp;
}

template <int WIDTH>
static inline SIMD<uint32_t, WIDTH> vorder(SIMD<int32_t, WIDTH> a)
{
	SIMD<uint32_t, WIDTH> tmp;
	for (int i = 0; i < WIDTH; ++i)
		tmp.v[i] = i;
	std::stable_sort(tmp.v, tmp.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
	return tmp;
}

template <int WIDTH>
static inline SIMD<uint64_t, WIDTH> vorder(SIMD<int64_t, WIDTH> a)
{
	SIMD<uint64_t, WIDTH> tmp;
	for (int i = 0; i < WIDTH; ++i)
		tmp.v[i] = i;
	std::stable_sort(tmp.v, tmp.v+WIDTH, [a](int i, int j){ return a.v[i] < a.v[j]; });
	return tmp;
}

template <int WIDTH>
static inline SIMD<float, WIDTH> vsort(SIMD<float, WIDTH> a)
{
	std::sort(a.v, a.v+WIDTH);
	return a;
}

template <int WIDTH>
static inline SIMD<double, WIDTH> vsort(SIMD<double, WIDTH> a)
{
	std::sort(a.v, a.v+WIDTH);
	return a;
}

template <int WIDTH>
static inline SIMD<int8_t, WIDTH> vsort(SIMD<int8_t, WIDTH> a)
{
	std::sort(a.v, a.v+WIDTH);
	return a;
}

template <int WIDTH>
static inline SIMD<int16_t, WIDTH> vsort(SIMD<int16_t, WIDTH> a)
{
	std::sort(a.v, a.v+WIDTH);
	return a;
}

template <int WIDTH>
static inline SIMD<int32_t, WIDTH> vsort(SIMD<int32_t, WIDTH> a)
{
	std::sort(a.v, a.v+WIDTH);
	return a;
}

template <int WIDTH>
static inline SIMD<int64_t, WIDTH> vsort(SIMD<int64_t, WIDTH> a)
{
	std::sort(a.v, a.v+WIDTH);
	return a;
}

