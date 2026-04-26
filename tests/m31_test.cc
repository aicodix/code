/*
Test for the Mersenne 2^31-1 prime field arithmetic

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <chrono>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <functional>
#include "mersenne_31.hh"

void random_test(uint32_t count)
{
	typedef CODE::Mersenne31 M31;
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand0 = std::bind(distribution(0, M31::P-1), generator(rd()));
	auto rand1 = std::bind(distribution(1, M31::P-1), generator(rd()));
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t a = rand0();
		for (uint32_t j = 0; j < count; ++j) {
			uint32_t b = rand0();
			assert((M31(a) * M31(b))() == uint32_t((uint64_t(a) * b) % M31::P));
		}
	}
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t a = rand1();
		assert(rcp(M31(a)) * M31(a) == M31(1));
	}
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t a = rand0();
		for (uint32_t j = 0; j < count; ++j) {
			uint32_t b = rand0();
			assert((M31(a) + M31(b))() == (a + b) % M31::P);
		}
	}
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t a = rand0();
		for (uint32_t j = 0; j < count; ++j) {
			uint32_t b = rand0();
			assert((M31(a) - M31(b))() == (a - b + M31::P) % M31::P);
		}
	}
}

int main()
{
	random_test(10000);
	std::cerr << "Mersenne 2^31-1 prime field arithmetic test passed!" << std::endl;
	return 0;
}

