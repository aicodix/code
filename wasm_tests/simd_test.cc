/*
Test for the SIMD wrappers in WebAssembly

Built twice, with and without -msimd128, so simd_test.mjs can compare the
results of the simd128*.hh backend with those of the generic code in simd.hh

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include "simd.hh"

template <typename TYPE>
struct Info;

#define INFO(TYPE, UINT, FLOAT, SIGNED) \
template <> \
struct Info<TYPE> \
{ \
	typedef UINT uint_type; \
	static constexpr bool is_float = FLOAT, is_signed = SIGNED; \
	static constexpr const char *name = #TYPE; \
};

INFO(float, uint32_t, true, true)
INFO(double, uint64_t, true, true)
INFO(int8_t, uint8_t, false, true)
INFO(int16_t, uint16_t, false, true)
INFO(int32_t, uint32_t, false, true)
INFO(int64_t, uint64_t, false, true)
INFO(uint8_t, uint8_t, false, false)
INFO(uint16_t, uint16_t, false, false)
INFO(uint32_t, uint32_t, false, false)
INFO(uint64_t, uint64_t, false, false)

extern "C" {

__attribute__((visibility("default")))
unsigned char out[1 << 20];
__attribute__((visibility("default")))
int offsets[4096];
__attribute__((visibility("default")))
char names[1 << 17];
__attribute__((visibility("default")))
int name_offsets[4096];

}

static uint64_t state = 1234567;
static int count, out_len, names_len;

static uint64_t rnd()
{
	state ^= state << 13;
	state ^= state >> 7;
	state ^= state << 17;
	return state;
}

template <typename TYPE>
static TYPE lowest()
{
	typedef typename Info<TYPE>::uint_type uint_type;
	if (Info<TYPE>::is_signed)
		return TYPE(uint_type(1) << (8 * sizeof(TYPE) - 1));
	return TYPE(0);
}

template <typename TYPE>
static TYPE highest()
{
	return TYPE(~typename Info<TYPE>::uint_type(lowest<TYPE>()));
}

template <typename TYPE>
static TYPE random_value()
{
	if (Info<TYPE>::is_float) {
		// quarters, so that sums, differences and products are exact
		return TYPE(int(rnd() % 65) - 32) / TYPE(4);
	}
	switch (rnd() % 8) {
	case 0:
		return lowest<TYPE>();
	case 1:
		return highest<TYPE>();
	case 2:
		return TYPE(rnd() % 3) - TYPE(1);
	case 3:
		return TYPE(rnd() % 7);
	}
	return TYPE(rnd());
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
		if (rnd() % 3 == 0)
			tmp.v[i] = a.v[i];
	return tmp;
}

static void add_name(const char *str)
{
	while (*str)
		names[names_len++] = *str++;
}

static void add_number(int num)
{
	char digits[12];
	int len = 0;
	do
		digits[len++] = '0' + num % 10;
	while (num /= 10);
	while (len)
		names[names_len++] = digits[--len];
}

// records the bits of the result, with a negative zero as a positive one for results compared by value
template <typename TYPE, typename VECTOR>
static void put(const char *op, VECTOR a, bool value = false)
{
	if (value)
		for (int i = 0; i < VECTOR::SIZE; ++i)
			if (a.v[i] == typename VECTOR::value_type(0))
				a.u[i] = 0;
	offsets[count] = out_len;
	name_offsets[count] = names_len;
	add_name(op);
	add_name(" ");
	add_name(Info<TYPE>::name);
	add_name(" x");
	add_number(VECTOR::SIZE);
	names[names_len++] = 0;
	const unsigned char *bytes = reinterpret_cast<const unsigned char *>(a.u);
	for (unsigned i = 0; i < sizeof(a.u); ++i)
		out[out_len++] = bytes[i];
	++count;
}

template <typename TYPE, int WIDTH>
static void test()
{
	typedef SIMD<TYPE, WIDTH> vector;
	typedef typename Info<TYPE>::uint_type uint_type;
	typedef SIMD<uint_type, WIDTH> uint_vector;
	constexpr bool is_float = Info<TYPE>::is_float;
	constexpr bool is_signed = Info<TYPE>::is_signed;
	vector a = random_vector<vector>();
	vector b = random_vector<vector>(a);
	vector c = random_vector<vector>();
	TYPE s = random_value<TYPE>();
	put<TYPE>("vdup", vdup<vector>(s));
	put<TYPE>("vzero", vzero<vector>());
	uint_vector index;
	for (int i = 0; i < WIDTH; ++i)
		index.v[i] = rnd() % WIDTH;
	put<TYPE>("vshuf", vshuf(a, index));
	if constexpr (is_float || is_signed) {
		put<TYPE>("vmask", vmask(a));
		put<TYPE>("vreinterpret", vreinterpret<vector>(vreinterpret<uint_vector>(a)));
		put<TYPE>("vceq", vceq(a, b));
		put<TYPE>("vcgt", vcgt(a, b));
		put<TYPE>("vceqz", vceqz(a));
		put<TYPE>("vcgtz", vcgtz(a));
		put<TYPE>("vcltz", vcltz(a));
		put<TYPE>("vclez", vclez(a));
		put<TYPE>("vmin", vmin(a, b), true);
		put<TYPE>("vmax", vmax(a, b), true);
		TYPE lo = random_value<TYPE>(), hi = random_value<TYPE>();
		if (hi < lo) {
			TYPE tmp = lo;
			lo = hi;
			hi = tmp;
		}
		put<TYPE>("vclamp", vclamp(a, lo, hi), true);
		put<TYPE>("vadd", vadd(a, b), true);
		put<TYPE>("vsub", vsub(a, b), true);
		put<TYPE>("vmul", vmul(a, b), true);
		put<TYPE>("vneg", vneg(a), true);
		put<TYPE>("vsignum", vsignum(a), true);
		put<TYPE>("vsign", vsign(a, b), true);
	}
	if constexpr (is_float) {
		put<TYPE>("vabs", vabs(a), true);
		vector z = b;
		for (int i = 0; i < WIDTH; ++i)
			if (rnd() % 4 == 0)
				z.v[i] = -TYPE(0);
		put<TYPE>("vcopysign", vcopysign(a, z));
	}
	if constexpr (!is_float && is_signed) {
		put<TYPE>("vqabs", vqabs(a));
		vector d = a;
		for (int i = 0; i < WIDTH; ++i)
			if (d.v[i] == lowest<TYPE>())
				d.v[i] = highest<TYPE>();
		put<TYPE>("vabs", vabs(d));
		put<TYPE>("vunsigned", vunsigned(a));
		if constexpr (sizeof(TYPE) <= 2) {
			put<TYPE>("vqadd", vqadd(a, b));
			put<TYPE>("vqsub", vqsub(a, b));
		}
	}
	if constexpr (!is_float && !is_signed) {
		put<TYPE>("vand", vand(a, b));
		put<TYPE>("vorr", vorr(a, b));
		put<TYPE>("veor", veor(a, b));
		put<TYPE>("vbic", vbic(a, b));
		put<TYPE>("vnot", vnot(a));
		put<TYPE>("vbsl", vbsl(a, b, c));
		put<TYPE>("vcgt", vcgt(a, b));
		put<TYPE>("vsigned", vsigned(a));
		if constexpr (sizeof(TYPE) <= 2)
			put<TYPE>("vqsub", vqsub(a, b));
	}
}

// the widths of one to four 128 bit registers, where the backend specializes, and an odd one only the generic code handles
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

extern "C" {

// one round of all tests, returns the number of results
__attribute__((visibility("default")))
int run()
{
	count = out_len = names_len = 0;
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
	offsets[count] = out_len;
	return count;
}

}

