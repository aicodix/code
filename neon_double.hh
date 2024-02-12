/*
ARM NEON acceleration times two

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <arm_neon.h>

template <>
union SIMD<float, 8>
{
	static const int SIZE = 8;
	typedef float value_type;
	typedef uint32_t uint_type;
	float32x4_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int8_t, 32>
{
	static const int SIZE = 32;
	typedef int8_t value_type;
	typedef uint8_t uint_type;
	int8x16_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int16_t, 16>
{
	static const int SIZE = 16;
	typedef int16_t value_type;
	typedef uint16_t uint_type;
	int16x8_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int32_t, 8>
{
	static const int SIZE = 8;
	typedef int32_t value_type;
	typedef uint32_t uint_type;
	int32x4_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<int64_t, 4>
{
	static const int SIZE = 4;
	typedef int64_t value_type;
	typedef uint64_t uint_type;
	int64x2_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint8_t, 32>
{
	static const int SIZE = 32;
	typedef uint8_t value_type;
	typedef uint8_t uint_type;
	uint8x16_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint16_t, 16>
{
	static const int SIZE = 16;
	typedef uint16_t value_type;
	typedef uint16_t uint_type;
	uint16x8_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint32_t, 8>
{
	static const int SIZE = 8;
	typedef uint32_t value_type;
	typedef uint32_t uint_type;
	uint32x4_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
union SIMD<uint64_t, 4>
{
	static const int SIZE = 4;
	typedef uint64_t value_type;
	typedef uint64_t uint_type;
	uint64x2_t m[2];
	value_type v[SIZE];
	uint_type u[SIZE];
};

template <>
inline SIMD<float, 8> vreinterpret(SIMD<uint32_t, 8> a)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = (float32x4_t)a.m[0];
	tmp.m[1] = (float32x4_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vreinterpret(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = (uint32x4_t)a.m[0];
	tmp.m[1] = (uint32x4_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vreinterpret(SIMD<uint8_t, 32> a)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = (int8x16_t)a.m[0];
	tmp.m[1] = (int8x16_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vreinterpret(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = (uint8x16_t)a.m[0];
	tmp.m[1] = (uint8x16_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vreinterpret(SIMD<uint16_t, 16> a)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = (int16x8_t)a.m[0];
	tmp.m[1] = (int16x8_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vreinterpret(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = (uint16x8_t)a.m[0];
	tmp.m[1] = (uint16x8_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vreinterpret(SIMD<uint32_t, 8> a)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = (int32x4_t)a.m[0];
	tmp.m[1] = (int32x4_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vreinterpret(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = (uint32x4_t)a.m[0];
	tmp.m[1] = (uint32x4_t)a.m[1];
	return tmp;
}

template <>
inline SIMD<float, 8> vdup(float a)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vdupq_n_f32(a);
	tmp.m[1] = vdupq_n_f32(a);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vdup(int8_t a)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vdupq_n_s8(a);
	tmp.m[1] = vdupq_n_s8(a);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vdup(int16_t a)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vdupq_n_s16(a);
	tmp.m[1] = vdupq_n_s16(a);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vdup(int32_t a)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = vdupq_n_s32(a);
	tmp.m[1] = vdupq_n_s32(a);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vdup(int64_t a)
{
	SIMD<int64_t, 4> tmp;
	tmp.m[0] = vdupq_n_s64(a);
	tmp.m[1] = vdupq_n_s64(a);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vdup(uint8_t a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vdupq_n_u8(a);
	tmp.m[1] = vdupq_n_u8(a);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vdup(uint16_t a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vdupq_n_u16(a);
	tmp.m[1] = vdupq_n_u16(a);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vdup(uint32_t a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vdupq_n_u32(a);
	tmp.m[1] = vdupq_n_u32(a);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vdup(uint64_t a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = vdupq_n_u64(a);
	tmp.m[1] = vdupq_n_u64(a);
	return tmp;
}

template <>
inline SIMD<float, 8> vzero()
{
	SIMD<float, 8> tmp;
	tmp.m[0] = (float32x4_t)veorq_u32((uint32x4_t)tmp.m[0], (uint32x4_t)tmp.m[0]);
	tmp.m[1] = (float32x4_t)veorq_u32((uint32x4_t)tmp.m[1], (uint32x4_t)tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vzero()
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = veorq_s8(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_s8(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vzero()
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = veorq_s16(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_s16(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vzero()
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = veorq_s32(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_s32(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vzero()
{
	SIMD<int64_t, 4> tmp;
	tmp.m[0] = veorq_s64(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_s64(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vzero()
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = veorq_u8(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_u8(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vzero()
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = veorq_u16(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_u16(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vzero()
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = veorq_u32(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_u32(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vzero()
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = veorq_u64(tmp.m[0], tmp.m[0]);
	tmp.m[1] = veorq_u64(tmp.m[1], tmp.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vadd(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vaddq_f32(a.m[0], b.m[0]);
	tmp.m[1] = vaddq_f32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vadd(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vaddq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vaddq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vadd(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vaddq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vaddq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vadd(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = vaddq_s32(a.m[0], b.m[0]);
	tmp.m[1] = vaddq_s32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vadd(SIMD<int64_t, 4> a, SIMD<int64_t, 4> b)
{
	SIMD<int64_t, 4> tmp;
	tmp.m[1] = vaddq_s64(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vqadd(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vqaddq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vqaddq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vqadd(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vqaddq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vqaddq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vsub(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vsubq_f32(a.m[0], b.m[0]);
	tmp.m[1] = vsubq_f32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vsub(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vsubq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vsubq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vsub(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vsubq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vsubq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vsub(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = vsubq_s32(a.m[0], b.m[0]);
	tmp.m[1] = vsubq_s32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vsub(SIMD<int64_t, 4> a, SIMD<int64_t, 4> b)
{
	SIMD<int64_t, 4> tmp;
	tmp.m[0] = vsubq_s64(a.m[0], b.m[0]);
	tmp.m[1] = vsubq_s64(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vqsub(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vqsubq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vqsubq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vqsub(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vqsubq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vqsubq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vqsub(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vqsubq_u8(a.m[0], b.m[0]);
	tmp.m[1] = vqsubq_u8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vqsub(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vqsubq_u16(a.m[0], b.m[0]);
	tmp.m[1] = vqsubq_u16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vmul(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vmulq_f32(a.m[0], b.m[0]);
	tmp.m[1] = vmulq_f32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vmul(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vmulq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vmulq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vabs(SIMD<float, 8> a)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vabsq_f32(a.m[0]);
	tmp.m[1] = vabsq_f32(a.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vqabs(SIMD<int8_t, 32> a)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vqabsq_s8(a.m[0]);
	tmp.m[1] = vqabsq_s8(a.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vqabs(SIMD<int16_t, 16> a)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vqabsq_s16(a.m[0]);
	tmp.m[1] = vqabsq_s16(a.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vsignum(SIMD<float, 8> a)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = (float32x4_t)vbicq_u32(
		veorq_u32((uint32x4_t)vdupq_n_f32(1.f), vandq_u32((uint32x4_t)vdupq_n_f32(-0.f), (uint32x4_t)a.m[0])),
		vceqq_f32(a.m[0], vdupq_n_f32(0.f)));
	tmp.m[1] = (float32x4_t)vbicq_u32(
		veorq_u32((uint32x4_t)vdupq_n_f32(1.f), vandq_u32((uint32x4_t)vdupq_n_f32(-0.f), (uint32x4_t)a.m[1])),
		vceqq_f32(a.m[1], vdupq_n_f32(0.f)));
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vsignum(SIMD<int8_t, 32> a)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = (int8x16_t)vorrq_u8(vcgtq_s8(vdupq_n_s8(0), a.m[0]),
		vandq_u8(vcgtq_s8(a.m[0], vdupq_n_s8(0)), (uint8x16_t)vdupq_n_s8(1)));
	tmp.m[1] = (int8x16_t)vorrq_u8(vcgtq_s8(vdupq_n_s8(0), a.m[1]),
		vandq_u8(vcgtq_s8(a.m[1], vdupq_n_s8(0)), (uint8x16_t)vdupq_n_s8(1)));
	return tmp;
}

template <>
inline SIMD<float, 8> vsign(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = (float32x4_t)vbicq_u32(
		veorq_u32((uint32x4_t)a.m[0], vandq_u32((uint32x4_t)vdupq_n_f32(-0.f), (uint32x4_t)b.m[0])),
		vceqq_f32(b.m[0], vdupq_n_f32(0.f)));
	tmp.m[1] = (float32x4_t)vbicq_u32(
		veorq_u32((uint32x4_t)a.m[1], vandq_u32((uint32x4_t)vdupq_n_f32(-0.f), (uint32x4_t)b.m[1])),
		vceqq_f32(b.m[1], vdupq_n_f32(0.f)));
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vsign(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = (int8x16_t)vorrq_u8(
		vandq_u8(vcgtq_s8(vdupq_n_s8(0), b.m[0]), (uint8x16_t)vnegq_s8(a.m[0])),
		vandq_u8(vcgtq_s8(b.m[0], vdupq_n_s8(0)), (uint8x16_t)a.m[0]));
	tmp.m[1] = (int8x16_t)vorrq_u8(
		vandq_u8(vcgtq_s8(vdupq_n_s8(0), b.m[1]), (uint8x16_t)vnegq_s8(a.m[1])),
		vandq_u8(vcgtq_s8(b.m[1], vdupq_n_s8(0)), (uint8x16_t)a.m[1]));
	return tmp;
}

template <>
inline SIMD<float, 8> vcopysign(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	uint32x4_t negz = (uint32x4_t)vdupq_n_f32(-0.f);
	tmp.m[0] = (float32x4_t)vorrq_u32(
		vbicq_u32((uint32x4_t)a.m[0], negz),
		vandq_u32((uint32x4_t)b.m[0], negz));
	tmp.m[1] = (float32x4_t)vorrq_u32(
		vbicq_u32((uint32x4_t)a.m[1], negz),
		vandq_u32((uint32x4_t)b.m[1], negz));
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vorr(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vorrq_u8(a.m[0], b.m[0]);
	tmp.m[1] = vorrq_u8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vorr(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vorrq_u16(a.m[0], b.m[0]);
	tmp.m[1] = vorrq_u16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vorr(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vorrq_u32(a.m[0], b.m[0]);
	tmp.m[1] = vorrq_u32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vorr(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = vorrq_u64(a.m[0], b.m[0]);
	tmp.m[1] = vorrq_u64(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vand(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vandq_u8(a.m[0], b.m[0]);
	tmp.m[1] = vandq_u8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vand(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vandq_u16(a.m[0], b.m[0]);
	tmp.m[1] = vandq_u16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vand(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vandq_u32(a.m[0], b.m[0]);
	tmp.m[1] = vandq_u32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vand(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = vandq_u64(a.m[0], b.m[0]);
	tmp.m[1] = vandq_u64(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> veor(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = veorq_u8(a.m[0], b.m[0]);
	tmp.m[1] = veorq_u8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> veor(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = veorq_u16(a.m[0], b.m[0]);
	tmp.m[1] = veorq_u16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> veor(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = veorq_u32(a.m[0], b.m[0]);
	tmp.m[1] = veorq_u32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> veor(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = veorq_u64(a.m[0], b.m[0]);
	tmp.m[1] = veorq_u64(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vbic(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vbicq_u8(a.m[0], b.m[0]);
	tmp.m[1] = vbicq_u8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vbic(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vbicq_u16(a.m[0], b.m[0]);
	tmp.m[1] = vbicq_u16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vbic(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vbicq_u32(a.m[0], b.m[0]);
	tmp.m[1] = vbicq_u32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vbic(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = vbicq_u64(a.m[0], b.m[0]);
	tmp.m[1] = vbicq_u64(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vbsl(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b, SIMD<uint8_t, 32> c)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vbslq_u8(a.m[0], b.m[0], c.m[0]);
	tmp.m[1] = vbslq_u8(a.m[1], b.m[1], c.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vbsl(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b, SIMD<uint16_t, 16> c)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vbslq_u16(a.m[0], b.m[0], c.m[0]);
	tmp.m[1] = vbslq_u16(a.m[1], b.m[1], c.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vbsl(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b, SIMD<uint32_t, 8> c)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vbslq_u32(a.m[0], b.m[0], c.m[0]);
	tmp.m[1] = vbslq_u32(a.m[1], b.m[1], c.m[1]);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vbsl(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b, SIMD<uint64_t, 4> c)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m[0] = vbslq_u64(a.m[0], b.m[0], c.m[0]);
	tmp.m[1] = vbslq_u64(a.m[1], b.m[1], c.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vceqz(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vceqq_f32(a.m[0], vdupq_n_f32(0.f));
	tmp.m[1] = vceqq_f32(a.m[1], vdupq_n_f32(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vceqz(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vceqq_s8(a.m[0], vdupq_n_s8(0));
	tmp.m[1] = vceqq_s8(a.m[1], vdupq_n_s8(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vceqz(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vceqq_s16(a.m[0], vdupq_n_s16(0));
	tmp.m[1] = vceqq_s16(a.m[1], vdupq_n_s16(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vceqz(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vceqq_s32(a.m[0], vdupq_n_s32(0));
	tmp.m[1] = vceqq_s32(a.m[1], vdupq_n_s32(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vceq(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vceqq_f32(a.m[0], b.m[0]);
	tmp.m[1] = vceqq_f32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vceq(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vceqq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vceqq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vceq(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vceqq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vceqq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vceq(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vceqq_s32(a.m[0], b.m[0]);
	tmp.m[1] = vceqq_s32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcgtz(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vcgtq_f32(a.m[0], vdupq_n_f32(0.f));
	tmp.m[1] = vcgtq_f32(a.m[1], vdupq_n_f32(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vcgtz(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vcgtq_s8(a.m[0], vdupq_n_s8(0));
	tmp.m[1] = vcgtq_s8(a.m[1], vdupq_n_s8(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vcgtz(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vcgtq_s16(a.m[0], vdupq_n_s16(0));
	tmp.m[1] = vcgtq_s16(a.m[1], vdupq_n_s16(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcgtz(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vcgtq_s32(a.m[0], vdupq_n_s32(0));
	tmp.m[1] = vcgtq_s32(a.m[1], vdupq_n_s32(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcltz(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vcltq_f32(a.m[0], vdupq_n_f32(0.f));
	tmp.m[1] = vcltq_f32(a.m[1], vdupq_n_f32(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vcltz(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vcltq_s8(a.m[0], vdupq_n_s8(0));
	tmp.m[1] = vcltq_s8(a.m[1], vdupq_n_s8(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vcltz(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vcltq_s16(a.m[0], vdupq_n_s16(0));
	tmp.m[1] = vcltq_s16(a.m[1], vdupq_n_s16(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcltz(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vcltq_s32(a.m[0], vdupq_n_s32(0));
	tmp.m[1] = vcltq_s32(a.m[1], vdupq_n_s32(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vclez(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vcleq_f32(a.m[0], vdupq_n_f32(0.f));
	tmp.m[1] = vcleq_f32(a.m[1], vdupq_n_f32(0.f));
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vclez(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vcleq_s8(a.m[0], vdupq_n_s8(0));
	tmp.m[1] = vcleq_s8(a.m[1], vdupq_n_s8(0));
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vclez(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m[0] = vcleq_s16(a.m[0], vdupq_n_s16(0));
	tmp.m[1] = vcleq_s16(a.m[1], vdupq_n_s16(0));
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vclez(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m[0] = vcleq_s32(a.m[0], vdupq_n_s32(0));
	tmp.m[1] = vcleq_s32(a.m[1], vdupq_n_s32(0));
	return tmp;
}

template <>
inline SIMD<float, 8> vmin(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vminq_f32(a.m[0], b.m[0]);
	tmp.m[1] = vminq_f32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vmin(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vminq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vminq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vmin(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vminq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vminq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vmin(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = vminq_s32(a.m[0], b.m[0]);
	tmp.m[1] = vminq_s32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vmax(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vmaxq_f32(a.m[0], b.m[0]);
	tmp.m[1] = vmaxq_f32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vmax(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vmaxq_s8(a.m[0], b.m[0]);
	tmp.m[1] = vmaxq_s8(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vmax(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vmaxq_s16(a.m[0], b.m[0]);
	tmp.m[1] = vmaxq_s16(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vmax(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = vmaxq_s32(a.m[0], b.m[0]);
	tmp.m[1] = vmaxq_s32(a.m[1], b.m[1]);
	return tmp;
}

template <>
inline SIMD<float, 8> vclamp(SIMD<float, 8> x, float a, float b)
{
	SIMD<float, 8> tmp;
	tmp.m[0] = vminq_f32(vmaxq_f32(x.m[0], vdupq_n_f32(a)), vdupq_n_f32(b));
	tmp.m[1] = vminq_f32(vmaxq_f32(x.m[1], vdupq_n_f32(a)), vdupq_n_f32(b));
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vclamp(SIMD<int8_t, 32> x, int8_t a, int8_t b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vminq_s8(vmaxq_s8(x.m[0], vdupq_n_s8(a)), vdupq_n_s8(b));
	tmp.m[1] = vminq_s8(vmaxq_s8(x.m[1], vdupq_n_s8(a)), vdupq_n_s8(b));
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vclamp(SIMD<int16_t, 16> x, int16_t a, int16_t b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m[0] = vminq_s16(vmaxq_s16(x.m[0], vdupq_n_s16(a)), vdupq_n_s16(b));
	tmp.m[1] = vminq_s16(vmaxq_s16(x.m[1], vdupq_n_s16(a)), vdupq_n_s16(b));
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vclamp(SIMD<int32_t, 8> x, int32_t a, int32_t b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m[0] = vminq_s32(vmaxq_s32(x.m[0], vdupq_n_s32(a)), vdupq_n_s32(b));
	tmp.m[1] = vminq_s32(vmaxq_s32(x.m[1], vdupq_n_s32(a)), vdupq_n_s32(b));
	return tmp;
}

#ifdef __aarch64__
template <>
inline SIMD<uint8_t, 32> vshuf(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m[0] = vorrq_u8(vqtbl1q_u8(a.m[0], b.m[0]), vqtbl1q_u8(a.m[1], vsubq_u8(b.m[0], vdupq_n_u8(16))));
	tmp.m[1] = vorrq_u8(vqtbl1q_u8(a.m[0], b.m[1]), vqtbl1q_u8(a.m[1], vsubq_u8(b.m[1], vdupq_n_u8(16))));
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vshuf(SIMD<int8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m[0] = vorrq_s8(vqtbl1q_s8(a.m[0], b.m[0]), vqtbl1q_s8(a.m[1], vsubq_u8(b.m[0], vdupq_n_u8(16))));
	tmp.m[1] = vorrq_s8(vqtbl1q_s8(a.m[0], b.m[1]), vqtbl1q_s8(a.m[1], vsubq_u8(b.m[1], vdupq_n_u8(16))));
	return tmp;
}
#endif

