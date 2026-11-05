/*
WebAssembly SIMD128 acceleration times three

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <wasm_simd128.h>

template <>
union SIMD<float, 12>
{
	static const int SIZE = 12;
	typedef float value_type;
	typedef uint32_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int8_t, 48>
{
	static const int SIZE = 48;
	typedef int8_t value_type;
	typedef uint8_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int16_t, 24>
{
	static const int SIZE = 24;
	typedef int16_t value_type;
	typedef uint16_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int32_t, 12>
{
	static const int SIZE = 12;
	typedef int32_t value_type;
	typedef uint32_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int64_t, 6>
{
	static const int SIZE = 6;
	typedef int64_t value_type;
	typedef uint64_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint8_t, 48>
{
	static const int SIZE = 48;
	typedef uint8_t value_type;
	typedef uint8_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint16_t, 24>
{
	static const int SIZE = 24;
	typedef uint16_t value_type;
	typedef uint16_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint32_t, 12>
{
	static const int SIZE = 12;
	typedef uint32_t value_type;
	typedef uint32_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint64_t, 6>
{
	static const int SIZE = 6;
	typedef uint64_t value_type;
	typedef uint64_t uint_type;
	v128_t m[3];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
inline SIMD<float, 12> vreinterpret(SIMD<uint32_t, 12> a)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vreinterpret(SIMD<float, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vreinterpret(SIMD<uint8_t, 48> a)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vreinterpret(SIMD<int8_t, 48> a)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vreinterpret(SIMD<uint16_t, 24> a)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vreinterpret(SIMD<int16_t, 24> a)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vreinterpret(SIMD<uint32_t, 12> a)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vreinterpret(SIMD<int32_t, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = a.m[i];
	return tmp;
}

template <>
inline SIMD<float, 12> vdup(float a)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_splat(a);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vdup(int8_t a)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_splat(a);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vdup(int16_t a)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_splat(a);
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vdup(int32_t a)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_splat(a);
	return tmp;
}

template <>
inline SIMD<int64_t, 6> vdup(int64_t a)
{
	SIMD<int64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(a);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vdup(uint8_t a)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u8x16_splat(a);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vdup(uint16_t a)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u16x8_splat(a);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vdup(uint32_t a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u32x4_splat(a);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> vdup(uint64_t a)
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u64x2_splat(a);
	return tmp;
}

template <>
inline SIMD<float, 12> vzero()
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vzero()
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vzero()
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vzero()
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<int64_t, 6> vzero()
{
	SIMD<int64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vzero()
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vzero()
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vzero()
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> vzero()
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_splat(0);
	return tmp;
}

template <>
inline SIMD<float, 12> vadd(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vadd(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vadd(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vadd(SIMD<int32_t, 12> a, SIMD<int32_t, 12> b)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int64_t, 6> vadd(SIMD<int64_t, 6> a, SIMD<int64_t, 6> b)
{
	SIMD<int64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_add(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vqadd(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_add_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vqadd(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_add_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 12> vsub(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vsub(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vsub(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vsub(SIMD<int32_t, 12> a, SIMD<int32_t, 12> b)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int64_t, 6> vsub(SIMD<int64_t, 6> a, SIMD<int64_t, 6> b)
{
	SIMD<int64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i64x2_sub(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vqsub(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vqsub(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vqsub(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u8x16_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vqsub(SIMD<uint16_t, 24> a, SIMD<uint16_t, 24> b)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u16x8_sub_sat(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 12> vmul(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_mul(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vmul(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_shuffle(wasm_i16x8_extmul_low_i8x16(a.m[i], b.m[i]), wasm_i16x8_extmul_high_i8x16(a.m[i], b.m[i]), 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30);
	return tmp;
}

template <>
inline SIMD<float, 12> vabs(SIMD<float, 12> a)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_abs(a.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vqabs(SIMD<int8_t, 48> a)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u8x16_min(wasm_i8x16_abs(a.m[i]), wasm_i8x16_splat(INT8_MAX));
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vqabs(SIMD<int16_t, 24> a)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_u16x8_min(wasm_i16x8_abs(a.m[i]), wasm_i16x8_splat(INT16_MAX));
	return tmp;
}

template <>
inline SIMD<float, 12> vsignum(SIMD<float, 12> a)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_andnot(
			wasm_v128_xor(wasm_f32x4_splat(1.f), wasm_v128_and(wasm_f32x4_splat(-0.f), a.m[i])),
			wasm_f32x4_eq(a.m[i], wasm_f32x4_splat(0.f)));
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vsignum(SIMD<int8_t, 48> a)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(wasm_i8x16_gt(wasm_i8x16_splat(0), a.m[i]),
			wasm_v128_and(wasm_i8x16_gt(a.m[i], wasm_i8x16_splat(0)), wasm_i8x16_splat(1)));
	return tmp;
}

template <>
inline SIMD<float, 12> vsign(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_andnot(
			wasm_v128_xor(a.m[i], wasm_v128_and(wasm_f32x4_splat(-0.f), b.m[i])),
			wasm_f32x4_eq(b.m[i], wasm_f32x4_splat(0.f)));
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vsign(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(
			wasm_v128_and(wasm_i8x16_gt(wasm_i8x16_splat(0), b.m[i]), wasm_i8x16_neg(a.m[i])),
			wasm_v128_and(wasm_i8x16_gt(b.m[i], wasm_i8x16_splat(0)), a.m[i]));
	return tmp;
}

template <>
inline SIMD<float, 12> vcopysign(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(
			wasm_v128_andnot(a.m[i], wasm_f32x4_splat(-0.f)),
			wasm_v128_and(b.m[i], wasm_f32x4_splat(-0.f)));
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vorr(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vorr(SIMD<uint16_t, 24> a, SIMD<uint16_t, 24> b)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vorr(SIMD<uint32_t, 12> a, SIMD<uint32_t, 12> b)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> vorr(SIMD<uint64_t, 6> a, SIMD<uint64_t, 6> b)
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_or(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vand(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vand(SIMD<uint16_t, 24> a, SIMD<uint16_t, 24> b)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vand(SIMD<uint32_t, 12> a, SIMD<uint32_t, 12> b)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> vand(SIMD<uint64_t, 6> a, SIMD<uint64_t, 6> b)
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_and(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> veor(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> veor(SIMD<uint16_t, 24> a, SIMD<uint16_t, 24> b)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> veor(SIMD<uint32_t, 12> a, SIMD<uint32_t, 12> b)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> veor(SIMD<uint64_t, 6> a, SIMD<uint64_t, 6> b)
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_xor(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vbic(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vbic(SIMD<uint16_t, 24> a, SIMD<uint16_t, 24> b)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vbic(SIMD<uint32_t, 12> a, SIMD<uint32_t, 12> b)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> vbic(SIMD<uint64_t, 6> a, SIMD<uint64_t, 6> b)
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_andnot(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vbsl(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b, SIMD<uint8_t, 48> c)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vbsl(SIMD<uint16_t, 24> a, SIMD<uint16_t, 24> b, SIMD<uint16_t, 24> c)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vbsl(SIMD<uint32_t, 12> a, SIMD<uint32_t, 12> b, SIMD<uint32_t, 12> c)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 6> vbsl(SIMD<uint64_t, 6> a, SIMD<uint64_t, 6> b, SIMD<uint64_t, 6> c)
{
	SIMD<uint64_t, 6> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_v128_bitselect(b.m[i], c.m[i], a.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vceqz(SIMD<float, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_eq(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vceqz(SIMD<int8_t, 48> a)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_eq(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vceqz(SIMD<int16_t, 24> a)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_eq(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vceqz(SIMD<int32_t, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_eq(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vceq(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vceq(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vceq(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vceq(SIMD<int32_t, 12> a, SIMD<int32_t, 12> b)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_eq(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vcgtz(SIMD<float, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_gt(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vcgtz(SIMD<int8_t, 48> a)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_gt(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vcgtz(SIMD<int16_t, 24> a)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_gt(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vcgtz(SIMD<int32_t, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_gt(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vcltz(SIMD<float, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_lt(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vcltz(SIMD<int8_t, 48> a)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_lt(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vcltz(SIMD<int16_t, 24> a)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_lt(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vcltz(SIMD<int32_t, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_lt(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vclez(SIMD<float, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_le(a.m[i], wasm_f32x4_splat(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vclez(SIMD<int8_t, 48> a)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_le(a.m[i], wasm_i8x16_splat(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 24> vclez(SIMD<int16_t, 24> a)
{
	SIMD<uint16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_le(a.m[i], wasm_i16x8_splat(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 12> vclez(SIMD<int32_t, 12> a)
{
	SIMD<uint32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_le(a.m[i], wasm_i32x4_splat(0));
	return tmp;
}

template <>
inline SIMD<float, 12> vmin(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vmin(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vmin(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vmin(SIMD<int32_t, 12> a, SIMD<int32_t, 12> b)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_min(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 12> vmax(SIMD<float, 12> a, SIMD<float, 12> b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vmax(SIMD<int8_t, 48> a, SIMD<int8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vmax(SIMD<int16_t, 24> a, SIMD<int16_t, 24> b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vmax(SIMD<int32_t, 12> a, SIMD<int32_t, 12> b)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_max(a.m[i], b.m[i]);
	return tmp;
}

template <>
inline SIMD<float, 12> vclamp(SIMD<float, 12> x, float a, float b)
{
	SIMD<float, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_f32x4_min(wasm_f32x4_max(x.m[i], wasm_f32x4_splat(a)), wasm_f32x4_splat(b));
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vclamp(SIMD<int8_t, 48> x, int8_t a, int8_t b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i8x16_min(wasm_i8x16_max(x.m[i], wasm_i8x16_splat(a)), wasm_i8x16_splat(b));
	return tmp;
}

template <>
inline SIMD<int16_t, 24> vclamp(SIMD<int16_t, 24> x, int16_t a, int16_t b)
{
	SIMD<int16_t, 24> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i16x8_min(wasm_i16x8_max(x.m[i], wasm_i16x8_splat(a)), wasm_i16x8_splat(b));
	return tmp;
}

template <>
inline SIMD<int32_t, 12> vclamp(SIMD<int32_t, 12> x, int32_t a, int32_t b)
{
	SIMD<int32_t, 12> tmp;
	for (int i = 0; i < 3; ++i)
		tmp.m[i] = wasm_i32x4_min(wasm_i32x4_max(x.m[i], wasm_i32x4_splat(a)), wasm_i32x4_splat(b));
	return tmp;
}

template <>
inline SIMD<uint8_t, 48> vshuf(SIMD<uint8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<uint8_t, 48> tmp;
	for (int i = 0; i < 3; ++i) {
		tmp.m[i] = wasm_i8x16_swizzle(a.m[0], b.m[i]);
		for (int j = 1; j < 3; ++j)
			tmp.m[i] = wasm_v128_or(tmp.m[i], wasm_i8x16_swizzle(a.m[j], wasm_i8x16_sub(b.m[i], wasm_i8x16_splat(16 * j))));
	}
	return tmp;
}

template <>
inline SIMD<int8_t, 48> vshuf(SIMD<int8_t, 48> a, SIMD<uint8_t, 48> b)
{
	SIMD<int8_t, 48> tmp;
	for (int i = 0; i < 3; ++i) {
		tmp.m[i] = wasm_i8x16_swizzle(a.m[0], b.m[i]);
		for (int j = 1; j < 3; ++j)
			tmp.m[i] = wasm_v128_or(tmp.m[i], wasm_i8x16_swizzle(a.m[j], wasm_i8x16_sub(b.m[i], wasm_i8x16_splat(16 * j))));
	}
	return tmp;
}

