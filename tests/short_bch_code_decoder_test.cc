/*
Test for the short BCH codes decoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include <functional>
#include "short_bch_code_decoder.hh"

int main()
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	if (1) {
		// Perfect binary Golay code using x^11+x^9+x^7+x^6+x^5+x+1
		const int N = 23, K = 12, T = 3, POLY = 0b101011100011;
		CODE::ShortBCHCodeDecoder<N, K> decode(POLY, T);
		int target = 0b10101101101111011111001;
		int damaged = target;
		typedef std::uniform_int_distribution<int> distribution;
		auto epos = std::bind(distribution(0, N-1), generator);
		for (int i = 0; i < T; ++i)
			damaged ^= 1 << epos();
		int recovered = decode(damaged);
		assert(recovered == target);
	}
	if (1) {
		// Perfect binary Golay code using x^11+x^10+x^6+x^5+x^4+x^2+1
		const int N = 23, K = 12, T = 3, POLY = 0b110001110101;
		CODE::ShortBCHCodeDecoder<N, K> decode(POLY, T);
		int target = 0b10101101101100100010101;
		int damaged = target;
		typedef std::uniform_int_distribution<int> distribution;
		auto epos = std::bind(distribution(0, N-1), generator);
		for (int i = 0; i < T; ++i)
			damaged ^= 1 << epos();
		int recovered = decode(damaged);
		assert(recovered == target);
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		const int N = 15, K = 5, T = 3, POLY = 0b10100110111;
		CODE::ShortBCHCodeDecoder<N, K> decode(POLY, T);
		int target = 0b110010001111010;
		int damaged = target;
		typedef std::uniform_int_distribution<int> distribution;
		auto epos = std::bind(distribution(0, N-1), generator);
		for (int i = 0; i < T; ++i)
			damaged ^= 1 << epos();
		int recovered = decode(damaged);
		assert(recovered == target);
	}
	std::cerr << "Short BCH code decoder test passed!" << std::endl;
	return 0;
}

