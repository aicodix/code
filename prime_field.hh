/*
Prime field arithmetic

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <typename TYPE, TYPE PRIME>
struct PrimeField
{
	static constexpr TYPE P = PRIME;
	TYPE v;
	PrimeField() = default;
	explicit PrimeField(TYPE v) : v(v)
	{
	}
	PrimeField<TYPE, PRIME> operator *= (PrimeField<TYPE, PRIME> a)
	{
		return *this = *this * a;
	}
	PrimeField<TYPE, PRIME> operator /= (PrimeField<TYPE, PRIME> a)
	{
		return *this = *this / a;
	}
	PrimeField<TYPE, PRIME> operator += (PrimeField<TYPE, PRIME> a)
	{
		return *this = *this + a;
	}
	PrimeField<TYPE, PRIME> operator -= (PrimeField<TYPE, PRIME> a)
	{
		return *this = *this - a;
	}
	TYPE operator () () const
	{
		return value(*this);
	}
};

template <typename TYPE, TYPE PRIME>
TYPE value(PrimeField<TYPE, PRIME> a)
{
	return a.v;
}

template <>
uint32_t value(PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> a)
{
	return a.v == a.P ? 0 : a.v;
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> reduce(PrimeField<TYPE, PRIME> a)
{
	return PrimeField<TYPE, PRIME>(a.v % a.P);
}

template <>
PrimeField<uint16_t, uint16_t(257)> reduce(PrimeField<uint16_t, uint16_t(257)> a)
{
	int16_t l = a.v & 255;
	int16_t h = a.v >> 8;
	int16_t r = l - h;
	r += (r >> 15) & 257;
	return PrimeField<uint16_t, uint16_t(257)>(r);
}

template <>
PrimeField<uint32_t, uint32_t(65537)> reduce(PrimeField<uint32_t, uint32_t(65537)> a)
{
	int32_t l = a.v & 65535;
	int32_t h = a.v >> 16;
	int32_t r = l - h;
	r += (r >> 31) & 65537;
	return PrimeField<uint32_t, uint32_t(65537)>(r);
}

template <>
PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> reduce(PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> a)
{
	return PrimeField<uint32_t, uint32_t(0x7FFFFFFF)>((a.v & 0x7FFFFFFF) + (a.v >> 31));
}

template <typename TYPE, TYPE PRIME>
bool operator == (PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return a() == b();
}

template <typename TYPE, TYPE PRIME>
bool operator != (PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return a() != b();
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> add(PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return PrimeField<TYPE, PRIME>(a.v + b.v);
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> operator + (PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return reduce(add(a, b));
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> sub(PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return add(a, neg(b));
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> operator - (PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return reduce(sub(a, b));
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> neg(PrimeField<TYPE, PRIME> a)
{
	return PrimeField<TYPE, PRIME>(a.P - a.v);
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> operator - (PrimeField<TYPE, PRIME> a)
{
	return reduce(neg(a));
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> mul(PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return PrimeField<TYPE, PRIME>(a.v * b.v);
}

template <>
PrimeField<uint16_t, uint16_t(257)> mul(PrimeField<uint16_t, uint16_t(257)> a, PrimeField<uint16_t, uint16_t(257)> b)
{
	uint16_t o = (a.v & b.v) >> 8;
	uint16_t v = a.v * b.v;
	return PrimeField<uint16_t, uint16_t(257)>(o | v);
}

template <>
PrimeField<uint32_t, uint32_t(65537)> mul(PrimeField<uint32_t, uint32_t(65537)> a, PrimeField<uint32_t, uint32_t(65537)> b)
{
	uint32_t o = (a.v & b.v) >> 16;
	uint32_t v = a.v * b.v;
	return PrimeField<uint32_t, uint32_t(65537)>(o | v);
}

template <>
PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> mul(PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> a, PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> b)
{
	uint64_t v = uint64_t(a.v) * uint64_t(b.v);
	uint32_t l = v & 0x7FFFFFFF;
	uint32_t h = v >> 31;
	return PrimeField<uint32_t, uint32_t(0x7FFFFFFF)>(l + h);
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> operator * (PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return reduce(mul(a, b));
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> pow(PrimeField<TYPE, PRIME> a, TYPE m)
{
	PrimeField<TYPE, PRIME> t(1);
	for (;m; m >>= 1, a *= a)
		if (m & 1)
			t *= a;
	return t;
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> rcp(PrimeField<TYPE, PRIME> a)
{
	return pow(a, a.P - 2);
}

template <>
PrimeField<uint16_t, uint16_t(257)> rcp(PrimeField<uint16_t, uint16_t(257)> a)
{
	PrimeField<uint16_t, uint16_t(257)> t = a;
	for (int i = 1; i < 8; ++i)
		t *= a *= a;
	return t;
}

template <>
PrimeField<uint32_t, uint32_t(65537)> rcp(PrimeField<uint32_t, uint32_t(65537)> a)
{
	PrimeField<uint32_t, uint32_t(65537)> t = a;
	for (int i = 1; i < 16; ++i)
		t *= a *= a;
	return t;
}

template <>
PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> rcp(PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> a)
{
	PrimeField<uint32_t, uint32_t(0x7FFFFFFF)> t = a;
	a *= a;
	for (int i = 0; i < 29; ++i)
		t *= a *= a;
	return t;
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> div(PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return mul(a, rcp(b));
}

template <typename TYPE, TYPE PRIME>
PrimeField<TYPE, PRIME> operator / (PrimeField<TYPE, PRIME> a, PrimeField<TYPE, PRIME> b)
{
	return reduce(div(a, b));
}

}
