/*
Simple Mersenne check

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "mersenne_31.hh"

namespace CODE {

class MersenneCheck
{
	static const uint32_t X = 362436069;
	static const uint32_t A = 48271;
	Mersenne31 x_, a_;
public:
	MersenneCheck(uint32_t x = X, uint32_t a = A) : x_(x), a_(a) {}
	void reset(uint32_t x = X, uint32_t a = A)
	{
		x_ = Mersenne31(x);
		a_ = Mersenne31(a);
	}
	Mersenne31 operator()(Mersenne31 in)
	{
		x_ *= a_;
		return x_ += in;
	}
	Mersenne31 operator()()
	{
		return x_;
	}
};

}

