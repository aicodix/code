/*
WebAssembly SIMD128 acceleration times four

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <wasm_simd128.h>

template <>
union SIMD<float, 16>
{
	static const int SIZE = 16;
	typedef float value_type;
	typedef uint32_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int8_t, 64>
{
	static const int SIZE = 64;
	typedef int8_t value_type;
	typedef uint8_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int16_t, 32>
{
	static const int SIZE = 32;
	typedef int16_t value_type;
	typedef uint16_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int32_t, 16>
{
	static const int SIZE = 16;
	typedef int32_t value_type;
	typedef uint32_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int64_t, 8>
{
	static const int SIZE = 8;
	typedef int64_t value_type;
	typedef uint64_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint8_t, 64>
{
	static const int SIZE = 64;
	typedef uint8_t value_type;
	typedef uint8_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint16_t, 32>
{
	static const int SIZE = 32;
	typedef uint16_t value_type;
	typedef uint16_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint32_t, 16>
{
	static const int SIZE = 16;
	typedef uint32_t value_type;
	typedef uint32_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint64_t, 8>
{
	static const int SIZE = 8;
	typedef uint64_t value_type;
	typedef uint64_t uint_type;
	v128_t m[4];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
inline SIMD<float, 16> vreinterpret(SIMD<uint32_t, 16> a)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vreinterpret(SIMD<float, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vreinterpret(SIMD<uint8_t, 64> a)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vreinterpret(SIMD<int8_t, 64> a)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vreinterpret(SIMD<uint16_t, 32> a)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vreinterpret(SIMD<int16_t, 32> a)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vreinterpret(SIMD<uint32_t, 16> a)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vreinterpret(SIMD<int32_t, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<float, 16> vdup(float a)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_splat(a);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vdup(int8_t a)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_splat(a);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vdup(int16_t a)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_splat(a);
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vdup(int32_t a)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_splat(a);
	return tmp;
}

template <>
inline SIMD<int64_t, 8> vdup(int64_t a)
{
	SIMD<int64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(a);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vdup(uint8_t a)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u8x16_splat(a);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vdup(uint16_t a)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u16x8_splat(a);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vdup(uint32_t a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u32x4_splat(a);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> vdup(uint64_t a)
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u64x2_splat(a);
	return tmp;
}

template <>
inline SIMD<float, 16> vzero()
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vzero()
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vzero()
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vzero()
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int64_t, 8> vzero()
{
	SIMD<int64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vzero()
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vzero()
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vzero()
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> vzero()
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<float, 16> vadd(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vadd(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vadd(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vadd(SIMD<int32_t, 16> a, SIMD<int32_t, 16> b)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int64_t, 8> vadd(SIMD<int64_t, 8> a, SIMD<int64_t, 8> b)
{
	SIMD<int64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vqadd(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_add_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vqadd(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_add_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 16> vsub(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vsub(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vsub(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vsub(SIMD<int32_t, 16> a, SIMD<int32_t, 16> b)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int64_t, 8> vsub(SIMD<int64_t, 8> a, SIMD<int64_t, 8> b)
{
	SIMD<int64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i64x2_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vqsub(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vqsub(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vqsub(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u8x16_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vqsub(SIMD<uint16_t, 32> a, SIMD<uint16_t, 32> b)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u16x8_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 16> vmul(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_mul(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vmul(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_shuffle(wasm_i16x8_extmul_low_i8x16(a.m[i], b.m[i]), wasm_i16x8_extmul_high_i8x16(a.m[i], b.m[i]), 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30);
	return tmp;
}

template <>
inline SIMD<float, 16> vabs(SIMD<float, 16> a)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_abs(a.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vqabs(SIMD<int8_t, 64> a)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u8x16_min(wasm_i8x16_abs(a.m[i]), wasm_i8x16_splat(INT8_MAX));
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vqabs(SIMD<int16_t, 32> a)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_u16x8_min(wasm_i16x8_abs(a.m[i]), wasm_i16x8_splat(INT16_MAX));
	return tmp;
}

template <>
inline SIMD<float, 16> vsignum(SIMD<float, 16> a)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_andnot(
			wasm_v128_xor(wasm_f32x4_splat(1.f), wasm_v128_and(wasm_f32x4_splat(-0.f), a.m[i])),
			wasm_f32x4_eq(a.m[i], wasm_f32x4_splat(0.f)));
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vsignum(SIMD<int8_t, 64> a)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(wasm_i8x16_gt(wasm_i8x16_splat(0), a.m[i]),
			wasm_v128_and(wasm_i8x16_gt(a.m[i], wasm_i8x16_splat(0)), wasm_i8x16_splat(1)));
	return tmp;
}

template <>
inline SIMD<float, 16> vsign(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_andnot(
			wasm_v128_xor(a.m[i], wasm_v128_and(wasm_f32x4_splat(-0.f), b.m[i])),
			wasm_f32x4_eq(b.m[i], wasm_f32x4_splat(0.f)));
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vsign(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(
			wasm_v128_and(wasm_i8x16_gt(wasm_i8x16_splat(0), b.m[i]), wasm_i8x16_neg(a.m[i])),
			wasm_v128_and(wasm_i8x16_gt(b.m[i], wasm_i8x16_splat(0)), a.m[i]));
	return tmp;
}

template <>
inline SIMD<float, 16> vcopysign(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(
			wasm_v128_andnot(a.m[i], wasm_f32x4_splat(-0.f)),
			wasm_v128_and(b.m[i], wasm_f32x4_splat(-0.f)));
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vorr(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vorr(SIMD<uint16_t, 32> a, SIMD<uint16_t, 32> b)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vorr(SIMD<uint32_t, 16> a, SIMD<uint32_t, 16> b)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> vorr(SIMD<uint64_t, 8> a, SIMD<uint64_t, 8> b)
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vand(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vand(SIMD<uint16_t, 32> a, SIMD<uint16_t, 32> b)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vand(SIMD<uint32_t, 16> a, SIMD<uint32_t, 16> b)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> vand(SIMD<uint64_t, 8> a, SIMD<uint64_t, 8> b)
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> veor(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> veor(SIMD<uint16_t, 32> a, SIMD<uint16_t, 32> b)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> veor(SIMD<uint32_t, 16> a, SIMD<uint32_t, 16> b)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> veor(SIMD<uint64_t, 8> a, SIMD<uint64_t, 8> b)
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vbic(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vbic(SIMD<uint16_t, 32> a, SIMD<uint16_t, 32> b)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vbic(SIMD<uint32_t, 16> a, SIMD<uint32_t, 16> b)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> vbic(SIMD<uint64_t, 8> a, SIMD<uint64_t, 8> b)
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vbsl(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b, SIMD<uint8_t, 64> c)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vbsl(SIMD<uint16_t, 32> a, SIMD<uint16_t, 32> b, SIMD<uint16_t, 32> c)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vbsl(SIMD<uint32_t, 16> a, SIMD<uint32_t, 16> b, SIMD<uint32_t, 16> c)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 8> vbsl(SIMD<uint64_t, 8> a, SIMD<uint64_t, 8> b, SIMD<uint64_t, 8> c)
{
	SIMD<uint64_t, 8> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vceqz(SIMD<float, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_eq(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vceqz(SIMD<int8_t, 64> a)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_eq(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vceqz(SIMD<int16_t, 32> a)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_eq(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vceqz(SIMD<int32_t, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_eq(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vceq(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vceq(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vceq(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vceq(SIMD<int32_t, 16> a, SIMD<int32_t, 16> b)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vcgtz(SIMD<float, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_gt(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vcgtz(SIMD<int8_t, 64> a)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_gt(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vcgtz(SIMD<int16_t, 32> a)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_gt(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vcgtz(SIMD<int32_t, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_gt(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vcltz(SIMD<float, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_lt(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vcltz(SIMD<int8_t, 64> a)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_lt(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vcltz(SIMD<int16_t, 32> a)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_lt(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vcltz(SIMD<int32_t, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_lt(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vclez(SIMD<float, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_le(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vclez(SIMD<int8_t, 64> a)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_le(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 32> vclez(SIMD<int16_t, 32> a)
{
	SIMD<uint16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_le(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 16> vclez(SIMD<int32_t, 16> a)
{
	SIMD<uint32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_le(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<float, 16> vmin(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vmin(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vmin(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vmin(SIMD<int32_t, 16> a, SIMD<int32_t, 16> b)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 16> vmax(SIMD<float, 16> a, SIMD<float, 16> b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vmax(SIMD<int8_t, 64> a, SIMD<int8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vmax(SIMD<int16_t, 32> a, SIMD<int16_t, 32> b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vmax(SIMD<int32_t, 16> a, SIMD<int32_t, 16> b)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 16> vclamp(SIMD<float, 16> x, float a, float b)
{
	SIMD<float, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_f32x4_min(wasm_f32x4_max(x.m[i], wasm_f32x4_splat(a)), wasm_f32x4_splat(b));
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vclamp(SIMD<int8_t, 64> x, int8_t a, int8_t b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i8x16_min(wasm_i8x16_max(x.m[i], wasm_i8x16_splat(a)), wasm_i8x16_splat(b));
	return tmp;
}

template <>
inline SIMD<int16_t, 32> vclamp(SIMD<int16_t, 32> x, int16_t a, int16_t b)
{
	SIMD<int16_t, 32> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i16x8_min(wasm_i16x8_max(x.m[i], wasm_i16x8_splat(a)), wasm_i16x8_splat(b));
	return tmp;
}

template <>
inline SIMD<int32_t, 16> vclamp(SIMD<int32_t, 16> x, int32_t a, int32_t b)
{
	SIMD<int32_t, 16> tmp;
	for (int i = 0; i < 4; ++i)
		tmp.m[i] = wasm_i32x4_min(wasm_i32x4_max(x.m[i], wasm_i32x4_splat(a)), wasm_i32x4_splat(b));
	return tmp;
}

template <>
inline SIMD<uint8_t, 64> vshuf(SIMD<uint8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<uint8_t, 64> tmp;
	for (int i = 0; i < 4; ++i) {
		tmp.m[i] = wasm_i8x16_swizzle(a.m[0], b.m[i]);
		for (int j = 1; j < 4; ++j)
			tmp.m[i] = wasm_v128_or(tmp.m[i], wasm_i8x16_swizzle(a.m[j], wasm_i8x16_sub(b.m[i], wasm_i8x16_splat(16 * j))));
	}
	return tmp;
}

template <>
inline SIMD<int8_t, 64> vshuf(SIMD<int8_t, 64> a, SIMD<uint8_t, 64> b)
{
	SIMD<int8_t, 64> tmp;
	for (int i = 0; i < 4; ++i) {
		tmp.m[i] = wasm_i8x16_swizzle(a.m[0], b.m[i]);
		for (int j = 1; j < 4; ++j)
			tmp.m[i] = wasm_v128_or(tmp.m[i], wasm_i8x16_swizzle(a.m[j], wasm_i8x16_sub(b.m[i], wasm_i8x16_splat(16 * j))));
	}
	return tmp;
}

