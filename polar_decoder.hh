/*
Successive cancellation decoding of polar codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <algorithm>
#include "polar_helper.hh"

namespace CODE {

template <typename TYPE, int M>
struct PolarTree
{
	typedef PolarHelper<TYPE> PH;
	static const int N = 1 << M;
	static void decode(TYPE **message, TYPE *hard, TYPE *soft, const uint8_t *frozen)
	{
		for (int i = 0; i < N/2; ++i)
			soft[i+N/2] = PH::prod(soft[i+N], soft[i+N/2+N]);
		PolarTree<TYPE, M-1>::decode(message, hard, soft, frozen);
		for (int i = 0; i < N/2; ++i)
			soft[i+N/2] = PH::madd(hard[i], soft[i+N], soft[i+N/2+N]);
		PolarTree<TYPE, M-1>::decode(message, hard+N/2, soft, frozen+N/2);
		for (int i = 0; i < N/2; ++i)
			hard[i] = PH::qmul(hard[i], hard[i+N/2]);
	}
};

template <typename TYPE>
struct PolarTree<TYPE, 0>
{
	typedef PolarHelper<TYPE> PH;
	static void decode(TYPE **message, TYPE *hard, TYPE *soft, const uint8_t *frozen)
	{
		if (*frozen) {
			*hard = PH::one();
		} else {
			*hard = PH::signum(soft[1]);
			*(*message)++ = *hard;
		}
	}
};

template <typename TYPE, int MAX_M>
class PolarDecoder
{
	typedef PolarHelper<TYPE> PH;
	static const int MAX_N = 1 << MAX_M;
	TYPE soft[2*MAX_N];
	TYPE hard[MAX_N];
public:
	void operator()(TYPE *message, const TYPE *codeword, const uint8_t *frozen, int level)
	{
		assert(level <= MAX_M);
		int length = 1 << level;
		for (int i = 0; i < length; ++i)
			soft[length+i] = codeword[i];

		switch (level) {
		case 0: PolarTree<TYPE, 0>::decode(&message, hard, soft, frozen); break;
		case 1: PolarTree<TYPE, 1>::decode(&message, hard, soft, frozen); break;
		case 2: PolarTree<TYPE, 2>::decode(&message, hard, soft, frozen); break;
		case 3: PolarTree<TYPE, 3>::decode(&message, hard, soft, frozen); break;
		case 4: PolarTree<TYPE, 4>::decode(&message, hard, soft, frozen); break;
		case 5: PolarTree<TYPE, 5>::decode(&message, hard, soft, frozen); break;
		case 6: PolarTree<TYPE, 6>::decode(&message, hard, soft, frozen); break;
		case 7: PolarTree<TYPE, 7>::decode(&message, hard, soft, frozen); break;
		case 8: PolarTree<TYPE, 8>::decode(&message, hard, soft, frozen); break;
		case 9: PolarTree<TYPE, 9>::decode(&message, hard, soft, frozen); break;
		case 10: PolarTree<TYPE, 10>::decode(&message, hard, soft, frozen); break;
		case 11: PolarTree<TYPE, 11>::decode(&message, hard, soft, frozen); break;
		case 12: PolarTree<TYPE, 12>::decode(&message, hard, soft, frozen); break;
		case 13: PolarTree<TYPE, 13>::decode(&message, hard, soft, frozen); break;
		case 14: PolarTree<TYPE, 14>::decode(&message, hard, soft, frozen); break;
		case 15: PolarTree<TYPE, 15>::decode(&message, hard, soft, frozen); break;
		case 16: PolarTree<TYPE, 16>::decode(&message, hard, soft, frozen); break;
		case 17: PolarTree<TYPE, 17>::decode(&message, hard, soft, frozen); break;
		case 18: PolarTree<TYPE, 18>::decode(&message, hard, soft, frozen); break;
		case 19: PolarTree<TYPE, 19>::decode(&message, hard, soft, frozen); break;
		case 20: PolarTree<TYPE, 20>::decode(&message, hard, soft, frozen); break;
		case 21: PolarTree<TYPE, 21>::decode(&message, hard, soft, frozen); break;
		case 22: PolarTree<TYPE, 22>::decode(&message, hard, soft, frozen); break;
		case 23: PolarTree<TYPE, 23>::decode(&message, hard, soft, frozen); break;
		case 24: PolarTree<TYPE, 24>::decode(&message, hard, soft, frozen); break;
		case 25: PolarTree<TYPE, 25>::decode(&message, hard, soft, frozen); break;
		case 26: PolarTree<TYPE, 26>::decode(&message, hard, soft, frozen); break;
		case 27: PolarTree<TYPE, 27>::decode(&message, hard, soft, frozen); break;
		case 28: PolarTree<TYPE, 28>::decode(&message, hard, soft, frozen); break;
		case 29: PolarTree<TYPE, 29>::decode(&message, hard, soft, frozen); break;
		default: assert(false);
		}
	}
};

}

