/*
Lehmer random number generator

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "mersenne_31.hh"

namespace CODE {

class LehmerRNG
{
	static const uint32_t X = 362436069;
	static const uint32_t A = 48271;
	Mersenne31 x_, a_;
public:
	typedef uint32_t result_type;
	static constexpr result_type min()
	{
		return 1;
	}
	static constexpr result_type max()
	{
		return Mersenne31::P-1;
	}
	LehmerRNG(uint32_t x = X, uint32_t a = A) : x_(x), a_(a) {}
	void reset(uint32_t x = X, uint32_t a = A)
	{
		x_ = Mersenne31(x);
		a_ = Mersenne31(a);
	}
	uint32_t operator()()
	{
		return (x_ *= a_)();
	}
	Mersenne31 update(Mersenne31 in)
	{
		x_ *= a_;
		return x_ += in;
	}
};

}

