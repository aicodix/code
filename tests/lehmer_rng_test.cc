/*
Test for the Lehmer RNG

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdint>
#include <cassert>
#include <iostream>
#include "lehmer_rng.hh"

void test(uint32_t X, uint32_t A)
{
	typedef CODE::PrimeField<uint32_t, 0x7FFFFFFF> M31;
	for (uint32_t f : {2, 3, 7, 11, 31, 151, 331})
		assert(pow(M31(A), (M31::P-1) / f) != M31(1));
	CODE::LehmerRNG rng(X, A);
	int length = 0x7FFFFFFE;
	for (int i = 1; i < length; ++i)
		assert(rng() != X);
	assert(rng() == X);
}

int main()
{
	test(362436069, 48271);
	std::cerr << "Lehmer RNG test passed!" << std::endl;
	return 0;
}

