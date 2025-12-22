/*
Encoder for Polarization-Adjusted Convolutional codes

Copyright 2025 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "polar_helper.hh"

namespace CODE {

template <typename TYPE>
class PACEncoder
{
	typedef PolarHelper<TYPE> PH;
	static bool get(const uint32_t *bits, int idx)
	{
		return (bits[idx/32] >> (idx%32)) & 1;
	}
	static bool conv(int *state, bool input)
	{
		bool s1 = *state & 1;
		bool s2 = *state & 2;
		bool output = input ^ s1 ^ s2;
		*state = (s1 << 1) | output;
		return output;
	}
public:
	void operator()(TYPE *codeword, const TYPE *message, const uint32_t *frozen, int level)
	{
		int length = 1 << level;
		int state = 0;
		for (int i = 0; i < length; i += 2) {
			TYPE msg0 = get(frozen, i) ? PH::one() : 1 - 2 * conv(&state, *message++ < 0);
			TYPE msg1 = get(frozen, i+1) ? PH::one() : 1 - 2 * conv(&state, *message++ < 0);
			codeword[i] = PH::qmul(msg0, msg1);
			codeword[i+1] = msg1;
		}
		for (int h = 2; h < length; h *= 2)
			for (int i = 0; i < length; i += 2 * h)
				for (int j = i; j < i + h; ++j)
					codeword[j] = PH::qmul(codeword[j], codeword[j+h]);
	}
};

}

