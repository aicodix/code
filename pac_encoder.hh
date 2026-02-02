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
	static bool conv(int *state, bool input)
	{
		// 1011011
		bool b1 = (*state >> 1) & 1;
		bool b3 = (*state >> 3) & 1;
		bool b4 = (*state >> 4) & 1;
		bool b6 = (*state >> 6) & 1;
		bool output = input ^ b1 ^ b3 ^ b4 ^ b6;
		*state = ((*state & 126) << 1) | (input ? 2 : 0) | (output ? 1 : 0);
		return output;
	}
public:
	void operator()(TYPE *codeword, const TYPE *message, const uint8_t *rank_map, int mesg_bits, int level)
	{
		int length = 1 << level;
		int state = 0;
		int frozen = length - mesg_bits;
		for (int i = 0; i < length; i += 2) {
			TYPE msg0 = rank_map[i] < frozen ? PH::one() : *message++;
			TYPE msg1 = rank_map[i+1] < frozen ? PH::one() : *message++;
			msg0 = 1 - 2 * conv(&state, msg0 < 0);
			msg1 = 1 - 2 * conv(&state, msg1 < 0);
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

