/*
Test for the prime field arithmetic

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <limits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "prime_field.hh"

template <typename TYPE, TYPE PRIME>
void exhaustive_test()
{
	assert(std::is_unsigned<TYPE>::value);
	assert(std::numeric_limits<TYPE>::max() / (PRIME-1) >= (PRIME-1));
	typedef CODE::PrimeField<TYPE, PRIME> PF;
	for (TYPE a = 0; a < PRIME; ++a)
		for (TYPE b = 0; b < PRIME; ++b)
			assert((PF(a) * PF(b))() == (a * b) % PRIME);
	for (TYPE a = 1; a < PRIME; ++a)
		assert(rcp(PF(a)) * PF(a) == PF(1));
	for (TYPE a = 0; a < PRIME; ++a)
		for (TYPE b = 0; b < PRIME; ++b)
			assert((PF(a) + PF(b))() == (a + b) % PRIME);
	for (TYPE a = 0; a < PRIME; ++a)
		for (TYPE b = 0; b < PRIME; ++b)
			assert((PF(a) - PF(b))() == (a - b + PRIME) % PRIME);
}

int main()
{
	exhaustive_test<uint32_t, 257>();
	exhaustive_test<uint64_t, 65537>();
	std::cerr << "Prime field arithmetic test passed!" << std::endl;
	return 0;
}

