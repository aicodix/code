/*
Lehmer random number generator

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "prime_field.hh"

namespace CODE {

class LehmerRNG
{
	typedef PrimeField<uint32_t, 0x7FFFFFFF> M31;
	static const uint32_t X = 362436069;
	static const uint32_t A = 48271;
	M31 x_, a_;
public:
	typedef uint32_t result_type;
	static constexpr result_type min()
	{
		return 1;
	}
	static constexpr result_type max()
	{
		return M31::P-1;
	}
	LehmerRNG(uint32_t x = X, uint32_t a = A) : x_(x), a_(a) {}
	void reset(uint32_t x = X, uint32_t a = A)
	{
		x_ = M31(x);
		a_ = M31(a);
	}
	uint32_t operator()()
	{
		return (x_ *= a_)();
	}
};

}

