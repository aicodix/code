/*
Test for the prime field arithmetic

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <chrono>
#include <limits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <functional>
#include <type_traits>
#include "prime_field.hh"

template <typename TYPE, TYPE PRIME>
void exhaustive_test()
{
	assert(std::is_unsigned<TYPE>::value);
	assert(std::numeric_limits<TYPE>::max() >= PRIME);
	typedef CODE::PrimeField<TYPE, PRIME> PF;
	for (TYPE a = 0; a < PRIME; ++a)
		for (TYPE b = 0; b < PRIME; ++b)
			assert((PF(a) * PF(b))() == TYPE((uint64_t(a) * b) % PRIME));
	for (TYPE a = 1; a < PRIME; ++a)
		assert(rcp(PF(a)) * PF(a) == PF(1));
	for (TYPE a = 0; a < PRIME; ++a)
		for (TYPE b = 0; b < PRIME; ++b)
			assert((PF(a) + PF(b))() == (a + b) % PRIME);
	for (TYPE a = 0; a < PRIME; ++a)
		for (TYPE b = 0; b < PRIME; ++b)
			assert((PF(a) - PF(b))() == (a + (PRIME - b)) % PRIME);
}

template <typename TYPE, TYPE PRIME>
void random_test(int count)
{
	assert(std::is_unsigned<TYPE>::value);
	assert(std::numeric_limits<TYPE>::max() >= PRIME);
	typedef CODE::PrimeField<TYPE, PRIME> PF;
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand0 = std::bind(distribution(0, PRIME-1), generator(rd()));
	auto rand1 = std::bind(distribution(1, PRIME-1), generator(rd()));
	for (int i = 0; i < count; ++i) {
		TYPE a = rand0();
		for (int j = 0; j < count; ++j) {
			TYPE b = rand0();
			assert((PF(a) * PF(b))() == TYPE((uint64_t(a) * b) % PRIME));
		}
	}
	for (int i = 0; i < count * count; ++i) {
		TYPE a = rand1();
		assert(rcp(PF(a)) * PF(a) == PF(1));
	}
	for (int i = 0; i < count; ++i) {
		TYPE a = rand0();
		for (int j = 0; j < count; ++j) {
			TYPE b = rand0();
			assert((PF(a) + PF(b))() == (a + b) % PRIME);
		}
	}
	for (int i = 0; i < count; ++i) {
		TYPE a = rand0();
		for (int j = 0; j < count; ++j) {
			TYPE b = rand0();
			assert((PF(a) - PF(b))() == (a + (PRIME - b)) % PRIME);
		}
	}
}

int main()
{
	exhaustive_test<uint16_t, 257>();
	exhaustive_test<uint32_t, 257>();
	exhaustive_test<uint32_t, 65537>();
	exhaustive_test<uint64_t, 65537>();
	random_test<uint32_t, 0x7FFFFFFF>(100);
	std::cerr << "Prime field arithmetic test passed!" << std::endl;
	return 0;
}

