/*
Polar encoder for non-systematic and systematic codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "polar_helper.hh"

namespace CODE {

template <typename TYPE>
class PolarEncoder
{
	typedef PolarHelper<TYPE> PH;
public:
	void operator()(TYPE *codeword, const TYPE *message, const uint8_t *frozen, int level)
	{
		int length = 1 << level;
		for (int i = 0; i < length; i += 2) {
			TYPE msg0 = frozen[i] ? PH::one() : *message++;
			TYPE msg1 = frozen[i+1] ? PH::one() : *message++;
			codeword[i] = PH::qmul(msg0, msg1);
			codeword[i+1] = msg1;
		}
		for (int h = 2; h < length; h *= 2)
			for (int i = 0; i < length; i += 2 * h)
				for (int j = i; j < i + h; ++j)
					codeword[j] = PH::qmul(codeword[j], codeword[j+h]);
	}
};

template <typename TYPE>
class PolarSysEnc
{
	typedef PolarHelper<TYPE> PH;
public:
	void operator()(TYPE *codeword, const TYPE *message, const uint8_t *frozen, int level)
	{
		int length = 1 << level;
		for (int i = 0; i < length; i += 2) {
			TYPE msg0 = frozen[i] ? PH::one() : *message++;
			TYPE msg1 = frozen[i+1] ? PH::one() : *message++;
			codeword[i] = PH::qmul(msg0, msg1);
			codeword[i+1] = msg1;
		}
		for (int h = 2; h < length; h *= 2)
			for (int i = 0; i < length; i += 2 * h)
				for (int j = i; j < i + h; ++j)
					codeword[j] = PH::qmul(codeword[j], codeword[j+h]);
		for (int i = 0; i < length; i += 2) {
			TYPE msg0 = frozen[i] ? PH::one() : codeword[i];
			TYPE msg1 = frozen[i+1] ? PH::one() : codeword[i+1];
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

