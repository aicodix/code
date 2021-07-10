/*
Successive cancellation list decoding of polar codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <algorithm>
#include "polar_helper.hh"

namespace CODE {

template <typename TYPE, int M>
struct PolarListTree
{
	typedef PolarHelper<TYPE> PH;
	typedef typename PH::PATH PATH;
	typedef typename PH::MAP MAP;
	static const int N = 1 << M;
	static MAP decode(PATH *metric, TYPE *message, MAP *maps, int *count, TYPE *hard, TYPE *soft, const uint8_t *frozen)
	{
		for (int i = 0; i < N/2; ++i)
			soft[i+N/2] = PH::prod(soft[i+N], soft[i+N/2+N]);
		MAP lmap = PolarListTree<TYPE, M-1>::decode(metric, message, maps, count, hard, soft, frozen);
		for (int i = 0; i < N/2; ++i)
			soft[i+N/2] = PH::madd(hard[i], vshuf(soft[i+N], lmap), vshuf(soft[i+N/2+N], lmap));
		MAP rmap = PolarListTree<TYPE, M-1>::decode(metric, message, maps, count, hard+N/2, soft, frozen+N/2);
		for (int i = 0; i < N/2; ++i)
			hard[i] = PH::qmul(vshuf(hard[i], rmap), hard[i+N/2]);
		return vshuf(lmap, rmap);
	}
};

template <typename TYPE>
struct PolarListTree<TYPE, 0>
{
	typedef PolarHelper<TYPE> PH;
	typedef typename PH::PATH PATH;
	typedef typename PH::MAP MAP;
	static MAP decode(PATH *metric, TYPE *message, MAP *maps, int *count, TYPE *hard, TYPE *soft, const uint8_t *frozen)
	{
		MAP map;
		TYPE hrd, sft = soft[1];
		if (*frozen) {
			for (int k = 0; k < TYPE::SIZE; ++k)
				if (sft.v[k] < 0)
					metric[k] -= sft.v[k];
			hrd = PH::one();
			for (int k = 0; k < TYPE::SIZE; ++k)
				map.v[k] = k;
		} else {
			PATH fork[2*TYPE::SIZE];
			for (int k = 0; k < TYPE::SIZE; ++k)
				fork[k] = fork[k+TYPE::SIZE] = metric[k];
			for (int k = 0; k < TYPE::SIZE; ++k)
				if (sft.v[k] < 0)
					fork[k] -= sft.v[k];
				else
					fork[k+TYPE::SIZE] += sft.v[k];
			int perm[2*TYPE::SIZE];
			for (int k = 0; k < 2*TYPE::SIZE; ++k)
				perm[k] = k;
			std::nth_element(perm, perm+TYPE::SIZE, perm+2*TYPE::SIZE, [fork](int a, int b){ return fork[a] < fork[b]; });
			for (int k = 0; k < TYPE::SIZE; ++k)
				metric[k] = fork[perm[k]];
			for (int k = 0; k < TYPE::SIZE; ++k)
				map.v[k] = perm[k] % TYPE::SIZE;
			for (int k = 0; k < TYPE::SIZE; ++k)
				hrd.v[k] = perm[k] < TYPE::SIZE ? 1 : -1;
			message[*count] = hrd;
			maps[*count] = map;
			++*count;
		}
		*hard = hrd;
		return map;
	}
};

template <typename TYPE, int MAX_M>
class PolarListDecoder
{
	typedef PolarHelper<TYPE> PH;
	typedef typename TYPE::value_type VALUE;
	typedef typename PH::PATH PATH;
	typedef typename PH::MAP MAP;
	static const int MAX_N = 1 << MAX_M;
	TYPE soft[2*MAX_N];
	TYPE hard[MAX_N];
	MAP maps[MAX_N];
public:
	void operator()(PATH *metric, TYPE *message, const VALUE *codeword, const uint8_t *frozen, int level)
	{
		assert(level <= MAX_M);
		int count = 0;
		metric[0] = 0;
		for (int k = 1; k < TYPE::SIZE; ++k)
			metric[k] = 1000;
		int length = 1 << level;
		for (int i = 0; i < length; ++i)
			soft[length+i] = vdup<TYPE>(codeword[i]);

		switch (level) {
		case 0: PolarListTree<TYPE, 0>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 1: PolarListTree<TYPE, 1>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 2: PolarListTree<TYPE, 2>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 3: PolarListTree<TYPE, 3>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 4: PolarListTree<TYPE, 4>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 5: PolarListTree<TYPE, 5>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 6: PolarListTree<TYPE, 6>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 7: PolarListTree<TYPE, 7>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 8: PolarListTree<TYPE, 8>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 9: PolarListTree<TYPE, 9>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 10: PolarListTree<TYPE, 10>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 11: PolarListTree<TYPE, 11>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 12: PolarListTree<TYPE, 12>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 13: PolarListTree<TYPE, 13>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 14: PolarListTree<TYPE, 14>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 15: PolarListTree<TYPE, 15>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 16: PolarListTree<TYPE, 16>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 17: PolarListTree<TYPE, 17>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 18: PolarListTree<TYPE, 18>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 19: PolarListTree<TYPE, 19>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 20: PolarListTree<TYPE, 20>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 21: PolarListTree<TYPE, 21>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 22: PolarListTree<TYPE, 22>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 23: PolarListTree<TYPE, 23>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 24: PolarListTree<TYPE, 24>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 25: PolarListTree<TYPE, 25>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 26: PolarListTree<TYPE, 26>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 27: PolarListTree<TYPE, 27>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 28: PolarListTree<TYPE, 28>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		case 29: PolarListTree<TYPE, 29>::decode(metric, message, maps, &count, hard, soft, frozen); break;
		default: assert(false);
		}

		MAP acc = maps[count-1];
		for (int i = count-2; i >= 0; --i) {
			message[i] = vshuf(message[i], acc);
			acc = vshuf(maps[i], acc);
		}
	}
};

}

