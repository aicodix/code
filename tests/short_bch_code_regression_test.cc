/*
Regression Test for the short BCH codes encoder and decoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include <functional>
#include "short_bch_code_decoder.hh"
#include "short_bch_code_encoder.hh"

template <int N, int K, int T, int POLY>
void bch_test(int trials)
{
	CODE::ShortBCHCodeEncoder<N, K, POLY> encode;
	CODE::ShortBCHCodeDecoder<N, K, POLY, T> decode;
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto data = std::bind(distribution(0, (1<<K)-1), generator(rd()));
	auto epos = std::bind(distribution(0, N-1), generator(rd()));
	while (--trials) {
		int message = data();
		int codeword = encode(message);
		assert((codeword>>(N-K)) == message);
		int decoded = decode(codeword);
		assert(decoded == codeword);
		int damaged = codeword;
		for (int i = 0; i < T; ++i)
			damaged ^= 1 << epos();
		int recovered = decode(damaged);
		assert(recovered == codeword);
	}
}

int main()
{
	if (1) {
		// Perfect binary Golay code using x^11+x^9+x^7+x^6+x^5+x+1
		bch_test<23, 12, 3, 0b101011100011>(1000000);
	}
	if (1) {
		// Perfect binary Golay code using x^11+x^10+x^6+x^5+x^4+x^2+1
		bch_test<23, 12, 3, 0b110001110101>(1000000);
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		bch_test<15, 5, 3, 0b10100110111>(1000000);
	}
	std::cerr << "Short BCH code regression test passed!" << std::endl;
	return 0;
}

