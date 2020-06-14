/*
Test for the short BCH codes encoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "short_bch_code_encoder.hh"

template<typename TYPE>
int popcnt(TYPE x)
{
	int cnt = 0;
	while (x) {
		++cnt;
		x &= x-1;
	}
	return cnt;
}

template <int N, int K, int T, int POLY>
void bch_test(int message, int target)
{
	CODE::ShortBCHCodeEncoder<N, K, POLY> encode;
	int hist[N+1] = { 0 };
	for (int j = 0; j < (1 << K); ++j)
		for (int i = j + 1; i < (1 << K); ++i)
			++hist[popcnt(encode(i)^encode(j))];
	std::cout << "BCH(" << N << ", " << K << ") T=" << T << " hist:";
	for (int i = 0; i < N + 1; ++i)
		if (hist[i])
			std::cout << " " << i << ":" << hist[i];
	std::cout << std::endl;
	int d = 0;
	while (!hist[d])
		++d;
	assert((d-1)/2 == T);
	int codeword = encode(message);
	assert(target == codeword);
}

int main()
{
	if (1) {
		// Perfect binary Golay code using x^11+x^9+x^7+x^6+x^5+x+1
		bch_test<23, 12, 3, 0b101011100011>(0b101011011011, 0b10101101101111011111001);
	}
	if (1) {
		// Perfect binary Golay code using x^11+x^10+x^6+x^5+x^4+x^2+1
		bch_test<23, 12, 3, 0b110001110101>(0b101011011011, 0b10101101101100100010101);
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		bch_test<15, 5, 3, 0b10100110111>(0b11001, 0b110010001111010);
	}
	std::cerr << "Short BCH code encoder test passed!" << std::endl;
	return 0;
}

