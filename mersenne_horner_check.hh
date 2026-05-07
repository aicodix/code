/*
Simple Mersenne Horner's scheme based check

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "prime_field.hh"

namespace CODE {

class MersenneHornerCheck
{
	typedef PrimeField<uint32_t, 0x7FFFFFFF> M31;
	static const uint32_t X = 362436069;
	static const uint32_t A = 48271;
	M31 x_, a_;
public:
	MersenneHornerCheck(uint32_t x = X, uint32_t a = A) : x_(x), a_(a) {}
	void reset(uint32_t x = X, uint32_t a = A)
	{
		x_ = M31(x);
		a_ = M31(a);
	}
	M31 operator()(M31 in)
	{
		x_ *= a_;
		return x_ += in;
	}
	M31 operator()()
	{
		return x_;
	}
};

}

