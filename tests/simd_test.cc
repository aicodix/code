/*
Test for the SIMD wrappers

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <cstring>
#include <limits>
#include <random>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include "simd.hh"

static std::mt19937_64 generator(1234567);

template <typename TYPE>
struct Bits
{
	typedef typename std::conditional<sizeof(TYPE) == 1, uint8_t,
		typename std::conditional<sizeof(TYPE) == 2, uint16_t,
		typename std::conditional<sizeof(TYPE) == 4, uint32_t, uint64_t>::type>::type>::type type;
};

template <typename TYPE>
static TYPE random_value()
{
	if constexpr (std::is_floating_point<TYPE>::value) {
		// quarters, so that sums, differences and products are exact
		return TYPE(int(generator() % 65) - 32) / TYPE(4);
	} else {
		typedef std::numeric_limits<TYPE> limits;
		switch (generator() % 8) {
		case 0:
			return limits::min();
		case 1:
			return limits::max();
		case 2:
			return TYPE(generator() % 3) - TYPE(1);
		case 3:
			return TYPE(generator() % 7);
		}
		return TYPE(generator());
	}
}

template <typename VECTOR>
static VECTOR random_vector()
{
	VECTOR tmp;
	for (int i = 0; i < VECTOR::SIZE; ++i)
		tmp.v[i] = random_value<typename VECTOR::value_type>();
	return tmp;
}

// copies some of the values of a, so that comparisons see equal values
template <typename VECTOR>
static VECTOR random_vector(VECTOR a)
{
	VECTOR tmp = random_vector<VECTOR>();
	for (int i = 0; i < VECTOR::SIZE; ++i)
		if (generator() % 3 == 0)
			tmp.v[i] = a.v[i];
	return tmp;
}

template <typename TYPE>
static typename Bits<TYPE>::type bits(TYPE a)
{
	typename Bits<TYPE>::type tmp;
	std::memcpy(&tmp, &a, sizeof(a));
	return tmp;
}

template <typename TYPE>
static TYPE from_bits(typename Bits<TYPE>::type a)
{
	TYPE tmp;
	std::memcpy(&tmp, &a, sizeof(a));
	return tmp;
}

template <typename TYPE>
static typename Bits<TYPE>::type mask(bool a)
{
	return a ? ~typename Bits<TYPE>::type(0) : 0;
}

// wrapping arithmetic like the hardware, without signed overflow in C++
template <typename TYPE>
static TYPE wrap(uint64_t a)
{
	return from_bits<TYPE>(typename Bits<TYPE>::type(a));
}

template <typename TYPE>
static TYPE saturate(int64_t a)
{
	return TYPE(std::min<int64_t>(std::max<int64_t>(a, std::numeric_limits<TYPE>::min()), std::numeric_limits<TYPE>::max()));
}

template <typename VECTOR, typename FUNC>
static void check(const char *name, VECTOR a, FUNC ref)
{
	for (int i = 0; i < VECTOR::SIZE; ++i) {
		if (a.u[i] != ref(i)) {
			std::cerr << name << " failed for " << sizeof(typename VECTOR::value_type) << " byte " << (std::is_floating_point<typename VECTOR::value_type>::value ? "float" : std::is_signed<typename VECTOR::value_type>::value ? "signed" : "unsigned") << " values at width " << VECTOR::SIZE << " lane " << i << std::endl;
			assert(false);
		}
	}
}

// compares values, so that negative and positive zero are the same
template <typename VECTOR, typename FUNC>
static void check_value(const char *name, VECTOR a, FUNC ref)
{
	for (int i = 0; i < VECTOR::SIZE; ++i) {
		if (a.v[i] != ref(i)) {
			std::cerr << name << " failed for " << sizeof(typename VECTOR::value_type) << " byte float values at width " << VECTOR::SIZE << " lane " << i << std::endl;
			assert(false);
		}
	}
}

template <typename TYPE, int WIDTH>
static void test()
{
	typedef SIMD<TYPE, WIDTH> vector;
	typedef typename Bits<TYPE>::type uint_type;
	typedef SIMD<uint_type, WIDTH> uint_vector;
	constexpr bool is_float = std::is_floating_point<TYPE>::value;
	constexpr bool is_signed = std::is_signed<TYPE>::value;
	for (int round = 0; round < 1000; ++round) {
		vector a = random_vector<vector>();
		vector b = random_vector<vector>(a);
		vector c = random_vector<vector>();
		TYPE s = random_value<TYPE>();
		check("vdup", vdup<vector>(s), [&](int) { return bits(s); });
		check("vzero", vzero<vector>(), [&](int) { return uint_type(0); });
		uint_vector index;
		for (int i = 0; i < WIDTH; ++i)
			index.v[i] = generator() % WIDTH;
		check("vshuf", vshuf(a, index), [&](int i) { return bits(a.v[index.v[i]]); });
		if constexpr (is_float || is_signed) {
			check("vmask", vmask(a), [&](int i) { return bits(a.v[i]); });
			check("vreinterpret", vreinterpret<vector>(vreinterpret<uint_vector>(a)), [&](int i) { return bits(a.v[i]); });
			check("vceq", vceq(a, b), [&](int i) { return mask<TYPE>(a.v[i] == b.v[i]); });
			check("vcgt", vcgt(a, b), [&](int i) { return mask<TYPE>(a.v[i] > b.v[i]); });
			check("vceqz", vceqz(a), [&](int i) { return mask<TYPE>(a.v[i] == TYPE(0)); });
			check("vcgtz", vcgtz(a), [&](int i) { return mask<TYPE>(a.v[i] > TYPE(0)); });
			check("vcltz", vcltz(a), [&](int i) { return mask<TYPE>(a.v[i] < TYPE(0)); });
			check("vclez", vclez(a), [&](int i) { return mask<TYPE>(a.v[i] <= TYPE(0)); });
			check("vmin", vmin(a, b), [&](int i) { return bits(std::min(a.v[i], b.v[i])); });
			check("vmax", vmax(a, b), [&](int i) { return bits(std::max(a.v[i], b.v[i])); });
			TYPE lo = random_value<TYPE>(), hi = random_value<TYPE>();
			if (hi < lo)
				std::swap(lo, hi);
			check("vclamp", vclamp(a, lo, hi), [&](int i) { return bits(std::min(std::max(a.v[i], lo), hi)); });
		}
		if constexpr (is_float) {
			check_value("vadd", vadd(a, b), [&](int i) { return a.v[i] + b.v[i]; });
			check_value("vsub", vsub(a, b), [&](int i) { return a.v[i] - b.v[i]; });
			check_value("vmul", vmul(a, b), [&](int i) { return a.v[i] * b.v[i]; });
			check_value("vneg", vneg(a), [&](int i) { return -a.v[i]; });
			check_value("vabs", vabs(a), [&](int i) { return std::abs(a.v[i]); });
			check_value("vsignum", vsignum(a), [&](int i) { return TYPE((a.v[i] > TYPE(0)) - (a.v[i] < TYPE(0))); });
			check_value("vsign", vsign(a, b), [&](int i) { return b.v[i] > TYPE(0) ? a.v[i] : b.v[i] < TYPE(0) ? -a.v[i] : TYPE(0); });
			vector z = b;
			for (int i = 0; i < WIDTH; ++i)
				if (generator() % 4 == 0)
					z.v[i] = -TYPE(0);
			uint_type negz = bits(-TYPE(0));
			check("vcopysign", vcopysign(a, z), [&](int i) { return uint_type((bits(a.v[i]) & ~negz) | (bits(z.v[i]) & negz)); });
		}
		if constexpr (!is_float && is_signed) {
			check("vadd", vadd(a, b), [&](int i) { return bits(wrap<TYPE>(uint64_t(a.v[i]) + uint64_t(b.v[i]))); });
			check("vsub", vsub(a, b), [&](int i) { return bits(wrap<TYPE>(uint64_t(a.v[i]) - uint64_t(b.v[i]))); });
			check("vmul", vmul(a, b), [&](int i) { return bits(wrap<TYPE>(uint64_t(a.v[i]) * uint64_t(b.v[i]))); });
			check("vneg", vneg(a), [&](int i) { return bits(wrap<TYPE>(-uint64_t(a.v[i]))); });
			check("vsignum", vsignum(a), [&](int i) { return bits(TYPE((a.v[i] > 0) - (a.v[i] < 0))); });
			check("vsign", vsign(a, b), [&](int i) { return bits(b.v[i] > 0 ? a.v[i] : b.v[i] < 0 ? wrap<TYPE>(-uint64_t(a.v[i])) : TYPE(0)); });
			check("vqabs", vqabs(a), [&](int i) { return bits(a.v[i] == std::numeric_limits<TYPE>::min() ? std::numeric_limits<TYPE>::max() : TYPE(a.v[i] < 0 ? -a.v[i] : a.v[i])); });
			vector d = a;
			for (int i = 0; i < WIDTH; ++i)
				if (d.v[i] == std::numeric_limits<TYPE>::min())
					d.v[i] = std::numeric_limits<TYPE>::max();
			check("vabs", vabs(d), [&](int i) { return bits(TYPE(d.v[i] < 0 ? -d.v[i] : d.v[i])); });
			check("vunsigned", vunsigned(a), [&](int i) { return bits(a.v[i]); });
			if constexpr (sizeof(TYPE) <= 2) {
				check("vqadd", vqadd(a, b), [&](int i) { return bits(saturate<TYPE>(int64_t(a.v[i]) + int64_t(b.v[i]))); });
				check("vqsub", vqsub(a, b), [&](int i) { return bits(saturate<TYPE>(int64_t(a.v[i]) - int64_t(b.v[i]))); });
			}
		}
		if constexpr (!is_float && !is_signed) {
			check("vand", vand(a, b), [&](int i) { return uint_type(a.v[i] & b.v[i]); });
			check("vorr", vorr(a, b), [&](int i) { return uint_type(a.v[i] | b.v[i]); });
			check("veor", veor(a, b), [&](int i) { return uint_type(a.v[i] ^ b.v[i]); });
			check("vbic", vbic(a, b), [&](int i) { return uint_type(a.v[i] & ~b.v[i]); });
			check("vnot", vnot(a), [&](int i) { return uint_type(~a.v[i]); });
			check("vbsl", vbsl(a, b, c), [&](int i) { return uint_type((a.v[i] & b.v[i]) | (~a.v[i] & c.v[i])); });
			check("vcgt", vcgt(a, b), [&](int i) { return mask<TYPE>(a.v[i] > b.v[i]); });
			check("vsigned", vsigned(a), [&](int i) { return bits(a.v[i]); });
			if constexpr (sizeof(TYPE) <= 2)
				check("vqsub", vqsub(a, b), [&](int i) { return uint_type(a.v[i] > b.v[i] ? a.v[i] - b.v[i] : 0); });
		}
	}
}

// the widths of 128, 256, 384 and 512 bit registers, where the backends specialize, and an odd one only the generic code handles
template <typename TYPE>
static void test_widths()
{
	constexpr int LANES = 16 / sizeof(TYPE);
	test<TYPE, LANES>();
	test<TYPE, 2 * LANES>();
	test<TYPE, 3 * LANES>();
	test<TYPE, 4 * LANES>();
	test<TYPE, 3>();
}

int main()
{
	test_widths<float>();
	test_widths<double>();
	test_widths<int8_t>();
	test_widths<int16_t>();
	test_widths<int32_t>();
	test_widths<int64_t>();
	test_widths<uint8_t>();
	test_widths<uint16_t>();
	test_widths<uint32_t>();
	test_widths<uint64_t>();
	std::cerr << "SIMD wrappers test passed!" << std::endl;
	return 0;
}

