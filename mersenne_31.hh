/*
Mersenne 2^31-1 prime field arithmetic

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

struct Mersenne31;

Mersenne31 operator + (Mersenne31, Mersenne31);
Mersenne31 operator - (Mersenne31, Mersenne31);
Mersenne31 operator * (Mersenne31, Mersenne31);
Mersenne31 operator / (Mersenne31, Mersenne31);

struct Mersenne31
{
	static constexpr uint32_t P = 0x7FFFFFFF;
	uint32_t v;
	Mersenne31() = default;
	explicit Mersenne31(uint32_t v) : v(v)
	{
	}
	Mersenne31 operator *= (Mersenne31 a)
	{
		return *this = *this * a;
	}
	Mersenne31 operator /= (Mersenne31 a)
	{
		return *this = *this / a;
	}
	Mersenne31 operator += (Mersenne31 a)
	{
		return *this = *this + a;
	}
	Mersenne31 operator -= (Mersenne31 a)
	{
		return *this = *this - a;
	}
	uint32_t operator () ()
	{
		return v == P ? 0 : v;
	}
};

Mersenne31 reduce(Mersenne31 a)
{
	return Mersenne31((a.v & 0x7FFFFFFF) + (a.v >> 31));
}

bool operator == (Mersenne31 a, Mersenne31 b)
{
	return a.v == b.v;
}

bool operator != (Mersenne31 a, Mersenne31 b)
{
	return a.v != b.v;
}

Mersenne31 add(Mersenne31 a, Mersenne31 b)
{
	return Mersenne31(a.v + b.v);
}

Mersenne31 operator + (Mersenne31 a, Mersenne31 b)
{
	return reduce(add(a, b));
}

Mersenne31 neg(Mersenne31 a)
{
	return Mersenne31(a.P - a.v);
}

Mersenne31 operator - (Mersenne31 a)
{
	return reduce(neg(a));
}

Mersenne31 sub(Mersenne31 a, Mersenne31 b)
{
	return add(a, -b);
}

Mersenne31 operator - (Mersenne31 a, Mersenne31 b)
{
	return reduce(sub(a, b));
}

Mersenne31 mul(Mersenne31 a, Mersenne31 b)
{
	uint64_t v = uint64_t(a.v) * uint64_t(b.v);
	uint32_t l = v & 0x7FFFFFFF;
	uint32_t h = v >> 31;
	return Mersenne31(l + h);
}

Mersenne31 operator * (Mersenne31 a, Mersenne31 b)
{
	return reduce(mul(a, b));
}

Mersenne31 pow(Mersenne31 a, uint32_t m)
{
	Mersenne31 t(1);
	for (;m; m >>= 1, a *= a)
		if (m & 1)
			t *= a;
	return t;
}

Mersenne31 rcp(Mersenne31 a)
{
	return pow(a, a.P - 2);
}

Mersenne31 div(Mersenne31 a, Mersenne31 b)
{
	return mul(a, rcp(b));
}

Mersenne31 operator / (Mersenne31 a, Mersenne31 b)
{
	return reduce(div(a, b));
}

}
