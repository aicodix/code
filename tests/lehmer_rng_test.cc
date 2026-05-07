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

